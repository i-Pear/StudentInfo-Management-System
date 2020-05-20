#include "Encryptor.h"

static ifSetPassword = false;
static char* keyString;
static int keyLength;

void setPassword(char* password) {
	keyString = password;
	ifSetPassword = true;
	keyLength = strlen(keyString);
}

static inline int getDecodeChar(int x) {
	return x % keyLength;
}

void decode(char* in) {
	// Using XOR Algorithm
	int len = strlen(in);
	for (int i = 0; i < len; i++) {
		in[i] = keyString[getDecodeChar(i)] ^ in[i];
	}
}
