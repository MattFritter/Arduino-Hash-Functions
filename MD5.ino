/* MD5
 * An implementation of the MD5 hash algorithm for the Arduino microcontroller that
 * produces a 128 bit hash output. This algorithm is Little-Endian. Note that due to
 * memory constraints of the Arduino microcontroller, maximum input length is limited,
 * and long inputs may cause crashes.
 * Output is in the form of an array of four 32 bit unsigned integers, for a total of
 * 128 bits.
 * Author: Matthew Fritter
 */
/* Include the ProgMem library */
#include <avr/pgmspace.h>

/* Constants (round offsets and constants), stored in the Program Memory to save dynamic memory */
const uint32_t sine[] PROGMEM = {0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501, 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, 0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8, 0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a, 0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, 0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665, 0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};
const byte offset[] PROGMEM = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};   

/* Define the rotation function */
#define rot(x, c) ((x << c) | ( x >> (32 - c)))

/* Perform MD5 hashing on a given input message byte array of size arraySize. Input message
 * is padded to multiple of 512 bits in length in accordance to the MD5 specifications
 * (specifically, append a 1, and then pad with 0 until you are 64 bits away from the next
 * multiple of 512, and then append the original message size in bits as a 64 bit little
 * endian number. A 512 bit block is mixed through 4 rounds of bit operations with 16 iterations per
 * round. Subsequent 512 bit blocks use the previous block's final output as an initial value.
 * Final output is in the form of four unsigned 32 bit integers, for a total of 128 bits. These are
 * placed in a uint32_t array of size four, initialized and passed to the function by the caller.
 */
void hashMD5(uint8_t* message, size_t arraySize, uint32_t* outArray) {
  uint32_t len = (arraySize << 3) + 1;
  uint32_t sizeBit = len - 1;
  if((len % 512) > 448){
    len += 512 - (len % 512) + 448;
  }else if((len % 512) < 448){
    len += 448 - (len % 512);
  }
  len = (len + 64) >> 3;
  uint8_t newArr[len];
  memset(newArr, 0, len);
  memcpy(newArr, message, arraySize);
  newArr[arraySize] = 128;
  memcpy(newArr + (len-8), &sizeBit, 4);
  outArray[0] = 0x67452301;
  outArray[1] = 0xefcdab89;
  outArray[2] = 0x98badcfe;
  outArray[3] = 0x10325476;
  while(len != 0){
    uint32_t *block = (uint32_t *) (newArr + ((sizeof(newArr)) - len));
    uint32_t A = outArray[0];
    uint32_t B = outArray[1];
    uint32_t C = outArray[2];
    uint32_t D = outArray[3];
    for(int i = 0; i < 64; i++){
      uint32_t F, g;
      if((0 <= i) && (i <= 15)){
        F = (B & C) | ((~B) & D);
        g = i;
      }else if((16 <= i) && (i <= 31)){
        F = (D & B) | ((~D) & C);
        g = (5 * i + 1) % 16;
      }else if((32 <= i) && (i <= 47)){
        F = B ^ C ^ D;
        g = (3 * i + 5) % 16;
      }else if((48 <= i) && (i <= 63)){
        F = C ^ (B | (~D));
        g = (7 * i) % 16;
      }
      F = F + A + pgm_read_dword(sine + i) + block[g];
      A = D;
      D = C;
      C = B;
      B = B + rot(F, pgm_read_byte_near(offset + i));
    }
    outArray[0] += A;
    outArray[1] += B;
    outArray[2] += C;
    outArray[3] += D;
    len -= 64;
  }
}

/* Begin Serial Monitor Connection */
void setup(){
  Serial.begin(9600);
}

/* For a given test string, print the 128 bit MD5 hash over Serial. The
 * test byte array "The quick brown fox jumps over the lazy dog" should
 * return 0x9E107D9D372BB6826BD81D3542A419D6 if the MD5 algorithm is
 * properly implemented.
 */
void loop(){
  uint8_t message[] = "The quick brown fox jumps over the lazy dog";
  size_t arraySize = sizeof(message)-1;
  uint32_t outArray[4] = {0};
  hashMD5(message, arraySize, outArray);
  char out[32];
  uint8_t *hexb;
  hexb = (uint8_t *)&outArray[0];
  sprintf(out, "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X", hexb[0], hexb[1], hexb[2], hexb[3], hexb[4], hexb[5], hexb[6], hexb[7], hexb[8], hexb[9], hexb[10], hexb[11], hexb[12], hexb[13], hexb[14], hexb[15]);
  Serial.println(out);
}