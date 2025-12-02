#include <stdio.h>
#include "fourni/V2/bloc.h"
#include "fourni/V2/inode.h"
#include "fourni/V2/sf.h"

int main() {
    tSF systeme = CreerSF("SYSTEME");

    Ecrire1BlocFichierSF(systeme, "makefile2", ORDINAIRE);
    
    AfficherSF(systeme);

    DetruireSF(&systeme);

    return 0;
}