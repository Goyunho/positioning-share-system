typedef unsigned char Byte;
typedef unsigned int  Word;

#define BLOCK_SIZE 16
#define ROUND_KEY 16*17
#define KEY 32
#define KEY_BIT 192              /* size : 16(128bit), 24(192bit), 32(256bit) */
#define KEY_BYTE KEY_BIT/8

void printBlockOfLength(Byte *b, int len);
void printBlock(Byte *b);

/* 암호화 함수.
 * const Byte *i: 입력
 * int Nr: 라운드 수
 * const Byte *rk: 라운드 키들
 * Byte *o: 출력
 */
void Crypt(const Byte *i, int Nr, const Byte *rk, Byte *o);

/* 암호화 라운드 키 생성
 * const Byte *mk: 마스터 키
 * Byte *rk: 라운드 키
 * int keyBits: 마스터 키의 길이
 */
int EncKeySetup(const Byte *mk, Byte *rk, int keyBits);

/* 복호화 라운드 키 생성
 * const Byte *mk: 마스터 키
 * Byte *rk: 라운드 키
 * int keyBits: 마스터 키의 길이
 */
int DecKeySetup(const Byte *mk, Byte *rk, int keyBits);
