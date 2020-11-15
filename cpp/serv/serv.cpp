#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */ 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../ys_public/ys_error.h"
#include <string.h>
using namespace std;

void do_serv(int conn)
{
    char recvbuf[1024];
    while (1)
    {
        memset(recvbuf, 0, sizeof(recvbuf));
        int ret = read(conn, recvbuf, sizeof(recvbuf));
        fputs(recvbuf, stdout);
        write(conn, recvbuf, sizeof(recvbuf));
    }
}

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

    struct sockaddr_in peeraddr;
    socklen_t peerlen = sizeof(peeraddr);
    int conn;

    pid_t pid;
    while (1)
    {
        if ((conn = accept(listenfd, (struct sockaddr*)&peeraddr, &peerlen)) < 0)
        {
            ERR_EXIT("accept!");
        }
        
        printf("ip:%s port:%d \n", inet_ntoa(peeraddr.sin_addr),  ntohs(peeraddr.sin_port));

        pid = fork();

        if (pid == -1)
        {
            ERR_EXIT("fork");
        }
        else if (pid == 0)
        {
            close(listenfd);
            do_serv(conn);
        }
        else
        {
            close(conn);
        } 
    }

    close(listenfd);
     
    return 0;
}