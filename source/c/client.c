#include <stdio.h>
#include <string.h>
#include "chiper.h" // aria encryption library
#include "sockcomm.h"

typedef struct conf {
    Byte mode; // 0: download, 1: upload
    char ip[16];
    int port;
    char filepath;
    int bcm; // block chiper mode of operation
} CONF;

void init(){
    Byte *b;
    const Word NUMBER=0x00000042;

    printf("BEGIN testing endianness...\n");
    printf("Since you are running this, it means that you have defined \
    either LITTLE_ENDIAN or BIG_ENDIAN.  Let's see if you were correct.\n");
    b=(Byte *)(&NUMBER);
    if (b[0]==0x42) {
        #undef BIG_ENDIAN
        printf("We are on LITTLE_ENDIAN platform.\n");
        #ifdef BIG_ENDIAN
        printf("WARNING: BIG_ENDIAN defined on LITTLE_ENDIAN platform.\n");
        printf("         You should define LITTLE_ENDIAN instead of BIG_ENDIAN.\n");
        #else
        printf("Okay.  You were correct.\n");
        #endif
    } else {
        #undef LITTLE_ENDIAN
        printf("We are on BIG_ENDIAN platform.\n");
        #ifdef LITTLE_ENDIAN
        printf("WARNING: LITTLE_ENDIAN defined on BIG_ENDIAN platform.\n");
        printf("         You should define BIG_ENDIAN instead of LITTLE_ENDIAN.\n");
        #else
        printf("Okay.  You were correct.\n");
        #endif
    }
    printf("END   testing endianness.\n\n");
}

void upload_file(char ip[], int port, char filepath[]) {
    Byte key[KEY] = {0,};
    Byte roundkey[ROUND_KEY];
    Byte plain_block[BLOCK_SIZE], crypt_block[BLOCK_SIZE];
    FILE* file;
    SOCKET client_socket = sock(ip, port); /* socket init */
    char state[3] = {0,};

    // send process
    // 1. make EncKey
    init(); /* checking platform for encryption environment */
    EncKeySetup(key, roundkey, 192);
    // 2. open file
    file = fopen(filepath, "rb");
    // 3. request server state
    send(client_socket, strcat("u", filepath), strlen(filepath), 0);
    // 4. send file after block encrypt
    while(strcmp(state, "ok")) {
        // encryption block
        fread(plain_block, 1, BLOCK_SIZE, file);
        Crypt(plain_block, 14, roundkey, crypt_block);
        send(client_socket, plain_block, BLOCK_SIZE, 0);
        recv(client_socket, state, 2, 0);
    }
    fclose(file);
    closesocket(client_socket);
    printf("%s\n", "done");
}

void download_file(char ip[], int port, char filepath[]) {
    Byte key[KEY] = {0,};
    Byte roundkey[ROUND_KEY];
    Byte plain_block[BLOCK_SIZE], crypt_block[BLOCK_SIZE];
    FILE* file;
    SOCKET client_socket = sock(ip, port); /* socket init */

    // download process
    // 1. init crypt platform & make DecKey
    init(); /* checking platform for encryption environment */
    DecKeySetup(key, roundkey, 192);
    // 2. open file
    file = fopen(strcat("./download/", filepath), "wb");
    // 3. request file to server
    send(client_socket, strcat("d", filepath), strlen(filepath), 0);
    // 4. recv encrypted file
    while(recv(client_socket, crypt_block, BLOCK_SIZE, 0)) {
        // decryption block
        Crypt(crypt_block, 14, roundkey, plain_block);
        fwrite(plain_block, 1, BLOCK_SIZE, file);
        send(client_socket, "ok", 2, 0);
    }
    fclose(file);
    closesocket(client_socket);
    printf("%s\n", "done");
}

void set_conf(CONF* conf, int argc, char *argv[]){
    if(strcmp(argv[1],"-dl")) {
        conf->mode = 0;
    }
    else if(strcmp(argv[1], "-ul")) {
        conf->mode = 1;
    }
    strcpy(conf->ip, strtok(argv[2], ":"));
    conf->port = atoi(strtok(NULL, ":"));
    conf->filepath = argv[3];
    if(argc>=4) {
        if(strcmp(argv[4], "-bcm")) {
            if(strcmp(argv[5], "ecb")) conf->bcm = 0;
            else if(strcmp(argv[5], "ctr")) conf->bcm = 1;
            else conf->bcm = 0;
        } else conf->bcm = 0;
    }
}

int main(int argc, char *argv[]){ // ex) prog (-dl|-ul) 127.0.0.1:9001 eisle.pdf [-e (ecb|ctr)]
    // initialization
    SOCKET client_socket;
    CONF conf;

    init(); /* checking platform for encryption environment */
    set_conf(&conf, argc, argv); /* basic info setting */

    if(conf.mode) { // mode == 1 : upload mode
        upload_file(conf.ip, conf.port, conf.filepath);
    }
    else {
        download_file(conf.ip, conf.port, conf.filepath);
    }

    getchar();
    return 0;
}
