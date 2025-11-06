/**
 * ProgC - Projet Automne 25-26 : Gestion de systèmes de fichiers
 * VERSION 2
 * Fichier : sf.h
 * Module de gestion d'un systèmes de fichiers (simulé)
 * Fournit des fonctions pour créer, détruire et manipuler un système de fichiers (simulé).
 **/

#ifndef __SF_H__
#define __SF_H__

#include <stdbool.h>
#include "inode.h"

// Type (public) représentant un système de fichiers (SF)
typedef struct sSF *tSF;

// Type (public) représentant un super-bloc.
typedef struct sSuperBloc *tSuperBloc;

/* V2
 * Crée un nouveau système de fichiers.
 * Entrée : nom du disque à associer au système de fichiers créé
 * Retour : le système de fichiers créé, ou NULL en cas d'erreur
 */
extern tSF CreerSF (char nomDisque[]);

/* V2
 * Détruit un système de fichiers et libère la mémoire associée.
 * Entrée : le SF à détruire
 * Sortie : aucune
 */
extern void DetruireSF(tSF *pSF);

/* V2
 * Affiche les informations relative à un système de fichiers i.e;
 * le contenu du super-bloc et celui des différents inodes du SF
 * Entrée : le SF à afficher
 * Sortie : aucune
 */
extern void AfficherSF (tSF sf);

/* V2
 * Ecrit un fichier d'un seul bloc dans le système de fichiers.
 * Entrées : le système de fichiers, le nom du fichier (sur disque) et son type dans le SF (simulé)
 * Sortie : le nombre d'octets effectivement écrits, -1 en cas d'erreur.
 */
extern long Ecrire1BlocFichierSF(tSF sf, char nomFichier[], natureFichier type);

#endif
