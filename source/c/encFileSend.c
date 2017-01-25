#include <stdio.h>
#include <string.h>
#include "chiper.h" // aria encryption library
#include "sockcomm.h"

#define PATH_SIZE 255

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

void send_struct(SOCKET client_socket, Byte sign, int ID, char filename[PATH_SIZE]){
    int i;
    Byte info[1+4+256] = {0};
    char* state; // 0: datastruct error!

    *(info) = sign;
    *(info+1) = ID;
    for(i=0; i!=0; i++){
        *(info+1+4+i) = filename[i];
    }

    send(client_socket, info, sizeof(info), 0);
    recv(client_socket, state, 1, 0);
    if(*state == '0') {
        return;
    }
    else if(*state == '1'){
        printf("datastruct error!\n");
        getchar();
        exit(0);
    }
}

void save_file(SOCKET client_socket, Byte key[KEY], char file_path[PATH_SIZE]){
    Byte roundkey[ROUND_KEY];
    Byte plain_text[BLOCK_SIZE], crypt_value[BLOCK_SIZE];
    char recv_sign[10] = {0};
    FILE* file;
    char* i;

    send_struct(client_socket, '0', 1, file_path);
    EncKeySetup(key, roundkey, 192);
    file = fopen(file_path, "rb");
    /*for(i=fgets(plain_text, BLOCK_SIZE, (FILE*)file) ; i != NULL ; i=fgets(plain_text, BLOCK_SIZE, (FILE*)file)){
        printf("plaintext : "); printBlock(plain_text); printf("\n");
        Crypt(plain_text, 14, roundkey, crypt_value);
        printf("ciphertext: "); printBlock(crypt_value); printf("\n");
        send(client_socket, crypt_value, BLOCK_SIZE, 0);
        recv(client_socket, recv_sign, 10, 0);
        printf("recv : %s", recv_sign);
        if(recv_sign == NULL){
            exit(0);
        }
    }*/
}

void load_file(SOCKET client_socket, Byte key[KEY], char filename[PATH_SIZE]){
    Byte roundkey[ROUND_KEY];
    Byte plain_text[BLOCK_SIZE], crypt_value[BLOCK_SIZE];
    FILE* file;

    send_struct(client_socket, 0x01, 0x01, filename);
    file = fopen(strcat("./download/", filename), "wb");
    DecKeySetup(key, roundkey, 192);
    for(;;){
        //send(client_socket, "");
        Crypt(crypt_value, 14, roundkey, plain_text);
    }
    printf("decrypted : "); printBlock(plain_text); printf("\n");
}

int main(){
    // initialization
    SOCKET client_socket;

    init(); /* checking platform */
    client_socket = sock("127.0.0.1", 9001); /* socket init */

    save_file(client_socket, "hi, hello", ".\\doc\\ARIA-testvector.pdf");
    //load_file(client_socket, "hi, hello", "ARIA-testvector.pdf");

    getchar();

    return 0;
}
