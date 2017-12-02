/* FNV-1a
 * An implementation of the Fowler–Noll–Vo FNV-1a Hashing algorithm for the Arduino microcontroller that produces a 32 bit output hash
 * Using the recommended prime of 0x1000193 and the recommended offset of 0x811C9DC5.
 * Output is in the form a uint32_t unsigned 32 bit integer hash
 * Author: Matthew Fritter
 */

/* Begin Serial Monitor Connection */
void setup(){
  Serial.begin(9600);
}

/* Perform FNV-1a hashing for a given input message byte array of size arraySize.
 * Value of the hash is first set to the offset value, then for each byte in the
 * message the hash is XORed with the message byte and then multiplied by the FNV
 * prime. Once all message bytes have been XORed, a 32 bit unsigned integer hash
 * is returned.
 */
uint32_t hashFNV(byte* message, int arraySize){
  const uint32_t prime = 0x1000193;
  uint32_t hash = 0x811C9DC5;
  for(int i = 0; i < arraySize; i++){
    hash ^= message[i];
    hash *= prime;
  }
  return hash;
}

/* For a given test string, print the 32 bit FNV-1a hash over Serial.
 * The test byte array "foobar" should return 0xBF9CF968 if the FNV-1a
 * algorithm is properly implemented with the recommended prime and
 * offset values.
 */
void loop(){
  byte testString[] = "foobar";
  Serial.println(hashFNV(testString, sizeof(testString) - 1));
}

