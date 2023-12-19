#include<stdlib.h>
#include"supermorpion.h"
int main(int argc, char**agrv){
    //initialiser le smp  l'horizon
    supermorpion smp={0};
    int coup=0;
    int horizon,gagner;
    if(argc==3){
        smp = FEN2smp(agrv[1]);
        horizon=atoi(agrv[2]);
        showsmp(&smp);
        gagner = smp.joueur;
        char c;
        while(evalsmp(&smp)){
            printf("Saisir le prochain coup : \n");
            c = getchar();
            coup=10*(c-'0');
            getchar();
            c = getchar();
            coup+=(c-'a')*3;
            c=getchar();
            coup+=c-'0';
            if(modifsmp(&smp,coup))
            {
                minimaxsmp(smp,horizon,gagner);
                modifsmp(&smp,coup);
            }else{
                printf("Coup invalid, reessaye.\n");
                continue;
        }


    }
    }
}