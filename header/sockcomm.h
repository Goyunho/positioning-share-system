#include<stdio.h>
#include<winsock2.h>

typedef unsigned char Byte;
#define  BUFF_SIZE   1024*4

SOCKET sock(char addr[16], int port);
