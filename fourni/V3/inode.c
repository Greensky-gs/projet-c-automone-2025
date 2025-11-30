// Code par Greensky-gs (David Heslière)
/**
* ProgC - Projet Automne 25-26 : Gestion de systèmes de fichiers
* VERSION 3
* Fichier : inode.c
* Module de gestion des inodes.
**/
#include "inode.h"
#include "bloc.h"
#include <stdlib.h>
#include <stdio.h>

#define NB_BLOCS_DIRECTS 10

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

	// On initialise les blocs à NULL
	for (int i = 0; i < NB_BLOCS_DIRECTS; i++) {
		(iNode->blocDonnees)[i] = NULL;
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
	int indice = 0;
	while (indice * TAILLE_BLOC < (*pInode)-> taille) {
		DetruireBloc(&((*pInode)->blocDonnees[indice]));
		indice++;
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


/* V1 & V3
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

	// Puisqu'on va allouer une chaine de la taille inode->taille + 1, on utilise un calloc plutôt qu'une VLA  (c'est bien hein)
	unsigned char * chaine = calloc(5 > inode->taille + 1 ? 5 : inode->taille + 1, sizeof(unsigned char));
	if (chaine == NULL) {
		perror("AfficherInode : erreur allocation");
		return;
	}

	long lus = LireDonneesInode(inode, chaine, inode->taille, 0);
	chaine[lus] = '\0';

	if (inode->taille == 0) {
		sprintf((char *)chaine, "Vide");
	}

	printf("----------Inode [%d]----\n    Type : %s\n    Taille : %ld octets\n    Date de dernier access : %s    Date de derniere modification inode : %s    Date de derniere modification fichier : %s    Contenu :\n%s\n    Octets lus : %ld\n--------------------\n", inode->numero, typeText, inode->taille, ctime(&derAccess), ctime(&derModifInode), ctime(&derModifFichier), chaine, lus);

	free(chaine);
}

/* V1
* Copie dans un inode les taille octets situés à l’adresse contenu.
* Si taille est supérieure à la taille d’un bloc, seuls les TAILLE_BLOC premiers octets doivent être copiés.
* Entrées : l'inode, l'adresse de la zone à recopier et sa taille en octets
* Retour : le nombre d'octets effectivement écrits dans l'inode ou -1 en cas d'erreur
*/
long LireDonneesInode1bloc(tInode inode, unsigned char *contenu, long taille) {
	if (inode->taille == 0) {
		return 0;
	}
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
	if (inode->taille == 0) {
		// Les blocs n'ont tout simplement pas été initialisés
		inode->blocDonnees[0] = CreerBloc();
	}
	// On utlise simplement la fonction d'écriture sur le premier bloc
	long octets_ecrits = EcrireContenuBloc(inode->blocDonnees[0], contenu, taille);
	ActualiserDateDerModif(inode); // Modification de la date de modification du fichier car il a été écrit

	inode->taille = octets_ecrits; // La taille a également changé
	ActualiserDateDerModifInode(inode); // Donc on modifie la date de modification de l'inode

	// On a peut-être une nouvelle taille de 0, auquel cas on va détruire le bloc
	if (inode->taille == 0) {
		DetruireBloc(&(inode->blocDonnees[0]));
	}

	return octets_ecrits;
}

/* V3
* Lit les données d'un inode avec décalage, et les stocke à une adresse donnée
* Entrées : l'inode d'où les données sont lues, la zone où recopier ces données, la taille en octets
* des données à lire et le décalage à appliquer (voir énoncé)
* Sortie : le nombre d'octets effectivement lus, 0 si le décalage est au-delà de la taille
*/
long LireDonneesInode(tInode inode, unsigned char * contenu, long taille, long decalage) {
	int i = decalage / TAILLE_BLOC; // On initialise l'indice du bloc par le quotient de decalage par bloc : 0 si decalage < 64, 1 si decalage < 128 ...
	int c = decalage % TAILLE_BLOC; // On initialise l'indice du caractère du bloc par le reste de decalage par bloc : decalage si decalage < 64n decalage - 64 si decalage < 128 ...
	int index = 0; // La variable est redondante, on pourrait remplacer ses occurences par i * TAILLE_BLOC + c, mais il faudrait rajouter des cycles de calcul lors de l'exécution, on va la laisser pour "l'optimisation"

	// En principe on ne peut pas taper dans de la mémoire non-initialisée
	while (index < taille && index < TAILLE_BLOC * NB_BLOCS_DIRECTS && index < inode->taille) {
		unsigned char car = inode->blocDonnees[i][c];

		contenu[index] = car;

		c++;
		if (c == TAILLE_BLOC) {
			c = 0;
			i++;
		}
		index++;
	}

	return index;
}

/* V3
* Ecrit dans un inode, avec décalage, ls données stockées à une adresse donnée
* Entrées : l'inode où écrire le contenu, l'adesse de la zone depuis laquelle lire les données, la taille en octets
* de ces données et le décalage à appliquer (voir énoncé)
* Sortie : le nombre d'octets effectivement écrits, ou -1 en cas d'erreur
*/
long EcrireDonneesInode(tInode inode, unsigned char *contenu, long taille, long decalage) {
	// Bon, mêmes commentaires que LireDonneesInode, c'est essentiellement le même algorithme
	int i = decalage / TAILLE_BLOC;
	int c = decalage % TAILLE_BLOC;
	int index = 0;

	while (index < taille && index < TAILLE_BLOC * NB_BLOCS_DIRECTS) {
		if (inode->blocDonnees[i] == NULL) {
			inode->blocDonnees[i] = CreerBloc();
			if (inode->blocDonnees[i] == NULL) {
				perror("EcrireDonneesInode : Erreur allocation bloc");
				return -1; // Erreur d'allocation
			}
		}
		inode->blocDonnees[i][c] = contenu[index];

		c++;
		if (c == TAILLE_BLOC) {
			c = 0;
			i++;
		}
		index++;
	}

	// Ici on remplace, comme le ferait un véritable système de fichiers, la taille par la nouvelle taille : si elle est plus grande que l'ancienne, elle la remplace, sinon elle ne change pas.
	// On fait ça car c'est plus ou moins ce que ferait le fichier : on change X bits au milieu du fichier, mais en gardant la fin (quand même)
	inode->taille = inode->taille > decalage + index ? inode->taille : decalage + index;
	// Une autre implémentation serait la suivante :
	// inode->taille = decalage + index; // On remplace la taille du fichier par ce qu'on a écrit

	return index;
}

// Fonction qui modifie resultat pour tracer les erreurs dans SauvegarderInode
// Au final, resultat vaudra le nombre d'erreurs rencontrées
static void resultatIncrementeur(int * resultat, int retour) {
	*resultat = *resultat + (retour == EOF ? 1 : 0);
}

/* V3
* Sauvegarde toutes les informations contenues dans un inode dans un fichier (sur disque,
* et préalablement ouvert en écriture et en mode binaire)
* Entrées : l'inode concerné, l'identificateur du fichier
* Sortie : 0 en cas de succès, -1 en cas d'erreur
*/
int SauvegarderInode(tInode inode, FILE * fichier) {
	// On va stocker toutes les informations qu'on peut sauvegarder ligne par ligne en premier (dates, nombre de blocs, etc)
	char tampon[34] = {0}; // On met 34, mais y'aura certainement besoin de moins en runtime
	int resultat = 0;
	int * ptr = &resultat;

	sprintf(tampon, "%ld\n", inode->dateDerAcces);
	resultatIncrementeur(ptr, fputs(tampon, fichier));
	sprintf(tampon, "%ld\n", inode->dateDerModif);
	resultatIncrementeur(ptr, fputs(tampon, fichier));
	sprintf(tampon, "%ld\n", inode->dateDerModifInode);
	resultatIncrementeur(ptr, fputs(tampon, fichier));
	sprintf(tampon, "%d\n", inode->numero);
	resultatIncrementeur(ptr, fputs(tampon, fichier));
	sprintf(tampon, "%ld\n", inode->taille);
	resultatIncrementeur(ptr, fputs(tampon, fichier));
	sprintf(tampon, "%d\n", inode->type); // natureFicher, c'est jamais qu'un entier
	resultatIncrementeur(ptr, fputs(tampon, fichier));

	if (resultat > 0) {
		perror("SauvegarderInode : Au moins une erreur a eu lieu lors de l'ecriture des donnees statiques");
		return -1;
	}
	// C'est toutes les données qu'on peut représenter ligne par ligne sans avoir à se soucier du fait qu'on pourrait avoir un retour à la ligne à cause du contenu

	// Enregistrement des blocs
	long total = inode->taille;
	int i = 0;
	while (i < NB_BLOCS_DIRECTS && i * TAILLE_BLOC < total) {
		tBloc bloc = inode->blocDonnees[i];
		long octets = total - (i * TAILLE_BLOC);
		if (octets > TAILLE_BLOC) octets = TAILLE_BLOC;
		int res = SauvegarderBloc(bloc, octets, fichier);
		if (res == -1) {
			perror("SauvegarderInode : Erreur enregistrement blocs");
			return -1;
		}
		i++;
	}

	return 0;
}

/* V3
* Charge toutes les informations d'un inode à partir d'un fichier (sur disque,
* et préalablement ouvert en lecture et en mode binaire)
* Entrées : l'inode concerné, l'identificateur du fichier
* Sortie : 0 en cas de succès, -1 en cas d'erreur
*/
int ChargerInode(tInode *pInode, FILE *fichier) {
	// On peut utiliser un scanf avec une format un peu balèze pour récupérer les données
	int res = fscanf(fichier, "%ld\n%ld\n%ld\n%d\n%ld\n%d\n", &(*pInode)->dateDerAcces, &(*pInode)->dateDerModif, &(*pInode)->dateDerModifInode, &(*pInode)->numero, &(*pInode)->taille, (int *)&(*pInode)->type); // L'appel est infâme

	if (res != 6) {
		perror("ChargerInode : toutes les proprietes n'ont pas pu etre trouvees");
		return -1;
	}
	
	int i = 0;
	while (i < NB_BLOCS_DIRECTS && i * TAILLE_BLOC < (*pInode)->taille) {
		// On charge chaque bloc
		if ((*pInode)->blocDonnees[i] == NULL) {
			(*pInode)->blocDonnees[i] = CreerBloc();
			if ((*pInode)->blocDonnees[i] == NULL) {
				perror("ChargerInode : Erreur allocation bloc");
				return -1;
			}
		}
		int res = ChargerBloc((*pInode)->blocDonnees[i], (*pInode)->taille - (i * TAILLE_BLOC), fichier);
		if (res == -1) {
			perror("ChargerInode : Erreur chargement blocs");
			return -1;
		}
		i++;
	}

	return 0;
}
