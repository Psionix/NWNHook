#include "CryptString.h"

char CharEncrypt(char c)
{
	return (((c ^ 0x09) ^ 0x44) + 2);
}

char CharDecrypt(char c)
{
	return (((c - 2) ^ 0x44) ^ 0x09);
}

void Decrypt(char *str)
{
	for (unsigned int i = 0; i < strlen(str); i++)
		str[i] = CharDecrypt(str[i]);
}
