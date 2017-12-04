#include <avrHash.h>

//pearson hashFunc;
//jenkins hashFunc;
//fnv1a hashFunc;
crc32 hashFunc;
//sdbm hashFunc;
//lookup3 hashFunc;
//md5 hashFunc;

int sampleSize = 1000;
int maxLength = 256;


void setup() {
  Serial.begin(9600);
  uint8_t* bytes = (uint8_t*)calloc(maxLength, sizeof(uint8_t));
  uint32_t outArray[4] = {0};
  for(int i = 1; i <= maxLength; i++){
     long InitTime = micros();
     uint32_t val;
     for(int j = 0; j < sampleSize; j++){
         val = hashFunc.hash(0, bytes, i);
     }
     long totalTime = micros() - InitTime;
     Serial.print(i);
     Serial.print(", ");
     Serial.print(totalTime);
     Serial.print(", ");
     Serial.println(val);
     //char out[32];
     //uint8_t *hexb;
     //hexb = (uint8_t *)&outArray[0];
     //sprintf(out, "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X", hexb[0], hexb[1], hexb[2], hexb[3], hexb[4], hexb[5], hexb[6], hexb[7], hexb[8], hexb[9], hexb[10], hexb[11], hexb[12], hexb[13], hexb[14], hexb[15]);
     //Serial.println(out);
  }
  Serial.println("Done");
}

void loop() {
  
}
