# Arduino-Hash-Functions
This repository is a set of common non-cryptographic hash functions implemented for usage with Arduino and Arduino-compatible boards. It includes the following functions:
  * CRC-32
  * FNV-1A
  * Jenkins
  * Lookup3
  * MD5
  * Pearson
  * SDBM
<!-- Weird Github Markdown fuckery needed to end a list. WTF? -->
These algorithms are provided in a single library file (```avrHash```) that can be imported, or as individual implementations using serial communication (see the ```Examples``` folder).

**These implementations can be used for general C/C++ purposes, but some such as MD5 contain PROGMEM declarations for use with AVR to save RAM usage.**
