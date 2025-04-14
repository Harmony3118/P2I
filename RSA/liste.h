#ifndef LISTE_H
#define LISTE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <gmp.h> // librairie à installer

struct maillon_s
{
    mpz_t cryptee;
    struct maillon_s *suivant;
};
typedef struct maillon_s *maillon;

struct liste_s
{
    maillon tete;
};
typedef struct liste_s *liste;

liste initialiserListe();
void supprimerListe(liste l);
bool estVide(liste l);

maillon creerMaillon(mpz_t valeur);
void ajouterMaillon(liste l, mpz_t valeur);
void supprimerMaillon(liste l);

#endif