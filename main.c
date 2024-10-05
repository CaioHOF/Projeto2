#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

typedef struct Skill
{

    char Name[20];
    char Element[10][4];
    char UseType[2][10];
    
    double AttackScale;
    double MagicAttackScale;

}Skill, *SkPointer;

typedef struct Item
{
    char Name[10];
    char Type[10];


}Item, *ItPointer;

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
    int BagCurrentSize;
    int BagMaxSize;

    Pikomon Pikomons[6]; 
    
    ItPointer Bag;

}Player, *PlPointer;
 

int main(){
    setlocale(LC_ALL, "portuguese");
    setenv("TZ","America/Sao_Paulo",1);
    tzset();
    srand(time(NULL));
    
    const char *quantidadeDados = "QuantidadesDados.txt", *players = "Players.bin", *items = "Items.bin", *pikomons = "Pokimons.bin", *skills = "Skills.bin";
    char readLine[256];

    int playersQuantity = -1, pikomonsQuantity = -1, itemsQuantity = -1, skillsQuantity = -1;
    int playerOneIndex = -1, playerTwoIndex = -1;

    FILE *dBQuantidadeDados, *dBPlayers, *dBItens, *dBPikomons, *dBSkills; 
    //dB é de "data base"

    Player PlayerOne, PlayerTwo;

    PlPointer pPlayers;

    PiPointer pPikomons;
    
    ItPointer pItems;

    SkPointer pSkills; 


    dBQuantidadeDados = fopen(quantidadeDados, "r");
    if(dBQuantidadeDados == NULL){
        perror("Falha ao abrir \"quantidadeDados\"");
        return 1;
    } 
    fgets(readLine,256,dBQuantidadeDados);
    fgets(readLine,256,dBQuantidadeDados);
    sscanf(readLine, "%d,%d,%d,%d", playersQuantity, pikomonsQuantity, itemsQuantity, skillsQuantity);
    fclose(dBQuantidadeDados);
    if(playersQuantity == -1 || pikomonsQuantity == -1 || itemsQuantity == -1 || skillsQuantity == -1){
        perror("Falha ao aderir dados");
        return 3;
    }

    pPlayers = (PlPointer)calloc(playersQuantity, sizeof(Player));
    if(dBPlayers == NULL){
        perror("Falha ao allocar \"pPlayers\"");
        free(pPlayers);
        return 2;
    }
    dBPlayers = fopen(players, "rb");
    if(dBPlayers == NULL){
        perror("Falha ao abrir \"players\"");
        free(pPlayers);
        return 1;
    }
    fread(pPlayers, sizeof(Player), playersQuantity, dBPlayers);
    fclose(dBPlayers);


    pPikomons = (PiPointer)calloc(pikomonsQuantity, sizeof(Pikomon));
    if(pPikomons == NULL){
        perror("Falha ao allocar \"pPikomons\"");
        free(pPlayers);
        free(pPikomons);
        return 2;
    }
    dBPikomons = fopen(pikomons, "rb");
    if(dBPikomons == NULL){
        perror("Falha ao abrir \"players\"");
        free(pPlayers);
        free(pPikomons);
        return 1;
    }
    fread(pPikomons, sizeof(Pikomon), pikomonsQuantity, dBPikomons);
    fclose(dBPikomons);


    pItems = (ItPointer)calloc(itemsQuantity, sizeof(Item));
    if(pItems == NULL){
        perror("Falha ao allocar \"pItems\"");
        free(pPlayers);
        free(pPikomons);
        free(pItems);
        return 2;
    }
    dBItens = fopen(items, "rb");
    if(dBItens == NULL){
        perror("Falha ao abrir \"items\"");
        free(pPlayers);
        free(pPikomons);
        free(pItems);
        return 1;
    }
    fread(pItems,sizeof(Item), itemsQuantity, dBItens);
    fclose(dBItens);

    pSkills = (SkPointer)calloc(skillsQuantity, sizeof(Skill));
    if(pSkills == NULL){
        perror("Falha ao allocar \"pSkills\"");
        free(pPlayers);
        free(pPikomons);
        free(pItems);
        free(pSkills);
        return 2;
    }
    dBSkills = fopen(skills, "rb");
    if(dBSkills == NULL){
        perror("Falha ao abrir \"skills\"");
        free(pPlayers);
        free(pPikomons);
        free(pItems);
        free(pSkills);
        return 1;
    }
    fread(pSkills,sizeof(Skill), skillsQuantity, dBSkills);
    fclose(dBSkills);


}

void DebugPlayers(PlPointer pPlayers, int index, int playersQuantity){

}

void DebugPikomons(PiPointer pPikomon, int index, int pikomonsQuantity){
    
}

void DebugItems(ItPointer pItems, int index, int ItemsQuantity){
    
}

void DebugSkills(SkPointer pSkills, int index, int skillsQuantity){
    
}

void SavePlayers(PlPointer pPlayers, int playersQuantity){

}

void SavePikomons(PiPointer pPikomons, int pikomonsQuantity){
    
}

void SaveItems(ItPointer pItems, int ItemsQuantity){
    
}

void SaveSkills(SkPointer pSkills, int skillsQuantity){
    
}