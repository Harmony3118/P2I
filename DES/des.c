#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

unsigned char texte_c[] = "Ceci est un texte qui va être chiffré pour vérifier la fonctionalité de mon code";
// ce texte a une taille de 64 bits (il faudra mieux généraliser)
// unsigned char texte_c[] = "Texte 8b";
// unsigned char texte_c[] = "12345678";

#define N_i 64

int permutationCle[N_i - N_i / 8] = {56, 48, 40, 32, 24, 16, 8, 0, 57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2, 59, 51, 43, 35, 62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 60, 52, 44, 36, 28, 20, 12, 4, 27, 19, 11, 3};
int decalageCle[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
int permutationCompressive[48] = {13, 16, 10, 23, 0, 4, 2, 27, 14, 5, 20, 9, 22, 18, 11, 3, 25, 7, 15, 6, 26, 19, 12, 1, 40, 51, 30, 36, 46, 54, 29, 39, 50, 44, 32, 47, 43, 48, 38, 55, 33, 52, 45, 41, 49, 35, 28, 31};

int permutationD[N_i] = {57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3, 61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7, 56, 48, 40, 32, 24, 16, 8, 0, 58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4, 62, 54, 46, 38, 30, 22, 14, 6};
int permutationF[N_i] = {39, 7, 47, 15, 55, 23, 63, 31, 38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29, 36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27, 34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25, 32, 0, 40, 8, 48, 16, 56, 24};
int permutationPure[N_i / 2] = {15, 6, 19, 20, 28, 11, 27, 16, 0, 14, 22, 25, 4, 17, 30, 9, 1, 7, 23, 13, 31, 26, 2, 8, 18, 12, 29, 5, 21, 10, 3, 24};
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

#define nbCleFaible_i 64
unsigned long cleFaible[nbCleFaible_i] = {
    0x0101010101010101, 0x1F1F1F1F0E0E0E0E,
    0xE0E0E0E0F1F1F1F1, 0xFEFEFEFEFEFEFEFE,

    0x01FE01FE01FE01FE, 0xFE01FE01FE01FE01,
    0x1FE01FE00EF10EF1, 0xE01FE01FF10EF10E,
    0x01E001E001F101F1, 0xE001E001F101F101,
    0x1FFE1FFE0EFE0EFE, 0xFE1FFE1FFE0EFE0E,
    0x011F011F010E010E, 0x1F011F010E010E01,
    0xE0FEE0FEF1FEF1FE, 0xFEE0FEE0FEF1FEF1,

    0x1F1F01010E0E0101, 0xE00101E0F10101F1,
    0x011F1F01010E0E01, 0xFE1F01E0FE0E01F1,
    0x1F01011F0E01010E, 0xFE011FE0FE010EF1,
    0x01011F1F01010E0E, 0xE01F1FE0F10E0EF1,
    0xE0E00101F1F10101, 0xFE0101FEFE0101FE,
    0xFEFE0101FEFE0101, 0xE01F01FEF10E01FE,
    0xFEE01F01FEF10E01, 0xE0011FFEF1010EFE,
    0xE0FE1F01F1FE0E01, 0xFE1F1FFEFE0E0EFE,
    0xFEE0011FFEF1010E, 0x1FFE01E00EFE01F1,
    0xE0FE011FF1FE010E, 0x01FE1FE001FE0EF1,
    0xE0E01F1FF1F10E0E, 0x1FE001FE0EF101FE,
    0xFEFE1F1FFEFE0E0E, 0x01E01FFE01F10EFE,
    0xFE1FE001FE0EF101, 0x0101E0E00101F1F1,
    0xE01FFE01F10EFE01, 0x1F1FE0E00E0EF1F1,
    0xFE01E01FFE01F10E, 0x1F01FEE00E01FEF1,
    0xE001FE1FF101FE0E, 0x011FFEE0010EFEF1,
    0x01E0E00101F1F101, 0x1F01E0FE0E01F1FE,
    0x1FFEE0010EFEF001, 0x011FE0FE010EF1FE,
    0x1FE0FE010EF1FE01, 0x0101FEFE0101FEFE,
    0x01FEFE0101FEFE01, 0x1F1FFEFE0E0EFEFE,
    0x1FE0E01F0EF1F10E, 0xFEFEE0E0FEFEF1F1,
    0x01FEE01F01FEF10E, 0xE0FEFEE0F1FEFEF1,
    0x01E0FE1F01F1FE0E, 0xFEE0E0FEFEF1F1FE,
    0x1FFEFE1F0EFEFE0E, 0xE0E0FEFEF1F1FEFE};

void afficher_bin(unsigned long val_l, int taille_i)
{
    for (int i = taille_i - 1; i >= 0; i--)
    {
        printf("%lu", (val_l >> i) & 1);
        if (i % 8 == 0)
            printf(" ");
    }
    printf("\n");
}

/* tableau [58, 50, ...] -> le bit 58 est déplacé à la position 1, le 50 à la position 2, ...*/
unsigned long permuter(unsigned long serie_l, char type_c)
{
    unsigned long seriePermut_l = 0;
    int taille_i;
    if (type_c == 'P')
    {                       // permutation pure
        taille_i = N_i / 2; // 32 bits
    }
    else if (type_c == 'C')
    {
        taille_i = 3 * N_i / 4; // 48 bits
    }
    else if (type_c == 'K')
    {
        taille_i = N_i - N_i / 8; // 56 bits
    }
    else
    {
        taille_i = N_i; // 64 bits
    }

    // parcours du tableau de permutation
    for (int i = 0; i < taille_i; i++)
    {
        int k;
        if (type_c == 'D')
        { // permutation initiale
            k = permutationD[i];
        }
        else if (type_c == 'F')
        { // permutation finale
            k = permutationF[i];
        }
        else if (type_c == 'P')
        { // permutation pure
            k = permutationPure[i];
        }
        else if (type_c == 'K')
        { // permutation de la clé
            k = permutationCle[i];
        }
        else if (type_c == 'C')
        { // permutation compressive
            k = permutationCompressive[i];
        }
        else
        { // erreur
            k = -1;
        }
        // le bit k est déplacé à la position i
        if (serie_l & (1ul << k)) // bit k == 1 ?
        {
            seriePermut_l = seriePermut_l | (1ul << i); // 00..010..00
        }
        else
        {                                                // est ce vraiment utile ?
            seriePermut_l = seriePermut_l & ~(1ul << i); // 11..101..11
        }
    }
    return seriePermut_l;
}

unsigned long permuterExpansive(unsigned R0_i) // R0 = 32 bits
{
    unsigned long R0Expansive_l = 0;
    for (int i = 0; i < 3 * N_i / 4; i++)
    {
        int k = permutationExpansive[i];
        // le bit k est déplacé à la position i
        if (R0_i & (1ul << k)) // bit k == 1 ?
        {
            R0Expansive_l = R0Expansive_l | (1ul << i); // 00..010..00
        }
        else
        {
            R0Expansive_l = R0Expansive_l & ~(1ul << i); // 11..101..11
        }
    }
    return R0Expansive_l; // 48 bits = 3*N_i/4 = N_i-N_i/4
}

unsigned decalerCle(unsigned semiKey_i, int ronde_i)
{
    unsigned newSemiKey_i = 0;
    int k = decalageCle[ronde_i];
    for (int i = 0; i < 28; i++)
    {
        if (semiKey_i & (1ul << i))
        {
            newSemiKey_i = newSemiKey_i | (1ul << (i + k) % 28); // 00..010..00
        }
        else
        {
            newSemiKey_i = newSemiKey_i & ~(1ul << (i + k) % 28); // 11..101..11
        }
    }
    return newSemiKey_i;
}

// Décalage de la clé pendant la ronde
void calculerNouvelleCle(unsigned long *key_l, int ronde_i)
{
    unsigned RKey_i = *key_l & 0xFFFFFFF;
    unsigned LKey_i = (*key_l >> 28) & 0xFFFFFFF;
    RKey_i = decalerCle(RKey_i, ronde_i);
    LKey_i = decalerCle(LKey_i, ronde_i);

    *key_l = LKey_i & 0xFFFFFFF;
    *key_l = (*key_l << 28) | RKey_i;
}

unsigned fonctionF(unsigned R_i, unsigned long sousCle_l)
{
    // permutation expansive
    unsigned long RExpansive_l = permuterExpansive(R_i); // 48 bits

    // ou exclusif bloc puis clé
    unsigned long RCombi_l = RExpansive_l ^ sousCle_l;

    // Opération de substitution -> 8 blocs de 6 bits traités par 8 tableaux
    unsigned RSuivant_i = 0; // 32 bits

    for (int k = 0; k < 8; k++)
    {
        int d = 6 * (8 - k - 1);
        int i = 0;
        
        unsigned short tmp_s = (RCombi_l & (0x3FL << d)) >> d;
        // le premier et dernier bits forment un nb = ligne
        if (tmp_s & (1ul << 5))
            i += 2;
        if (tmp_s & (1ul << 0))
            i += 1;
        // les 4 bits du milieu forment un nb = colonne
        int j = (tmp_s & 0x1E) >> 1; // 1E = 00011110 = 30
        RSuivant_i <<= 4;
        RSuivant_i |= tableS[k][i][j];
    }
    // permutation pure
    RSuivant_i = permuter(RSuivant_i, 'P');

    return RSuivant_i;
}

bool verifierCle(unsigned long key_l)
{
    for (int i = 0; i < nbCleFaible_i; i++)
    {
        if (key_l == cleFaible[i])
        {
            return false;
        }
    }
    return true;
}

int main()
{
    // initialisation de la clé
    srand(time(NULL));

    unsigned long key_l = rand() % 0xFFFFFFFFFFFFFFFF; // 64 bits
    // vérification clé faible
    while (!verifierCle(key_l))
    {
        key_l = rand() % 0xFFFFFFFFFFFFFFFF; // 64 bits
        // key_l = 2048195009 & 0xFFFFFFFFFFFFFFFF; // 64 bits
    }
    // utilité des bits de parité ?
    // permutation de la clé en ignorant les bits de parité
    key_l = permuter(key_l, 'K'); // 56 bits

    int tailleTexte_i = strlen(texte_c);
    printf("taille texte = %d\n", tailleTexte_i);

    for (int imotif_i = 0; imotif_i < tailleTexte_i; imotif_i += 8) // /!\ gestion dernier, taille <8
    {
        // création du tableau de 64 bits à crypté
        // si le texte fait plus de 64 bits, le cryptage se fait en plusieurs fois
        unsigned long result_l = texte_c[imotif_i];
        for (int k = imotif_i; k < 8 + imotif_i; k++) // <- gestion si taille n'est pas un multiple de 8 (le dernier tour est hors du tableau)
        {
            result_l <<= 8;
            result_l |= texte_c[k];
        }

        // permutation initiale
        unsigned long newResult_l = permuter(result_l, 'D');

        for (int ronde_i = 0; ronde_i < 16; ronde_i++)
        {
            // partage en 2 bloc de 32 bits M = (L0,R0)
            unsigned R_i = newResult_l & 0xFFFFFFFF;
            unsigned L_i = (newResult_l >> 32) & 0xFFFFFFFF;

            calculerNouvelleCle(&key_l, ronde_i); // 56 bits
            // permutation compressive
            unsigned long sousCle_l = permuter(key_l, 'C'); // 48 bits

            unsigned RSuivant_i = fonctionF(R_i, sousCle_l);

            // concaténation
            newResult_l = R_i;
            newResult_l = (newResult_l << N_i / 2) | (L_i ^ RSuivant_i);
        }
        // permutation finale
        unsigned long serieFinal_l = permuter(newResult_l, 'F');
        printf("Série Permutation finale : ");
        afficher_bin(serieFinal_l, N_i);
        for (int k = 7; k >= 0; k--)
        {
            texte_c[k + imotif_i] = serieFinal_l & 0xFF;
            serieFinal_l >>= 8;
        }
        printf("petit texte crypté : %s\n", texte_c);
    }

    return 0;
}