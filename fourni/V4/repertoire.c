// Code par Greensky-gs (David Heslière)
/**
* ProgC - Projet Automne 25-26 : Gestion de systèmes de fichiers
* VERSION 4
* Fichier : repertoire.c
* Module de gestion d'un répertoire d'un systèmes de fichiers (simulé)
**/
#include "repertoire.h"
#include "bloc.h"
#include "inode.h"
#include "sf.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Variables personnelles
#define TAILLE_MAXIMALE_NUM_INODES 5
// Cette constante sert à donner la taille de maximale des numéros d'inode en nombre de chiffres


// Définition d'un répertoire
struct sRepertoire {
	tEntreesRepertoire * table;
};

static unsigned long tailleEntreesTab() {
    return ((TAILLE_BLOC * 10) / (TAILLE_NOM_FICHIER + sizeof(unsigned int)));
}

/* V4
* Crée un nouveau répertoire.
* Entrée : aucune
* Sortie : le répertoire créé, ou NULL si problème
*/
tRepertoire CreerRepertoire(void) {
	struct sRepertoire * rep = malloc(sizeof(struct sRepertoire));
	if (rep == NULL) {
		perror("CreerRepertoire : probleme creation");
		return NULL;
	}

    struct sEntreesRepertoire ** entreeRep = calloc(tailleEntreesTab(), sizeof(struct sEntreesRepertoire));
    if (entreeRep == NULL) {
		perror("CreerRepertoire : probleme creation");
		fprintf(stderr, "Erreur d'allocation de memoire pour l'entree repertoire\n");
		free(rep);
		return NULL;
	}
	int i = 0;
	int error = 0;
	while (i < tailleEntreesTab() && error == 0) {
		struct sEntreesRepertoire * ptr = malloc(sizeof(struct sEntreesRepertoire));
		if (ptr == NULL) {
			perror("CreerRepertoire : probleme creation");
			fprintf(stderr, "Erreur d'allocation de memoire pour case entrees repertoire\n");
			error = 1;
			continue;
		}
		ptr->nomEntree[0] = '\0';
		ptr->numeroInode = 0;
		entreeRep[i] = ptr;
		i++;
	}

	if (error) {
		int j = 0;
		while (j < i) {
			free(entreeRep[j]);
			j++;
		}
		free(entreeRep);
		free(rep);
		return NULL;
	}

	rep->table = entreeRep;

	return rep;
}

/* V4
* Détruit un répertoire et libère la mémoire associée.
* Entrée : le répertoire à détruire
* Sortie : aucune
*/
void DetruireRepertoire(tRepertoire *pRep) {
	int i = 0;
	while (i < tailleEntreesTab()) {
		free((*pRep)->table[i]);
		i++;
	}
	free((*pRep)->table);
	free((*pRep));
	*pRep = NULL;
}

// Renvoie 1 si les deux textes sont égaux, renvoie 0 sinon
// Max est le nombre d'itérations possibles
static int compStr(char * a, char * b, int max) {
	int i = 0;
	int eq = 1;
	while (a[i] != '\0' && b[i] != '\n' && eq && i < max) {
		if (a[i] != b[i]) {
			eq = 0;
		}
		i++;
	}
	eq = a[i] == b[i];

	return eq;
}
// Copie le texte source à l'emplacement target
// Max est le nombre maximum d'itérations
static void copyStr(char * target, char * source) {
	int i = 0;
	while (source[i] != '\0') {
		target[i] = source[i];
		i++;
	}
	target[i] = '\0';
}

/* V4
* Écrit une entrée dans un répertoire.
* Si l'entrée existe déjà dans le répertoire, le numéro d'inode associé est mis à jour.
* Si l'entrée n'existe pas, elle est ajoutée dans le répertoire.
* Entrées : le répertoire destination, le nom de l'entrée à écrire,
*           le numéro d'inode associé à l'entrée
* Retour : 0 si l'entrée est écrite avec succès, -1 en cas d'erreur
*/
int EcrireEntreeRepertoire(tRepertoire rep, char nomEntree[], unsigned int numeroInode) {
	int found = 0;
	int index = 0;
	int taille = tailleEntreesTab();
	int firstNul = -1;
	while (found == 0 && index < taille) {
		if (firstNul == -1 && rep->table[index]->nomEntree[0] == '\0') {
			firstNul = index;
		}
		if (compStr(rep->table[index]->nomEntree, nomEntree, TAILLE_NOM_FICHIER + 1)) {
			rep->table[index]->numeroInode = numeroInode;
			found = 1;
		}
		index++;
	}
	if (!found) {
		if (firstNul == -1) {
			// Il n'y a plus de place dans le répertoire pour y écrire un fichier
			perror("EcrireEntreeRepertoire : Espace insuffisant");
			return -1;
		}

		rep->table[firstNul]->numeroInode = numeroInode;
		copyStr(rep->table[firstNul]->nomEntree, nomEntree);
		return 0;
	}
	return 0;
}

