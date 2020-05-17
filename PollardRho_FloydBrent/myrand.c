#ifndef __myrand_c
#define __myrand_c

#include <stdint.h>
#include <stdlib.h>
#include "sha256.h"

// hashing 64bit
uint64_t h256_to_i64(uint64_t p[4])
{
	return p[0] ^ p[1] ^ p[2] ^ p[3];
}

uint64_t H64(const uint64_t p)
{
	uint64_t v[8] =
	{
		0xA0B7DCFE8956310E,
		0xFFAA6594BCD013CC,
		0x75ED98BC645BEDAF,
		0xABCD6585366EDC89,
		0x6586876FFFEDDCAA,
		0xFED866BCDA755DDD,
		0x777755559999000E,
		0x10100177EDFBDCA8,
	};
	uint64_t sha256v[4];

	//v[0] = v[1] = v[2] = v[3] = v[4] = v[5] = v[6] = v[7] = 0;
	
	v[((p >> 19) ^ (p >> 47) ^ (p >> 7)) & 0x7] = p;
	sha256((uint8_t *)sha256v, (const uint8_t *)v);

	return h256_to_i64(sha256v);
}

uint64_t rand_u64(void)
{
	uint64_t r = 0;

	for (int i = 0; i < 8; i++)
	{
		r <<= 8; 
		r |= (uint64_t)(rand() & 0xff);
	}
	return H64(r);
}

int bits(int64_t a) { int b = 0; while (a) b++, a >>= 1; return b; }


#endif