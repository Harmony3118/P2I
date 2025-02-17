#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

char *texte = "Ceci est un texte qui va être chiffré pour vérifier la fonctionalité de mon code";

const int N = 8;

int permutationD[8];
int permutationF[8];

// est ce que les bits sont des int ?

/* tableau [58, 50, ...] -> le bit 58 est déplacé à la position 1, le 50 à la position 2, ...*/
void permuter(int tab[N])
{
    // copie de tab
    int tabTemp[N];
    for (int i = 0; i < N; i++)
    {
        tabTemp[i] = tab[i];
    }
    // permutation
    for (int i = 0; i < N; i++)
    {
        int k = permutationD[i];
        tab[i] = tabTemp[k - 1];
    }
}

void afficher(int tab[N])
{
    printf("[%d, ", tab[0]);
    for (int i = 1; i < N; i++)
    {
        if (i < N - 1)
        {
            printf("%d, ", tab[i]);
        }
        else
        {
            printf("%d]\n", tab[i]);
        }
    }
}

int main()
{
    for (int i = 0; i < N; i++)
    {
        permutationD[i] = (i + 2) % N + 1;
    }
    int test1[N];
    for (int i = 0; i < N; i++)
    {
        test1[i] = i;
    }
    afficher(test1);
    afficher(permutationD);
    permuter(test1);
    afficher(test1);
    return 0;
}