#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */ 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../../ys_public/ys_error.h"
#include <string.h>
#include <errno.h>
#include "../../ys_public/ys_socket.h"

#include <poll.h>

using namespace std;

int main()
{
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
    addr_in.sin_port = htons(9158);
    addr_in.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(listenfd, (struct sockaddr *)&addr_in, sizeof(addr_in)) < 0)
    {
        ERR_EXIT("bind!");
    }

    if (listen(listenfd, SOMAXCONN) < 0)
    {
        ERR_EXIT("listen!");
    }
    char recvbuf[1024];
    struct sockaddr_in peeraddr;
    socklen_t peerlen;
    int conn;

    int nMaxi = 0;
    struct pollfd client[1024];
    for (size_t i = 0; i < 1024; i++)
    {
        client[i].fd = -1;
    }
    
    int nready;
    client[0].fd = listenfd;
    client[0].events = POLLIN;

    while (1)
    {
        
        nready = poll(client, nMaxi + 1, 1);

        if (nready == -1)
        {
            if (errno == EINTR)
            {
                continue;
            }
            
            ERR_EXIT("select");
        }

        if (nready == 0)
        {
            continue;
        }
        
        if (client[0].revents & POLLIN)
        {
            peerlen = sizeof(peeraddr);
            if ((conn = accept(listenfd, (struct sockaddr*)&peeraddr, &peerlen)) < 0)
            {
                printf("Accept Conn error!");
                continue;
            }

            size_t i;
            for (i = 0; i < 1024; i++)
            {
                if (client[i].fd == -1)
                {
                    client[i].fd = conn;
                    if (i > nMaxi)
                    {
                        nMaxi = i;
                    }
                    break;
                }
            }

            if (i == 1024)
            {
                printf("Too Many Connections!\n"); 
                exit(EXIT_FAILURE);
            }
            
            printf("ip:%s port:%d \n", inet_ntoa(peeraddr.sin_addr),  ntohs(peeraddr.sin_port));
 
            client[i].events = POLLIN;

            if (--nready  <= 0)
            {
                continue;
            }  
        }
        
        for (size_t i = 1; i <= nMaxi; i++)
        {
            conn = client[i].fd;

            if (conn == -1)
            {
                continue;
            }

            if (client[i].revents & POLLIN)
            {
                memset(recvbuf, 0, sizeof(recvbuf));
                int ret = readn(conn, recvbuf, sizeof(recvbuf));
                if (ret == 0)
                {
                    printf("client close \n");
                    client[i].fd = -1;
                    close(conn);
                    continue;
                }
                else if (ret == -1)
                {       
                    ERR_EXIT("read");
                }  

                printf("%s\n",msgdirection(conn));
                
                fputs(recvbuf, stdout);

                writen(conn, recvbuf, sizeof(recvbuf));

                if (--nready  <= 0)
                {
                    break;
                } 
            }
        }
    }

    close(listenfd);
     
    return 0;
}