#include<stdio.h>
#include<winsock2.h>

typedef unsigned char Byte;
#define  BUFF_SIZE   1024*4

WSADATA wsaData;

void ErrorHandling(char *message)
{
        fputs(message, stderr);
        fputc('\n', stderr);
        exit(1);
}

void sock_init(SOCKET* client_socket) {
    // Load WinSocket 2.2 DLL
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
            ErrorHandling("WSAStartup(), error");
    }

    *client_socket  = socket( PF_INET, SOCK_STREAM, 0);

    if( INVALID_SOCKET == *client_socket)
    {
        printf( "socket create fail.\n");
        exit( 1);
    }
}

void sock_conn(SOCKET *client_socket, const char addr[], const int port){
    struct sockaddr_in server_addr;
    char buff[BUFF_SIZE+5];

    memset( &server_addr, 0, sizeof( server_addr));
    server_addr.sin_family     = AF_INET;
    server_addr.sin_port       = htons(port);
    server_addr.sin_addr.s_addr= inet_addr(addr);

    if( SOCKET_ERROR == connect( *client_socket, (SOCKADDR*)&server_addr, sizeof( server_addr)) )
    {
        ErrorHandling("Connect() error");
    }
}

void sock(SOCKET *client_socket, char addr[], int port){
    sock_init(client_socket);
    sock_conn(client_socket, addr, port);
}

#if 0 // main() test
void proc(){
    char buff[BUFF_SIZE+5] = {0};
    char plain_text[16] = "uTest.txt";
    char return_text[BUFF_SIZE+5] = {0};
    int i;
    SOCKET client_socket = {0};

    sock(&client_socket, "127.0.0.1", 9001);
    send(client_socket, plain_text, strlen(plain_text)+1, 0);
    recv(client_socket, return_text, BUFF_SIZE, 0);
    printf("%s\n", return_text);
    printf("\n");
    closesocket(client_socket);
}


int main(){
    proc();

    return 0;
}
#endif
