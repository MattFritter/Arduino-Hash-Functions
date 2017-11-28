/* SDBM
 * A 32 bit implementation of the SDBM hashing algorithm for the Arduino microcontroller. Originally designed for SDBM, an open 
 * source clone of the NDBM database library, this algorithm was proposed by Ozan Yigit (York) and Margo Seltzer (UC Berkeley) 
 * in 1991 and is based off an earlier algorithm created by Per-Ake Larson in 1988.
 * Output is in the form of an 8 character hexadecimal string.
 * Author: Matthew Fritter
 */

/* Begin Serial Monitor Connection */
void setup(){
  Serial.begin(9600);
}

/* Calculate the 32 bit SDBM hash for an input message of length arraySize
 * Hash is a running sum of the current message byte, the last hash iteration
 * shifted left 6, the last hash iteration shifted right 16, and -hash. The
 * resulting 32 bit integer is split into two 16 bit integers and then returned
 * as a String in hexadecimal format.
 */

String sdbm(byte* message, size_t arraySize){
  String output = "";
  char buffer[9];
  uint32_t hash = 0;
  for(int i = 0; i < arraySize; i++){
    hash = message[i] + (hash << 6) + (hash >> 16) - hash;
  }
  sprintf(buffer, "%lux", hash);
  //uint16_t sdbm1 = hash >> 16;
  //uint16_t sdbm2 = hash & 0xFFFF;
  //sprintf(buffer, "%04X", sdbm1);
  //sprintf(buffer+4, "%04X", sdbm2);
  output.concat(buffer);
  return output;
}

/* For a given test string, print the 32 bit SDBM hash over Serial
 */
void loop(){
  byte testString[] = "Hello World";
  Serial.println(sdbm(testString, sizeof(testString) - 1));
}
