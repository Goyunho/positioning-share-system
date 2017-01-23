#include <stdio.h>
#include "chiper.h" // aria encryption library

#define ENC_SIZE 128


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

int main(){
  // initialization
  Byte roundkey[16*17], crypt_value[16], key[32];
  int i;

  Byte plain_text[16] = {0};

  init(); /* checking platform */

  for (i=0; i<16; i++)
    key[i]=i*0x11;
  for (i=16; i<24; i++)
    key[i]=(i-16)*0x11;

  // encryption
  Crypt(plain_text, EncKeySetup(key, roundkey, 192), roundkey, crypt_value);
  printf("BEGIN testing basic encryption...\n");
  printf("key      : "); printBlockOfLength(key, 24); printf("\n");
  printf("plaintext: "); printBlock(plain_text); printf("\n");
  printf("result is: "); printBlock(c); printf("\n\n");


  printf("BEGIN testing the roundtrip...\n");
  EncKeySetup(key, roundkey, 192);
  printf("plaintext : "); printBlock(plain_text); printf("\n");
  Crypt(plain_text, 14, roundkey, crypt_value);
  printf("ciphertext: "); printBlock(crypt_value); printf("\n");
  DecKeySetup(key, roundkey, 192);
  Crypt(crypt_value, 14, roundkey, plain_text);
  printf("decrypted : "); printBlock(plain_text); printf("\n");

  return 0;
}
