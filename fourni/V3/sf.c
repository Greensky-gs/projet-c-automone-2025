// Code par Greensky-gs (David Heslière)
/**
* ProgC - Projet Automne 25-26 : Gestion de systèmes de fichiers
* VERSION 3
* Fichier : sf.c
* Module de gestion d'un systèmes de fichiers (simulé)
**/

#include "sf.h"
#include "bloc.h"
#include "inode.h"
#include <stdio.h>
#include <stdlib.h>

// Taille maximale du nom du SF (ou nom du disque)
#define TAILLE_NOM_DISQUE 24

// Définition du super-bloc
struct sSuperBloc
{
	// Le nom du disque (ou du système de fichiers) (avec le '\0')
	char nomDisque[TAILLE_NOM_DISQUE+1];
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

// Définirion d'un système de fichiers (simplifié)
struct sSF
{
	// Le super-bloc
	tSuperBloc superBloc;
	// La liste chaînée des inodes
	struct sListeInodes listeInodes;
};

/* V2
*  Crée et retourne un super-bloc.
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
* Entrée : le super-bloc à afficher
* Sortie : aucune
*/
static void AfficherSuperBloc(tSuperBloc superBloc) {
	printf("{      Super Bloc\n    nom disque: %s\n    Dernière date de modification: %s\n}\n", superBloc->nomDisque, ctime(&superBloc->dateDerModif));
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
	syst->superBloc=superBloc;
	syst->listeInodes.nbInodes = 0;
	syst->listeInodes.dernier = NULL;
	syst->listeInodes.premier = NULL;
	
	return syst;
}

// Puisque ce code doit être utilisé dans deux fonctions différentes, on le factorise
static void libererListeInodesSF(tSF * pSF) {
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

/* V2
* Détruit un système de fichiers et libère la mémoire associée.
* Entrée : le SF à détruire
* Sortie : aucune
*/
void DetruireSF(tSF *pSF) {
	DetruireSuperBloc(&(*pSF)->superBloc); // On commence par détruire le super-bloc
	
	if ((*pSF)->listeInodes.nbInodes > 0) {
		libererListeInodesSF(pSF);
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
	printf("===========[SF %s]===========\nSuper bloc :\n", sf->superBloc->nomDisque);
	
	// On affiche le super-bloc
	AfficherSuperBloc(sf->superBloc);
	
	if (sf->listeInodes.nbInodes > 0) {
		if (sf->listeInodes.premier == NULL) {
			// Cette situation est supposée ne jamais arriver, mais si elle arrive on est mal, donc on vérifie
			perror("AfficherSF : Erreur avec SF car premier element non defini");
			return;
		}
		
		int i = 0;
		struct sListeInodesElement * suivant = (sf->listeInodes.premier);
		while (suivant != NULL) { // Affichage itératif
			printf("-------------[Inode %d]-------------\n", i);
			AfficherInode(suivant->inode);
			
			suivant = suivant->suivant;
			i++;
		}
	} else {
		printf("Aucun inode\n");
	}
	
	printf("================================\n");
}

/* V2
* Ecrit un fichier d'un seul bloc dans le système de fichiers.
* Entrées : le système de fichiers, le nom du fichier (sur disque) et son type dans le SF (simulé)
* Sortie : le nombre d'octets effectivement écrits, -1 en cas d'erreur.
*/
long Ecrire1BlocFichierSF(tSF sf, char nomFichier[], natureFichier type) {
	tInode inode = CreerInode(sf->listeInodes.nbInodes, type); // Création d'un inode
	if (inode == NULL) {
		perror("Ecrire1BlocFichierSF : Erreur creation");
		return -1;
	}
	
	FILE * fichier = fopen(nomFichier, "rb"); // Ouverture du fichier
	if (fichier == NULL) {
		DetruireInode(&inode);
		perror("Ecrire1BlocFichierSF : Erreur acces fichier");
		return -1;
	}
	
	unsigned char contenu[TAILLE_BLOC + 1] = {0}; // Allocation d'un tableau vide qui contiendra le contenu du fichier
	size_t resultatLecture = fread(contenu, 1, TAILLE_BLOC, fichier);
	if (resultatLecture == 0 && ferror(fichier)) { // La lecture n'a rien donné ET une erreur a eu lieu
		perror("Ecrire1BlocFichierSF : Erreur lecture fichier. Abandon.");
		DetruireInode(&inode);
		fclose(fichier);
		return -1;
	}
	
	long ecrits = EcrireDonneesInode1bloc(inode, contenu, resultatLecture); // Écriture dans contenu
	sf->listeInodes.nbInodes++;
	
	struct sListeInodesElement * ptr = malloc(sizeof(struct sListeInodesElement)); // Allocation d'un nouvel élément de liste chainée (pourrait être factorisé en fonction statique, mais pour l'instant ce n'est ni nécessaire ni demandé)
	if (ptr == NULL) {
		perror("Ecrire1BlocFichierSF : Erreur creation element liste chainee");
		DetruireInode(&inode);
		fclose(fichier);
		return -1;
	}
	
	ptr->inode = inode;
	ptr->suivant = NULL;
	
	if (sf->listeInodes.premier == NULL) { // On se place dans le cas où la liste n'a pas été initialisée
		sf->listeInodes.premier = ptr;
		sf->listeInodes.dernier = sf->listeInodes.premier;
	} else { // On rajoute un élément
		sf->listeInodes.dernier->suivant = ptr;
		sf->listeInodes.dernier = ptr;
	}
	
	fclose(fichier);
	
	return ecrits;
}

/* V3
* Ecrit un fichier (d'un nombre de blocs quelconque) dans le système de fichiers.
* Si la taille du fichier à écrire dépasse la taille maximale d'un fichier dans le SF(10 x 64 octets),
* seuls les 640 premiers octets seront écrits dans le système de fichiers.
* Entrées : le système de fichiers, le nom du fichier (sur disque) et son type dans le SF (simulé)
* Sortie : le nombre d'octets effectivement écrits, -1 en cas d'erreur.
*/
long EcrireFichierSF(tSF sf, char nomFichier[], natureFichier type) {
	tInode inode = CreerInode(sf->listeInodes.nbInodes, type);
	
	if (inode == NULL) {
		perror("EcrireFichierSF : Erreur creation inode");
		return -1;
	}
	FILE * flux = fopen(nomFichier, "rb");
	if (flux == NULL) {
		perror("EcritureFichierSF : Erreur ouverture fichier");
		DetruireInode(&inode);
		return -1;
	}
	
	unsigned char * contenu = malloc(TAILLE_BLOC * 10 + 1); // Le 10 est une constante magique car NB_BLOCS_DIRECTS est inaccessible
	if (contenu == NULL) {
		perror("EcritureFichierSF : Erreur allocation memoire tampon");
		DetruireInode(&inode);
		fclose(flux);
		return -1;
	}
	
	size_t bitsLu = fread(contenu, sizeof(unsigned char), TAILLE_BLOC * 10, flux);
	if (bitsLu == 0 && ferror(flux)) {
		perror("EcritureFichierSF : Erreur lecture");
		DetruireInode(&inode);
		fclose(flux);
		free(contenu);
		return -1;
	}
	
	int res = EcrireDonneesInode(inode, contenu, bitsLu, 0);
	if (res == -1) {
		perror("EcritureFichierSF : Erreur ecriture dans inode");
		DetruireInode(&inode);
		fclose(flux);
		free(contenu);
		return -1;
	}
	
	struct sListeInodesElement * element = malloc(sizeof(struct sListeInodesElement));
	if (element == NULL) {
		perror("EcritureFichierSF : Erreur allocation nouvel inode");
		DetruireInode(&inode);
		fclose(flux);
		free(contenu);
		return -1;
	}
	element->inode = inode;
	element->suivant = NULL;
	
	if (sf->listeInodes.nbInodes == 0) {
		sf->listeInodes.premier = element;
		sf->listeInodes.dernier = element;
	} else {
		sf->listeInodes.dernier->suivant = element;
		sf->listeInodes.dernier = element;
	}
	sf->listeInodes.nbInodes++;
	
	free(contenu);
	fclose(flux);
	
	return res;
}

// Fonction statique qui calcule la taille d'une chaine de caractères
static int tailleStr(char * entree) {
	// Lol une implémentation de strlen, dans le doute je l'ai pas utilisé et faite moi-même
	int i = 0;
	while (entree[i] != '\0') { // Btw, on part du principe que ce texte fini à un moment donné
		i++;
	}
	return i;
}

/* V3
* Sauvegarde un système de fichiers dans un fichier (sur disque).
* Entrées : le système de fichiers, le nom du fichier sauvegarde (sur disque)
* Sortie : 0 en cas de succèe, -1 en cas d'erreur
*/
int SauvegarderSF(tSF sf, char nomFichier[]) {
	FILE * flux = fopen(nomFichier, "w+b");
	if (flux == NULL) {
		perror("SauvegarderSF : Erreur ouverture fichier");
		return -1;
	}
	// On va d'abord écrire les informations du superbloc, car elles peuvent tenir des lignes
	char tampon[TAILLE_NOM_DISQUE + 10] = {0}; // On met un certain nombre mais en réalité moins devrait suffire
	sprintf(tampon, "%ld\n", sf->superBloc->dateDerModif);
	int res = fputs(tampon, flux);
	if (res == EOF) {
		perror("SauvegarderSF : Erreur Ecriture superBloc (date)");
		fclose(flux);
		return -1;
	}
	// On va également stocker le nombre de caractères du nom du superBloc
	sprintf(tampon, "%d\n", tailleStr(sf->superBloc->nomDisque));
	res = fputs(tampon, flux);	
	if (res == EOF) {
		perror("SauvegarderSF : Erreur Ecriture superBloc (taille nom)");
		fclose(flux);
		return -1;
	}
	
	sprintf(tampon, "%s\n", sf->superBloc->nomDisque); // On peut faire ça parce que tampon est de taille supérieure à celle de la chaine
	res = fputs(tampon, flux); // Btw, nomDisque est supposément nul-terminé
	if (res == EOF) {
		perror("SauvegarderSF : Erreur Ecriture superBloc (nom)");
		fclose(flux);
		return -1;
	}
	
	// Maintenant on rigole, on va enregistrer tous les inodes
	// D'abord le nombre d'inodes
	sprintf(tampon, "%d\n", sf->listeInodes.nbInodes);
	res = fputs(tampon, flux);
	if (res == EOF) {
		perror("SauvegarderSF : Erreur Ecriture superBloc (nombre d'inodes)");
		fclose(flux);
		return -1;
	}
	if (sf->listeInodes.nbInodes > 0) {
		struct sListeInodesElement * element = sf->listeInodes.premier;
		while (element != NULL) {
			int resultat = SauvegarderInode(element->inode, flux);
			if (resultat == -1) {
				perror("SauvegarderSF : Erreur ecriture inode");
				fclose(flux);
				return -1;
			}
			// On ajoute un retour à la ligne (ça fonctionne pas sinon :])
			res = fputs("\n", flux);
			if (res == EOF) { // Vraiment pas de chance (pour un caractère sérieux)
				perror("SauvegarderSF : Erreur ecriture retour ligne");
				fclose(flux);
				return -1;
			}
			element = element->suivant;
		}
	}
	
	fclose(flux);
	return 0;
}

/* V3
* Restaure le contenu d'un système de fichiers depuis un fichier sauvegarde (sur disque).
* Entrées : le système de fichiers où restaurer, le nom du fichier sauvegarde (sur disque)
* Sortie : 0 en cas de succèe, -1 en cas d'erreur
*/
int ChargerSF(tSF *pSF, char nomFichier[]) {
	// On va faire exactement pareil que SauvegarderSF, dans le même ordre, mais en lecture
	FILE * flux = fopen(nomFichier, "rb");
	if (flux == NULL) {
		perror("ChargerSF : Erreur ouverture fichier");
		return -1;
	}
	
	// On lit par un fscanf, pas envie de devoir parser un entier dans une chaine de caractères
	int tailleNomDisque;
	int scanned = fscanf(flux, "%ld\n%d\n", &(*pSF)->superBloc->dateDerModif, &tailleNomDisque);
	if (scanned < 2) {
		perror("ChargerSF : Erreur lecture fichier (date&taille nom disque)");
		fclose(flux);
		return -1;
	}
	
	char tamponNom[TAILLE_NOM_DISQUE + 1] = {0};
	// int res = fread((*pSF)->superBloc->nomDisque, sizeof(unsigned char), tailleNomDisque, flux);
	// La version ci-dessus ne fonctionnant pas pour une raison obscure (elle ne copie pas les bits lus à l'adresse spécifiée), on va faire du copiage caractère par caractère
	int res = fread(tamponNom, sizeof(unsigned char), tailleNomDisque, flux);
	for (int i = 0; i < tailleNomDisque; i++) {
		(*pSF)->superBloc->nomDisque[i] = tamponNom[i];
	}
	(*pSF)->superBloc->nomDisque[tailleNomDisque] = '\0';
	
	if (res == 0 && ferror(flux)) {
		perror("ChargerSF : Erreur lecture fichier (nom disque)");
		fclose(flux);
		return -1;
	}
	int matches = fscanf(flux, "%d\n", &(*pSF)->listeInodes.nbInodes);
	if (matches < 1) {
		perror("ChargerSF : Erreur lecture de fichier (nombre d'inodes)");
		fclose(flux);
		return -1;
	}

	(*pSF)->listeInodes.premier = NULL;
	(*pSF)->listeInodes.dernier = NULL;
	
	int i = 0;
	int erreur = 0;
	while (i < (*pSF)->listeInodes.nbInodes && !erreur) {
		tInode inode = CreerInode(i, ORDINAIRE);
		if (inode == NULL) {
			erreur = 1;
			continue;
		}
		int resultatCharge = ChargerInode(&inode, flux);
		if (resultatCharge == -1) {
			DetruireInode(&inode);
			erreur = 1;
			continue;
		}

		struct sListeInodesElement * element = malloc(sizeof(struct sListeInodesElement));
		if (element == NULL) {
			DetruireInode(&inode);
			erreur = 1;
			continue;
		}
		element->inode = inode;
		element->suivant = NULL;

		if (i == 0) {
			(*pSF)->listeInodes.premier = element;
			(*pSF)->listeInodes.dernier = element;
		} else {
			(*pSF)->listeInodes.dernier->suivant = element;
			(*pSF)->listeInodes.dernier = element;
		}
		i++;
	}
	if (erreur == 1) {
		perror("ChargerSF : Erreur chargement d'un inode");
		fclose(flux);
		if (i > 0) {
			libererListeInodesSF(pSF);
		}
		return -1;
	}
	return 0;
}
