#include <iostream>
#include <string>
#include <signal.h>
#include <cmath>
#include <algorithm>

#include "GTASA_cheathashes.h"
#include "GTASA_CRC32.h"

bool s_interrupted = false;

/**
* C++ version 0.4 char* style "itoa":
* Written by Lukas Chmela
* Modified by Moondark to perform CRC32 bruteforce with given alphabet 
* Released under GPLv3.
*/

void ctrlCHandler(int s) {
	s_interrupted = true; 
};

char* itoaa(uint64_t value, int base, char* result, char* alphabet, int asize) {
	// check that the base if valid
	if (base < 2 || base > 36) { *result = '\0'; return result; } //Better change 36 to an actual size of alphabet

	char* ptr = result, *ptr1 = result, tmp_char;
	uint64_t tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = alphabet [asize + (tmp_value - value * base)]; //You may change the alphabet and its size-1. Ex.: *ptr++ = "ABCDEDCBA" [4 + (tmp_value - value * base)];
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
	uint32_t needle = 0;
	uint64_t startpos = 0;
	uint64_t lastpos = 0;
	std::string prefix;
	
	char* alphabet = "WASDSAW"; //TODO: rewrite
	int asize = 3;
	
	char buffer [50];
	std::string input;
	std::string tmp;
	
	bool allCheats = true;
	signal (SIGINT,ctrlCHandler);
	
	std::string tresult;
	
	
	std::cout << "Please enter start pos (default - 0): \n";
	std::getline(std::cin, input);
	
	if (atoi(input.c_str()) > 0) {
		startpos = atoi(input.c_str());
		std::cout << "* Will start from " << startpos << "\n";
	} else {
		std::cout << "* Will start from 0\n";
	};
	
	std::cout << "Please enter expected alphabet (default - WASD): \n";
	std::getline(std::cin, input);
	
	if (input.length() > 0) {
		tmp = input;
		reverse (tmp.begin(), tmp.end());
		tmp.erase(0, 1);
		asize = (int) input.size()-1;
		alphabet = (char *)input.append(tmp).c_str(); //TODO: rewrite
		std::cout << "* Using alphabet: " << alphabet << " (" << asize + 1 << " chars)\n";
	} else {
		std::cout << "* Using alphabet: WASDSAW (4 chars)\n";
	};
	
	std::cout << "Please enter maximum string length (default - 14): \n";
	std::getline(std::cin, input);
	
	if (atoi(input.c_str()) > 0) {
		lastpos = (uint64_t) std::pow(asize+1,atoi(input.c_str()));
		std::cout << "* Max string length was set to " << atoi(input.c_str()) << ", last pos to check will be " << lastpos << "\n";
	} else {
		lastpos = (uint64_t) std::pow(asize+1,14);
		std::cout << "* Max string length was set to 14, last pos to check will be " << lastpos << "\n";
	};
	
	if (lastpos == 0) {
		std::cout << "ERROR: alphabet was too large, or you've tried to bruteforce too many characters. \n";
		std::cout << "Please reduce number of chars in alphabet, or maximum string length. \n";
		exit(0);
	};
	
	std::cout << "Please enter expected cheat code prefix (default - [none]): \n";
	std::getline(std::cin, input);
	
	if (input.length() > 0) {
		prefix = input;
		reverse (prefix.begin(), prefix.end());
		std::cout << "* Using prefix: " << input << "\n";
	} else {
		std::cout << "* Using no prefix\n";
	};
	
	std::cout << "Please enter a cheat code to bruteforce (default - all cheat codes): \n";
	std::getline(std::cin, input);
	
	if (input.length() > 0) {
		reverse(input.begin(), input.end()); 
		const char *chrtocrc = input.c_str();
		
		needle = GTASA_CRC32_fromString(chrtocrc);
		std::cout << "* Bruteforcing for cheat code with hash 0x" << std::hex << std::uppercase << needle << "\n";
		allCheats = false;
	} else {
		std::cout << "* Bruteforcing for all known cheat codes...\n";
	};
	
	for (uint64_t len = startpos; len <= lastpos; len++ ) {
		//cout << "itoaa called\n" << len << 4 << alphabet << asize;
		itoaa (len,(asize+1),buffer,alphabet,asize); //transform number into string of symbols from alphabet
		std::string ccode(buffer);
		
		if (prefix.length() > 0) {
			ccode.append(prefix);
		};
		
		if (allCheats && (std::find(cheatHashes, cheatHashes+88, GTASA_CRC32_fromString(ccode.c_str())) != cheatHashes+88)) {
			tresult = ccode;
			reverse (tresult.begin(),tresult.end());
			std::cout << "\r" << std::hex << std::uppercase << GTASA_CRC32_fromString(ccode.c_str()) << " found - (" << tresult << ")\n"; //HASH found - (key combination)
		} else if (GTASA_CRC32_fromString(ccode.c_str()) == needle) {
			tresult = ccode;
			reverse (tresult.begin(),tresult.end());
			std::cout << "\r" << std::hex << std::uppercase << GTASA_CRC32_fromString(ccode.c_str()) << " found - (" << tresult << ")\n"; //HASH found - (key combination)
		};
		
		if (((len % (lastpos / 100)) < 2) || s_interrupted) {
			std::cout << "\r" << std::dec << (len / (lastpos / 100)) << "%, last pos: " << len; //x% (# of strings tested)
		};
		
		if (s_interrupted) {
			std::cout << "\n";
			exit(0);
		};
	};
    return 0;
}