static int tailleStr(unsigned char * str) {
	int c = 0;
	while (str[c] != '\0') {
		c++;
	}
	return c;
}
static int log10int(int n) {
	if (n <= 0) return -1;
	// n > 0
	int c = 1;
	while (n >= 10) {
		n = n / 10;
		c++;
	}
	return c;
}

/* V4
* Lit le contenu d'un répertoire depuis un inode.
* Entrées : le répertoire mis à jour avec le contenu lu,
*           l'inode source.
* Retour : 0 si le répertoire est lu avec succès, -1 en cas d'erreur
*/
int LireRepertoireDepuisInode(tRepertoire *pRep, tInode inode) {
	// Même étapes que EcrireRepertoireDansInode mais en sens inverse
	if (Taille(inode) == 0) return 0; // Le répertoire est techniquement vide, donc pas d'erreur

	unsigned char * buffer = calloc(1, Taille(inode) + 3);
	if (buffer == NULL) {
		perror("LireRepertoireDepuisInode : Erreur allocation");
		return -1;
	}

	long lus = LireDonneesInode(inode, buffer, Taille(inode), 0);
	int indice = 0;
	int tableIndex = 0;

	while (indice < lus) {
		unsigned int lecture;
		if (sscanf((char *)(buffer + indice), "%u.", &lecture) < 1) {
			perror("LireRepertoireDepuisInode : Erreur lecture numero inode");
			free(buffer);
			return -1;
		}

		(*pRep)->table[tableIndex]->numeroInode = lecture;
		
		indice += log10int((*pRep)->table[tableIndex]->numeroInode) + 1; // Le +1 est pour passer le point
		int i = 0;

		while (buffer[indice] != '\n' && indice < lus) {
			(*pRep)->table[tableIndex]->nomEntree[i] = buffer[indice];
			i++;
			indice++;
		}

		(*pRep)->table[tableIndex]->nomEntree[i] = '\0';
		indice++; // Pour passer le \n

		tableIndex++;
	}

	free(buffer);
	return 0;
}

/* V4
* Écrit un répertoire dans un inode.
* Entrées : le répertoire source et l'inode destination
* Sortie : 0 si le répertoire est écrit avec succès, -1 en cas d'erreur
*/
int EcrireRepertoireDansInode(tRepertoire rep, tInode inode) {
	// On va écrire les informations sous cette forme : numInode.nomFichier\n, on ne peut pas mettre les deux tout à fait à côté alors on les sépare avec un caractère (qui peut être n'importe lequel sauf un chiffre [0-9]) On assume également que le nom du fichier ne contient pas de retour à la ligne (ce qui est une condition réaliste)

	// Essayons d'être précis
	unsigned char * buffer = malloc(TAILLE_NOM_FICHIER + 1 + TAILLE_MAXIMALE_NUM_INODES + 1); // Les deux 1 sont pour le point et le \n
	
	long ecrits = 0;
	int indice = 0;
	while (rep->table[indice]->nomEntree[0] != '\0' && indice < tailleEntreesTab()) {
		sprintf((char *)buffer, "%d.%s\n", rep->table[indice]->numeroInode, rep->table[indice]->nomEntree);
		int taille = tailleStr(buffer);

		int res = EcrireDonneesInode(inode, buffer, taille, ecrits);
		if (res == -1) {
			perror("EcrireRepertoireDansInode : Erreur ecriture");
			return -1;
		}
		ecrits+=res;
		indice++;
	}

	free(buffer);

	return 0;
}

/* V4
* Récupère les entrées contenues dans un répertoire.
* Entrées : le répertoire source, un tableau récupérant les numéros d'inodes des entrées du rpertoire
* Retour : le nombre d'entrées dans le répertoire
*/
int EntreesContenuesDansRepertoire(tRepertoire rep, struct sEntreesRepertoire tabNumInodes[]) {
	int indice = 0;
	while (indice < tailleEntreesTab() && rep->table[indice]->nomEntree[0] != '\0') {
		tabNumInodes[indice].numeroInode = rep->table[indice]->numeroInode;
		copyStr(tabNumInodes[indice].nomEntree, rep->table[indice]->nomEntree);
		indice++;
	}
	return indice;
}

/* V4
* Compte le nombre d'entrées d'un répertoire.
* Entrée : le répertoire source
* Retour : le nombre d'entrées du répertoire
*/
int NbEntreesRepertoire(tRepertoire rep) {
	int i = 0;
	while (i < tailleEntreesTab() && rep->table[i]->nomEntree[0] != '\0') {
		i++;
	}
	return i;
}

/* V4
* Affiche le contenu d'un répertoire.
* Entrée : le répertoire à afficher
* Retour : aucun
*/
void AfficherRepertoire(tRepertoire rep) {
	printf("===========[REPERTOIRE]===========\n");
	int i = 0;
	while (i < tailleEntreesTab() && rep->table[i]->nomEntree[0] != '\0') {
		printf("%s : inode %d\n", rep->table[i]->nomEntree, rep->table[i]->numeroInode);
		i++;
	}
	printf("==================================\n");
}
