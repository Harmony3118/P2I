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
int tableS1[4][16] = {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
                      {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
                      {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
                      {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}};
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

    // Calcul de la clé -> après
    // ou exclusif bloc puis clé
    unsigned long RCombi = RExp;
    // Opération de substitution -> 8 blocs de 6 bits traités par 8 tableaux
    // le tableau est inutile, on peut utiliser directement la série de bits... -> à modifier
    // unsigned short blocs[8];
    unsigned RSuivant = 0; // 32 bits
    for (int k = 0; k < 8; k++)
    {
        // blocs[i] = RCombi & 0x7F;
        unsigned short tmp = RCombi & 0x7F;
        printf("bloc 6 bit : ");
        afficher_bin(tmp, 6);
        printf("\t");
        RCombi >>= 6;
        int i = 0;
        if (tmp & (1ul << 5))
            i += 2;
        if (tmp & (1ul << 0))
            i += 1;
        int j = (tmp & 0x1E) >> 1; // 1E = 00011110 = 30
        // taille complète = 32 = N/2
        // position actuelle = 32-1-4*k
        // on décale de 32-4-4*k
        // RSuivant = (RSuivant << 4)< | (tableS1[i][j] & 0xF); -> pas dans le bon ordre
        RSuivant |= (tableS1[i][j] & 0xF) << 4 * k;
        printf("i = %d, j = %d, tabS1 = %d,\t", i, j, tableS1[i][j]);
        afficher_bin((tableS1[i][j] & 0xF), 4);
    }
    printf("Rsuivant : ");
    afficher_bin(RSuivant, N / 2);
    // for (int i = 0; i < 8; i++)
    // {
    //     printf("Bloc %d : ", i);
    //     afficher_bin(blocs[i], 6);
    // }
    unsigned long serieFinal = RSuivant;
    serieFinal = (serieFinal << N / 2) | L;
    printf("serieFinal = ");
    afficher_bin(serieFinal, N);
    for (int k = 7; k >= 0; k--)
    {
        texte[k] = serieFinal & 0xFF;
        serieFinal >>= 8;
    }
    printf("serieFinal = ");
    afficher_bin(serieFinal, N);
    printf("%s\n", texte);

    return 0;
}