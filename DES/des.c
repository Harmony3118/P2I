#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

char *texte = "Ceci est un texte qui va être chiffré pour vérifier la fonctionalité de mon code";

const int N = 64;

int permutationD[64];
int permutationF[64];

// est ce que les bits sont des int ? -> non
// l’opérateur « ou exclusif » : ^

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
    // for (int i = 0; i < N; i++)
    // {
    //     permutationD[i] = (i + 2) % N + 1;
    // }
    // int test1[N];
    // for (int i = 0; i < N; i++)
    // {
    //     test1[i] = i;
    // }
    // afficher(test1);
    // afficher(permutationD);
    // permuter(test1);
    // afficher(test1);

    printf("%s\n", texte);
    /* tester l'état du bit 2 de a : */
    unsigned a = 0x000F; /* 0000 0000 0000 1111 */
    unsigned c = 65527;
    printf("a = %d\nc = %d\n", a, c);
    // modification de 3eme ou 4eme bit
    if (a & (1u << 4))
    {
        puts("bit 3 = 1");
        a = a & 65527; // 1111 1111 1111 0111
        printf("a changé = %d\n", a);
    }
    else
    {
        puts("bit 4 = 0");
        a = a | 16; // 0000 0000 0001 0000
        printf("a changé = %d\n", a);
    }
    unsigned b = !!(a & (1u << 3));

    /* 1u est un entier non signé qui vaut 1 lorsque signé*/
    /* on peut aussi directement recuperer la valeur 0 ou 1
       a l'aide des proprietes de l'operateur logique ! : */
    printf("bit 4 = %d\n", b);
    printf("a = %d\n", a);
    return 0;
}