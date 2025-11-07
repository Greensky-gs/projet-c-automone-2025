#include <stdio.h>
#include "fourni/V1/bloc.h"
#include "fourni/V1/inode.h"

void printBloc(tBloc bloc, long taille) {
    long i = 0;
    while (i < taille && i < TAILLE_BLOC) {
        printf("%c", bloc[i]);
        i++;
    }
    printf("\n");
}

int main() {
    tInode iNode = CreerInode(1, AUTRE);

    AfficherInode(iNode);

    EcrireDonneesInode1bloc(iNode, (tBloc)"Je suis un génie", 17);

    AfficherInode(iNode);

    tBloc resultat = CreerBloc();

    LireDonneesInode1bloc(iNode, resultat, 17);

    AfficherInode(iNode);
    printBloc(resultat, 17);

    DetruireBloc(&resultat);
    DetruireInode(&iNode);

}
int _main() {
    tBloc bloc = CreerBloc();

    EcrireContenuBloc(bloc, (tBloc)"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz", 78);

    printBloc(bloc, 78);

    tBloc resultat = CreerBloc();

    LireContenuBloc(bloc, resultat, 78);

    printBloc(resultat, 78);
    
    DetruireBloc(&bloc);
    DetruireBloc(&resultat);
    return 0;
}