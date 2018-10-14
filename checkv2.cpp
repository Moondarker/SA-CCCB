#include "GTASA_CRC32.cpp"
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

/**
* C++ version 0.4 char* style "itoa":
* Written by Lukas Chmela
* Modified by Moondark to perform CRC32 bruteforce with given alphabet 
* Released under GPLv3.
*/
char* itoaa(unsigned long long value, char* result, int base) {
	// check that the base if valid
	if (base < 2 || base > 36) { *result = '\0'; return result; } //Better change 36 to an actual size of alphabet

	char* ptr = result, *ptr1 = result, tmp_char;
	unsigned long long tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "WASDSAW" [3 + (tmp_value - value * base)]; //You may change the alphabet and its size-1. Ex.: *ptr++ = "ABCDEDCBA" [4 + (tmp_value - value * base)];
	} while ( value );

	// Apply negative sign
	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
};

int main() 
{
	unsigned long long maxchar = std::pow(4,17); //4 chars alphabet, string length up to 17
	char buffer [30];
	std::string tresult;
	
	std::cout << "Please enter a cheat code to bruteforce: \n";
	
	std::string intocrc;
	std::cin >> intocrc;
	
	reverse(intocrc.begin(), intocrc.end()); 
    const char *chrtocrc = intocrc.c_str();
	unsigned long needle = GTASA_CRC32_fromString(chrtocrc);
	
	for (unsigned long long len = 1; len <= maxchar; len++ ) {
		itoaa (len,buffer,4); //transform number into string of symbols from alphabet
		
		if (GTASA_CRC32_fromString(buffer) == needle) { 
			tresult = string(buffer);
			reverse (tresult.begin(),tresult.end());
			std::cout << std::hex << std::uppercase << GTASA_CRC32_fromString(buffer) << " found - (" << tresult << ")\n"; //HASH found - (key combination)
		};
		if ((len % (maxchar / 100)) == 0) {
			std::cout << std::dec << (len / (maxchar / 100)) << "% (" << len << ")\n"; //x% (# of strings tested)
		}
	};
    return 0;
}
