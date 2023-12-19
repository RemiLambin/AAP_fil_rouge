#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include"supermorpion.h"

void showmp(const morpion* mp){
    /*
    print the morpion in the shell
    just for examination
    */
    int i,j;
    char dict[3] = {'.','o','x'};
    for(j=0;j<3;j++){
    for(i=0;i<3;i++)printf("%c",dict[mp->table[i+3*j]]);
    printf("\n");
    }
    printf(" %c\n",dict[mp->joueur]);
}
void showsmp(const supermorpion*smp){
    /*
    print the supermorpion, every small morpion in one line, not the real shape
    just for examination
    delete it if necessaire
    */
    int i;
    int j;
    for(i=0;i<9;i++){
        printf("%d ",smp->macro.table[i]);
        for(j=0;j<9;j++){
            printf("%d",smp->micro[i].table[j]);
        }
        printf("\n");
    }
}

morpion FEN2mp(const char* FEN){
    /*
    function : transform a FEN code into a morpion
    */
    int i;int times;
    morpion mp={0};
    mp.joueur=0;
    char* p = strdup(FEN);
    int*ip=mp.table; 
    while(*p!='\0'){
        if(mp.joueur==0){
        switch(*p){
            case 'o':
                *ip=1;
                ip++;
                break;
            case 'x':
                *ip=2;
                ip++;
                break;
            case ' ':
                mp.joueur=3;
                ip++;
                break;
            default:
                times = (*p)-'0';
                for(i=0;i<times;i++){
                    *ip=0;
                    ip++;
                }
                break;
        }
        }
        p++;
        if(mp.joueur==3){
            switch(*p){
                case 'o':
                    mp.joueur=1;
                    break;
                case 'x':
                    mp.joueur=2;
                    break;
                default:
                    break;
            }
        }
    }
    return mp;
}
supermorpion FEN2smp(const char* FEN){
    /*
    transform a FEN code into a supermorpion
    */
    int times;
    int j;
    supermorpion smp;
    char* p = strdup(FEN);
    char s[3][100];
    int layer=0;
    int k=0;
    
    while(*p!='\0'){
        if(*p==' '){
            layer++;
            k=0;
            p++;
        }
        else{
            s[layer][k++]=*p++;
        }
    }
    int imp=0;
    int i=0;
    char *sp = s[0];
    while(*sp!='\0'){
        if(i==9){
            smp.macro.table[imp]=0;
            imp++;
            i=0;
        }
        switch(*sp){
            case 'O':
                smp.macro.table[imp]=1;
                for(i=0;i<9;i++){
                    smp.micro[imp].table[i]=1;
                }
                imp++;
                i=0;
                break;
            case 'X':
                smp.macro.table[imp]=2;
                for(i=0;i<9;i++){
                    smp.micro[imp].table[i]=2;
                }
                imp++;
                i=0;
                break;
            case 'o':
                smp.micro[imp].table[i]=1;
                i++;
                break;
            case 'x':
                smp.micro[imp].table[i]=2;
                i++;
                break;
            default:
                times = (*sp)-'0';
                for(j=0;j<times;j++){
                    smp.micro[imp].table[i]=0;
                    i++;
                }
                break;
        }
        sp++;
    }
    return smp;
}

