//Lauren Rolan

/* Fichier source initial pour le TP sur le codage de Hamming (Theorie de
l'information en 2Ainformatique). Ce fichier doit etre modifié et renommé. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int code( int message );
int decode( int coded_message );
int bit(int x, int i, int n);
int flip(int x, int i, int n);
int get_int(int x[], int n);
int canal_binaire(int input, int n, double p);
int correction(int y, int n, int s);
int syndrome(int y);
int distance(int *code1, int *code2, int n);
int * hamming();

int main()
{
	int test = 8;
	srand(time(NULL));
	double p = rand() % 100;
	int cod, decod, canal, synd, resul;
	cod = code(test);
	decod = decode(cod);
	canal = canal_binaire(cod, 7, p);
	synd = syndrome(canal);
	resul = correction(canal, 7, synd);

	printf("Coded = %d\n", cod);
	printf("Decoded = %d\n", decod);
	printf("Avec prob %.2f\n", p);
	printf("After channel = %d\n", canal);
	printf("syndrome = %d\tResultat = %d\n", synd, resul);

	int *codes = hamming();
}

int comparaison(int n, int K, int * tableau)
{
	for(int i = 0; i < K; i++)
	{
		distance(message, ham)
	}
}

int * hamming()
{
	int *codes;
	codes = malloc(16*sizeof(int));
	for(int i =0; i < 16; i++)
	{
		codes[i] = code(i);
		printf("N = %d \t Code = %d\n", i, codes[i]);
	}
	return codes;
}

int distance(int *code1, int *code2, int n)
{
	int dist = 0;
	for(int i = 0; i < n; i++)
	{
		if(code1[i] != code2[i])
		{
			dist++;
		}
	}
	return dist;
}

int correction(int y, int n, int s)
{
	if(s == 0)
		return y;
	return flip(y, s, n);
}

int syndrome(int y)
{
	int _y[7];
	for(int i = 0; i < 7; i++)
	{
		_y[i] = bit(y, i, 7);
	}
	int s0, s1, s2;
	s0 = (_y[0] + _y[2] + _y[4] + _y[6]) % 2;
	s1 = (_y[1] + _y[2] + _y[5] + _y[6]) % 2;
	s2 = (_y[3] + _y[4] + _y[5] + _y[6]) % 2;
	return s0 + 2*s1 + 4*s2;

}

int canal_binaire(int input, int n, double p)
{
	int result[n];
	for(int i = 0; i < n; i++)
	{
		if(p > (rand() % 100))
		{
			result[i] = bit(input, i, n);
		} else {
			result[i] = (bit(input, i, n) + 1) % 2;
		}
	}
	return get_int(result, n);
}

int code( int message )
{
	int m[4];
	int x[7];

	x[2] = m[0] = bit(message, 1, 4);
	x[4] = m[1] = bit(message, 2, 4);
	x[5] = m[2] = bit(message, 3, 4);
	x[6] = m[3] = bit(message, 4, 4);

	x[0] = (x[2] + x[4] + x[6]) % 2;
	x[1] = (x[2] + x[5] + x[6]) % 2;
	x[3] = (x[4] + x[5] + x[6]) % 2;
	
	return get_int(x, 7);
}

int decode( int coded_message )
{
	int x[7], m[4];
	for(int i = 1; i < 8; i++)
	{
		x[i] = bit(coded_message, i, 7);
	}
	m[0] = x[2];
	m[1] = x[4];
	m[2] = x[5];
	m[3] = x[6];

	return get_int(m, 4);
}

int bit(int x, int i, int n)
{
	return (x>>(n-i)) & 1;
}

int flip(int x, int i, int n)
{
	return x^(1<<(n-i));
}

int get_int(int x[], int n)
{
	int result = 0;
	int pot = 1;
	printf("Code : ");
	for(int i = n-1; i >= 0; i--)
	{
		result += x[i] * pot;
		pot *= 2;
		printf("%d ", x[i]);
	}
	printf("\n");
	return result;
}