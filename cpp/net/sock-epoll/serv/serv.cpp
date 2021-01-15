#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */ 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ys_error.h"
#include <string.h>
#include <errno.h>
#include "ys_socket.h"
#include <vector>
#include <sys/epoll.h>
#include "zookeeper/zookeeper.h"
#include "CZkCreateEphemeralNode.h"

using namespace std;

typedef vector<struct epoll_event> EventList;

int create_root(zhandle_t *zkhandle, const char *node_name, const char *data) {
    int ret;
    struct Stat stat;
    char node[512] = {0};

    printf("zoo_create /zkwatcher node\n");

    ret = zoo_exists(zkhandle, node_name, true, &stat);

    if (ret == ZOK) {
        printf("create_root %s already create\n", node_name);
    } else if (ret == ZNONODE){
        ret = zoo_create(zkhandle,
                node_name, 
                data, 
                sizeof(data),
                &ZOO_OPEN_ACL_UNSAFE,  /* a completely open ACL */
                0,
                node,
                sizeof(node)-1);
        if (ret) {
            fprintf(stderr, "zoo_create error [%d]\n", ret);
        }
    } else {
        printf("create_root error\n"); 
    }
    return ret;
}
//https://blog.csdn.net/yangzhen92/article/details/53248294
//https://blog.csdn.net/weixin_30919571/article/details/99812566?utm_medium=distribute.pc_relevant_t0.none-task-blog-BlogCommendFromBaidu-1.control&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-BlogCommendFromBaidu-1.control
//https://download.csdn.net/download/wcwtianxia/10571835?utm_medium=distribute.pc_relevant_t0.none-task-download-BlogCommendFromBaidu-1.control&depth_1-utm_source=distribute.pc_relevant_t0.none-task-download-BlogCommendFromBaidu-1.control
//https://github.com/fdxuwei/cppzk
int main(int argc, char const *argv[])
{
    const char* host = "127.0.0.1:2181";
    zhandle_t* zkhandle;
    int timeout = 5000;
    zoo_set_debug_level(ZOO_LOG_LEVEL_DEBUG);
    zoo_deterministic_conn_order(1);
    zkhandle = zookeeper_init(host, NULL, timeout, 
            0, (void *)"Zookeeper examples: config center services", 0);
    if (zkhandle == NULL) {
        fprintf(stderr, "Connecting to zookeeper servers error...\n");
        exit(EXIT_FAILURE);
    }

    int port = 9158;
    if (argc == 2)
    {
        port=strtol(argv[1],NULL,10);
    }
    else
    {
        ERR_EXIT("argv error!");
    }

    printf("zoo_create /servers node\n");
    create_root(zkhandle, "/servers", NULL);

    std::string strError;
	CZkCreateEphemeralNode* pZk = new CZkCreateEphemeralNode("127.0.0.1:2181", 3000, "/servers/serv", argv[1], strError);
	if ( strError.length() > 0 )
	{
		wcout << L"CZkCreateEphemeralNode error." << endl;
	}

    int listenfd;
    int on = 1;
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
       ERR_EXIT("socket!");
    }
    if ((setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,  &on,  sizeof(on))) < 0)
    {
        ERR_EXIT("setsockopt");
    }

    sockaddr_in addr_in;
    memset(&addr_in, 0, sizeof(addr_in));
    addr_in.sin_family = AF_INET;
    addr_in.sin_port = htons(port);
    addr_in.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(listenfd, (struct sockaddr *)&addr_in, sizeof(addr_in)) < 0)
    {
        ERR_EXIT("bind!");
    }

    if (listen(listenfd, SOMAXCONN) < 0)
    {
        ERR_EXIT("listen!");
    }

    int epollfd;
    epollfd = epoll_create1(EPOLL_CLOEXEC);

    struct epoll_event event;
    EventList events(16);
    event.data.fd = listenfd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &event);

    char recvbuf[1024];
    char backbuff[1024];
    struct sockaddr_in peeraddr;
    socklen_t peerlen;
    int conn;

    int nready;
    while (1)
    {
        nready = epoll_wait(epollfd, &*events.begin(), events.size(), -1);

        if (nready == -1)
        {
            if (errno == EINTR)
            {
                continue;
            }
            
            ERR_EXIT("epoll_wait");
        }

        if (nready == 0)
        {
            continue;
        }

        if ((size_t)nready == events.size())
        {
            events.resize(events.size() * 2);
        }
        
        for (size_t i = 0; i < nready; i++)
        {
            if (events[i].data.fd == listenfd)
            {
                peerlen = sizeof(peeraddr);
                conn = accept(listenfd, (struct sockaddr*)&peeraddr, &peerlen);
                if (conn == -1)
                {
                    printf("Accept Conn error!");
                    continue;
                }

                printf("ip:%s port:%d \n", inet_ntoa(peeraddr.sin_addr),  ntohs(peeraddr.sin_port));
                //clients.push_back(conn);
                
                activate_nonblock(conn);
                event.data.fd = conn;
                event.events = EPOLLIN | EPOLLET;
                epoll_ctl(epollfd, EPOLL_CTL_ADD, conn, &event);
            }
            else if (events[i].events & EPOLLIN)
            {
                conn = events[i].data.fd;
                if (conn < 0)
                {
                    continue;
                }
                
                memset(recvbuf, 0, sizeof(recvbuf));
                int ret = readn(conn, recvbuf, sizeof(recvbuf));
                if (ret == 0)
                {
                    printf("client close \n");
                    close(conn);
                    event = events[i];
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, conn, &event);
                    continue;
                }
                else if (ret == -1)
                {       
                    ERR_EXIT("read");
                }  

                printf("%s\n",msgdirection(conn));
                
                fputs(recvbuf, stdout);

                memset(backbuff, 0, sizeof(backbuff));
                snprintf(backbuff, sizeof(backbuff),"Ans from [port:%d]: %s", port, recvbuf);
                writen(conn, backbuff, sizeof(backbuff));
            }  
        }
    }
     
    return 0;
}