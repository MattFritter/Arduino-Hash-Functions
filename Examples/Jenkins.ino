/* Jenkins One At a Time
 * An implementation of the Jenkins One-At-A-Time (JOAAT) algorithm for the Arduino microcontroller that 
 * produces a 32 bit output hash. Based on code from Bob Jenkins.
 * Output is in the form a uint32_t unsigned 32 bit integer hash
 * Author: Matthew Fritter
 */

/* Begin Serial Monitor Connection */
void setup(){
  Serial.begin(9600);
}

/* Perform Jenkins One-At-A-Time hashing. Initial hash value is zero, and each iteration the hash is combined
 * with a byte from the message, and recombined with the hash shifted left 10, before being XORed with the hash 
 * shifted right 6. After all bytes of the message have been handled, the hash goes through a final combination
 * with the hash shifted left 3, XORed with the hash shifted right 11, and then combined once more with the hash
 * shifted left 15. The result is a uint32 unsigned 32 bit integer hash.
 */
uint32_t hashJenkins(byte* message, int arraySize){
  uint32_t hash = 0;
  for(int i = 0; i < arraySize; i++){
    hash += message[i];
    hash += hash << 10;
    hash ^= hash >> 6;
  }
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;
  return hash;
}

/* For a given test string, print the 32 bit JOAAT hash over Serial.
 */
void loop(){
  byte testString[] = "Hello World";
  Serial.println(hashJenkins(testString, sizeof(testString) - 1));
}

