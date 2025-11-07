/**
* ProgC - Projet Automne 25-26 : Gestion de systèmes de fichiers
* VERSION 2 = VERSION 1
* Fichier : inode.c
* Module de gestion des inodes.
**/
#include "inode.h"
#include "bloc.h"
#include <time.h>
#include <stdlib.h>

// Nombre maximal de blocs dans un inode
#define NB_BLOCS_DIRECTS 10

// Définition d'un inode
struct sInode
{
	// Numéro de l'inode
	unsigned int numero;

	// Le type du fichier : ordinaire, répertoire ou autre
	natureFichier type;

	// La taille en octets du fichier
	long taille;

	// Les adresses directes vers les blocs (NB_BLOCS_DIRECTS au maximum)
	tBloc blocDonnees[NB_BLOCS_DIRECTS];

	// Les dates : dernier accès à l'inode, dernière modification du fichier
	// et de l'inode
	time_t dateDerAcces, dateDerModif, dateDerModifInode;
};

/* V1
* Crée et retourne un inode.
* Entrées : numéro de l'inode et le type de fichier qui y est associé
* Retour : l'inode créé ou NULL en cas de problème
*/
tInode CreerInode(int numInode, natureFichier type) {
	// Allocation dynamique d'un espace pour l'inode
	struct sInode * iNode = malloc(sizeof(struct sInode));
	if (iNode == NULL) {
		// Problème de création -> stderr
		fprintf(stderr, "CreerInode : Probleme creation");
		return NULL;
	}

	// Variables spécifiées
	iNode->numero = numInode;
	iNode->type = type;
	iNode->taille=0; // Taille à 0 par défaut

	// Variables par défaut
	time(&(iNode->dateDerAcces));
	time(&(iNode->dateDerModif));
	time(&(iNode->dateDerModifInode));

	// Création des NB_BLOCS_DIRECTS lors de la création de l'inode
	for (int k = 0; k < NB_BLOCS_DIRECTS; k++) {
		tBloc bloc = CreerBloc();
		if (bloc == NULL) {
			// problème sur un bloc = problème sur l'inode -> arrêt de la fonction
			fprintf(stderr, "CreerInode : probleme creaation");
			perror("Erreur lors de la creation des blocs de donnees");

			return NULL;
		}
		iNode->blocDonnees[k] = bloc;
	}

	return iNode;
}

/* V1
* Détruit un inode.
* Entrée : l'inode à détruire
* Retour : aucun
*/
void DetruireInode(tInode *pInode) {
	// D'abord détruire les NB_BLOCS_DIRECTS
	for (int k = 0; k < NB_BLOCS_DIRECTS; k++) {
		DetruireBloc(&(*pInode)->blocDonnees[k]);
	}
	// Libération et pointage sur NULL
	free(*pInode);
	*pInode = NULL;
}

// 3 fonctions statiques de modification des dates
static void ActualiserDateDerAccess(tInode inode) {
	time(&(inode->dateDerAcces));
}
static void ActualiserDateDerModif(tInode inode) {
	time(&(inode->dateDerModif));
}
static void ActualiserDateDerModifInode(tInode inode) {
	time(&(inode->dateDerModifInode));
}

/* V1
* Récupère la date de dernier accès à un inode.
* Entrée : l'inode pour lequel on souhaite connaître la date de dernier accès
* Retour : la date de dernier accès à l'inode
*/
time_t DateDerAcces(tInode inode) {
	// On récupère avant d'actualiser, autrement on perd l'information
	time_t date = inode->dateDerAcces;
	ActualiserDateDerAccess(inode);

	return date;
}

/* V1
* Récupère la date de dernière modification d'un inode.
* Entrée : l'inode pour lequel on souhaite connaître la date de dernière modification
* Retour : la date de dernière modification de l'inode
*/
time_t DateDerModif(tInode inode) {
	ActualiserDateDerAccess(inode);
	return inode->dateDerModifInode;
}

