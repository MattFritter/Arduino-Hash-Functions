/* Lookup3
 * An implementation of the Lookup3 hash algorithm by Bob Jenkins that produces a 32 bit output hash
 * This is a cut down version of the full Lookup3 implementation released by Jenkins, removing the big-endian
 * functions, tests, and other functions not necessary for generating 32 bit hashes
 * Output is in the form a 32 bit unsigned integer
 * Author: Matthew Fritter
 */

/* Define the rotation macro */
#define rot(x,k) (((x)<<(k)) | ((x)>>(32-(k))))

/* Defining the mix function, which mixes three 32 bit unsigned integers */
void mix(uint32_t *a, uint32_t *b, uint32_t *c){
  *a -= *c; *a ^= rot(*c,4); *c += *b;
  *b -= *a; *b ^= rot(*a,6); *a += *c;
  *c -= *b; *c ^= rot(*b,8); *b += *a;
  *a -= *c; *a ^= rot(*c,16); *c += *b;
  *b -= *a; *b ^= rot(*a,19); *a += *c;
  *c -= *b; *c ^= rot(*b,4); *b += *a;
}

/* Create the Lookup3 hash for an input message byte array of size arraySize, seeded
 * with an unsigned 32 bit integer. It processes 12 bytes at a time using three uint32_t
 * values reading in chunks of 32, 16, or 8 bits. Initial value for these is 0xDEADBEEF
 * plus the size of the input message and the seed. The values are combined with four bytes
 * of message data, and then are mixed. This is iterated over until all bytes from the message
 * data have been read and mixed. Returns a uint32_t unsigned 32 bit integer hash.
 */
uint32_t lookup3(byte* message, size_t arraySize, uint32_t seed)
{
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

/* Begin Serial Monitor Connection */
void setup(){
  Serial.begin(9600);
}

/* For a given test string, print the 32 bit Lookup3 hash over Serial */
void loop()
{
  uint32_t c = lookup3("Four score and seven years ago", 30, 1);
  Serial.println(c);
}
