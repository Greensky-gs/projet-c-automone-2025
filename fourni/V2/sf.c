// Code par Greensky-gs (David Heslière)
/**
* ProgC - Projet Automne 25-26 : Gestion de systèmes de fichiers
* VERSION 2
* Fichier : sf.c
* Module de gestion d'un systèmes de fichiers (simulé)
**/

#include "sf.h"
#include "bloc.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Taille maximale du nom du SF (ou nom du disque)
#define TAILLE_NOM_DISQUE 24

// Définition du super-bloc
struct sSuperBloc
{
	// Le nom du disque (ou du système de fichiers) (avec le '\0')
	char nomDisque[TAILLE_NOM_DISQUE + 1];
	// La date de dernière modification du système de fichiers
	time_t dateDerModif;
};

// Type représentant le super-bloc
typedef struct sSuperBloc *tSuperBloc;

// Définition de la liste chaînée des inodes
struct sListeInodes
{
	// Références vers le premier et le dernier élément de la liste chaînée
	struct sListeInodesElement *premier, *dernier;
	// Nombre d'inodes dans la liste
	int nbInodes;
};

// Définition d'un élement de la liste chaînée des inodes dans le SF
struct sListeInodesElement
{
	// L'inode proprement dit
	tInode inode;
	// L'élément suivant dans la liste chaînée
	struct sListeInodesElement *suivant;
};

// Définition d'un système de fichiers (simplifié)
struct sSF
{
	// Le super-bloc
	tSuperBloc superBloc;
	// La liste chaînée des inodes
	struct sListeInodes listeInodes;
};

/* V2
*  Crée et retourne un super-bloc.
* Fonction non publique (static)
* Entrée : le nom du disque (ou du SF)
* Sortie : le super-bloc, ou NULL en cas de problème
*/
static tSuperBloc CreerSuperBloc(char nomDisque[]) {
	struct sSuperBloc * ptr = malloc(sizeof(struct sSuperBloc)); // Allocation
	if (ptr == NULL) {
		fprintf(stderr, "CreerSuperBloc : probleme creation\n");
		return NULL;
	}

	// Compteur pour copie du nom du disque (si jamais il dépasse la taille maximale)
	int i = 0;
	while (i < TAILLE_NOM_DISQUE && nomDisque[i] != '\0') {
		ptr->nomDisque[i] = nomDisque[i];
		i++;
	}
	ptr->nomDisque[i] = '\0';
	time(&(ptr->dateDerModif));

	return ptr;
}

/* V2
*  Détruit un super-bloc.
* Fonction non publique (static)
* Entrée : le super-bloc à détruire
* Sortie : aucune
*/
static void DetruireSuperBloc(tSuperBloc *pSuperBloc) {
	// Libération classique
	free(*pSuperBloc);
	*pSuperBloc = NULL;
}

/* V2
*  Affiche le contenu d'un super-bloc.
* Fonction non publique (static)
* Entrée : le super-bloc à afficher
* Sortie : aucune
*/
static void AfficherSuperBloc(tSuperBloc superBloc) {
	printf("Sf de nom %s, super bloc :\ntaille bloc = %d, date der modif: %s", superBloc->nomDisque, TAILLE_BLOC, ctime(&superBloc->dateDerModif));
}

/* V2
* Crée un nouveau système de fichiers.
* Entrée : nom du disque à associer au système de fichiers créé
* Retour : le système de fichiers créé, ou NULL en cas d'erreur
*/
tSF CreerSF (char nomDisque[]){
	struct sSF * syst = malloc(sizeof(struct sSF)); // Allocation
	if (syst == NULL) {
		fprintf(stderr, "CreerSF : probleme creation\n");
		perror("Erreur d'allocation de memoire pour le systeme");
		return NULL;
	}
	tSuperBloc superBloc = CreerSuperBloc(nomDisque); // Création du super-bloc
	if (superBloc == NULL) {
		fprintf(stderr, "CreerSF : probleme creation");
		perror("Erreur d'allocation de memoire pour le superBloc dans la creation du systeme\n");
		free(syst);
		return NULL;
	}

	// Initialisation avec des valeurs par défaut
	syst->superBloc = superBloc;
	syst->listeInodes.nbInodes = 0;
	syst->listeInodes.dernier = NULL;
	syst->listeInodes.premier = NULL;

	return syst;
}

