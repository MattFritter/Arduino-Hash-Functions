/* avrHash - a library of small hash functions for use with AVR micro controllers
 * Author: Matthew Fritter, m.fritter@alumni.ubc.ca
 * GNU GPLv3 License 2017
 */

#ifndef avrHash
#define avrHash

#include <avr/pgmspace.h>
 
class pearson
 {
	public:
		pearson();
		uint32_t hash(uint8_t* message, uint8_t* permutationTable, size_t arraySize);
	private:
		uint8_t round;
		uint32_t _hash;
		int i;
 };
 
class crc32
 {
	public:
		crc32();
		uint32_t hash(uint32_t* table, uint8_t* message, size_t arraySize);
	private:
		uint32_t _hash;
		uint32_t computeCRC(int i);
		uint32_t remainder;
		int i;
		int j;
 };
 
class sdbm
 {
	public:
		sdbm();
		uint32_t hash(uint8_t* message, size_t arraySize);
	private:
		uint32_t _hash;
		int i;
 };
 
class fnv1a
 {
	public:
		fnv1a();
		uint32_t hash(uint8_t* message, size_t arraySize);
	private:
		uint32_t _hash;
		static const uint32_t prime;
		int i;
 };
 
class jenkins
 {
	public:
		jenkins();
		uint32_t hash(uint8_t* message, size_t arraySize);
	private:
		uint32_t _hash;
		int i;
 };
 
#endif