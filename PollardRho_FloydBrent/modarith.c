#ifndef __modarith_c
#define __modarith_c

#include <stdint.h>
#include <math.h>

// modular arithmetics up to 62 bits to avoid overflow
// modulo is prime number

#define modulo 65036420395579ll
//#define modulo 4162330905307087ll
//#define modulo 17179897643ll
//#define modulo 100000007ll
//#define modulo 1099511627791ll


#define norm(_n,_mod) ((_n)<0?(_n)+_mod:(_n))


int64_t AddMod(int64_t a, int64_t b, int64_t mod)
{
	a = (a + b) % mod;
	return norm(a, mod);
}

int64_t SubMod(int64_t a, int64_t b, int64_t mod)
{
	a = (a - b) % mod;
	return norm(a, mod);
}

int64_t Mulx2_pow_n_Mod(int64_t a, int pow_n, int64_t mod)
{
	while (pow_n--)
		a = (a + a) % mod;
	return a;
}


int64_t MulMod(int64_t a, int64_t b, int64_t mod)
{
	int64_t r = 0;
	a = norm(a, mod);
	b = norm(b, mod);
	while (b)
	{
		if (b & 1) r = (r + a) % mod;
		a = (a + a) % mod;
		b >>= 1;
	}
	return r;
}

int64_t InvMod(int64_t a, int64_t mod)
{

	a = norm(a, mod);
	int64_t pow = mod - 2, r = 1;

	while (pow)
	{
		if (pow & 1) r = MulMod(r, a, mod);
		a = MulMod(a, a, mod);
		pow >>= 1;
	}
	return r;
}

int64_t DivMod(int64_t a, int64_t b, int64_t mod)
{
	return MulMod(a, InvMod(b, mod), mod);
}

int isprime(int64_t a)
{
	if ((a & 1) == 0) return 0;
	int64_t n = (int64_t)sqrt((double)a);
	int i = 3;
	while (i <= n)
	{
		if (a%i == 0) return 0;
		i += 2;
	}
	return 1;
}


#endif
