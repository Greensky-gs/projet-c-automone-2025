/**
 * ProgC - Projet Automne 25-26 : Gestion de systèmes de fichiers
 * VERSION 4
 * Fichier : repertoire.h
 * Module de gestion d'un répertoire d'un systèmes de fichiers (simulé)
 * Fournit des fonctions pour créer, détruire et manipuler des répertoires.
 **/

#ifndef __REPERTOIRE_H__
#define __REPERTOIRE_H__

#include "inode.h"
#include <stdbool.h>

// Définition d'une entrée de répertoire
struct sEntreesRepertoire
{
  // Le nom du fichier
  char nomEntree[TAILLE_NOM_FICHIER+1];
  // Le numéro d'inode associé
  unsigned int numeroInode;
};

// Type représentant une entrée de répertoire
typedef struct sEntreesRepertoire *tEntreesRepertoire;

// Type public représentant un répertoire
typedef struct sRepertoire *tRepertoire;

/* V4
 * Crée un nouveau répertoire.
 * Entrée : aucune
 * Sortie : le répertoire créé, ou NULL si problème
 */
extern tRepertoire CreerRepertoire(void);

/* V4
 * Détruit un répertoire et libère la mémoire associée.
 * Entrée : le répertoire à détruire
 * Sortie : aucune
 */
extern void DetruireRepertoire(tRepertoire *pRep);

/* V4
 * Écrit une entrée dans un répertoire.
 * Si l'entrée existe déjà dans le répertoire, le numéro d'inode associé est mis à jour.
 * Si l'entrée n'existe pas, elle est ajoutée dans le répertoire.
 * Entrées : le répertoire destination, le nom de l'entrée à écrire,
 *           le numéro d'inode associé à l'entrée
 * Retour : 0 si l'entrée est écrite avec succès, -1 en cas d'erreur
 */
extern int EcrireEntreeRepertoire(tRepertoire rep, char nomEntree[], unsigned int numeroInode);

/* V4
 * Lit le contenu d'un répertoire depuis un inode.
 * Entrées : le répertoire mis à jour avec le contenu lu,
 *           l'inode source.
 * Retour : 0 si le répertoire est lu avec succès, -1 en cas d'erreur
 */
extern int LireRepertoireDepuisInode(tRepertoire *pRep, tInode inode);

/* V4
 * Écrit un répertoire dans un inode.
 * Entrées : le répertoire source et l'inode destination
 * Sortie : 0 si le répertoire est écrit avec succès, -1 en cas d'erreur
 */
extern int EcrireRepertoireDansInode(tRepertoire rep, tInode inode);

/* V4
 * Récupère les entrées contenues dans un répertoire.
 * Entrées : le répertoire source, un tableau récupérant les numéros d'inodes des entrées du rpertoire
 * Retour : le nombre d'entrées dans le répertoire
 */
extern int EntreesContenuesDansRepertoire(tRepertoire rep,  struct sEntreesRepertoire tabNumInodes[]);

/* V4
 * Compte le nombre d'entrées d'un répertoire.
 * Entrée : le répertoire source
 * Retour : le nombre d'entrées du répertoire
 */
extern int NbEntreesRepertoire(tRepertoire rep);

/* V4
 * Affiche le contenu d'un répertoire.
 * Entrée : le répertoire à afficher
 * Retour : aucun
 */
extern void AfficherRepertoire(tRepertoire rep);

#endif // __REPERTOIRE_H__