/* V2
* Détruit un système de fichiers et libère la mémoire associée.
* Entrée : le SF à détruire
* Sortie : aucune
*/
void DetruireSF(tSF *pSF) {
	DetruireSuperBloc(&(*pSF)->superBloc); // On commence par détruire le super-bloc

	if ((*pSF)->listeInodes.nbInodes > 0) {
		struct sListeInodesElement ** suivant = &(*pSF)->listeInodes.premier; // Double pointeur pour libérer à la fois le contenu et le contenant
		struct sListeInodesElement * temp = NULL; // Variable temporaire utilisée dans la suite

		while (*suivant != NULL) { // Libération itérative
			struct sListeInodesElement * supprimer = *suivant; // On stocke le pointeur (contenant) à libérer par la suite
  			DetruireInode(&((*suivant)->inode)); // On détruit l'inode correspondant
			temp = (*suivant)->suivant; // On stocke la suite avant de la libérer
			free(supprimer); // On libère le contenant

			// Passage au suivant
			if (temp == NULL) {
				*suivant = NULL;
			} else {
				suivant = &temp;
			}
		}
	}

	free(*pSF);
	*pSF = NULL;
}

/* V2
* Affiche les informations relative à un système de fichiers i.e;
* le contenu du super-bloc et celui des différents inodes du SF
* Entrée : le SF à afficher
* Sortie : aucune
*/
void AfficherSF (tSF sf){
	// On affiche le super-bloc
	AfficherSuperBloc(sf->superBloc);

	printf("Inodes :\n");
	if (sf->listeInodes.nbInodes > 0) {
		if (sf->listeInodes.premier == NULL) {
			// Cette situation est supposée ne jamais arriver, mais si elle arrive on est mal, donc on vérifie
			perror("AfficherSF : Erreur avec SF car premier element non defini");
			return;
		}

		int i = 0;
		struct sListeInodesElement * suivant = (sf->listeInodes.premier);
		while (suivant != NULL) { // Affichage itératif
			AfficherInode(suivant->inode);
	
			suivant = suivant->suivant;
			i++;
		}
	} else {
		printf("Aucun inode\n");
	}

	printf("---------------[ FIN SF ]---------------\n");
}

static struct sListeInodesElement * CreerListeInodeElement(tInode inode, struct sListeInodesElement * suivant) {
	struct sListeInodesElement * element = malloc(sizeof(struct sListeInodesElement));
	if (element == NULL) {
		perror("CreerListeInodesElement : erreur allocation");
		return NULL;
	}

	element -> inode = inode;
	element -> suivant = suivant;

	return element;
}

/* V2
* Ecrit un fichier d'un seul bloc dans le système de fichiers.
* Entrées : le système de fichiers, le nom du fichier (sur disque) et son type dans le SF (simulé)
* Sortie : le nombre d'octets effectivement écrits, -1 en cas d'erreur.
*/
long Ecrire1BlocFichierSF(tSF sf, char nomFichier[], natureFichier type) {
	FILE * fichier = fopen(nomFichier, "rb");
	if (fichier == NULL) {
		perror("Ecrire1BlocFichierSF : Erreur ouverture");
		return -1;
	}

	unsigned char chaine[TAILLE_BLOC] = {0};
	long resultatLecture = fread(chaine, sizeof(unsigned char), TAILLE_BLOC, fichier);

	if (resultatLecture == EOF) {
		perror("Ecrire1BlocFichierSF : Erreur lecture");
		fclose(fichier);
		return -1;
	}

	if (sf -> listeInodes.nbInodes > 0) {
		EcrireDonneesInode1bloc(sf -> listeInodes.premier -> inode, chaine, resultatLecture);
	} else {
		tInode inode = CreerInode(1, type);
		if (inode == NULL) {
			perror("Ecrire1BlocFichierSF : erreur allocation inode");
			fclose(fichier);
			return -1;
		}

		EcrireDonneesInode1bloc(inode, chaine, resultatLecture);

		sf -> listeInodes.nbInodes = 1;
		struct sListeInodesElement * element = CreerListeInodeElement(inode, NULL);
		if (element == NULL) {
			perror("Ecrire1BlocFichierSF : Erreur allocation ellement");
			DetruireInode(&inode);
			fclose(fichier);
			return -1;
		}

		sf -> listeInodes.premier = element;
		sf -> listeInodes.dernier = element;
	}

	return resultatLecture;
}
