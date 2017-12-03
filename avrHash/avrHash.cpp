/* avrHash - a library of small hash functions for use with AVR micro controllers
 * Author: Matthew Fritter, m.fritter@alumni.ubc.ca
 * GNU GPLv3 License 2017
 */

#include "avrHash.h"
#include <string.h>
#include <avr/pgmspace.h>
#include <Arduino.h>

#define rot(x,k) (((x)<<(k)) | ((x)>>(32-(k))))
#define rot2(x, c) ((x << c) | ( x >> (32 - c)))

const uint32_t sine[] PROGMEM = {0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501, 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, 0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8, 0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a, 0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, 0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665, 0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};
const uint8_t offset[] PROGMEM = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};   
  

pearson::pearson(){}

uint32_t pearson::hash(uint8_t* message, uint8_t* permutationTable, size_t arraySize){
  uint32_t _hash = 0;
  for(int i = 0; i < 4; i++){
    uint8_t round = permutationTable[(message[0] + i) % 256];
    for(int j = 0; j < arraySize; j++){
      round = permutationTable[(round ^ message[j]) % 256];
    }
    _hash |= ((uint32_t)round << (i << 3));
  }
  return _hash;
}

crc32::crc32(){}

uint32_t crc32::hash(uint8_t* message, size_t arraySize){
  uint32_t _hash = 0xFFFFFF;
  for(int i = 0; i < arraySize; i++){
    _hash = (_hash >> 8) ^ computeCRC((_hash & 0xff) ^ message[i]);
  }
  return _hash;
}

uint32_t crc32::computeCRC(int i){
	uint32_t remainder = i;
	for (int j = 0; j < 8; j++){
		if (remainder & 1){
			remainder = 0xEDB88320 ^ (remainder >> 1);
		}else{
			remainder >>= 1;
		}
	}
	return remainder;
}

sdbm::sdbm(){}

uint32_t sdbm::hash(uint8_t* message, size_t arraySize){
  uint32_t _hash = 0;
  for(int i = 0; i < arraySize; i++){
    _hash = message[i] + (_hash << 6) + (_hash >> 16) - _hash;
  }
  return _hash;
}

fnv1a::fnv1a(){}

uint32_t fnv1a::hash(uint8_t* message, size_t arraySize){
  uint32_t _hash = 0x811C9DC5;
  static const uint32_t prime = 0x1000193;
  for(int i = 0; i < arraySize; i++){
    _hash ^= message[i];
    _hash *= prime;
  }
  return _hash;
}

jenkins::jenkins(){}

uint32_t jenkins::hash(uint8_t* message, size_t arraySize){
  uint32_t _hash = 0;
  for(int i = 0; i < arraySize; i++){
    _hash += message[i];
    _hash += _hash << 10;
    _hash ^= _hash >> 6;
  }
  _hash += _hash << 3;
  _hash ^= _hash >> 11;
  _hash += _hash << 15;
  return _hash;
}

lookup3::lookup3(){}

void lookup3::mix(uint32_t *a, uint32_t *b, uint32_t *c){
  *a -= *c; *a ^= rot(*c,4); *c += *b;
  *b -= *a; *b ^= rot(*a,6); *a += *c;
  *c -= *b; *c ^= rot(*b,8); *b += *a;
  *a -= *c; *a ^= rot(*c,16); *c += *b;
  *b -= *a; *b ^= rot(*a,19); *a += *c;
  *c -= *b; *c ^= rot(*b,4); *b += *a;
}