void generatePNGmp(const morpion* mp,const char*name){
    /*
    data : morpion, the name of file
    function : generate the name.dot file and execute the command in the shell to generate the image name.png
    return : no
    */
    char* dotname = strdup(name);
    strcat(dotname,".dot");
    FILE* fp = fopen(dotname,"w");
    fprintf(fp,"digraph  {\n  a0 [shape=none label=<\n <TABLE border=\"0\" cellspacing=\"10\" cellpadding=\"10\" style=\"rounded\" bgcolor=\"black\">\n");
    int i,j;
    for(i=0;i<3;i++){
        fprintf(fp,"<TR>\n");
        for(j=0;j<3;j++){
                int stat=mp->table[3*i+j];
                char c = '.';
                if(stat==1)c='o';
                if(stat==2)c='x';
                fprintf(fp,"<TD bgcolor=\"white\">%c</TD>\n",c);
        }
        fprintf(fp,"</TR>\n"); 
    }
    fprintf(fp,"<TR>\n");
    int joueur = mp->joueur;
    char c = 'o';
    if(joueur==1)c='o';
    if(joueur==2)c='x';
    fprintf(fp,"<TD bgcolor=\"red\" colspan=\"3\">jouer %c</TD>\n",c);
    fprintf(fp,"</TR>\n"); 
    fprintf(fp,"</TABLE>\n>];\n}\n");
    fclose(fp);
    char command[100];
    sprintf(command,"dot %s -T png -o %s.png",dotname,name);
    system(command);
}
void generatePNGsmp(const supermorpion* smp,const char*name){
    /*
    data : supermorpion, the name of file
    function : generate the name.dot file and execute the command in the shell to generate the image name.png
    return : no
    */
    char* dotname = strdup(name);
    strcat(dotname,".dot");
    FILE* fp = fopen(dotname,"w");
    fprintf(fp,"digraph  {\na0 [shape=none label=<\n<TABLE border=\"0\" cellspacing=\"10\" cellpadding=\"10\" style=\"rounded\" bgcolor=\"black\">\n");
    int i,j;
    for(i=0;i<3;i++){
        fprintf(fp,"<TR>\n");
        for(j=0;j<3;j++){
                int stat=smp->macro.table[3*i+j];
                if(stat==1)fprintf(fp,"<TD bgcolor=\"black\">\n<TABLE border=\"0\" cellspacing=\"10\" cellpadding=\"10\" style=\"rounded\" bgcolor=\"white\">\n<TR>\n<TD bgcolor=\"white\">&nbsp;</TD>\n<TD bgcolor=\"white\">&nbsp;</TD>\n<TD bgcolor=\"white\">&nbsp;</TD>\n</TR>\n<TR>\n<TD bgcolor=\"white\">&nbsp;</TD>\n<TD bgcolor=\"white\">&nbsp;</TD>\n<TD bgcolor=\"white\">&nbsp;</TD>\n</TR>\n<TR>\n<TD bgcolor=\"white\">&nbsp;</TD>\n<TD bgcolor=\"white\">&nbsp;</TD>\n<TD bgcolor=\"white\">&nbsp;</TD>\n</TR>\n</TABLE>\n</TD>\n");
                if(stat==2)fprintf(fp,"<TD bgcolor=\"white\">\n<TABLE border=\"0\" cellspacing=\"10\" cellpadding=\"10\" style=\"rounded\" bgcolor=\"black\">\n<TR>\n<TD bgcolor=\"black\">&nbsp;</TD>\n<TD bgcolor=\"black\">&nbsp;</TD>\n<TD bgcolor=\"black\">&nbsp;</TD>\n</TR>\n<TR>\n<TD bgcolor=\"black\">&nbsp;</TD>\n<TD bgcolor=\"black\">&nbsp;</TD>\n<TD bgcolor=\"black\">&nbsp;</TD>\n</TR>\n<TR>\n<TD bgcolor=\"black\">&nbsp;</TD>\n<TD bgcolor=\"black\">&nbsp;</TD>\n<TD bgcolor=\"black\">&nbsp;</TD>\n</TR>\n</TABLE>\n</TD>\n");
                if(stat==0){
                    fprintf(fp,"<TD bgcolor=\"white\">\n<TABLE border=\"0\" cellspacing=\"10\" cellpadding=\"10\" style=\"rounded\" bgcolor=\"black\">\n");
                    int m,n;
                    for(m=0;m<3;m++){
                        fprintf(fp,"<TR>\n");
                        for(n=0;n<3;n++){
                            int stat=smp->micro[3*i+j].table[3*m+n];
                            char c = '.';
                            if(stat==1)c='o';
                            if(stat==2)c='x';
                            fprintf(fp,"<TD bgcolor=\"white\">%c</TD>\n",c);
                            }
                        fprintf(fp,"</TR>\n");
                        }
                        
                    fprintf(fp,"</TABLE>\n</TD>\n");
                    }
        }
        fprintf(fp,"</TR>\n"); 
    }
    fprintf(fp,"</TABLE>\n>]\n}\n");
    /*int joueur = mp->joueur;
    char c = 'o';
    if(joueur==1)c='o';
    if(joueur==2)c='x';
    fprintf(fp,"<TD bgcolor=\"red\" colspan=\"3\">jouer %c</TD>\n",c);
    fprintf(fp,"</TR>\n"); 
    fprintf(fp,"</TABLE>\n>];\n}\n");*/
    fclose(fp);
    char command[100];
    sprintf(command,"dot %s -T png -o %s.png",dotname,name);
    printf("%s\n",command);
    system(command);
}

