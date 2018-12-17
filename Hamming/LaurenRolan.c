//Author: Lauren Rolan

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int code( int message );
int decode( int coded_message );
int bit(int x, int i, int n);
int flip(int x, int i, int n);
int get_int(int x[], int n);
int * get_array(int x, int n);
int canal_binaire(int input, int n, double p);
int correction(int y, int n, int s);
int syndrome(int y);
int distance(int *code1, int *code2, int n);
int * hamming();
int fat(int n);
int binomial(int n, int k);
double prob_decodage(int n, double p, int e);
double pow(double base, int exp);
int comparaison(int n, int K, int * tableau);

int debug = 0;

int main()
{
	int test = 8;
	srand(time(NULL));
	double p = rand() % 100;
	int cod, decod, canal, synd, result;
    
    /* Question 1:
     * Message = 8
     * Code = 112
    */
	cod = code(test);
	decod = decode(cod);
	canal = canal_binaire(cod, 7, 15);
	synd = syndrome(canal);
	result = correction(canal, 7, synd);

	printf("Coded = %d\t", cod);
	printf("Decoded = %d\n", decod);
	printf("With prob %.2f\t", p);
	printf("After channel = %d\n", canal);
	printf("syndrome = %d\tresultat = %d\n\n", synd, decode(result));
    
    /* Question 3:
     * Syndrome = 1
     * y_corrigé = 112
     * y_decodé = 8
     */
    printf("y = %d\tsyndrome = %d\tresultat = %d\n\n", 48, syndrome(48), decode(correction(48, 7, syndrome(48))));

    /* Question 4: 
     * On peut vérfier ici que la distance minimale pour
     * le code de Hamming(7, 4) est 3. Ça veut dire que 
     * les codages plus proches ont au moins 3 bits de 
     * différence.
    */
	int *codes = hamming();
    int dist = comparaison(7, 4, codes);
    printf("Minimal distance = %d\n", dist);
   
    /* Question 5: 
     * Aprés éxécuter le code dessus, on peut vérifier
     * qui la probabilité est presque la théorique 
     * (0.445).
    */
    double prob_dec = prob_decodage(7, 0.25, 1);
    printf("The decode probability of H(7,4) is %.3f\n", prob_dec);
    
    int correct = 0;
    
    for(int i = 0; i < 1000; i++)
    {
        int current = rand() % 16;
        cod = code(current);
        canal = canal_binaire(cod, 7, 25); 
        synd = syndrome(canal);
        result = correction(canal, 7, synd);
        if(decode(result) == current)
        {
            correct++;
        }
    }
    printf("Empiric decode probability = %.3f\n", (float) correct/1000);
}

double pow(double base, int exp)
{
    double res = 1;
    for(int i = 0; i < exp; i++)
    {
        res *= base;
    }
    return res;
}

int fat(int n)
{
    int fat = 1;
    for(int i = n; i > 1; i--)
    {
        fat *= i;
    }
    return fat;
}

int binomial(int n, int k)
{
    return fat(n) / (fat(n-k) * fat(k));
}

double prob_decodage(int n, double p, int e)
{
    double sum = 0;
    for(int i = 0; i < e + 1; i++)
    {
        sum += binomial(n, i) * pow(p, i) * pow(1-p, n-i);
    }
    return sum;
}

int comparaison(int n, int K, int * tableau)
{
    int min_dist = n;
    int *current, *compar;
	for(int i = 0; i < 2 << (K-1); i++)
	{
        current = get_array(tableau[i], n);
        for(int j = i + 1; j < 2 << (K-1); j++)
        {
            compar = get_array(tableau[j], n);
            if(distance(current, compar, n) < min_dist)
            {
                min_dist = distance(current, compar, n);
            }
        }
	}
    return min_dist;
}

int * hamming()
{
	int *codes;
	codes = malloc(16*sizeof(int));
	for(int i =0; i < 16; i++)
	{
		codes[i] = code(i);
        if(debug){
            printf("N = %d \t Code = %d\n", i, codes[i]);
        }
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
	int *_y;
    
    _y = get_array(y, 7);
    
	int s0, s1, s2;
	s0 = (_y[0] + _y[2] + _y[4] + _y[6]) % 2;
	s1 = (_y[1] + _y[2] + _y[5] + _y[6]) % 2;
	s2 = (_y[3] + _y[4] + _y[5] + _y[6]) % 2;
	return s0 + 2*s1 + 4*s2;

}

int canal_binaire(int input, int n, double p)
{
	int result[n];
	for(int i = n-1; i >= 0; i--)
	{
		if(p <= (rand() % 100))
		{
			result[i] = bit(input, i+1, n); //n-i
		} else {
            if(debug){
                printf("----There is a flip.\n");
            }
			result[i] = (bit(input, i+1, n) + 1) % 2;
		}
	}
	return get_int(result, n);
}

int code( int message )
{
	int *m;
	int x[7];

    m = get_array(message, 4);
    
	x[2] = m[0];
	x[4] = m[1];
	x[5] = m[2]; 
	x[6] = m[3];

	x[0] = (x[2] + x[4] + x[6]) % 2;
	x[1] = (x[2] + x[5] + x[6]) % 2;
	x[3] = (x[4] + x[5] + x[6]) % 2;
	
	return get_int(x, 7);
}

int decode( int coded_message )
{
	int *x, m[4];
	
    x = get_array(coded_message, 7);

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

int * get_array(int x, int n)
{
    int *y = malloc(n * sizeof(int));
    if(debug) printf("Array: ");
    for(int i = n-1; i >= 0; i--)
	{
		y[i] = bit(x, i + 1, n); // i-1
        if(debug) printf("[%d]%d ", i, y[i]);
	}
    if(debug) printf("\n");
    return y;
}

int get_int(int x[], int n)
{
	int result = 0;
	int pot = 1;
	if(debug) printf("Binary : ");
	for(int i = n-1; i >= 0; i--)
	{
		result += x[i] * pot;
		pot *= 2;
		if(debug) printf("[%d]%d ", i, x[i]);
	}
	if(debug) printf("\n");
    
	return result;
}