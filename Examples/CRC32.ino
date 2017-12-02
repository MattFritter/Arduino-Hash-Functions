/* CRC-32
 * An implementation of the Cyclic Redundancy Check (CRC) algorithm for the Arduino microcontroller that produces a 32 bit output checksum
 * This implementation is designed to comply with RFC 1952, producing the same checksum output as the implementations used for
 * PNG (Portable Network Graphics) and the PKZIP/GZIP utilities.
 * Output is in the form an 8 character hexadecimal string
 * Author: Matthew Fritter
 */

/* Lookup table stores 256 unsigned 32 bit integers, for holding precomputed CRC checksums for 1 byte values */
uint32_t lookupTable[256];

/* Begin Serial Monitor Connection, precompute the lookupTable */
void setup(){
  Serial.begin(9600);
  buildTable(lookupTable);
}

/* Precomputes a table of all possible CRCs for 8-bit messages (256 possible values)
 * Uses 0xEDB88320 as the polynomial constant
 */
void buildTable(uint32_t* table){
  uint32_t remainder = 0;
  for (int i = 0; i < 256; i++){
      remainder = i;
      for (int j = 0; j < 8; j++){
        if (remainder & 1){
          remainder = 0xEDB88320 ^ (remainder >> 1);
        }else{
          remainder >>= 1;
        }
      }
      table[i] = remainder;
    }
}

/* Calculate the 32 bit CRC checksum for an input message of length arraySize using the lookupTable
 * Take an input crc value (use 0 for default), invert, shift right 8 bits, and XOR with
 * the table lookup value of the last byte of the CRC XORed with the current byte of the message
 * Inverts the final result, performs some black magic to split the uint32 into two uint16s, and
 * then returns an 8 character hexadecimal representative string
 */

String crc32(uint32_t crc, uint32_t* table, byte* message, size_t arraySize){
  String output = "";
  char buffer[9];
  crc = ~crc;
  for(int i = 0; i < arraySize; i++){
    crc = (crc >> 8) ^ table[(crc & 0xff) ^ message[i]];
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
  Serial.println(crc32(0, lookupTable, testString, sizeof(testString) - 1));
}