#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <gmp.h> // librairie à installer

#include "liste.h"

maillon creerMaillon(mpz_t valeur)
{
    maillon nouveau = malloc(sizeof(struct maillon_s));
    if (!nouveau)
    {
        perror("Erreur allocation maillon");
        exit(1);
    }
    mpz_init_set(nouveau->cryptee, valeur);
    nouveau->suivant = NULL;
    return nouveau;
}

void ajouterMaillon(liste liste, mpz_t valeur)
{
    printf("Ajout d'un maillon\n");
    maillon nouveau = creerMaillon(valeur);
    printf("Ok131\n");

    if (liste->tete == NULL)
    {
        liste->tete = nouveau;
        return;
    }
    printf("Ok132\n");

    maillon courant = liste->tete;
    while (courant->suivant != NULL)
    {
        courant = courant->suivant;
    }
    courant->suivant = nouveau;
    printf("Ok133\n");
}

void supprimerMaillon(liste liste)
{
    // si la liste est vide
    if (liste->tete == NULL)
    {
        return;
    }
    // sinon, on supprime le premier maillon
    maillon tmp = liste->tete->suivant;
    mpz_clear(liste->tete->cryptee);
    free(liste->tete);
    liste->tete = tmp;
    return;
}

liste initialiserListe()
{
    liste l = malloc(sizeof(struct liste_s));
    l->tete = NULL;
    return l;
}

void supprimerListe(liste l)
{
    while (l->tete != NULL)
    {
        supprimerMaillon(l);
    }
    free(l);
}

bool estVide(liste l)
{
    return l->tete == NULL;
}