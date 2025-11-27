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
    // Création d'un inode de test
    tInode iNode = CreerInode(1, AUTRE);

    AfficherInode(iNode);

    // Test d'écriture
    EcrireDonneesInode1bloc(iNode, (tBloc)"Je suis un genie", 17);

    AfficherInode(iNode);

    // Création d'un bloc pour l'extraction
    tBloc resultat = CreerBloc();

    LireDonneesInode1bloc(iNode, resultat, 17);

    // Affichages
    AfficherInode(iNode);
    printBloc(resultat, 17);

    // Destructions
    DetruireBloc(&resultat);
    DetruireInode(&iNode);

}
int _main() {
    // Bloc de test
    tBloc bloc = CreerBloc();

    // Écriture d'un texte trop grand
    EcrireContenuBloc(bloc, (tBloc)"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz", 78);

    // Affichage
    printBloc(bloc, 78);

    // Extractions
    tBloc resultat = CreerBloc();

    LireContenuBloc(bloc, resultat, 78);

    printBloc(resultat, 78);
    
    // Destructions
    DetruireBloc(&bloc);
    DetruireBloc(&resultat);
    return 0;
}