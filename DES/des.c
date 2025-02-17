#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// char *texte = "Ceci est un texte qui va être chiffré pour vérifier la fonctionalité de mon code";
// ce texte a une taille de 64 bits (il faudra mieux généraliser)
unsigned char texte[] = "Texte 8b";
// unsigned char texte[] = "12345678";

const int N = 64;

int permutationD[64];
int permutationF[64];

// est ce que les bits sont des int ? -> non
// l’opérateur « ou exclusif » : ^

/* tableau [58, 50, ...] -> le bit 58 est déplacé à la position 1, le 50 à la position 2, ...*/
// il faudra adapter cette fonction
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

    // création du tableau de 64 bits -> à terme sera dans une fonction /!\ le texte pourra faire
    // plus de 64 bits, il faudra alors traiter avec plusieurs tableaux
    unsigned long int result = texte[0];
    for (int k = 1; k < 8; k++)
    {
        result <<= 8;
        result |= texte[k];
    }
    // unsigned long int result = (texte[0] << 8) | texte[1];
    // result <<= 8;
    // result |= texte[2];
    // result <<= 8;
    // result |= texte[3];
    // result <<= 8;
    // result |= texte[4];
    // result <<= 8;
    // result |= texte[5];
    // result <<= 8;
    // result |= texte[6];
    // result <<= 8;
    // printf("ok3 result = %ld\n", result);
    // result |= texte[7];
    // printf("texte[7] = %d\n", texte[7]);
    printf("ok4 result = %ld\n", result);

    printf("result = %ld\t sizeof result = %ld\n", result, sizeof(result));
    printf("texte[0] = %d, texte[1] = %d\n", texte[0], texte[1]);
    printf("texte[0] = %c, texte[1] = %c\n", texte[0], texte[1]);
    // modification du 3eme bit
    if (result & (1u << 3))
    {
        puts("bit 3 = 1");
        result = result & 65527; // 1111 1111 1111 0111
    }
    else
    {
        puts("bit 3 = 0");
        puts("ok1");
        result = result | 8; // 0000 0000 0000 1000
        puts("ok2");
    }
    /* 1u est un entier non signé qui vaut 1 lorsque signé (il y a un 1 sur le 1er bit)*/
    /* on peut aussi directement recuperer la valeur 0 ou 1
       a l'aide des proprietes de l'operateur logique ! : */
    // unsigned b = !!(a & (1u << 3));

    printf("%ld\n", result);
    for (int k = 7; k >= 0; k--)
    {
        texte[k] = result & 0xFF;
        result >>= 8;
    }
    // texte[7] = result & 0xFF;
    // result >>= 8;
    // texte[6] = result & 0xFF;
    // result >>= 8;
    // texte[5] = result & 0xFF;
    // result >>= 8;
    // printf("dernier result = %ld\n", result);
    // texte[4] = result & 0xFF;
    // result >>= 8;
    // printf("dernier result = %ld\n", result);
    // texte[3] = result & 0xFF;
    // result >>= 8;
    // texte[2] = result & 0xFF;
    // result >>= 8;
    // texte[1] = result & 0xFF;
    // result >>= 8;
    // texte[0] = result & 0xFF;
    // result >>= 8;
    // texte[1] = result & 0x00FF000000000000;
    // texte[0] = (result & 0xFF00000000000000) >> 8;
    printf("texte[0] = %d, texte[1] = %d\n", texte[0], texte[1]);
    printf("texte[0] = %c, texte[1] = %c\n", texte[0], texte[1]);
    printf("%s\n", texte);

    return 0;
}