#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include "modarith.c"
#include "myrand.c"


#ifndef M_PI
# define M_PI ((double)3.1415926535897932385)
#endif

typedef struct
{
	int64_t a, b, R;
} tEquation;


void SetEquation(tEquation *Em, int64_t a, int64_t b, int64_t mod, int64_t G, int64_t P)
{
	Em->a = a;
	Em->b = b;
	Em->R = AddMod(MulMod(a, G, mod), MulMod(b, P, mod), mod);
}

void IterateAmBm(tEquation *Em, int64_t mod, int64_t G, int64_t P)
{
	uint64_t r = H64(*(const uint64_t *)&Em->R);

	switch (r % 3)
	{
	case 0:
		Em->b = AddMod(Em->b, 1, mod);
		Em->R = AddMod(Em->R, P, mod);
		break;

	case 1:

		Em->a = AddMod(Em->a, Em->a, mod);
		Em->b = AddMod(Em->b, Em->b, mod);
		Em->R = AddMod(Em->R, Em->R, mod);
		break;

	case 2:

		Em->a = AddMod(Em->a, 1, mod);
		Em->R = AddMod(Em->R, G, mod);
		break;
	}
}




int64_t RunPollardRho_FloydTH(int64_t k, int64_t G, int64_t P)
{
	
	int64_t sol;

	tEquation Ei, E2i;

	SetEquation(&Ei, rand_u64() % modulo, rand_u64() % modulo, modulo, G, P);
	int64_t GroupOperations = 0;

	puts("\n");

	do
	{
		IterateAmBm(&Ei, modulo, G, P);

		if (!GroupOperations)
		{
			E2i.R = Ei.R;
			E2i.a=Ei.a; E2i.b = Ei.b;
		}

		IterateAmBm(&E2i, modulo, G, P);
		IterateAmBm(&E2i, modulo, G, P); 
		GroupOperations += 3;

		if (!(GroupOperations % 100000))
			printf("Group operations=%lld\r", GroupOperations);

	} while (Ei.R != E2i.R || E2i.b == Ei.b);

	sol = DivMod(SubMod(E2i.a, Ei.a, modulo), SubMod(Ei.b, E2i.b, modulo), modulo);
	printf("modulo=%lld                ", modulo);
	printf("\n\nSecret k=%lld, found k=%lld\ngroup operations with Floyd cycle search=%lld", k, sol, GroupOperations);
	printf("\n====================\n\n");
	return GroupOperations;
}

int64_t RunPollardRho_BrentTH(int64_t k, int64_t G, int64_t P)
{

	int64_t sol;

	tEquation Ei, E2i;

	SetEquation(&Ei, rand_u64() % modulo, rand_u64() % modulo, modulo, G, P);
	int64_t GroupOperations = 0;

	int64_t power=1, lambda = 1;

	puts("\n");

	IterateAmBm(&Ei, modulo, G, P);

	E2i.R = Ei.R;
	E2i.a = Ei.a; E2i.b = Ei.b;
	IterateAmBm(&E2i, modulo, G, P);

	GroupOperations += 2;

	while (Ei.R != E2i.R || E2i.b == Ei.b)
	{
		if (power == lambda) 
		{
			Ei.R = E2i.R;
			Ei.a = E2i.a; Ei.b = E2i.b;

			power <<= 1;
			lambda = 0;
		}
		
		IterateAmBm(&E2i, modulo, G, P); 
		GroupOperations++;
		lambda++;

		if (!(GroupOperations % 100000))
			printf("Group operations=%lld\r", GroupOperations);
	}

	sol = DivMod(SubMod(E2i.a, Ei.a, modulo), SubMod(Ei.b, E2i.b, modulo), modulo);
	printf("modulo=%lld                ", modulo);
	printf("\n\nSecret k=%lld, found k=%lld\ngroup operations with Brent cycle search=%lld", k, sol, GroupOperations);
	printf("\n====================\n\n");
	return GroupOperations;
}


int main()
{

int64_t fl = 0, br = 0;

const int rounds = 10;

	if (!isprime(modulo))
	{
		printf("Modulo %lld is not a prime number, exit.\n\n", modulo);
		return 0;
	}

	srand((unsigned int)time(NULL));

	for (int i = 0; i < rounds; i++)
	{
		
		int64_t k = rand_u64() % modulo;
		int64_t G = (modulo / 23) * 11;
		int64_t P = MulMod(k, G, modulo);

		printf("Round [%d]\n\n", i+1);
		fl += RunPollardRho_FloydTH(k, G, P);
		br += RunPollardRho_BrentTH(k, G, P);
	}

	fl /= rounds;
	br /= rounds;

	printf("\n\n average: floyd=%lld; brent=%lld floyd/brent=%1.2f\n\n", fl, br, (float)fl/(float)br);

	return 0;
}