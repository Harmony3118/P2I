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
int permutationExpansive[58] = {31, 0, 1, 2, 3, 4, 3, 4, 5, 6, 7, 8, 7, 8, 9, 10, 11, 12, 11, 12, 13, 14, 15, 16, 15, 16, 17, 18, 19, 20, 19, 20, 21, 22, 23, 24, 23, 24, 25, 26, 27, 28, 27, 28, 29, 30, 31, 0};
// est ce que les bits sont des int ? -> non
// l’opérateur « ou exclusif » : ^

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

void afficher_bin(unsigned long val, int taille)
{
    for (int i = taille - 1; i >= 0; i--)
    {
        // printf("%d", !!(val & (1u << i)));
        printf("%lu", (val >> i) & 1);
        if (i % 8 == 0)
            printf(" ");
    }
    printf("\n");
}

/* tableau [58, 50, ...] -> le bit 58 est déplacé à la position 1, le 50 à la position 2, ...*/
// unsigned long permuter(unsigned long serie, int taille)
unsigned long permuter(unsigned long serie)
{
    unsigned long seriePermut = serie;

    // parcours du tableau de permutation
    for (int i = 0; i < N; i++)
    {
        int k = permutationD[i];
        // le bit k est déplacé à la position i
        if (serie & (1ul << k)) // bit k == 1 ?
        {
            seriePermut = seriePermut | (1ul << i); // 00..010..00
        }
        else
        {
            seriePermut = seriePermut & ~(1ul << i); // 11..101..11
        }
    }
    return seriePermut;
}

unsigned long permuterExpansive(unsigned R0) // R0 = 32 bits
{
    printf("Avant permutation exp : ");
    afficher_bin(R0, N / 2);
    unsigned long R0Expansive = R0;
    for (int i = 0; i < 3 * N / 4; i++)
    {
        int k = permutationExpansive[i];
        // le bit k est déplacé à la position i
        if (R0 & (1ul << k)) // bit k == 1 ?
        {
            R0Expansive = R0Expansive | (1ul << i); // 00..010..00
        }
        else
        {
            R0Expansive = R0Expansive & ~(1ul << i); // 11..101..11
        }
    }
    printf("Permutation expansive : ");
    afficher_bin(R0Expansive, 3 * N / 4);
    return R0Expansive;
}

int main()
{
    for (int i = 0; i < N; i++)
    {
        permutationD[i] = (i + 8) % N;
    }
    // printf("******************************** Tableau PermutationD ********************************\n");
    // afficher_tab(permutationD);
    printf("************************************ Texte initial ***********************************\n");
    printf("%s\n", texte);

    // création du tableau de 64 bits -> à terme sera dans une fonction
    // /!\ le texte pourra faire plus de 64 bits, il faudra alors traiter avec plusieurs tableaux
    unsigned long int result = texte[0];
    for (int k = 0; k < 8; k++)
    {
        result <<= 8;
        result |= texte[k];
    }
    afficher_bin(result, N);

    unsigned long int newResult = permuter(result);

    // partage en 2 bloc de 32 bits M = (L0,R0)
    unsigned R = newResult & 0xFFFFFFFF;
    unsigned L = (newResult >> 32) & 0xFFFFFFFF;

    printf("************************************** Résultat **************************************\n");
    if (newResult == result)
    {
        printf("⚠️ La permutation n'a rien changé !\n");
    }
    afficher_bin(newResult, N);

    afficher_bin(L, N / 2);
    afficher_bin(R, N / 2);
    unsigned long RExp = permuterExpansive(R);

    // Calcul de la clé
    // ou exclusif bloc puis clé
    // opération de substitution

    for (int k = 7; k >= 0; k--)
    {
        texte[k] = newResult & 0xFF;
        newResult >>= 8;
    }
    printf("%s\n", texte);

    return 0;
}