#include <stdio.h>
#include <string.h>
#include "chiper.h" // aria encryption library
#include "sockcomm.h"

#define DEBUG_TEST 1
#define DEBUG_PRT printf("[DEBUG] %d : %s\n", __LINE__, __func__);

int main(int argc, char *argv[]){ // ex) prog (-dl|-ul) 127.0.0.1:9001 eisle.pdf [-e (ecb|ctr)]

    SOCKET socket;
    char test[20];
    sock(&socket, "", 9001);
DEBUG_PRT
    send(socket, "test", 5, 0);
DEBUG_PRT
    recv(socket, test, 20, 0);
DEBUG_PRT
    printf("%s\n", test);

    getchar();
    return 0;
}
