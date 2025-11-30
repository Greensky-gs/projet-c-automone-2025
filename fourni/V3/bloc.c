// Code par Greensky-gs (David Heslière)
/**
* ProgC - Projet Automne 25-26 : Gestion de systèmes de fichiers
* VERSION 3
* Fichier : bloc.c
* Module de gestion des blocs de données.
**/

#include "bloc.h"
#include <stdlib.h>
#include <stdio.h>

// Dans le .h : typedef unsigned char *tBloc;

/* V1
* Crée et retourne un nouveau bloc de données.
* Entrée : Aucune
* Retour : le bloc créé ou NULL en cas de problème
*/
tBloc CreerBloc (void) {
	// Allocation de TAILLE_BLOC octets
	tBloc ref = calloc(sizeof(unsigned char), TAILLE_BLOC); // On utilise calloc pour avoir des bits initialisés à 0, c'est mieux
	if (ref == NULL) {
		// Erreur dans stderr
		fprintf(stderr, "CreerBloc : probleme creation");
		return NULL;
	}
	return ref;
}

/* V1
* Détruit un bloc de données.
* Entrée : le bloc à détruire (libération mémoire allouée)
* Retour : aucun
*/
void DetruireBloc(tBloc *pBloc) {
	// Libération et pointage vers NULL
	free(*pBloc);
	*pBloc = NULL;
}

/* V1
* Copie dans un bloc les taille octets situés à l’adresse contenu.
* Si taille est supérieure à la taille d’un bloc, seuls les TAILLE_BLOC premiers octets doivent être copiés.
* Entrées : le bloc, l'adresse du contenu à copier et sa taille en octets
* Retour : le nombre d'octets effectivement écrits dans le bloc
*/
long EcrireContenuBloc (tBloc bloc, unsigned char *contenu, long taille) {
	long i = 0;
	// S'arrête quand i dépasse la taille taille ou la taille d'un bloc
	while (i < taille && i < TAILLE_BLOC) {
		bloc[i] = contenu[i];
		i++;
	}

	// I vaut donc l'écriture effective
	return i;
}

/* V1
* Copie à l'adresse contenu, les taille octets stockés dans un bloc.
* Si taille est supérieure à la taille d’un bloc, seuls les TAILLE_BLOC premiers octets doivent être copiés.
* Entrées : le bloc, l'adresse contenu à laquelle recopier et la taille en octets du bloc
* Retour : le nombre d'octets effectivement lus dans le bloc
*/
long LireContenuBloc(tBloc bloc, unsigned char *contenu, long taille) {
	long i = 0;
	// S'arrête quand i dépasse la taille taille ou la taille d'un bloc
	while (i < taille && i < TAILLE_BLOC) {
		*(contenu + i) = bloc[i];
		i++;
	}
	// i vaut donc la lecture effective
	return i;
}

/* V3
* Sauvegarde les données d'un bloc en les écrivant dans un fichier (sur disque).
* Entrées : le bloc à sauvegarder, sa taille en octets, le nom du fichier cible
* Retour : 0 en cas de succès, -1 en cas d'erreur
*/
int SauvegarderBloc(tBloc bloc, long taille, FILE *fichier){
	unsigned char lecture[TAILLE_BLOC + 1] = {0}; // On créé un tableau car on va avoir besoin de rajouter le caractère nul, car le contenu pourrait être non-nul terminé
	// Remarque : Le caractère nul est ajouté par défaut à l'initialisation, et il ne sera jamais écrasé

	LireContenuBloc(bloc, lecture, taille); // On remarque ici que le contenu copié maximal est TAILLE_BLOC puisque c'est le maximum de LireContenuBloc

	long result = fputs((const char *)lecture, fichier);
	if (result == EOF) {
		perror("SauvegarderBloc : Erreur ecriture");
		return -1;
	}

	// On ne ferme pas le fichier, car il a été ouvert par l'appellant, on peut donc supposer qu'il sera fermé par l'appellant
	return 0;
}

/* V3
* Charge dans un bloc les données lues dans un fichier (sur disque).
* Entrées : le bloc dans lequel charger, la taille en octets du fichier contenant les données, son nom (sur le disque))
* Retour : 0 en cas de succès, -1 en cas d'erreur
*/
int ChargerBloc(tBloc bloc, long taille, FILE *fichier){
	unsigned char lecture[TAILLE_BLOC] = {0}; // On crée un tableau vide de taille exactement TAILLE_BLOC car on pourra spécifier la taille plus tard

	size_t lus = fread(lecture, 1, taille > TAILLE_BLOC ? TAILLE_BLOC : taille, fichier);
	if (lus == 0 && ferror(fichier)) { // La lecture n'a rien donné ET une erreur a eu lieu
		perror("ChargerBloc : Erreur lecture fichier. Abandon.");
		// On ne ferme pas le fichier car c'est à l'appellant de le faire
		return -1;
	}

	EcrireContenuBloc(bloc, lecture, lus); // lus vaut au plus TAILLE_BLOC
	return 0;
}

// Fonction non-statique car doit être pouvoir utilisée dans d'autres fichiers et permet de créer le gabarit d'affichage d'un bloc (car sa taille en caractères peut exéder TAILLE_BLOC si il est non-nul terminé (ce qui peut arriver à cause de l'implémentation (volontaire)))
char * creerGabarit() {
	char * gabarit = calloc(sizeof(char), TAILLE_TAILLE_BLOC + 4);
	sprintf(gabarit, "%%.%ds", TAILLE_BLOC);

	return gabarit;
}