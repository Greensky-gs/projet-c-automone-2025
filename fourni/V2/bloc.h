/**
 * ProgC - Projet Automne 25-26 : Gestion de systèmes de fichiers
 * VERSION 2 = VERSION 1
 * Fichier : bloc.h
 * Module de gestion des blocs de données.
 * Fournit des fonctions pour créer, détruire, lire et écrire des blocs de données de taille fixe.
 **/
#ifndef __BLOC_H__
#define __BLOC_H__

// Taille d’un bloc en octets
#define TAILLE_BLOC        64

// Type public représentant un bloc de données
typedef unsigned char *tBloc;

/* V1
 * Crée et retourne un nouveau bloc de données.
 * Entrée : Aucune
 * Retour : le bloc créé ou NULL en cas de problème
 */
extern tBloc CreerBloc (void);

/* V1
 * Détruit un bloc de données.
 * Entrée : le bloc à détruire (libération mémoire allouée)
 * Retour : aucun
 */
extern void DetruireBloc(tBloc *pBloc);

/* V1
 * Copie dans un bloc les taille octets situés à l’adresse contenu.
 * Si taille est supérieure à la taille d’un bloc, seuls les TAILLE_BLOC premiers octets doivent être copiés.
 * Entrées : le bloc, l'adresse du contenu à copier et sa taille en octets
 * Retour : le nombre d'octets effectivement écrits dans le bloc
 */
extern long EcrireContenuBloc (tBloc bloc, unsigned char *contenu, long taille);

/* V1
 * Copie à l'adresse contenu, les taille octets stockés dans un bloc.
 * Si taille est supérieure à la taille d’un bloc, seuls les TAILLE_BLOC premiers octets doivent être copiés.
 * Entrées : le bloc, l'adresse contenu à laquelle recopier et la taille en octets du bloc
 * Retour : le nombre d'octets effectivement lus dans le bloc
 */
extern long LireContenuBloc(tBloc bloc, unsigned char *contenu, long taille);

#endif
