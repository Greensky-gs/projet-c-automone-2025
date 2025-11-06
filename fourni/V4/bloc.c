/**
 * ProgC - Projet Automne 25-26 : Gestion de systèmes de fichiers
 * VERSION 4 = VERSION 3
 * Fichier : bloc.c
 * Module de gestion des blocs de données.
 **/

#include "bloc.h"

// Dans le .h : typedef unsigned char *tBloc;

/* V1
 * Crée et retourne un nouveau bloc de données.
 * Entrée : Aucune
 * Retour : le bloc créé ou NULL en cas de problème
 */
tBloc CreerBloc (void) {
  // A COMPLETER
}

/* V1
 * Détruit un bloc de données.
 * Entrée : le bloc à détruire (libération mémoire allouée)
 * Retour : aucun
 */
void DetruireBloc(tBloc *pBloc){
  // A COMPLETER
}

/* V1
 * Copie dans un bloc les taille octets situés à l’adresse contenu.
 * Si taille est supérieure à la taille d’un bloc, seuls les TAILLE_BLOC premiers octets doivent être copiés.
 * Entrées : le bloc, l'adresse du contenu à copier et sa taille en octets
 * Retour : le nombre d'octets effectivement écrits dans le bloc
 */
long EcrireContenuBloc (tBloc bloc, unsigned char *contenu, long taille){
  // A COMPLETER
}

/* V1
 * Copie à l'adresse contenu, les taille octets stockés dans un bloc.
 * Si taille est supérieure à la taille d’un bloc, seuls les TAILLE_BLOC premiers octets doivent être copiés.
 * Entrées : le bloc, l'adresse contenu à laquelle recopier et la taille en octets du bloc
 * Retour : le nombre d'octets effectivement lus dans le bloc
 */
long LireContenuBloc(tBloc bloc, unsigned char *contenu, long taille){
  // A COMPLETER
}

/* V3
 * Sauvegarde les données d'un bloc en les écrivant dans un fichier (sur disque).
 * Entrées : le bloc à sauvegarder, sa taille en octets, le nom du fichier cible
 * Retour : 0 en cas de succès, -1 en cas d'erreur
 */
int SauvegarderBloc(tBloc bloc, long taille, FILE *fichier){
  // A COMPLETER
}

/* V3
 * Charge dans un bloc les données lues dans un fichier (sur disque).
 * Entrées : le bloc dans lequel charger, la taille en octets du fichier contenant les données, son nom (sur le disque))
 * Retour : 0 en cas de succès, -1 en cas d'erreur
 */
int ChargerBloc(tBloc bloc, long taille, FILE *fichier){
  // A COMPLETER
}
