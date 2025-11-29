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
    return 0;
}