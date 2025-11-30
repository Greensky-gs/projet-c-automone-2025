#include "fourni/V4/sf.h"
#include "fourni/V4/inode.h"
#include "fourni/V4/bloc.h"
#include "fourni/V4/repertoire.h"
#include <stdio.h>

int main() {
// int testAffichage() {
    tRepertoire rep = CreerRepertoire();

    AfficherRepertoire(rep);

    printf("entrees = %d\n", NbEntreesRepertoire(rep));

    EcrireEntreeRepertoire(rep, "abc", 1);
    
    AfficherRepertoire(rep);

    printf("entrees = %d\n", NbEntreesRepertoire(rep));
    
    EcrireEntreeRepertoire(rep, "def.txt", 3);

    AfficherRepertoire(rep);

    printf("entrees = %d\n", NbEntreesRepertoire(rep));

    EcrireEntreeRepertoire(rep, "abc", 3);

    AfficherRepertoire(rep);
    
    printf("entrees = %d\n", NbEntreesRepertoire(rep));
    
    tInode inode = CreerInode(1, REPERTOIRE);

    EcrireRepertoireDansInode(rep, inode);

    FILE * save = fopen("inode-rep.inode.txt", "wb");

    SauvegarderInode(inode, save);

    fclose(save);

    DetruireInode(&inode);
    DetruireRepertoire(&rep);


    // Test de lecture
    printf("\x1b[34m--- Test de lecture ---\x1b[0m\n");

    FILE * load = fopen("inode-rep.inode.txt", "rb");

    tInode inode2 = CreerInode(-1, REPERTOIRE);
    ChargerInode(&inode2, load);

    tRepertoire rep2 = CreerRepertoire();

    LireRepertoireDepuisInode(&rep2, inode2);

    AfficherRepertoire(rep2);

    DetruireInode(&inode2);
    DetruireRepertoire(&rep2);

    fclose(load);
    printf("\x1b[32m--- Fin des tests de lecture ---\x1b[0m\n");
    printf("\x1b[32m--- Ouverture tests affichage & système fichiers ---\x1b[0m\n");

    tSF sf = CreerSF("SSD");
    
    EcrireFichierSF(sf, "LICENCE", ORDINAIRE);
    EcrireFichierSF(sf, "README.md", ORDINAIRE);
    EcrireFichierSF(sf, ".gitignore", ORDINAIRE);

    AfficherSF(sf);

    Ls(sf, true);
    Ls(sf, false);

    DetruireSF(&sf);
    printf("\x1b[32m--- Fin des tests affichage & système fichiers ---\x1b[0m\n");
    return 0;
}