/* V1
* Récupère la date de dernière modification d'u fichier associé à un inode.
* Entrée : l'inode pour lequel on souhaite connaître la date de dernière modification du fichier associé
* Retour : la date de dernière modification du fichier associé à l'inode
*/
time_t DateDerModifFichier(tInode inode) {
	ActualiserDateDerAccess(inode);
	return inode->dateDerModif;
}

/* V1
* Récupère le numéro d'un inode.
* Entrée : l'inode pour lequel on souhaite connaître le numéro
* Retour : le numéro de l'inode
*/
unsigned int Numero(tInode inode) {
	ActualiserDateDerAccess(inode);
	return inode->numero;
}

/* V1
* Récupère la taille en octets du fichier associé à un inode.
* Entrée : l'inode pour lequel on souhaite connaître la taille
* Retour : la taille en octets du fichier associé à l'inode
*/
long Taille(tInode inode) {
	ActualiserDateDerAccess(inode);
	return inode->taille;
}

/* V1
* Récupère le type du fichier associé à un inode.
* Entrée : l'inode pour lequel on souhaite connaître le tyep de fichier associé
* Retour : le type du fichier associé à l'inode
*/
natureFichier Type(tInode inode) {
	ActualiserDateDerAccess(inode);
	return inode->type;
}

/* V1
* Affiche les informations d'un inode
* Entrée : l'inode dont on souhaite afficher les informations
* Retour : aucun
*/
void AfficherInode(tInode inode) {
	// On récupère la dernière date d'accès avant toute autre chose sinon on perd l'information
	time_t derAccess = DateDerAcces(inode);
	time_t derModifFichier = DateDerModifFichier(inode);
	time_t derModifInode = DateDerModif(inode);

	natureFichier type = Type(inode);
	char * typeText = type == ORDINAIRE ? "Ordinaire" : type == REPERTOIRE ? "Repertoire" : type == AUTRE ? "Autre" : "never";
	tBloc lecture = CreerBloc();
	LireDonneesInode1bloc(inode, lecture, TAILLE_BLOC);

	// Joli affichage sous forme d'objet javascript (sans couleur mais ça pourrait)
	printf("{\n    numero: %d\n    type: %d (%s)\n    taille: %ld\n    dernier access: %s\n    derniere modif. fichier: %s\n    derniere modif. inode: %s\n    contenu: %s\n}\n", Numero(inode), type, typeText, Taille(inode), ctime(&derAccess), ctime(&derModifFichier), ctime(&derModifInode), lecture);
	DetruireBloc(&lecture);
}

/* V1
* Copie dans un inode les taille octets situés à l’adresse contenu.
* Si taille est supérieure à la taille d’un bloc, seuls les TAILLE_BLOC premiers octets doivent être copiés.
* Entrées : l'inode, l'adresse de la zone à recopier et sa taille en octets
* Retour : le nombre d'octets effectivement écrits dans l'inode ou -1 en cas d'erreur
*/
long LireDonneesInode1bloc(tInode inode, unsigned char *contenu, long taille) {
	// On utilise simplement la fonction de lecture d'un bloc sur le premier bloc
	long octets_lus = LireContenuBloc(inode->blocDonnees[0], contenu, taille);
	ActualiserDateDerAccess(inode); // Modification de la date d'accès au fichier, pusiqu'il a été lu

	return octets_lus;
}

/* V1
* Copie à l'adresse contenu les taille octets stockés dans un inode.
* Si taille est supérieure à la taille d’un bloc, seuls les TAILLE_BLOC premiers octets doivent être copiés.
* Entrées : l'inode, l'adresse de la zone où recopier et la taille en octets de l'inode
* Retour : le nombre d'octets effectivement lus dans l'inode ou -1 en cas d'erreur
*/
long EcrireDonneesInode1bloc(tInode inode, unsigned char *contenu, long taille) {
	// On utlise simplement la fonction d'écriture sur le premier bloc
	long octets_ecrits = EcrireContenuBloc(inode->blocDonnees[0], contenu, taille);
	ActualiserDateDerModif(inode); // Modification de la date de modification du fichier car il a été écrit

	inode->taille = octets_ecrits; // La taille a également changé
	ActualiserDateDerModifInode(inode); // Donc on modifie la date de modification de l'inode
	return octets_ecrits;
	
}
