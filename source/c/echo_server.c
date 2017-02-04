#include <stdio.h>
#include <stdlib.h> // exit()
#include <string.h>
#include "sys/types.h"
#include "sys/socket.h" // UNIX socket
#include <netinet/in.h>

#define Byte unsigned char
#define true 1
#define false 0
#define BLOCK_SIZE 16
#define STREAM_SIZE 256
#define STATE_SIZE 4
#define MODE_SIZE 1
#define FILENAME_SIZE 256
#define GETINFO_SIZE MODE_SIZE + FILENAME_SIZE

#define DEBUG_TEST 1
#define DEBUG_PRT printf("[DEBUG] %d : %s\n", __LINE__, __func__);

void ErrorHandling(char msg[]) {
    printf("%s", msg);
    exit(0);
}

int sock(int port) {
    static int server_fd_listen;
    static struct sockaddr_in server; // = {AF_INET, port, INADDR_ANY};

    server.sin_family = AF_INET;
    server.sin_port = port; //htons(port);
    server.sin_addr.s_addr = INADDR_ANY; //htonl(INADDR_ANY);

    // create socket
    if((server_fd_listen = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        ErrorHandling("Server : Can't open stream socket\n");

    // call bind()
    if(bind(server_fd_listen, (struct sockaddr *)&server, sizeof(server)) <0)
        ErrorHandling("Server : Can't bind local address.\n");

    // socket listening mode
    if(listen(server_fd_listen, 5) < 0)
        ErrorHandling("Server : Can't listening connect.\n");

    printf("created socket!\n");
    return server_fd_listen;
}


int main(int argc, char *argv[]){
    // initialization
    int server_fd_listen, server_fd_connect;
    char accept_info[GETINFO_SIZE]; // FILENAME_SIZE + (send or recv mode)

    server_fd_listen = sock(9000);
    printf("ready!\n");
    server_fd_connect = accept(server_fd_listen, NULL, NULL);

    printf("connection!\n");

    recv(server_fd_connect, accept_info, GETINFO_SIZE, 0);
    printf("recv: %s\n", accept_info);
    send(server_fd_connect, strcat(accept_info, " return!"), GETINFO_SIZE, 0);

    close(server_fd_connect);
    close(server_fd_listen);

    return 0;
}
