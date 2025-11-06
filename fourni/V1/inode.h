/**
 * ProgC - Projet Automne 25-26 : Gestion de systèmes de fichiers
 * VERSION 1
 * Fichier : inode.h
 * Module de gestion des inodes.
 * Fournit des fonctions pour créer, détruite et manipuler des inodes.
 **/
 #ifndef __INODE_H__
#define __INODE_H__

#include <time.h>
#include <stdio.h>

// Taille maximale du nom d'un fichier dans un système de fichiers
#define TAILLE_NOM_FICHIER 24

// Type représentant un inode
typedef struct sInode *tInode;

// Type représentant la nature d'un fichier dans un système de fichiers
// ORDINAIRE : Fichier ordinaire
// REPERTOIRE : Répertoire
// AUTRE : Autre type de fichier
typedef enum
{
    ORDINAIRE,
    REPERTOIRE,
    AUTRE
} natureFichier;

/* V1
 * Crée et retourne un inode.
 * Entrées : numéro de l'inode et le type de fichier qui y est associé
 * Retour : l'inode créé ou NULL en cas de problème
 */
extern tInode CreerInode(int numInode, natureFichier type);

/* V1
 * Détruit un inode.
 * Entrée : l'inode à détruire
 * Retour : aucun
 */
extern void DetruireInode(tInode *pInode);

/* V1
 * Récupère la date de dernier accès à un inode.
 * Entrée : l'inode pour lequel on souhaite connaître la date de dernier accès
 * Retour : la date de dernier accès à l'inode
 */
extern time_t DateDerAcces(tInode inode);

/* V1
 * Récupère la date de dernière modification d'u fichier associé à un inode.
 * Entrée : l'inode pour lequel on souhaite connaître la date de dernière modification du fichier associé
 * Retour : la date de dernière modification du fichier associé à l'inode
 */
extern time_t DateDerModifFichier(tInode inode);

/* V1
 * Récupère la date de dernière modification d'un inode.
 * Entrée : l'inode pour lequel on souhaite connaître la date de dernière modification
 * Retour : la date de dernière modification de l'inode
 */
extern time_t DateDerModif(tInode inode);

/* V1
 * Récupère le numéro d'un inode.
 * Entrée : l'inode pour lequel on souhaite connaître le numéro
 * Retour : le numéro de l'inode
 */
extern unsigned int Numero(tInode inode);

/* V1
 * Récupère la taille en octets du fichier associé à un inode.
 * Entrée : l'inode pour lequel on souhaite connaître la taille
 * Retour : la taille en octets du fichier associé à l'inode
 */
extern long Taille(tInode inode);

/* V1
 * Récupère le type du fichier associé à un inode.
 * Entrée : l'inode pour lequel on souhaite connaître le type de fichier associé
 * Retour : le type du fichier associé à l'inode
 */
extern natureFichier Type(tInode inode);

/* V1
 * Affiche les informations d'un inode
 * Entrée : l'inode dont on souhaite afficher les informations
 * Retour : aucun
 */
extern void AfficherInode(tInode inode);

/* V1
 * Copie dans un inode les taille octets situés à l’adresse contenu.
 * Si taille est supérieure à la taille d’un bloc, seuls les TAILLE_BLOC premiers octets doivent être copiés.
 * Entrées : l'inode, l'adresse de la zone à recopier et sa taille en octets
 * Retour : le nombre d'octets effectivement écrits dans l'inode ou -1 en cas d'erreur
 */
extern long LireDonneesInode1bloc(tInode inode, unsigned char *contenu, long taille);

/* V1
 * Copie à l'adresse contenu les taille octets stockés dans un inode.
 * Si taille est supérieure à la taille d’un bloc, seuls les TAILLE_BLOC premiers octets doivent être copiés.
 * Entrées : l'inode, l'adresse de la zone où recopier et la taille en octets de l'inode
 * Retour : le nombre d'octets effectivement lus dans l'inode ou -1 en cas d'erreur
 */
extern long EcrireDonneesInode1bloc(tInode inode, unsigned char *contenu, long taille);

#endif
