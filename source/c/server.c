#include <stdio.h>
#include <string.h>
#include "sys/types.h"
#include "sys/socket.h" // UNIX socket
#include <netinet/in.h>

#define BLOCK_SIZE 16
#define STREAM_SIZE 256
#define STATE_SIZE 4
#define MODE_SIZE 1
#define FILENAME_SIZE 256
#define GETINFO_SIZE MODE_SIZE + FILENAME_SIZE

void ErrorHandling(char msg[]) {
    printf("%s", msg);
    exit(0);
}

int sock(int port=9000) {
    int server_fd_listen;
    struct sockaddr_in server = {AF_INET, port, INADDR_ANY}

    // create socket
    if((server_fd_listen = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        ErrorHandling("Server : Can't open stream socket\n");

    // initialize server_Addr as NULL
    memset(&socket, 0x00, sizeof(server));

    // call bind()
    if(bind(server_fd_listen, (struct sockaddr *)&server, sizeof(server)) <0)
        ErrorHandling("Server : Can't bind local address.\n");

    // socket listening mode
    if(listen(server_fd_listen, 5) < 0)
        ErrorHandling("Server : Can't listening connect.\n");

    return server_fd_listen;
}

void for_upload(char filename[]) { // client -> server
    Byte stream_block[STREAM_SIZE];
    FILE* file;

    // open file
    if(file = fopen(stream_block, "wb") == 0) {
        send(server_fd_connect, "fe", STATE_SIZE, 0);
        ErrorHandling("file open fail.");
    }

    // send ok sign
    send(server_fd_connect, "ok", STATE_SIZE, 0);

    // recv file stream
    while(true) {
        if(recv(server_fd_connect, stream_block, STREAM_SIZE, 0) == 0)
            break;
        fwrite(stream_block, 1, strlen(stream_block), file);
        send(server_fd_connect, "ok", STATE_SIZE, 0);
    }

    // finish process
    close(sockfd_connect);
    fclose(file);
}

void for_download(char filename[]) { // client -> server
    Byte stream_block[STREAM_SIZE];
    char state[STATE_SIZE];
    FILE* file;

    // open file
    if(file = fopen(stream_block, "rb") == 0) {
        send(server_fd_connect, "fe", STATE_SIZE, 0);
        ErrorHandling("file open fail.");
    }

    // send file stream
    while(fread(stream_block, 1, BLOCK_SIZE, file)) {
        send(server_fd_connect, stream_block, BLOCK_SIZE, 0);
        recv(server_fd_connect, state, STATE_SIZE, 0);
        if(strcmp(state, "ok"))
            break;
    }

    // finish process
    close(sockfd_connect);
    fclose(file);
}

int main(int argc, char *argv[]){
    // initialization
    int server_fd_listen, server_fd_connect;
    char accept_info[GETINFO_SIZE]; // FILENAME_SIZE + (send or recv mode)

    server_fd_listen = sock(9000);

    server_fd_connect = accept(server_fd_listen, NULL, NULL);

    recv(server_fd_connect, accept_info, GETINFO_SIZE, 0);
    if(accept_info[0] == 'u') {
        for_upload(*(accept_info+1));
    }
    else if(accept_info[0] == 'd') {
        for_download(*(accept_info+1));
    }
    else {
        send(server_fd_connect, "no", STATE_SIZE, 0);
    }

    return 0;
}
