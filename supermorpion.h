#include<stdio.h>
#ifndef __MORPION__
#define __MORPION__
typedef struct{
int table[9];
int joueur;
}morpion;

typedef struct
{
    morpion macro;
    morpion micro[9];
    int joueur;
    int last;
}supermorpion;
int depthmp(morpion*);
morpion FEN2mp(const char*);
supermorpion FEN2smp(const char*);
void showmp(const morpion*);
void showsmp(const supermorpion*);
void generatePNGmp(const morpion*,const char*);
void generatePNGsmp(const supermorpion*,const char*);
int minimaxmp(morpion,int,int);
int evalmp(morpion*,int);
void generatePNGmpTree(morpion,const char*);
char* mp2dot(morpion*,int,int,int);
int minimaxMpTreePNG(morpion,int,int,int,FILE*);
void printMpTreeDot(morpion);
int minimaxMpTreeDot(morpion,int,int,int);
#endif