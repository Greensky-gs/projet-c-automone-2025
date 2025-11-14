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

    DetruireRepertoire(&rep);
    return 0;
}