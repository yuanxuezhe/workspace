#include <iostream>
#include <stdio.h>
#include <iomanip>   
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

int main()
{
    unsigned int x = 0x12345678;
    unsigned char *p = (unsigned char*)&x;
    printf("%0x %0x %0x %0x\n", p[0],p[1],p[2],p[3]);

    unsigned int y = htonl(x);
    p = (unsigned char*)&y;
    printf("%0x %0x %0x %0x \n", p[0],p[1],p[2],p[3]);

    //inet_aton()

    unsigned long addr = inet_addr("192.168.2.2");

    printf("%u\n", ntohl(addr));

    in_addr addrs;
    addrs.s_addr = addr;

    printf("%s\n", inet_ntoa(addrs));
    return 0;
}