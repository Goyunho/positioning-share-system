#include<stdio.h>
#include<winsock2.h>

#define  BUFF_SIZE   1024*4


void ErrorHandling(char *message)
{
        fputs(message, stderr);
        fputc('\n', stderr);
        exit(1);
}

SOCKET sock_init(){
    WSADATA wsaData;
    SOCKET client_socket;

    // Load WinSocket 2.2 DLL
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
            ErrorHandling("WSAStartup(), error");
    }

    client_socket  = socket( PF_INET, SOCK_STREAM, 0);

    if( INVALID_SOCKET == client_socket)
    {
        printf( "socket 생성 실패\n");
        exit( 1);
    }

    return client_socket;
}

void sock_conn(SOCKET client_socket, char addr[16], int port){
    struct sockaddr_in server_addr;
    char buff[BUFF_SIZE+5];

    memset( &server_addr, 0, sizeof( server_addr));
    server_addr.sin_family     = AF_INET;
    server_addr.sin_port       = htons(port);
    server_addr.sin_addr.s_addr= inet_addr(addr);

    if( SOCKET_ERROR == connect( client_socket, (SOCKADDR*)&server_addr, sizeof( server_addr) ) )
    {
          ErrorHandling("Connect() error");
    }
}

void proc(SOCKET client_socket){
    char buff[BUFF_SIZE+5] = {0};

    send( client_socket, "hi, hello, my name is yoon", strlen("hi, hello, my name is yoon")+1, 0);      // +1: NULL까지 포함해서 전송
    recv( client_socket, buff, BUFF_SIZE, 0);
    printf( "%s\n", buff);
}

#if 1 // main() test
int main(){
    SOCKET client_socket = 0;

    client_socket = sock_init();
    sock_conn(client_socket, "127.0.0.1", 9001);
    proc(client_socket);
    closesocket(client_socket);

    return 0;
}
#endif
