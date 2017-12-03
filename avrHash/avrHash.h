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
		uint32_t hash(uint8_t* message, size_t arraySize);
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
 
class lookup3
 {
	public:
		lookup3();
		uint32_t hash(uint8_t* message, size_t arraySize, uint32_t seed);
	private:
		void mix(uint32_t *a, uint32_t *b, uint32_t *c);
		uint32_t a;
		uint32_t b;
		uint32_t c;
		uint8_t* k8;
		uint8_t* k;
 };
 
class md5
 {
	public:
		md5();
		void hash(uint8_t* message, size_t arraySize, uint32_t* outArray);
	private:
		uint32_t len;
		uint32_t sizeBit;
		uint8_t newArr[];
		uint32_t A;
		uint32_t B;
		uint32_t C;
		uint32_t D;
		uint32_t F;
		uint32_t g;
		uint32_t* block;
		int i;	
 };
 
#endif