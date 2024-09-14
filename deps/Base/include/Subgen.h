#pragma once

#define SUBGENMAX 1000000000

typedef struct Subgen
{
    int state[55];
    int si;
    int sj;
    int seed;
}
Subgen;

int SubgenNext(Subgen* subgen)
{
	if (!subgen->si--) subgen->si = 54;
	if (!subgen->sj--) subgen->sj = 54;
    int x = subgen->state[subgen->si] - subgen->state[subgen->sj];
	if (x < 0) x += SUBGENMAX;
	return subgen->state[subgen->si] = x;
}
Subgen SubgenCreate(int seed)
{
    Subgen subgen;

    subgen.si = 0;
    subgen.sj = 24;
    subgen.seed = seed;

    int p2 = 1;
	subgen.state[0] = seed % SUBGENMAX;

    for (int i = 1, j = 21; i < 55; i++, j += 21)
    {
		if (j >= 55) j -= 55;
		subgen.state[j] = p2;
        p2 = seed - p2;
		if (p2 < 0) p2 += SUBGENMAX;
		seed = subgen.state[j];
	}

	for (int i = 0; i < 165; i++)
        SubgenNext(&subgen);

    return subgen;
}
float SubgenFractionUnsigned(Subgen* subgen)
{
    return SubgenNext(subgen) / (float)SUBGENMAX;
}
float SubgenFractionSigned(Subgen* subgen)
{
    return SubgenFractionUnsigned(subgen) * 2 - 1;
}
float SubgenRange(Subgen* subgen, float min, float max)
{
    return min + ((max - min) * SubgenFractionUnsigned(subgen));
}
