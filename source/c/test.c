#include "chiper.h" // aria encryption library

int main(){
  Byte rk[16*17], c[16], *b, mk[32];
  int i, flag;
  const Word NUMBER=0x00000042;
  Byte p[16]={0x11, 0x11, 0x11, 0x11, 0xaa, 0xaa, 0xaa, 0xaa,
	      0x11, 0x11, 0x11, 0x11, 0xbb, 0xbb, 0xbb, 0xbb};
  const Byte cryptResult[] = {
    0x8d, 0x14, 0x70, 0x62, 0x5f, 0x59, 0xeb, 0xac,
    0xb0, 0xe5, 0x5b, 0x53, 0x4b, 0x3e, 0x46, 0x2b};

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

  for (i=0; i<16; i++)
    mk[i]=i*0x11;
  for (i=16; i<24; i++)
    mk[i]=(i-16)*0x11;

  Crypt(p, EncKeySetup(mk, rk, 192), rk, c);
  printf("BEGIN testing basic encryption...\n");
  printf("Testing whether the encryption would come out correctly, \
for 14-round ARIA.\n");
  printf("key      : "); printBlockOfLength(mk, 24); printf("\n");
  printf("plaintext: "); printBlock(p); printf("\n");
  printf("result is: "); printBlock(c); printf("\n");
  printf("should be: "); printBlock((Byte *)cryptResult); printf("\n");
  flag=0;
  for (i=0; i<16; i++)
    if (c[i]!=cryptResult[i])
      flag=1;
  if (flag==1)
    printf("The result is incorrect!\n");
  else
    printf("Okay.  The result is correct.\n");
  printf("END   testing basic encryption.\n\n");

  for (i=0; i<32; i++)
    mk[i]=0;

  for (i=0; i<16; i++)
    p[i]=0;

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
  flag=0;
  for (i=0; i<16; i++)
    if (p[i]!=0)
      flag=1;
  if (flag==1)
    printf("The result is incorrect!\n");
  else
    printf("Okay.  The result is correct.\n");
  printf("END   testing the roundtrip.\n");
  return 0;
}
