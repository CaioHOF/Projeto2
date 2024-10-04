#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

typedef struct Skill
{
    char Element[10][4];

}Skill, *SkPointer;

typedef struct Itens
{
    char Name[10];
    char Type[10];
    
}Itens, *ItPointer;

typedef struct Pikomon
{
    char Name[10];
    char Element[4];
    int HPCurrent;
    int HPMax;
    int Defense;
    int MagicDefense;
    int Attack;
    int MagicAttack;
    int Speed;
    Skill Skills[4];
}Pikomon, *PiPointer;


typedef struct Player
{

    char Name[20];
    char Pass[6];
    int Pikocoins;
    Pikomon Pikomons[6]; 
    Item

}Player, *PlPointer;
 

int main(){
    setlocale(LC_ALL, "portuguese");
    setenv("TZ","America/Sao_Paulo",1);
    tzset();
    srand(time(NULL));

}