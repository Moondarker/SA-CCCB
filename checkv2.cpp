#include "GTASA_CRC32.cpp"
#include <iostream>
#include <signal.h>
#include <cmath>
#include <algorithm>

using namespace std;

bool s_interrupted = false;
unsigned long cheatHashes[] = { 0xDE4B237D, 0xB22A28D1, 0x5A783FAE, 0xEECCEA2B, 0x42AF1E28, 0x555FC201, 0x2A845345, 0xE1EF01EA, 
								0x771B83FC, 0x5BF12848, 0x44453A17, 0xFCFF1D08, 0xB69E8532, 0x8B828076, 0xDD6ED9E9, 0xA290FD8C, 
								0x3484B5A7, 0x43DB914E, 0xDBC0DD65, 0x00000000, 0xD08A30FE, 0x37BF1B4E, 0xB5D40866, 0xE63B0D99, 
								0x675B8945, 0x4987D5EE, 0x2E8F84E8, 0x1A9AA3D6, 0xE842F3BC, 0x0D5C6A4E, 0x74D4FCB1, 0xB01D13B8, 
								0x66516EBC, 0x4B137E45, 0x78520E33, 0x3A577325, 0xD4966D59, 0x5FD1B49D, 0xA7613F99, 0x1792D871, 
								0xCBC579DF, 0x4FEDCCFF, 0x44B34866, 0x2EF877DB, 0x2781E797, 0x2BC1A045, 0xB2AFE368, 0xFA8DD45B, 
								0x8DED75BD, 0x1A5526BC, 0xA48A770B, 0xB07D3B32, 0x80C1E54B, 0x5DAD0087, 0x7F80B950, 0x6C0FA650, 
								0xF46F2FA4, 0x70164385, 0x885D0B50, 0x151BDCB3, 0xADFA640A, 0xE57F96CE, 0x040CF761, 0xE1B33EB9, 
								0xFEDA77F7, 0x8CA870DD, 0x9A629401, 0xF53EF5A5, 0xF2AA0C1D, 0xF36345A8, 0x8990D5E1, 0xB7013B1B, 
								0xCAEC94EE, 0x31F0C3CC, 0xB3B3E72A, 0xC25CDBFF, 0xD5CF4EFF, 0x680416B1, 0xCF5FDA18, 0xF01286E9, 
								0xA841CC0A, 0x31EA09CF, 0xE958788A, 0x02C83A7C, 0xE49C3ED4, 0x171BA8CC, 0x86988DAE, 0x2BDD2FA1 };

/**
* C++ version 0.4 char* style "itoa":
* Written by Lukas Chmela
* Modified by Moondark to perform CRC32 bruteforce with given alphabet 
* Released under GPLv3.
*/

void ctrlCHandler(int s) {
	s_interrupted = true; 
};

char* itoaa(unsigned long long value, char* result, int base, char* alphabet, int asize) {
	// check that the base if valid
	if (base < 2 || base > 36) { *result = '\0'; return result; } //Better change 36 to an actual size of alphabet

	char* ptr = result, *ptr1 = result, tmp_char;
	unsigned long long tmp_value;

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
	unsigned long needle = 0;
	unsigned long long maxchar = 0;
	unsigned long long startpos = 0;
	
	char* alphabet = "WASDSAW"; //TODO: rewrite
	int asize = 3;
	
	char buffer [30];
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
		asize = input.size()-1;
		alphabet = (char *)input.append(tmp).c_str(); //TODO: rewrite
		std::cout << "* Using alphabet: " << alphabet << " (" << asize + 1 << " chars)\n";
	} else {
		std::cout << "* Using alphabet: WASDSAW (4 chars)\n";
	};
	
	std::cout << "Please enter maximum string length (default - 14): \n";
	std::getline(std::cin, input);
	
	if (atoi(input.c_str()) > 0) {
		maxchar = std::pow(asize+1,atoi(input.c_str()));
		std::cout << "* Max string length was set to " << atoi(input.c_str()) << ", last pos to check will be " << maxchar << "\n";
	} else {
		maxchar = std::pow(asize+1,14);
		std::cout << "* Max string length was set to 14, last pos to check will be " << maxchar << "\n";
	};
	
	if (maxchar == 0) {
		std::cout << "ERROR: alphabet was too large, or you've tried to bruteforce too many characters. \n";
		std::cout << "Please reduce number of chars in alphabet, or maximum string length. \n";
		exit(0);
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
	
	for (unsigned long long len = startpos; len <= maxchar; len++ ) {
		itoaa (len,buffer,4,alphabet,asize); //transform number into string of symbols from alphabet
		
		if (allCheats && (std::find(cheatHashes, cheatHashes+88, GTASA_CRC32_fromString(buffer)) != cheatHashes+88)) {
			tresult = string(buffer);
			reverse (tresult.begin(),tresult.end());
			std::cout << "\r" << std::hex << std::uppercase << GTASA_CRC32_fromString(buffer) << " found - (" << tresult << ")\n"; //HASH found - (key combination)
		} else if (GTASA_CRC32_fromString(buffer) == needle) {
			tresult = string(buffer);
			reverse (tresult.begin(),tresult.end());
			std::cout << "\r" << std::hex << std::uppercase << GTASA_CRC32_fromString(buffer) << " found - (" << tresult << ")\n"; //HASH found - (key combination)
		};
		if (((len % (maxchar / 100)) < 2) || s_interrupted) {
			std::cout << "\r" << std::dec << (len / (maxchar / 100)) << "%, last pos: " << len; //x% (# of strings tested)
		};
		if (s_interrupted) {
			std::cout << "\n";
			exit(0);
		};
	};
    return 0;
}