int evalmp(morpion* mp,int gagner){
    // renvoyer le gagnant 1/2
    /*
    data : morpion, gagner(one who want to win)
    function : evaluate the morpion for gagner
    return : if the gagner win (1) else (-1), no result: 0
    */
    int check[8][3]={{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    int i;
    for(i=0;i<8;i++){
            if(mp->table[check[i][0]]!=0&&mp->table[check[i][0]]==mp->table[check[i][1]]&&mp->table[check[i][0]]==mp->table[check[i][2]])
                {if(mp->table[check[i][0]]==gagner)return 1;else return -1;}
    }
    return 0;
};
int depthmp(morpion* mp){
    /*
    calculate the depth of the morpion in the decision tree
    for function minimax
    */
    int i;
    int depth=0;
    for(i=0;i<9;i++){
        if(mp->table[i]==0)depth++;
    }
    return depth;
}

int minimaxmp(morpion mp,int depth,int gagner){
    /*
    minimax without any output
    */
    if(depth==0||evalmp(&mp,gagner)!=0){
        return evalmp(&mp,gagner);        
    }
    else{
        if(mp.joueur==gagner){
            int i;
            int max = -2;
            for(i=0;i<9;i++){
                if(mp.table[i]==0){
                    morpion mp2 = mp;
                    mp2.table[i]=mp2.joueur;
                    mp2.joueur = -mp.joueur;
                    int maxNext = minimaxmp(mp2,depth-1,gagner);
                    if(maxNext>max)max=maxNext;
                }
            }
            return max;
        }else{
            int i;
            int min = 2;
            for(i=0;i<9;i++){
                if(mp.table[i]==0){
                    morpion mp2 = mp;
                    mp2.table[i]=mp2.joueur;
                    mp2.joueur = -mp.joueur;
                    int minNext = minimaxmp(mp2,depth-1,gagner);
                    if(minNext<min)min=minNext;
                }
            }
            return min;
        }
    }
}

char* mp2dot(morpion*mp,int gagner,int num,int label){
    /*
    transform a morpion into dot
    for generating the decision tree
    */
    char* s = (char*)malloc(sizeof(char)*10000);
    char* buffer = (char*)malloc(sizeof(char)*1000);
    sprintf(s,"m%d [shape=none label=<\n <TABLE border=\"0\" cellspacing=\"10\" cellpadding=\"10\" style=\"rounded\" bgcolor=\"black\">",num);
    int i,j; 
    for(i=0;i<3;i++){
        sprintf(buffer,"<TR>\n");
        strcat(s,buffer);
        for(j=0;j<3;j++){
                int stat=mp->table[3*i+j];
                char c = '.';
                if(stat==1)c='o';
                if(stat==2)c='x';
                sprintf(buffer,"<TD bgcolor=\"white\">%c</TD>\n",c);
                strcat(s,buffer);
        }
        sprintf(buffer,"</TR>\n"); 
        strcat(s,buffer);
    }
    int joueur = mp->joueur;
    char c = 'o';
    if(joueur==1)c='o';
    if(joueur==2)c='x';
    if(joueur==gagner)sprintf(buffer,"<TR>\n<TD bgcolor=\"green\" colspan=\"3\">m%d joueur %c</TD>\n",num,c);
    else{sprintf(buffer,"<TR\n><TD bgcolor=\"red\" colspan=\"3\">m%d joueur %c</TD>\n",num,c);
    }
    strcat(s,buffer);
    sprintf(buffer,"</TR>\n</TABLE>\n>];\n");
    strcat(s,buffer);
    sprintf(buffer,"m%d [xlabel=\"%d\"]\n",num,label);
    strcat(s,buffer);
    return s;
}

void printMpTreeDot(morpion mp){
    /*
    call the minimax function with printing the dot in the shell
    */
    printf("digraph{\n");
    int res = minimaxMpTreeDot(mp,depthmp(&mp),mp.joueur,0);
    printf("%s",mp2dot(&mp,mp.joueur,0,evalmp(&mp,res)));
    printf("}\n");
    }
int minimaxMpTreeDot(morpion mp, int depth, int gagner,int numPre){
    /*
    minimax with printing the dot file in shell
    */
    int e = evalmp(&mp,gagner);
    static int num=0;
    if(depth==0||e!=0){
        return e;        
    }
    else{
        if(mp.joueur==gagner){
            int i;
            int max = -2;
            for(i=0;i<9;i++){
                if(mp.table[i]==0){
                    morpion mp2 = mp;
                    mp2.table[i]=mp.joueur;
                    mp2.joueur = (mp.joueur)%2+1;
                    num++;
                    int m = num;
                    int maxNext = minimaxMpTreeDot(mp2,depth-1,gagner,m);
                    printf("%s",mp2dot(&mp2,gagner,m,maxNext));
                    printf("m%d->m%d[label=\"%d,%d\"]\n",numPre,m,i/3,i%3);
                    if(maxNext>max)max=maxNext;
                }
            }
            return max;
        }else{
            int i;
            int min = 2;
            for(i=0;i<9;i++){
                if(mp.table[i]==0){
                    morpion mp2 = mp;
                    mp2.table[i]=mp.joueur;
                    mp2.joueur = (mp.joueur)%2+1;
                    num++;
                    int m = num;
                    int minNext =  minimaxMpTreeDot(mp2,depth-1,gagner,m);
                    printf("%s",mp2dot(&mp2,gagner,m,minNext));
                    printf("m%d->m%d[label=\"%d,%d\"]\n",numPre,m,i/3,i%3);
                    if(minNext<min)min=minNext;
                }
            }
            return min;
        }
    }
}

void generatePNGmpTree(morpion mp,const char*name){
    /*
    generate directly the png for the decision tree
    */
    char*dotname = strdup(name);
    strcat(dotname,".dot");
    FILE*fp = fopen(dotname,"w");
    fprintf(fp,"digraph{\n");
    int res = minimaxMpTreePNG(mp,depthmp(&mp),mp.joueur,0,fp);
    fprintf(fp,"%s",mp2dot(&mp,mp.joueur,0,evalmp(&mp,res)));
    fprintf(fp,"}\n");
    fclose(fp);
    char command[100];
    sprintf(command,"dot %s -T png -o %s.png",dotname,name);
    system(command);
}
int minimaxMpTreePNG(morpion mp,int depth, int gagner,int numPre,FILE*fp){
    /*
    minimax with generating the png
    */
    int e = evalmp(&mp,gagner);
    static int num=0;
    if(depth==0||e!=0){
        return e;        
    }
    else{
        if(mp.joueur==gagner){
            int i;
            int max = -2;
            for(i=0;i<9;i++){
                if(mp.table[i]==0){
                    morpion mp2 = mp;
                    mp2.table[i]=mp.joueur;
                    mp2.joueur = (mp.joueur)%2+1;
                    num++;
                    int m = num;
                    int maxNext = minimaxMpTreePNG(mp2,depth-1,gagner,m,fp);
                    fprintf(fp,"%s",mp2dot(&mp2,gagner,m,maxNext));
                    fprintf(fp,"m%d->m%d[label=\"%d,%d\"]\n",numPre,m,i/3,i%3);
                    if(maxNext>max)max=maxNext;
                }
            }
            return max;
        }else{
            int i;
            int min = 2;
            for(i=0;i<9;i++){
                if(mp.table[i]==0){
                    morpion mp2 = mp;
                    mp2.table[i]=mp.joueur;
                    mp2.joueur = (mp.joueur)%2+1;
                    num++;
                    int m = num;
                    int minNext =  minimaxMpTreePNG(mp2,depth-1,gagner,m,fp);
                    fprintf(fp,"%s",mp2dot(&mp2,gagner,m,minNext));
                    fprintf(fp,"m%d->m%d[label=\"%d,%d\"]\n",numPre,m,i/3,i%3);
                    if(minNext<min)min=minNext;
                }
            }
            return min;
        }
    }
}