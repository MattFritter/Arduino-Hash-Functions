/* CRC-32
 * An implementation of the Cyclic Redundancy Check (CRC) algorithm for the Arduino microcontroller that produces a 32 bit output checksum
 * This implementation is designed to comply with RFC 1952, producing the same checksum output as the implementations used for
 * PNG (Portable Network Graphics) and the PKZIP/GZIP utilities.
 * Output is in the form an 8 character hexidecimal string
 * Author: Matthew Fritter
 */

/* Begin Serial Monitor Connection, precompute the lookupTable */
void setup(){
  Serial.begin(9600);
}

/* Computes the CRC value for a given input integer
 */
uint32_t computeCRC(int i){
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

/* Calculate the 32 bit CRC checksum for an input message of length arraySize using the lookupTable
 * Take an input crc value (use 0 for default), invert, shift right 8 bits, and XOR with
 * the table lookup value of the last byte of the CRC XORed with the current byte of the message
 * Inverts the final result, performs some black magic to split the uint32 into two uint16s, and
 * then returns an 8 character hexadecimal representative string
 */

String crc32(uint32_t crc, byte* message, size_t arraySize){
  String output = "";
  char buffer[9];
  crc = ~crc;
  for(int i = 0; i < arraySize; i++){
    crc = (crc >> 8) ^ computeCRC((crc & 0xff) ^ message[i]);
  }
  uint16_t crc1 = ~crc >> 16;
  uint16_t crc2 = ~crc & 0xFFFF;
  sprintf(buffer, "%04X", crc1);
  sprintf(buffer+4, "%04X", crc2);
  output.concat(buffer);
  return output;
}

/* For a given test string, print the 32 bit CRC checksum over Serial
 * Note: This implementation should be compliant with RFC 1952, ISO 3309,
 * and thus GZIP/PKZIP and PNG. If implemented correctly, the input byte array
 * "The quick brown fox jumps over the lazy dog" should return a checksum of
 * 0x414FA339
 */
void loop(){
  byte testString[] = "The quick brown fox jumps over the lazy dog";
  Serial.println(crc32(0, testString, sizeof(testString) - 1));
}