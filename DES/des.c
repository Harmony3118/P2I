#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// char *texte = "Ceci est un texte qui va être chiffré pour vérifier la fonctionalité de mon code";
// ce texte a une taille de 64 bits (il faudra mieux généraliser)
unsigned char texte[] = "Texte 8b";
// unsigned char texte[] = "12345678";

#define N 64

int permutationD[N] = {57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3, 61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7, 56, 48, 40, 32, 24, 16, 8, 0, 58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4, 62, 54, 46, 38, 30, 22, 14, 6};
int permutationF[N] = {39, 7, 47, 15, 55, 23, 63, 31, 38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29, 36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27, 34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25, 32, 0, 40, 8, 48, 16, 56, 24};
int permutationPure[N / 2] = {16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10, 2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25};
int permutationExpansive[58] = {31, 0, 1, 2, 3, 4, 3, 4, 5, 6, 7, 8, 7, 8, 9, 10, 11, 12, 11, 12, 13, 14, 15, 16, 15, 16, 17, 18, 19, 20, 19, 20, 21, 22, 23, 24, 23, 24, 25, 26, 27, 28, 27, 28, 29, 30, 31, 0};
int tableS[8][4][16] = {
    {
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13},
    },
    {
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9},
    },
    {
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12},
    },
    {
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14},
    },
    {
        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3},
    },
    {
        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13},
    },
    {
        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12},
    },
    {
        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11},
    }};

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
unsigned long permuter(unsigned long serie, char type)
{
    int taille;
    if (type == 'P')
    {                   // si c'est une permutation pure
        taille = N / 2; // 32 bits
    }
    else
    {
        taille = N; // 64 bits
    }
    unsigned long seriePermut = serie;

    // parcours du tableau de permutation
    for (int i = 0; i < taille; i++)
    {
        int k;
        if (type == 'D')
        { // permutation initiale
            k = permutationD[i];
        }
        else if (type == 'F')
        { // permutation finale
            k = permutationF[i];
        }
        else if (type == 'P')
        { // permutation pure
            k = permutationPure[i];
        }
        else
        { // erreur
            k = -1;
        }
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
    return R0Expansive;
}

int main()
{
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

    unsigned long int newResult = permuter(result, 'D');

    // partage en 2 bloc de 32 bits M = (L0,R0)
    unsigned R = newResult & 0xFFFFFFFF;
    unsigned L = (newResult >> 32) & 0xFFFFFFFF; // -> il va falloir traiter la ensuite

    // permutation expansive
    unsigned long RExp = permuterExpansive(R);

    printf("************************************** Résultat **************************************\n");
    if (newResult == result)
    {
        printf("⚠️ La permutation n'a rien changé !\n");
    }
    afficher_bin(newResult, N);

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
        // RSuivant = (RSuivant << 4)< | (tableS[i][j] & 0xF); -> pas dans le bon ordre
        RSuivant |= (tableS[k][i][j] & 0xF) << 4 * k;
        printf("i = %d, j = %d, tabS%d = %d,\t", i, j, k, tableS[k][i][j]);
        afficher_bin((tableS[k][i][j] & 0xF), 4);
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