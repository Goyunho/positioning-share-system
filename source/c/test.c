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
    printf("We are on LITTLE_ENDIAN platform.\n");
#ifdef BIG_ENDIAN
    printf("WARNING: BIG_ENDIAN defined on LITTLE_ENDIAN platform.\n");
    printf("         You should define LITTLE_ENDIAN instead of BIG_ENDIAN.\n");
#else
    printf("Okay.  You were correct.\n");
#endif
  } else {
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
  Byte rk[16*17], c[16], mk[32];
  int i;

  Byte p[16] = {0};

  init(NUMBER); /* checking platform */

  for (i=0; i<16; i++)
    mk[i]=i*0x11;
  for (i=16; i<24; i++)
    mk[i]=(i-16)*0x11;

  // encryption
  Crypt(p, EncKeySetup(mk, rk, 192), rk, c);
  printf("BEGIN testing basic encryption...\n");
  printf("Testing whether the encryption would come out correctly, \
for 14-round ARIA.\n");
  printf("key      : "); printBlockOfLength(mk, 24); printf("\n");
  printf("plaintext: "); printBlock(p); printf("\n");
  printf("result is: "); printBlock(c); printf("\n");
  printf("should be: "); printBlock((Byte *)cryptResult); printf("\n");


  printf("BEGIN testing the roundtrip...\n");
  printf("For key size of 192 bits, starting with \
the zero plaintext and the zero key, let's see if \
we may recover the plaintext by decrypting the \
encrypted ciphertext.\n");
  EncKeySetup(mk, rk, 192);
  printf("plaintext : "); printBlock(p); printf("\n");
  Crypt(p, 14, rk,c);
  printf("ciphertext: "); printBlock(c); printf("\n");
  DecKeySetup(mk, rk, 192);
  Crypt(c, 14, rk, p);
  printf("decrypted : "); printBlock(p); printf("\n");

  return 0;
}
