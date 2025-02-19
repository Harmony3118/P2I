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

/*
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
   a l'aide des proprietes de l'operateur logique ! :
// unsigned b = !!(a & (1u << 3));
*/

unsigned long puissance(int exposant)
{
    unsigned long rslt = 1;
    for (int n = 0; n < exposant; n++)
    {
        rslt *= 2;
    }
    return rslt;
}

void afficher_tab(int tab[N])
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

void afficher_bin(unsigned long val)
{
    for (int i = N - 1; i >= 0; i--)
    {
        // printf("%d", !!(val & (1u << i)));
        printf("%lu", (val >> i) & 1);
        if (i % 8 == 0)
            printf(" "); // Ajouter des espaces tous les 8 bits
    }
    printf("\n");
}

/* tableau [58, 50, ...] -> le bit 58 est déplacé à la position 1, le 50 à la position 2, ...*/
// il faudra adapter cette fonction
unsigned long permuter(unsigned long serie)
{
    printf("************************************* Déroulement *************************************\n");
    // copie de tab
    unsigned long seriePermut = serie;
    // permutation
    // parcours de permutationD

    printf("Avant la permutation, seriePermut = %lu\n", serie);
    for (int i = 0; i < 3; i++)
    {
        printf("Début boucle %d: ", i);
        afficher_bin(seriePermut);
        int k = permutationD[i];
        unsigned long modif;
        // le bit k est déplacé à la position i
        if (serie & (1ul << k)) // bit k == 1 ?
        {
            // Modif = que des 0 sauf un 1 à la position i
            // modif = puissance(i); // 00..010..00
            seriePermut = seriePermut | (1ul << i);
            // printf("bit = %d, modif = %lu\n", !!(serie & (1ul << k)), modif);
            printf("bit %d = 1\t", k);
        }
        else
        {
            // Modif = que des 1 sauf un 0 à la position i
            // modif = 0;
            // for (int j = 0; j < N; j++)
            // {
            //     if (j != i)
            //     {
            //         modif += puissance(j);
            //     }
            // }
            seriePermut = seriePermut & ~(1ul << i);
            printf("bit %d = 0\t", k);
            // printf("bit = %d, modif = %lu\n", !!(serie & (1ul << k)), modif);
        }
        // printf("le bit %d est déplacé à la position %d\t", k, i);
        afficher_bin(seriePermut);
    }
    printf("Après la permutation, seriePermut = %lu\n", seriePermut);
    afficher_bin(seriePermut);
    return seriePermut;
}

int main()
{
    for (int i = 0; i < N; i++)
    {
        permutationD[i] = (i + 8) % N;
    }
    // int test1[N];
    // for (int i = 0; i < N; i++)
    // {
    //     test1[i] = i;
    // }
    // afficher_tab(test1);
    // printf("******************************** Tableau PermutationD ********************************\n");
    // afficher_tab(permutationD);
    printf("************************************ Texte initial ***********************************\n");
    printf("%s\n", texte);
    // permuter(test1);
    // afficher_tab(test1);

    // création du tableau de 64 bits -> à terme sera dans une fonction /!\ le texte pourra faire
    // plus de 64 bits, il faudra alors traiter avec plusieurs tableaux
    unsigned long int result = texte[0];
    // unsigned long int result = 0;
    for (int k = 0; k < 8; k++)
    {
        result <<= 8;
        result |= texte[k];
        // result |= ((unsigned long)texte[k] << (8 * (7 - k)));
    }
    printf("result = %lu\t sizeof result = %lu\n", result, sizeof(result));
    afficher_bin(result);

    unsigned long int newResult = permuter(result);

    printf("************************************** Résultat **************************************\n");
    if (newResult == result)
    {
        printf("⚠️ La permutation n'a rien changé !\n");
    }
    printf("%lu\n", newResult);
    afficher_bin(result);
    for (int k = 7; k >= 0; k--)
    {
        texte[k] = result & 0xFF;
        result >>= 8;
    }
    printf("%s\n", texte);

    return 0;
}