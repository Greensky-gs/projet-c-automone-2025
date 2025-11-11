#include <stdio.h>
#include <stdlib.h>
#include "fourni/V3/bloc.h"
#include "fourni/V3/inode.h"
#include "fourni/V3/sf.h"

int main() {
// int testEcrireSF() {
    tSF sf = CreerSF("SSD");

    AfficherSF(sf);

    EcrireFichierSF(sf, "LICENSE", ORDINAIRE);

    AfficherSF(sf);

    EcrireFichierSF(sf, "README.md", ORDINAIRE); // Attention, ce fichier est de taille 640, mais il ne contient pas 640 caractères, c'est à cause du fait que certains caractères ne tiennent pas sur un unsigned char. Coup de bol, il s'affiche correctement, mais il n'y a que 626 caractères

    AfficherSF(sf);

    DetruireSF(&sf);
    return 0;
}

// int main() {
int testAffichageInodeVide() {
    tInode inode = CreerInode(0, ORDINAIRE);

    AfficherInode(inode);

    DetruireInode(&inode);
    return 0;
}

// int main() {
int testEcritureLongue() {
    tInode inode = CreerInode(0, ORDINAIRE);

    EcrireDonneesInode(inode, (unsigned char *)"Avec un texte relativement long, on doit pouvoir depasser facilement la limitation de 64 characteres pour un bloc.", 114, 0);

    AfficherInode(inode);

    EcrireDonneesInode(inode, (unsigned char *)"j'ai ecrase un bout de tout ca", 30, 5);

    AfficherInode(inode);

    FILE * output = fopen("ecriture.txt", "w+b");
    SauvegarderInode(inode, output);

    fclose(output);
    DetruireInode(&inode);
    return 0;
}

// int main() {
int testLectureLongue() {
    // Fuck les charactères spéciaux (à accents) ne tiennent pas sur un seul unsigned char, c'était pas dit ça
    FILE * flux = fopen("inode.txt", "r+b");

    tInode inode = CreerInode(-1, AUTRE);

    ChargerInode(&inode, flux);

    AfficherInode(inode);

    // Le * 10 est censé être * NB_BLOCS_DIRECTS, mais comme il n'est pas défini au bon endroit on va faire une constante magique
    unsigned char * content = malloc(TAILLE_BLOC * 10 + 1);
    
    // Idem pour le 152, c'est censé être inode->taille
    LireDonneesInode(inode, content, 115, 0);
    content[115] = '\0';
    content[TAILLE_BLOC * 10] = '\0';

    printf("%s\n", content);

    free(content);
    DetruireInode(&inode);
    return 0;
}

// int main() {
int testChargeEtSauvegardeEtAffichage() {
    char * gabarit = creerGabarit();

    FILE * input = fopen("inode.txt", "r+b");

    tInode inode = CreerInode(-1, AUTRE); // Valeurs par défaut, qui sont supposées changer
    
    AfficherInode(inode);

    ChargerInode(&inode, input);

    AfficherInode(inode);

    fclose(input);

    FILE * output = fopen("output.txt", "w+b");

    SauvegarderInode(inode, output);

    fclose(output);
    DetruireInode(&inode);

    free(gabarit);
    return 0;
}

// int main() {
int creerSauvegardeBasique() {
    char * gabarit = creerGabarit();
    
    tInode inode = CreerInode(0, ORDINAIRE);
    EcrireDonneesInode1bloc(inode, (unsigned char *)"C'est un certain nombre de données. Visiblement on peut augmenter", 65);

    FILE * flux = fopen("inode.txt", "w+b");

    SauvegarderInode(inode, flux);

    fclose(flux);
    DetruireInode(&inode);
    
    free(gabarit);
    
    return 0;
}