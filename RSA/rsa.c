// GMP = GNU Multiple Precision Arithmetic Library = calcul arithmétique sur les entiers de grandes tailles
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <gmp.h> // librairie à installer

#include "liste.h"

#define TAILLE_BLOC 60

// Fonction pour générer un nombre premier
// premier = stockage du nb premier, state pour l'aléatoire, bits = nb de bit du nombre;
void genererNbPremier(mpz_t premier, gmp_randstate_t state, int bits_i)
{
    do
    {
        mpz_urandomb(premier, state, bits_i);
        mpz_setbit(premier, bits_i - 1);
    } while (mpz_probab_prime_p(premier, 25) == 0);
    gmp_printf("premier final = %Zd\n", premier);
}

// Fonction pour calculer le modulo inverse
// rst = a⁻¹ [modulo]
void modInverse(mpz_t result, const mpz_t a, const mpz_t modulo)
{
    mpz_t pgcd;
    mpz_init(pgcd);
    // calcul de l'expression de Bezout
    // pgcd(a, modulo) = a*result + modulo*y
    mpz_gcdext(pgcd, result, NULL, a, modulo);

    // pgcd == 1 ?
    if (mpz_cmp_ui(pgcd, 1) != 0)
    {
        fprintf(stderr, "a et modulo ne sont pas premiers entre eux\n");
        exit(1);
    }

    // result < 0 ? => result <- result + modulo
    if (mpz_cmp_ui(result, 0) < 0)
    {
        mpz_add(result, result, modulo);
    }
    mpz_clear(pgcd);
}

void charToAscii(char *texte_c, liste listeAscii)
{
    size_t len = strlen(texte_c);
    size_t positionActuelle = 0;

    mpz_t message;
    mpz_init(message);
    printf("len = %zu\n", len);

    while (positionActuelle < len)
    {
        size_t taille = (len - positionActuelle > TAILLE_BLOC)
                            ? TAILLE_BLOC
                            : (len - positionActuelle);
        mpz_import(message, taille, 1, sizeof(texte_c[0]), 0, 0, texte_c + positionActuelle);

        ajouterMaillon(listeAscii, message);
        positionActuelle += TAILLE_BLOC;
    }
}

void chiffrerListe(liste listeCryptee, mpz_t e, mpz_t n)
{
    if (estVide(listeCryptee))
    {
        return;
    }

    maillon courant = listeCryptee->tete;
    while (courant != NULL)
    {
        // mpz_set_ui(message, );
        mpz_powm(courant->cryptee, courant->cryptee, e, n);
        gmp_printf("Message chiffré: %Zd\n", courant->cryptee);
        courant = courant->suivant;
    }
}

void dechiffrerListe(liste liste, const mpz_t d, const mpz_t n)
{
    maillon courant = liste->tete;
    mpz_t decrypte;
    mpz_init(decrypte);

    while (courant)
    {
        mpz_powm(decrypte, courant->cryptee, d, n);

        size_t count;
        char *buffer_c = (char *)mpz_export(NULL, &count, 1, sizeof(char), 0, 0, decrypte);
        fwrite(buffer_c, sizeof(char), count, stdout);
        free(buffer_c);

        courant = courant->suivant;
    }

    mpz_clear(decrypte);
    printf("\n");
}

int main()
{
    // initialisation des variables GMP
    mpz_t p, q, n, phi, e, d, crypte, decrypte;
    mpz_inits(p, q, n, phi, e, d, crypte, decrypte, NULL);

    // aléatoire
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    // Initialise la graine (12345 ou t) du générateur de nombres aléatoires
    // Graine = nb utilisé pour initialiser le générateur de nombres aléatoires
    int t = clock();
    gmp_randseed_ui(state, t);

    char *texte_c = "Ceci est un texte qui va être chiffré pour vérifier la fonctionnalité de mon code";
    liste listeCryptee = initialiserListe();
    charToAscii(texte_c, listeCryptee);

    // Génération de p et q
    genererNbPremier(p, state, 256);
    genererNbPremier(q, state, 256);

    // Calcul de n = p*q
    mpz_mul(n, p, q);

    // calcul de phi(n) = (p-1)*(q-1)
    // Soustract° op1(mpz_t) - op2(ui) et stocke le résultat dans rop(mpz_t)
    mpz_sub_ui(p, p, 1);
    mpz_sub_ui(q, q, 1);
    mpz_mul(phi, p, q);

    // Choix de e -> source 65537
    mpz_set_ui(e, 65537);
    // Calcul de d = e⁽⁻¹⁾
    modInverse(d, e, phi);

    // chiffrement C = M^e mod n
    chiffrerListe(listeCryptee, e, n);

    // dechiffrement M = C^d mod n
    dechiffrerListe(listeCryptee, d, n);

    // Libération de la mémoire
    supprimerListe(listeCryptee);
    mpz_clears(p, q, n, phi, e, d, crypte, decrypte, NULL);
    gmp_randclear(state);

    return 0;
}