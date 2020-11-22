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
   
    char sendbuf[1024] = {0};
    char recvbuf[1024] = {0};

    while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
    {
        write(sock, sendbuf, sizeof(sendbuf));
        read(sock,recvbuf, sizeof(recvbuf));
        fputs(recvbuf, stdout);

        memset(sendbuf, 0, sizeof(sendbuf));
        memset(recvbuf, 0, sizeof(recvbuf));
    }

    close(sock);
    return 0;
}