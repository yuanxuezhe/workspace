#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */ 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../../ys_public/ys_error.h"
#include "../../ys_public/ys_socket.h"
#include <string.h>

#include <sys/select.h>

using namespace std;

void do_cli(int sock)
{
    fd_set rset;
    FD_ZERO(&rset);
    
    int fd_stdin = fileno(stdin);
    int nfds = sock > fd_stdin ? sock : fd_stdin;
    int nready;

    struct sockaddr_in peeraddr;
    socklen_t peerlen;

    char sendbuf[1024] = {0};
    char recvbuf[1024] = {0};

    while (1)
    {
        FD_SET(fd_stdin, &rset);
        FD_SET(sock, &rset);

        nready = select(nfds + 1, &rset, NULL, NULL, NULL);
        printf("select nready %d \n",nready );
        if (nready == -1)
        {
            ERR_EXIT("select");
        }

        if (nready == 0)
        {
            continue;
        }

        if (FD_ISSET(sock,&rset))
        {
            int ret = readn(sock,recvbuf, sizeof(recvbuf));
            if (ret == -1)
            {
                ERR_EXIT("read");
            }
            else if (ret == 0)
            {
                printf("server close\n");     
                break;
            }  
            printf("%s\n",msgdirection(sock));
            fputs(recvbuf, stdout);

            
            memset(recvbuf, 0, sizeof(recvbuf));
        }

        if (FD_ISSET(fd_stdin,&rset))
        {
            if (fgets(sendbuf, sizeof(sendbuf), stdin) == NULL)
                break;
            writen(sock, sendbuf, sizeof(sendbuf));
            memset(sendbuf, 0, sizeof(sendbuf));
        }
    }
  
    close(sock);
}
int main()
{
    int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        ERR_EXIT("socket!");
    }

    sockaddr_in addr_in;
    memset(&addr_in, 0, sizeof(addr_in));
    addr_in.sin_family = AF_INET;
    addr_in.sin_port = htons(9158);
    addr_in.sin_addr.s_addr = htonl(INADDR_ANY);//inet_addr("127.0.0.1");

    if(connect(sock, (struct sockaddr*)&addr_in, sizeof(addr_in)) < 0)
    {
        ERR_EXIT("connect!");
    }   
   
    do_cli(sock);

    return 0;
}