/*
 * Project 2
 * Liam McArdle
 * Version 2.0
 */

#include <iostream>
#include <cmath>
#include <string>



#ifndef MARMOSET_TESTING
int main();
#endif

char *encode(char *plaintext, unsigned long key);

char *decode(char *ciphertext, unsigned long key);

void swap(unsigned int &x, unsigned int &y);

void scramble(unsigned char *S, unsigned long key, int &i, int &j);

unsigned char get_R(unsigned char *S, int &i, int &j);

std::size_t get_length(char *length);

unsigned char * copy_str (char * str);

char *to_ascii_armour(unsigned char* unamored_text, std::size_t size);

char* from_ascii_armour(char* armored_text, std::size_t size);



#ifndef MARMOSET_TESTING
int main() {
	char test[] {};

		unsigned long key{51323};

		std::cout << test << std::endl;

		char *ciphertext { encode(test, key) };

		std::cout << ciphertext << std::endl;

		char *plaintext { decode(ciphertext, key) };

		std::cout << plaintext << std::endl;

		delete[] ciphertext;
		ciphertext = nullptr;
		delete[] plaintext;
		plaintext = nullptr;

		return 0;
}
#endif

char *encode(char *plaintext, unsigned long key) {

	unsigned char S[256];
	int i = 0;
	int j = 0;
	int &x{i};
	int &y{j};
	unsigned int size_w;
	unsigned int size_i = get_length(plaintext);
	unsigned int remainder = size_i % 4;

	if (size_i % 4 != 0) {
	size_w = size_i + (4 - remainder);
	} else {
	size_w = size_i;
	}
	unsigned char * A = new unsigned char [size_w];

	for (int i = 0; i < 256; i++) {
		S[i] = i;
	}

	scramble(S, key, x, y);

	A = copy_str(plaintext);

	for(unsigned int i = 0; i < size_w; i++) {
		A[i] = (get_R(S, x, y)) ^ A[i];
	}

	to_ascii_armour(A, size_w);
	return 0;
}

char *decode(char *ciphertext, unsigned long key) {
	unsigned int size_ciphertext = get_length(ciphertext) + 1;
	unsigned int size_plaintext = {((size_ciphertext + 2) / 5) * 4};
	unsigned char S[256];
	int i = 0;
	int j = 0;
	int &x{i};
	int &y{j};

	char *plaintext = new char[size_plaintext + 1];
	char * A = new char [size_ciphertext + 1];

	A = from_ascii_armour(ciphertext, size_ciphertext);

	for (int i = 0; i < 256; i++) {
		S[i] = i;
	}

	scramble(S, key, x, y);

	for(unsigned int i = 0; i < size_plaintext; i++) {
		A[i] = (get_R(S, x, y)) ^ A[i];
	}

	for (unsigned int i = 0; i < size_plaintext; i++) {
		plaintext[i] = A[i];
	}

	plaintext[size_plaintext] = '\0';
	return plaintext;
}

void swap(unsigned char &x, unsigned char &y) {
	int temp{};

	temp = x;
	x = y;
	y = temp;
}

void scramble(unsigned char *S, unsigned long key, int &i, int &j) {
	int k{};

	for (int z = 0; z < 256; z++) {
		k = i % 64;
		j = (j + S[i] + ((key >> k) & 1)) % 256;
		swap(S[i], S[j]);

		i = (i + 1) % 256;
	}
}

unsigned char get_R(unsigned char *S, int &i, int &j) {
	unsigned int r = 0;
	unsigned int R = 0;

	i = (i + 1) % 256;
	j = (j + S[i]) % 256;
	swap(S[i],S[j]);

	r = (S[i] + S[j]) % 256;
	R = S[r];

	return R;
}

std::size_t get_length(char *string) {
	unsigned int length;

	for(std::size_t i{0}; true; ++i){
			if(string[i] == '\0') {
				length = i;
				break;
			}
	}
	return length;
}

unsigned char * copy_str (char * str) {
	unsigned int length;
	unsigned int remainder;
	unsigned int length_w_null;

	length = get_length(str);
	remainder = length % 4;

	if(length % 4 != 0) {
		length_w_null = length + (4 - remainder);
	} else if (length % 4 == 0) {
		length_w_null = length;
	}

	unsigned char * A = new unsigned char[length_w_null];

	for (unsigned int i = 0; i < length; i++) {
		A[i] = str[i];
	}

	if (length != length_w_null) {
		for(unsigned int j = 0; j <= remainder; j++) {
			A[(length + j)] = '\0';
		}
	}
	return A;
}

char *to_ascii_armour(unsigned char* unarmored_text, std::size_t size) {
	unsigned int size_f = size / 4.0;

		char *ciphertext;
		int l = 5 * std::ceil(size / 4.0) + 1;
		ciphertext = new char[l];
		ciphertext[(l-1)] = '\0';

		for(unsigned int i = 0; i < size_f; i++) {
				unsigned int sum{0};

				char a = unarmored_text[4*i];
				char b = unarmored_text[4*i + 1];
				char c = unarmored_text[4*i + 2] ;
				char d = unarmored_text[4*i + 3];
				sum = sum | (a & 0xff);
				sum = sum << 8;
				sum = sum | (b & 0xff);
				sum = sum << 8;
				sum = sum | (c & 0xff);
				sum = sum << 8;
				sum = sum | (d & 0xff);

				ciphertext[5*i + 4] = (sum % 85) + 33;
				sum /= 85;
				ciphertext[5*i + 3] = (sum % 85) + 33;
				sum/= 85;
				ciphertext[5*i + 2] = (sum % 85) + 33;
				sum /= 85;
				ciphertext[5*i + 1] = (sum % 85) + 33;
				sum/= 85;
				ciphertext[5*i] = (sum % 85) + 33;
		}
	return ciphertext;
}

char* from_ascii_armour(char* armored_text, std::size_t size) {
	unsigned int size_plaintext = ((size)/5)*4;;
	unsigned char t_bytes[4] {};
	char decomposition[5] {};


	char * plaintext = new char [size_plaintext + 1];

	for(unsigned int i = 0; i < (size)/5; i++) {
		unsigned int temp {};
			for(int j = 0; j < 5; j++) {
				decomposition[j] = armored_text[(i*5) + j];
			}
			//from_ascii_armor
			for(int i = 4; i >= 0; i--) {
				temp = temp + (decomposition[4 - i] - 33)*std::pow(85, i);
			}
			for(int i = 3; i >= 0; i--) {
				t_bytes[i] = static_cast<unsigned char>(temp >> (24 - (i*8)));
			}
			//
			for(int j = 0; j < 4; j++) {
				plaintext[(i*4) + j] = t_bytes[j];
			}
		}
	return plaintext;
}




