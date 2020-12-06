#ifndef YS_SOCKET_H 
#define YS_SOCKET_H 

#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../ys_public/ys_define.h"

struct tcp_packet {
    int len;
    char buf[1024];
};

ssize_t readn(int fd, void *buf, size_t count)
{
    size_t nleft = count;
    ssize_t nread;
    char *bufp = (char*)buf;
    while (nleft > 0)
    {
        if ((nread = read(fd, bufp, nleft)) < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            return 1;
        }
        else if (nread == 0)
        {
            return count - nleft;
        }
        bufp += nread;
        nleft -= nread;
    }

    return count;
}

ssize_t writen(int fd, const void *buf, size_t count)
{
    size_t nleft = count;
    ssize_t nwrite;
    char *bufp = (char*)buf;
    while (nleft > 0)
    {
        if ((nwrite = write(fd, bufp, nleft)) < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            return 1;
        }
        else if (nwrite == 0)
        {
            continue;
        }
        bufp += nwrite;
        nleft -= nwrite;
    }

    return count;
}

char* msgdirection(int sock)
{
    char128 p;
    char* a = p;
    struct sockaddr_in addr;
    socklen_t addrlen;
    addrlen = sizeof(addr);
    int n = 0;
    if (!getpeername(sock, (struct sockaddr *)&addr, &addrlen))
    {
        n = snprintf(a, sizeof(p),"Msg direction:[%s:%d]", inet_ntoa(addr.sin_addr),  ntohs(addr.sin_port));
    }

    if (!getsockname(sock, (struct sockaddr *)&addr, &addrlen))
    {
        snprintf(a + n, sizeof(p) - n," ==> [%s:%d]", inet_ntoa(addr.sin_addr),  ntohs(addr.sin_port));
    }

    return a;
}

#endif	// YS_SOCKET_H