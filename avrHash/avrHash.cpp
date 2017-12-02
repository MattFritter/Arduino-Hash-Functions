/* avrHash - a library of small hash functions for use with AVR micro controllers
 * Author: Matthew Fritter, m.fritter@alumni.ubc.ca
 * GNU GPLv3 License 2017
 */

#include "avrHash.h"

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

uint32_t crc32::hash(uint32_t* table, uint8_t* message, size_t arraySize){
  uint32_t _hash = 0xFFFFFF;
  for(int i = 0; i < arraySize; i++){
    _hash = (_hash >> 8) ^ table[(_hash & 0xff) ^ message[i]];
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