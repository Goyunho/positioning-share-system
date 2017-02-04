#include<stdio.h>
#include<winsock2.h>

typedef unsigned char Byte;
#define  BUFF_SIZE   1024*4

void sock(SOCKET *client_socket, char addr[], int port);
