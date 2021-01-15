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

#include <sys/select.h>

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
    int client[FD_SETSIZE];
    for (size_t i = 0; i < FD_SETSIZE; i++)
    {
        client[i] = -1;
    }
    
    fd_set rset, allrset;
    FD_ZERO(&rset);
    FD_ZERO(&allrset);
    FD_SET(listenfd, &allrset);

    int nready;
    int nMaxfds = listenfd;

    while (1)
    {
        rset = allrset;

        nready = select(nMaxfds + 1, &rset, NULL, NULL, NULL);

        if (nready == -1)
        {
            ERR_EXIT("select");
        }

        if (nready == 0)
        {
            continue;
        }
        
        if (FD_ISSET(listenfd,&rset))
        {
            peerlen = sizeof(peeraddr);
            if ((conn = accept(listenfd, (struct sockaddr*)&peeraddr, &peerlen)) < 0)
            {
                printf("Accept Conn error!");
                continue;
            }

            size_t i;
            for (i = 0; i < FD_SETSIZE; i++)
            {
                if (client[i] != -1)
                {
                    continue;
                }
                else
                {
                    client[i] = conn;
                    break;
                }
            }

            if (i == FD_SETSIZE)
            {
                printf("Too Many Connections!\n"); 
                exit(EXIT_FAILURE);
            }
            
            printf("ip:%s port:%d \n", inet_ntoa(peeraddr.sin_addr),  ntohs(peeraddr.sin_port));

            FD_SET(conn, &allrset);

            nMaxfds = conn > nMaxfds ? conn : nMaxfds;
 
            if (--nready  <= 0)
            {
                continue;
            }  
        }
        
        for (size_t i = 0; i < FD_SETSIZE; i++)
        {
            if (client[i] == -1)
            {
                continue;
            }
            conn = client[i];
            if (FD_ISSET(conn, &rset))
            {
                memset(recvbuf, 0, sizeof(recvbuf));
                int ret = readn(conn, recvbuf, sizeof(recvbuf));
                if (ret == 0)
                {
                    printf("client close \n");
                    FD_CLR(conn, &allrset);
                    client[i] = -1;
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