uint32_t lookup3::hash(uint8_t* message, size_t arraySize, uint32_t seed){
  uint32_t a,b,c;
  a = b = c = 0xDEADBEEF + ((uint32_t)arraySize) + seed;
  if (((message[arraySize] & 0x3) == 0)){
    const uint32_t* k = (const uint32_t*)message;
    while (arraySize > 12){
      a += k[0];
      b += k[1];
      c += k[2];
      mix(&a,&b,&c);
      arraySize -= 12;
      k += 3;
    }
    switch(arraySize){
      case 12: c+=k[2]; b+=k[1]; a+=k[0]; break;
      case 11: c+=k[2]&0xffffff; b+=k[1]; a+=k[0]; break;
      case 10: c+=k[2]&0xffff; b+=k[1]; a+=k[0]; break;
      case 9 : c+=k[2]&0xff; b+=k[1]; a+=k[0]; break;
      case 8 : b+=k[1]; a+=k[0]; break;
      case 7 : b+=k[1]&0xffffff; a+=k[0]; break;
      case 6 : b+=k[1]&0xffff; a+=k[0]; break;
      case 5 : b+=k[1]&0xff; a+=k[0]; break;
      case 4 : a+=k[0]; break;
      case 3 : a+=k[0]&0xffffff; break;
      case 2 : a+=k[0]&0xffff; break;
      case 1 : a+=k[0]&0xff; break;
      case 0 : return c;
    }
  } 
  else if (((message[arraySize] & 0x1) == 0)){
    const uint16_t* k = (const uint16_t *)message;
    const uint8_t* k8;
    while (arraySize > 12){
      a += k[0] + (((uint32_t)k[1])<<16);
      b += k[2] + (((uint32_t)k[3])<<16);
      c += k[4] + (((uint32_t)k[5])<<16);
      mix(&a,&b,&c);
      arraySize -= 12;
      k += 6;
    }
    k8 = (const uint8_t*)k;
    switch(arraySize){
      case 12: c+=k[4]+(((uint32_t)k[5])<<16);
               b+=k[2]+(((uint32_t)k[3])<<16);
               a+=k[0]+(((uint32_t)k[1])<<16);
               break;
      case 11: c+=((uint32_t)k8[10])<<16;
      case 10: c+=k[4];
               b+=k[2]+(((uint32_t)k[3])<<16);
               a+=k[0]+(((uint32_t)k[1])<<16);
               break;
      case 9 : c+=k8[8];
      case 8 : b+=k[2]+(((uint32_t)k[3])<<16);
               a+=k[0]+(((uint32_t)k[1])<<16);
               break;
      case 7 : b+=((uint32_t)k8[6])<<16;
      case 6 : b+=k[2];
               a+=k[0]+(((uint32_t)k[1])<<16);
               break;
      case 5 : b+=k8[4];
      case 4 : a+=k[0]+(((uint32_t)k[1])<<16);
               break;
      case 3 : a+=((uint32_t)k8[2])<<16;
      case 2 : a+=k[0];
               break;
      case 1 : a+=k8[0];
               break;
      case 0 : return c;
    }
  }
  else {
    const uint8_t* k = (const uint8_t*)message;
    while (arraySize > 12){
      a += k[0];
      a += ((uint32_t)k[1])<<8;
      a += ((uint32_t)k[2])<<16;
      a += ((uint32_t)k[3])<<24;
      b += k[4];
      b += ((uint32_t)k[5])<<8;
      b += ((uint32_t)k[6])<<16;
      b += ((uint32_t)k[7])<<24;
      c += k[8];
      c += ((uint32_t)k[9])<<8;
      c += ((uint32_t)k[10])<<16;
      c += ((uint32_t)k[11])<<24;
      mix(&a,&b,&c);
      arraySize -= 12;
      k += 12;
    }
    switch(arraySize){
      case 12: c+=((uint32_t)k[11])<<24;
      case 11: c+=((uint32_t)k[10])<<16;
      case 10: c+=((uint32_t)k[9])<<8;
      case 9 : c+=k[8];
      case 8 : b+=((uint32_t)k[7])<<24;
      case 7 : b+=((uint32_t)k[6])<<16;
      case 6 : b+=((uint32_t)k[5])<<8;
      case 5 : b+=k[4];
      case 4 : a+=((uint32_t)k[3])<<24;
      case 3 : a+=((uint32_t)k[2])<<16;
      case 2 : a+=((uint32_t)k[1])<<8;
      case 1 : a+=k[0];
               break;
      case 0 : return c;
    }
  }
  c ^= b; c -= rot(b,14);
  a ^= c; a -= rot(c,11);
  b ^= a; b -= rot(a,25);
  c ^= b; c -= rot(b,16);
  a ^= c; a -= rot(c,4);
  b ^= a; b -= rot(a,14);
  c ^= b; c -= rot(b,24);
  return c;
}

md5::md5(){}

void md5::hash(uint8_t* message, size_t arraySize, uint32_t* outArray) {
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
      B = B + rot2(F, pgm_read_byte_near(offset + i));
    }
    outArray[0] += A;
    outArray[1] += B;
    outArray[2] += C;
    outArray[3] += D;
    len -= 64;
  }
}