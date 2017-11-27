/* Pearson Hash
 * An implementation of the Pearson Hashing algorithm for the Arduino microcontroller that produces a 32 bit output hash
 * Output is in the form an 8 character hexidecimal string
 * Author: Matthew Fritter
 */

/* Permutation table is values 0 - 255 randomly distributed throughout the array */
byte permutationTable[256] = {15, 34, 223, 6, 104, 9, 202, 46, 194, 236, 95, 169, 120, 227, 92, 144, 123, 167, 30, 42, 68, 60, 131, 65, 79, 239, 160, 174, 99, 204, 52, 230, 96, 186, 219, 244, 91, 197, 53, 64, 43, 80, 127, 84, 22, 155, 56, 106, 249, 39, 133, 195, 82, 78, 21, 61, 86, 59, 158, 218, 107, 3, 211, 90, 44, 137, 26, 17, 112, 81, 253, 132, 38, 66, 1, 168, 213, 190, 89, 252, 182, 150, 183, 20, 243, 49, 153, 13, 178, 149, 118, 31, 181, 134, 35, 206, 245, 138, 141, 157, 24, 108, 93, 83, 212, 187, 226, 94, 247, 45, 238, 2, 171, 100, 32, 196, 240, 41, 214, 110, 161, 0, 216, 117, 33, 231, 184, 19, 57, 76, 246, 130, 85, 8, 29, 241, 176, 126, 10, 248, 63, 75, 159, 201, 203, 119, 27, 36, 48, 228, 135, 70, 165, 225, 148, 180, 145, 232, 172, 101, 124, 237, 209, 221, 62, 67, 166, 88, 193, 128, 28, 18, 156, 179, 5, 116, 14, 109, 177, 139, 11, 235, 188, 98, 224, 113, 191, 242, 4, 71, 136, 233, 40, 125, 192, 69, 74, 23, 55, 208, 205, 97, 163, 114, 220, 229, 105, 162, 254, 198, 217, 143, 147, 189, 255, 210, 175, 51, 12, 77, 170, 37, 199, 164, 103, 122, 102, 185, 207, 146, 54, 121, 50, 142, 129, 111, 58, 222, 152, 154, 215, 16, 173, 73, 250, 200, 25, 234, 151, 87, 115, 47, 72, 7, 140, 251}; 

/* Begin Serial Monitor Connection */
void setup(){
  Serial.begin(9600);
}

/* Perform Pearson hashing on an input byte array representing the message, with a given size arraySize, using the permutation table
 * Iterates 4 times to produce a 32 bit (4 byte) hash value, with the hash for each iteration changed via seeding with the iterator
 * Returns an 8 character String of the hash in hexadecimal format
 */
String hashPearson(byte* message, byte* permutationTable, int arraySize){
  String output = "";
  char buffer[2];
  for(int i = 0; i < 4; i++){
    byte hash = permutationTable[(message[0] + i) % 256];
    for(int j = 0; j <= arraySize; j++){
      hash = permutationTable[(hash ^ message[j]) % 256];
    }
    sprintf(buffer, "%02X", hash);
    output.concat(buffer);
  }
  return output;
}

/* For a given test string, print the 32 bit Pearson hash over Serial */
void loop(){
  byte testString[] = "Hello World";
  Serial.println(hashPearson(testString, permutationTable, sizeof(testString)));
}

