#include <stdio.h>
#include <string.h>
#include "chiper.h" // aria encryption library
#include "sockcomm.h"

#define DEBUG_TEST 1
#define DEBUG_PRT printf("[DEBUG] %d : %s\n", __LINE__, __func__);

typedef struct conf {
    Byte mode; // 0: download, 1: upload
    char ip[16];
    int port;
    char* filepath;
    int bcm; // block chiper mode of operation
} CONF;

int init(){
#if DEBUG_TEST
    DEBUG_PRT
#endif // DEBUG_TEST
    Byte *b;
    const Word NUMBER=0x00000042;

    #if DEBUG_TEST
    printf("BEGIN testing endianness...\n");
    printf("Since you are running this, it means that you have defined \
    either LITTLE_ENDIAN or BIG_ENDIAN.  Let's see if you were correct.\n");
    #endif // DEBUG_TEST
    b=(Byte *)(&NUMBER);
    if (b[0]==0x42) {
        #undef BIG_ENDIAN
        #if DEBUG_TEST
        printf("We are on LITTLE_ENDIAN platform.\n");
        printf("Okay.  You were correct.\n");
        #endif //DEBUG_TEST

    } else {
        #undef LITTLE_ENDIAN
        #if DEBUG_TEST
        printf("We are on BIG_ENDIAN platform.\n");
        printf("Okay.  You were correct.\n");
        #endif //DEBUG_TEST
    }
    return 0;
}

void upload_file(char ip[], int port, char filepath[]) {
#if DEBUG_TEST
    DEBUG_PRT
#endif // DEBUG_TEST
    Byte key[KEY] = {0,};
    Byte roundkey[ROUND_KEY];
    Byte plain_block[BLOCK_SIZE], crypt_block[BLOCK_SIZE];
    FILE *file;
    char *structur;
    char state[3] = {0,};
    WSADATA wsaData;
    SOCKET client_socket;

    // send process
    // connect server
DEBUG_PRT
    sock(&client_socket, ip, port); /* socket init */
    // 1. make EncKey
DEBUG_PRT
    EncKeySetup(key, roundkey, 192);
    // 2. open file
DEBUG_PRT
    file = fopen(filepath, "rb");
    // 3. request server state
    structur = (char*)malloc(strlen(filepath)+2);
    strcpy(structur, "u");
    strcat(structur, filepath);
    printf("try connection!\n");
    send(client_socket, structur, strlen(structur), 0);
    recv(client_socket, state, 2, 0);
    if(!strcmp(state, "ok")) printf("connect complite!\n");
    // 4. send file after block encrypt
    while(!strcmp(state, "ok")) {
        // encryption block
        fread(plain_block, 1, BLOCK_SIZE, file);
        Crypt(plain_block, 14, roundkey, crypt_block);
        send(client_socket, plain_block, BLOCK_SIZE, 0);
        recv(client_socket, state, 2, 0);
    }
    free(structur);
    fclose(file);
    closesocket(client_socket);
    printf("%s\n", "done");
}

void download_file(char ip[], int port, char filepath[]) {
#if DEBUG_TEST
    DEBUG_PRT
#endif // DEBUG_TEST
    Byte key[KEY] = {0,};
    Byte roundkey[ROUND_KEY];
    Byte plain_block[BLOCK_SIZE], crypt_block[BLOCK_SIZE];
    char *structur;
    FILE *file;
    SOCKET client_socket;

    // download process
    sock(&client_socket, ip, port); /* socket init */
    // 1. init crypt platform & make DecKey
    init(); /* checking platform for encryption environment */
    DecKeySetup(key, roundkey, 192);
    // 2. open file
    file = fopen(filepath, "wb");
    // 3. request file to server
    structur = (char*)malloc(strlen(filepath)+2);
    strcpy(structur, "d");
    strcat(structur, filepath);
    send(client_socket, structur, strlen(structur), 0);
    // 4. recv encrypted file
    while(recv(client_socket, crypt_block, BLOCK_SIZE, 0)) {
        // decryption block
        Crypt(crypt_block, 14, roundkey, plain_block);
        fwrite(plain_block, 1, BLOCK_SIZE, file);
        send(client_socket, "ok", 2, 0);
    }
    free(structur);
    fclose(file);
    closesocket(client_socket);
    printf("%s\n", "done");
}

void set_conf(CONF* conf, int argc, char *argv[]){
#if DEBUG_TEST
    DEBUG_PRT
#endif // DEBUG_TEST
    // set mode
    if(!strcmp(argv[1], "-dl")) conf->mode = 0; // download mode
    else if(!strcmp(argv[1], "-ul")) conf->mode = 1; // upload mode
    else {
        printf("error! check arrt 2. -dl|-ul is right\n");
    }
    // set ip
    strcpy(conf->ip, strtok(argv[2], ":"));
    // set port
    conf->port = atoi(strtok(NULL, ":"));
    // set filepath
    strcpy(conf->filepath, argv[3]);

    // set block chiper mode of operation
    if(argc > 4) {
        if(!strcmp(argv[4], "-ecb")) conf->bcm = 0; // ecb mode
        else if(!strcmp(argv[4], "-ctr")) conf->bcm = 1; // ctr mode
        else conf->bcm = 0;
    } else conf->bcm = 0;
}

int main(int argc, char *argv[]){ // ex) prog (-dl|-ul) 127.0.0.1:9001 eisle.pdf [-e (ecb|ctr)]
#if DEBUG_TEST
    DEBUG_PRT
#endif // DEBUG_TEST
    // initialization
    CONF conf;

    // check input data
    if(argc < 4){
        printf("need more input data!\n");
        return 1;
    }

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
