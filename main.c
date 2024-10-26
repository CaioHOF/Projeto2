#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>
#include <math.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <time.h>

typedef struct Personality{
    char Name[20];
    int rarity;

    //Definir os modificadores em double como porcentagem, Exemplo 120 é 120%, 85 é 85%
    int BaseHPModifier;
    int BaseDefenseModifier;
    int BaseMagicDefenseModifier;
    int BaseAcurracyModifier;
    int BaseAttackModifier;
    int BaseElementalAcurracyModifier;
    int BaseMagicAttackModifier;
    int BaseSpeedModifier;

}Personality;

typedef struct Effect
{

    int Quantity;
    int Timer;

}Effect;

typedef struct Atribute
{

    char Name[20];
    int Base;
    int Total;
    int BonusQuantity;
    int *Bonus;
    int *BonusTimer;

}Atribute;

typedef struct Element
{

    char Name[20];
    int Effectiveness[10];
    int ElementalDamageScale;
    Effect CurrentHPEffect;
    Effect StatusEffect[8];


}Element;

typedef struct DataQuantity
{

    int Skill;
    int Item;
    int Pikomon;
    int Player;

}DataQuantity;

typedef struct Skill
{
    
    char Name[20];
    char Type[20];
    char Description[3][255];
    char Active[20];
    char ActiveDescription[3][255];
    //Target pode ser 'S' para self, 'E' para enemy, e 'B' para both
    bool LearnablePersonalities[13];
    bool LearnableElements[10];
    int ElementEffectChance;
    Element Element;

    char Target;
    int  AttackBase;
    int AttackScale;
    int MagicBase;
    int MagicAttackScale;
    int CritChance;

    //Isso daqui é expecificamente da skill, sem contar o elemento
    char EffectTarget;
    //Target pode ser 'S' para self, 'E' para enemy, e 'B' para both
    int EnemyEffectChance;
    Effect EnemyEffect[8];

    int SelfEffectChance;
    Effect SelfEffect[8];

}Skill, *SkPointer;

typedef struct Item
{
    bool EffectEnded;
    char Name[20];
    char Type[20];
    char Description[3][255];
    int Value;

    bool CurrentHPDamageIsPhysic;
    char EffectCurrentHPTarget;
    //Target pode ser 'S' para self, 'E' para enemy, e 'B' para both
    Effect EnemyEffectCurrentHP;
    Effect SelfEffectCurrentHP;
    //Esse valor é usado pra definir o quanto uma poção de cura cura e em quanto tempo, ou um veneno. O outro serve para alterar os status da vida maxima 
    
    char EffectTarget;
    //Target pode ser 'S' para self, 'E' para enemy, e 'B' para both
    int EnemyStatusEffectChance;
    Effect EnemyStatusEffect[8];

    int SelfStatusEffectChance;
    Effect SelfStatusEffect[8];

}Item, *ItPointer;

typedef struct Pikomon
{

    char Name[10];
    char IconImg[7][20];
    char Passive[20];
    char PassiveDescription[3][255];
    Element Element;
    Personality Personality;

    Atribute CurrentHP;
    Atribute Atributes[8];
    //Atributes[0].Name, "HP"
    //Atributes[1].Name, "Defense"
    //Atributes[2].Name, "MagicDefense"
    //Atributes[3].Name, "Acurracy"
    //Atributes[4].Name, "Attack"
    //Atributes[5].Name, "ElementalAcurracy"
    //Atributes[6].Name, "MagicAttack"
    //Atributes[7].Name, "Speed"
    
    int ChargedSpeed;
    //isso é para o calculo de turno, é a velocidade que ele acumula pra poder ter mais de um turno

    Skill Skills[4];

}Pikomon, *PiPointer;


typedef struct Player
{

    char Name[20];
    char Pass[7];
    
    int Pikocoins;

    int SelectedPikomonIndex;
    Pikomon BatlePikomons[6];

    Pikomon PikomonsStorage[12];

    int BagCurrentSize;
    ItPointer Bag;

}Player, *PlPointer;



//funções da main
bool DebugPlayers(PlPointer pPlayers, int index, int playersQuantity);
bool DebugPikomons(PiPointer pPikomon, int index, int pikomonsQuantity);
bool DebugItems(ItPointer pItems, int index, int ItemsQuantity);
bool DebugSkills(SkPointer pSkills, int index, int skillsQuantity);
bool SavePersonalities(Personality allPersonalities[10], const char* destino);
bool SaveElements(Element allElements[10], const char* destino);
bool SaveSkills(SkPointer pSkills, int skillsQuantity, const char* destino);
bool SaveItems(ItPointer pItems, int ItemsQuantity, const char* destino);
bool SavePikomons(PiPointer pPikomons, int pikomonsQuantity, const char* destino);
bool SavePlayers(PlPointer pPlayers, int playersQuantity, const char* destino);
bool Login(PlPointer pPlayers,int playersQuantity, bool *login1, bool *login2, char *nomeUsuario1, char *nomeUsuario2);

int main(){ 
    /**Declarações**/
    //------------------------------------------------------------------------------------------------------------------//
    setlocale(LC_ALL, "portuguese");
    setenv("TZ","America/Sao_Paulo",1);
    tzset();
    srand(time(NULL));
    
    const char *dataQuantity = "DataQuantity.txt", *players = "Players.bin", *items = "Items.bin", *pikomoms = "Pikomoms.bin", *skills = "Skills.bin", *personalities = "Personalities.bin", *elements = "Elements.bin";
    char readLine[256];

    DataQuantity dataQuantities;
    dataQuantities.Skill = -1;
    dataQuantities.Item = -1;
    dataQuantities.Pikomon = -1;
    dataQuantities.Player = -1;

    FILE *dBDataQuantity = NULL, *dBPlayers = NULL, *dBItens = NULL, *dBPikomons = NULL, *dBSkills = NULL, *dBPersonalities = NULL, *dBElements = NULL; 
    //dB é de "data base"

    int playerOneIndex = -1, playerTwoIndex = -1;

    Personality allPersonalities[13];
    memset(allPersonalities, 0, sizeof(allPersonalities));

    Element allElements[10];
    memset(allElements, 0, sizeof(allElements));

    SkPointer pSkills = NULL; 

    ItPointer pItems = NULL;
    
    PiPointer pPikomons = NULL;

    PlPointer pPlayers = NULL;

    // --------------------  Login  --------------------------------------

    char *nomeUsuario1[20], *nomeUsuario2[20];



    //------------------------------------------------------------------------------------------------------------------//

    /**Loads**/
    //------------------------------------------------------------------------------------------------------------------//

    dBPersonalities = fopen(personalities, "rb");
    if(dBPersonalities == NULL){
        perror("Falha ao abrir \"personalities\"");
        return 1;
    }
    fread(allPersonalities, sizeof(Personality), 13, dBPersonalities);
    if(allPersonalities == NULL){
        perror("Falha ao ler as personalidades!");
        return 2;
    }
    fclose(dBPersonalities);

    dBDataQuantity = fopen(dataQuantity, "r");
    if(dBDataQuantity == NULL){
        perror("Falha ao abrir \"dataQuantity\"");
        return 1;
    }
    fgets(readLine,256,dBDataQuantity);
    fgets(readLine,256,dBDataQuantity);
    sscanf(readLine, "%d,%d,%d,%d", &dataQuantities.Player, &dataQuantities.Pikomon, &dataQuantities.Item, &dataQuantities.Skill);
    fclose(dBDataQuantity);
    if(dataQuantities.Player == -1 || dataQuantities.Pikomon == -1 || dataQuantities.Item == -1 || dataQuantities.Skill == -1){
        perror("Falha ao aderir dados");
        return 3;
    }


    pSkills = (SkPointer)calloc(dataQuantities.Skill, sizeof(Skill));
    if(pSkills == NULL){
        perror("Falha ao allocar \"pSkills\"");
        return 2;
    }
    dBSkills = fopen(skills, "rb");
    if(dBSkills == NULL){
        perror("Falha ao abrir \"skills\"");
        return 1;
    }
    fread(pSkills,sizeof(Skill), dataQuantities.Skill, dBSkills);
    fclose(dBSkills);


    pItems = (ItPointer)calloc(dataQuantities.Item, sizeof(Item));
    if(pItems == NULL){
        perror("Falha ao allocar \"pItems\"");
        free(pSkills);
        return 2;
    }
    dBItens = fopen(items, "rb");
    if(dBItens == NULL){
        perror("Falha ao abrir \"items\"");
        free(pSkills);
        return 1;
    }
    fread(pItems,sizeof(Item), dataQuantities.Item, dBItens);
    fclose(dBItens);


    pPikomons = (PiPointer)calloc(dataQuantities.Pikomon, sizeof(Pikomon));
    if(pPikomons == NULL){
        perror("Falha ao allocar \"pPikomons\"");
        free(pItems);
        free(pSkills);
        return 2;
    }
    dBPikomons = fopen(pikomoms, "rb");
    if(dBPikomons == NULL){
        perror("Falha ao abrir \"players\"");
        free(pItems);
        free(pSkills);
        return 1;
    }
    fread(pPikomons, sizeof(Pikomon), dataQuantities.Pikomon, dBPikomons);
    fclose(dBPikomons);

    
    pPlayers = (PlPointer)calloc(dataQuantities.Player, sizeof(Player));
    if(pPlayers == NULL){
        perror("Falha ao allocar \"pPlayers\"");
        free(pPikomons);
        free(pItems);
        free(pSkills);
        return 2;
    }
    dBPlayers = fopen(players, "rb");
    if(dBPlayers == NULL){
        perror("Falha ao abrir \"players\"");
        free(pPikomons);
        free(pItems);
        free(pSkills);
        return 1;
    }
    fread(pPlayers, sizeof(Player), dataQuantities.Player, dBPlayers);
    fclose(dBPlayers);
    //------------------------------------------------------------------------------------------------------------------//


    /**Principal Do Usuário**/
    //------------------------------------------------------------------------------------------------------------------//
    //login
    //menu
    //------------------------------------------------------------------------------------------------------------------//
    //toda a vez que acaba uma batalha tem que usar essa função
    FreeAllHeapMemoryAndSaveEverything(pSkills,pItems,pPikomons,pPlayers,dataQuantities,dataQuantity,skills,items,pikomoms,players);
}




/**Debug/Print/Base Functions**/
//------------------------------------------------------------------------------//
bool DebugPlayers(PlPointer pPlayers, int index, int playersQuantity){
    /*if(index == -1){
        int j, k;
        for(index = 0; index < playersQuantity; index++){
            printf("| Name:| %20s | |,| Pass:| %s | |,| Pikocoins:| %3d | |,| BagCurrentSize:| %2d | |,| BagMaxSize| %2d | |;\n", pPlayers[index].Name, pPlayers[index].Pass, pPlayers[index].Pikocoins, pPlayers[index].BagCurrentSize, pPlayers[index].BagMaxSize);
            printf("\nBag:\n");
            for (j = 0; j < pPlayers[index].Bagcurrentsize; j++){
                printf("| Name:| %10s | |,| Type:| %10s | |,| Target:| %c | |;\n", pPlayers[index].Bag[j].Name, pPlayers[index].Bag[j].Type, pPlayers[index].Bag[j].Target);
            }
            printf("\nPikomons:\n");
            for (j = 0; j < 6; j++){
                printf("| Nome:| %10s | |,| Element:| %10s | |,| CurrentHP:| %3d | |,| MaxHP:| %3d | |,| Defense:| %d | |,| MagicDefense:| %d | |,| Attack:| %d | |,| MagicAttack:| %d | |,| Speed:| %d | |\n", pPlayers[index].Pikomons[j].Name, pPlayers[index].Pikomons[j].Element, pPlayers[index].Pikomons[j].HPCurrent, pPlayers[index].Pikomons[j].HPMax, pPlayers[index].Pikomons[j].Defense, pPlayers[index].Pikomons[j].MagicDefense, pPlayers[index].Pikomons[j].Attack, pPlayers[index].Pikomons[j].MagicAttack, pPlayers[index].Pikomons[j].Speed);
                printf("\nPikomonSkills:\n");
                for(k = 0; k < 4; k++){
                    pPlayers[index].Pikomons[j].Skills[k];
                    printf("| Nome:| %20s | |,| Target:| %c | |,| AttackScale:| %1.2lf | |,| MagicAttackScale:| %1.2lf | |;\n", pPlayers[index].Pikomons[j].Skills.Name, pPlayers[index].Pikomons[j].Skills[k].Target, pPlayers[index].Pikomons[j].Skills[k].AttackScale, pPlayers[index].Pikomons[j].Skills[k].MagicAttackScale);
                    printf("| Element[0]:| %d | |,| Element[1]:| %d | |,| Element[2]:| %d | |,| Element[3]:| %d | |,| Element[4]:| %d | |;\n| Element[5]:| %d | |,| Element[6]:| %d | |,| Element[7]:| %d | |,| Element[8]:| %d | |,| Element[9]:| %d | |;\n",pPlayers[index].Pikomons[j].Skills[k].Element[0],pPlayers[index].Pikomons[j].Skills[k].Element[1],pPlayers[index].Pikomons[j].Skills[k].Element[2],pPlayers[index].Pikomons[j].Skills[k].Element[3],pPlayers[index].Pikomons[j].Skills[k].Element[4],pPlayers[index].Pikomons[j].Skills[k].Element[5],pPlayers[index].Pikomons[j].Skills[k].Element[6],pPlayers[index].Pikomons[j].Skills[k].Element[7],pPlayers[index].Pikomons[j].Skills[k].Element[8],pPlayers[index].Pikomons[j].Skills[k].Element[9]);
                }    
            }
            printf("\n");
        }
    }
    else if(index > -1 && index < playersQuantity){
        int j, k;
        printf("| Name:| %20s | |,| Pass:| %s | |,| Pikocoins:| %3d | |,| BagCurrentSize:| %2d | |,| BagMaxSize| %2d | |;\n", pPlayers[index].Name, pPlayers[index].Pass, pPlayers[index].Pikocoins, pPlayers[index].BagCurrentSize, pPlayers[index].BagMaxSize);
        printf("\nBag:\n");
        for (j = 0; j < pPlayers[index].BagMaxSize; j++){
            printf("| Name:| %10s | |,| Type:| %10s | |,| Target:| %c | |;\n", pPlayers[index].Bag[j].Name, pPlayers[index].Bag[j].Type, pPlayers[index].Bag[j].Target);
        }
        printf("\nPikomons:\n");
        for (j = 0; j < 6; j++){
            printf("| Nome:| %10s | |,| Element:| %10s | |,| CurrentHP:| %3d | |,| MaxHP:| %3d | |,| Defense:| %d | |,| MagicDefense:| %d | |,| Attack:| %d | |,| MagicAttack:| %d | |,| Speed:| %d | |\n", pPlayers[index].Pikomons[j].Name, pPlayers[index].Pikomons[j].Element, pPlayers[index].Pikomons[j].HPCurrent, pPlayers[index].Pikomons[j].HPMax, pPlayers[index].Pikomons[j].Defense, pPlayers[index].Pikomons[j].MagicDefense, pPlayers[index].Pikomons[j].Attack, pPlayers[index].Pikomons[j].MagicAttack, pPlayers[index].Pikomons[j].Speed);
            printf("\nPikomonSkills:\n");
            for(k = 0; k < 4; k++){
                printf("| Nome:| %20s | |,| Target:| %c | |,| AttackScale:| %1.2lf | |,| MagicAttackScale:| %1.2lf | |;\n", pPlayers[index].Pikomons[j].Skills[k].Name, pPlayers[index].Pikomons[j].Skills[k].Target, pPlayers[index].Pikomons[j].Skills[k].AttackScale, pPlayers[index].Pikomons[j].Skills[k].MagicAttackScale);
                printf("| Element[0]:| %d | |,| Element[1]:| %d | |,| Element[2]:| %d | |,| Element[3]:| %d | |,| Element[4]:| %d | |;\n| Element[5]:| %d | |,| Element[6]:| %d | |,| Element[7]:| %d | |,| Element[8]:| %d | |,| Element[9]:| %d | |;\n",pPlayers[index].Pikomons[j].Skills[k].Element[0],pPlayers[index].Pikomons[j].Skills[k].Element[1],pPlayers[index].Pikomons[j].Skills[k].Element[2],pPlayers[index].Pikomons[j].Skills[k].Element[3],pPlayers[index].Pikomons[j].Skills[k].Element[4],pPlayers[index].Pikomons[j].Skills[k].Element[5],pPlayers[index].Pikomons[j].Skills[k].Element[6],pPlayers[index].Pikomons[j].Skills[k].Element[7],pPlayers[index].Pikomons[j].Skills[k].Element[8],pPlayers[index].Pikomons[j].Skills[k].Element[9]);
            }    
        }
        printf("\n");
    }
    else{
        perror("index usado na função \"DegubPlayers\" não permitido");
        return false;
    }
    return true;*/
}

bool DebugPikomons(PiPointer pPikomon, int index, int pikomonsQuantity){
    /*if(index == -1){
        int j;
        for(index = 0; index < pikomonsQuantity; index++){
            printf("| Nome:| %10s | |,| Element:| %10s | |,| CurrentHP:| %3d | |,| MaxHP:| %3d | |,| Defense:| %d | |,| MagicDefense:| %d | |,| Attack:| %d | |,| MagicAttack:| %d | |,| Speed:| %d | |\n", pPikomon[index].Name, pPikomon[index].Element, pPikomon[index].HPCurrent, pPikomon[index].HPMax, pPikomon[index].Defense, pPikomon[index].MagicDefense, pPikomon[index].Attack, pPikomon[index].MagicAttack, pPikomon[index].Speed);
        }
    }
    else if(index > -1 && index < pikomonsQuantity){
        printf("| Nome:| %10s | |,| Element:| %10s | |,| CurrentHP:| %3d | |,| MaxHP:| %3d | |,| Defense:| %d | |,| MagicDefense:| %d | |,| Attack:| %d | |,| MagicAttack:| %d | |,| Speed:| %d | |\n", pPikomon[index].Name, pPikomon[index].Element, pPikomon[index].HPCurrent, pPikomon[index].HPMax, pPikomon[index].Defense, pPikomon[index].MagicDefense, pPikomon[index].Attack, pPikomon[index].MagicAttack, pPikomon[index].Speed);
    }
    else{
        perror("index usado na função \"DebugPikomons\" não permitido");
        return false;
    }
    return true;*/
}

bool DebugItems(ItPointer pItems, int index, int ItemsQuantity){
    /*if(index == -1){
        for(index = 0; index < ItemsQuantity; index++){
            printf("| Name:| %10s | |,| Type:| %10s | |,| Target:| %c | |;\n", pItems[index].Name, pItems[index].Type, pItems[index].Target);
        }
    }
    else if(index > -1 && index < ItemsQuantity){
            printf("| Name:| %10s | |,| Type:| %10s | |,| Target:| %c | |;\n", pItems[index].Name, pItems[index].Type, pItems[index].Target);
    }
    else{
        perror("index usado na função \"DebugItems\" não permitido");
        return false;
    }
    return true;*/
}

bool DebugSkills(SkPointer pSkills, int index, int skillsQuantity){
    /*if(index == -1){
        int i;
        for(i = 0; i < skillsQuantity; i++){
            printf("| Nome:| %20s | |,| Target:| %c | |,| AttackScale:| %1.2lf | |,| MagicAttackScale:| %1.2lf | |;\n", pSkills[index].Name, pSkills[index].Target, pSkills[index].AttackScale, pSkills[index].MagicAttackScale);
            printf("| Element[0]:| %d | |,| Element[1]:| %d | |,| Element[2]:| %d | |,| Element[3]:| %d | |,| Element[4]:| %d | |;\n| Element[5]:| %d | |,| Element[6]:| %d | |,| Element[7]:| %d | |,| Element[8]:| %d | |,| Element[9]:| %d | |;\n\n",pSkills[index].Element[0],pSkills[index].Element[1],pSkills[index].Element[2],pSkills[index].Element[3],pSkills[index].Element[4],pSkills[index].Element[5],pSkills[index].Element[6],pSkills[index].Element[7],pSkills[index].Element[8],pSkills[index].Element[9]);
        }
    }
    else if(index > -1 && index < skillsQuantity){
        printf("| Nome:| %20s | |,| Target:| %c | |,| AttackScale:| %1.2lf | |,| MagicAttackScale:| %1.2lf | |;\n", pSkills[index].Name, pSkills[index].Target, pSkills[index].AttackScale, pSkills[index].MagicAttackScale);
        printf("| Element[0]:| %d | |,| Element[1]:| %d | |,| Element[2]:| %d | |,| Element[3]:| %d | |,| Element[4]:| %d | |;\n| Element[5]:| %d | |,| Element[6]:| %d | |,| Element[7]:| %d | |,| Element[8]:| %d | |,| Element[9]:| %d | |;\n\n",pSkills[index].Element[0],pSkills[index].Element[1],pSkills[index].Element[2],pSkills[index].Element[3],pSkills[index].Element[4],pSkills[index].Element[5],pSkills[index].Element[6],pSkills[index].Element[7],pSkills[index].Element[8],pSkills[index].Element[9]);
    }
    else{
        perror("index usado na função \"DebugSkills\" não permitido");
        return false;
    }
    return true;*/
}
//------------------------------------------------------------------------------//



/**Save Functions**/
//------------------------------------------------------------------------------//
bool SavePersonalities(Personality allPersonalities[13], const char *destino){
    FILE* dBPersonalities;
    dBPersonalities = fopen(destino, "wb");
    if (dBPersonalities == NULL)
    {
        perror("falha ao abrir \"dBPersonalities\" na função \"SavePersonalities\"");
        return false;
    }
    fwrite(allPersonalities, sizeof(Personality), 13, dBPersonalities);
    fclose(dBPersonalities);
    return true;
}

bool SaveElements(Element allElements[10], const char *destino){
    FILE* dBElements;
    dBElements = fopen(destino, "wb");
    if (dBElements == NULL)
    {
        perror("falha ao abrir \"dBElements\" na função \"SaveElements\"");
        return false;
    }
    fwrite(allElements, sizeof(Element), 10, dBElements);
    fclose(dBElements);
    return true;
}

bool SaveDataQuantity(DataQuantity dataQuantities, const char *destino){
    FILE* dBDataQuantity;
    dBDataQuantity = fopen(destino, "w");
    if(dBDataQuantity == NULL){
        perror("falha ao abrir \"dBDataQuantity\" na função \"SaveDataQuantity\"");
        return false;
    }
    fprintf(dBDataQuantity, "Players,Pikomons,Itens,Skills\n%d,%d,%d,%d", dataQuantities.Player,dataQuantities.Pikomon,dataQuantities.Item,dataQuantities.Skill);
    fclose(dBDataQuantity);
    return true;
}

bool SaveSkills(SkPointer pSkills, int skillsQuantity, const char *destino){
    FILE* dBSkills;
    dBSkills = fopen(destino, "wb");
    if (dBSkills == NULL)
    {
        perror("falha ao abrir \"dBSkills\" na função \"SaveSkills\"");
        return false;
    }
    fwrite(pSkills, sizeof(Skill), skillsQuantity, dBSkills);
    fclose(dBSkills);
    return true;
}

bool SaveItems(ItPointer pItems, int ItemsQuantity, const char *destino){
    FILE* dBItems;
    dBItems = fopen(destino, "wb");
    if (dBItems == NULL)
    {
        perror("falha ao abrir \"dBItems\" na função \"SaveItems\"");
        return false;
    }
    fwrite(pItems, sizeof(Item), ItemsQuantity, dBItems);
    fclose(dBItems);
    return true;
}

bool SavePikomons(PiPointer pPikomons, int pikomonsQuantity, const char *destino){
    FILE* dBPikomons;
    dBPikomons = fopen(destino, "wb");
    if (dBPikomons == NULL)
    {
        perror("falha ao abrir \"dBPikomons\" na função \"SavePikomons\"");
        return false;
    }
    fwrite(pPikomons, sizeof(Pikomon), pikomonsQuantity, dBPikomons);
    fclose(dBPikomons);
    return true;
}

bool SavePlayers(PlPointer pPlayers, int playersQuantity, const char *destino){
    FILE* dBPlayers;
    dBPlayers = fopen(destino, "wb");
    if (dBPlayers == NULL)
    {
        perror("falha ao abrir \"dBPlayers\" na função \"SavePlayers\"");
        return false;
    }
    fwrite(pPlayers, sizeof(Player), playersQuantity, dBPlayers);
    fclose(dBPlayers);
    return true;
}

//favor usar isso quando as batalhas acabarem para liberar as memorias dinamicas
void FreeAllHeapMemoryAndSaveEverything(SkPointer pSkills, ItPointer pItems, PiPointer pPikomons, PlPointer pPlayers, DataQuantity dataquantities, const char *dataQuantity, const char *skills, const char *items, const char *pikomoms, const char *players){
    //pode ter um memset cagado aqui
    if(pSkills == NULL){
        perror("ERRO, \"pSkills\" é NULL em \"FreeAllHeapMemory\"");
    }
    if(pItems == NULL){
        perror("ERRO, \"pItems\" é NULL em \"FreeAllHeapMemory\"");
    }
    if(pPikomons == NULL){
        perror("\"pPikomons\" é NULL em \"FreeAllHeapMemory\"");
    }
    if(pPlayers == NULL){
        perror("ERRO, \"pPlayers\" não pode ser NULL em \"FreeAllHeapMemory\"");
    }
    SaveDataQuantity(dataquantities, dataQuantity);
    SaveSkills(pSkills, dataquantities.Skill, skills);
    free(pSkills);
    SaveItems(pItems, dataquantities.Item, items);
    free(pItems);
    SavePikomons(pPikomons, dataquantities.Pikomon, pikomoms);
    free(pPikomons);
    int i, j, k;
    for(i = 0; i < dataquantities.Player; i++){
        for(j = 0; j < pPlayers[i].BagCurrentSize; j++){
            pPlayers[i].Pikocoins += pPlayers[i].Bag[j].Value;
        }
        pPlayers[i].BagCurrentSize = 0;
        free(pPlayers[i].Bag);
        for(j = 0; j < 6; j++){
            for(k = 0; k < 8; k++){
                free(pPlayers[i].BatlePikomons[j].Atributes[k].Bonus);
                free(pPlayers[i].BatlePikomons[j].Atributes[k].BonusTimer);
                pPlayers[i].BatlePikomons[j].Atributes[k].BonusQuantity = 0;
            }
        }
        //esse cara aqui pode dar ruim
        memset(pPlayers[i].BatlePikomons, 0, 6 * sizeof(Pikomon));
    }
    SavePlayers(pPlayers, dataquantities.Player, players);
    free(pPlayers);
}
//------------------------------------------------------------------------------//



/**Manage Memory Functions**/
//------------------------------------------------------------------------------//
bool AddSkill(SkPointer pSkills, DataQuantity dataQuantities, char *name, char target, bool learnablePersonalities[13], bool LearnableElements[10], double elementEffectChance, Element element, int  attackBase, double attackScale, int magicBase, double magicAttackScale, double critChance, char effectTarget, double enemyEffectChance, Effect enemyEffect[8], double selfEffectChance, Effect selfEffect[8]){
    //Se o memset estiver errado ele estara apagando memoria de outras variaveis;
    if(pSkills == NULL){
        perror("ERRO, \"pSkills\" não pode ser NULL em \"AddSkill\"");
        return false;
    }
    if(name == NULL){
        perror("ERRO, \"name\" não pode ser NULL em \"AddSkill\"");
        return false;
    }
    if(target != 'S' && target != 'E' && target != 'B'){
        perror("ERRO, \"target\" não pode ser diferente de 'S', 'E' ou 'B' em \"AddSkill\"");
        return false;
    }
    if(elementEffectChance < 0){
        perror("ERRO, \"elementEffectChance\" não pode ser menor que zero em \"AddSkill\"");
        return false;
    }
    if(critChance < 0){
        perror("ERRO, \"critChance\" não pode ser menor que zero em \"AddSkill\"");
        return false;
    }
    if(effectTarget != 'S' && effectTarget != 'E' && effectTarget != 'B'){
        perror("ERRO, \"effectTarget\" não pode ser diferente de 'S', 'E' ou 'B' em \"AddSkill\"");
        return false;
    }
    if(enemyEffectChance < 0){
        perror("ERRO, \"enemyEffectChance\" não pode ser menor que zero em \"AddSkill\"");
        return false;
    }
    if(selfEffectChance < 0){
        perror("ERRO, \"selfEffectChance\" não pode ser menor que zero em \"AddSkill\"");
        return false;
    }

    dataQuantities.Skill++;
    pSkills = (SkPointer)realloc(pSkills, dataQuantities.Skill * sizeof(Skill));
    if(pSkills == NULL){
        perror("ERRO na realocação de memoria em \"AddSkill\"");
        return false;
    }
    memset(&pSkills[dataQuantities.Skill-1], 0, sizeof(Skill));
    strcpy(pSkills[dataQuantities.Skill-1].Name, name);
    pSkills[dataQuantities.Skill-1].Target = target;
    int i;
    for(i = 0; i < 13; i++){
        pSkills[dataQuantities.Skill-1].LearnablePersonalities[i] = learnablePersonalities[i];
        pSkills[dataQuantities.Skill-1].LearnableElements[i] = LearnableElements[i];
    }
    pSkills[dataQuantities.Skill-1].ElementEffectChance = elementEffectChance;
    pSkills[dataQuantities.Skill-1].Element = element;
    pSkills[dataQuantities.Skill-1].AttackBase = attackBase;
    pSkills[dataQuantities.Skill-1].AttackScale = attackScale;
    pSkills[dataQuantities.Skill-1].MagicBase = magicBase;
    pSkills[dataQuantities.Skill-1].MagicAttackScale = magicAttackScale;
    pSkills[dataQuantities.Skill-1].CritChance = critChance;
    pSkills[dataQuantities.Skill-1].EffectTarget = effectTarget;
    pSkills[dataQuantities.Skill-1].EnemyEffectChance = enemyEffectChance;
    for(i = 0; i < 8; i++){
        pSkills[dataQuantities.Skill-1].EnemyEffect[i] = enemyEffect[i];
    }
    pSkills[dataQuantities.Skill-1].SelfEffectChance = selfEffectChance;
    for(i = 0; i < 8; i++){
        pSkills[dataQuantities.Skill-1].SelfEffect[i] = selfEffect[i];
    }
}

bool AddItem(ItPointer pItems, DataQuantity dataQuantities, char *name, char *type, char *description[3], int value, char effectCurrentHPTarget, Effect enemyEffectCurrentHP, Effect selfEffectCurrentHP, char effectTarget, double enemyStatusEffectChance, Effect enemyStatusEffect[8], double selfStatusEffectChance, Effect selfStatusEffect[8]){
    //Se o memset estiver errado ele estara apagando memoria de outras variaveis;
    if(pItems == NULL){
        perror("ERRO, \"pItems\" não pode ser NULL em \"AddItem\"");
        return false;
    }
    if(name == NULL){
        perror("ERRO, \"name\" não pode ser NULL em \"AddItem\"");
        return false;
    }
    else if(strlen(name) > 19){
        perror("ERRO, \"name\" não pode ter mais que 19 caracteres em \"AddItem\"");
        return false;
    }
    if(type == NULL){
        perror("ERRO, \"name\" não pode ser NULL em \"AddItem\"");
        return false;
    }
    int i;
    for(i = 0; i < 3; i++){
        if(strlen(description[i]) > 254){
            perror("ERRO, nenhuma das 3 strings na variavel \"*description[3]\" pode ser maior que 254 caracteres em \"AddItem\"");
            return false;
        }
    }


    dataQuantities.Item++;
    pItems = (ItPointer)realloc(pItems, dataQuantities.Item * sizeof(Item));
    if(pItems == NULL){
        perror("ERRO na realocação de memoria em \"AddItem\"");
        return false;
    }
    memset(&pItems[dataQuantities.Item-1], 0, sizeof(Item));
    strcpy(pItems[dataQuantities.Item-1].Name, name);
    strcpy(pItems[dataQuantities.Item-1].Type, type);
    for(i = 0; i < 3; i++){
        strcpy(pItems[dataQuantities.Item-1].Description[i], description[i]);
    }
    pItems[dataQuantities.Item-1].Value = value;
    pItems[dataQuantities.Item-1].EffectCurrentHPTarget = effectCurrentHPTarget;
    pItems[dataQuantities.Item-1].EnemyEffectCurrentHP = enemyEffectCurrentHP;
    pItems[dataQuantities.Item-1].SelfEffectCurrentHP = selfEffectCurrentHP;
    pItems[dataQuantities.Item-1].EffectTarget = effectTarget;
    pItems[dataQuantities.Item-1].EnemyStatusEffectChance = enemyStatusEffectChance;
    for(i = 0; i < 8; i++){
        pItems[dataQuantities.Item-1].EnemyStatusEffect[i] = enemyStatusEffect[i];
    }
    pItems[dataQuantities.Item-1].SelfStatusEffectChance = selfStatusEffectChance;
    for(i = 0; i < 8; i++){
        pItems[dataQuantities.Item-1].SelfStatusEffect[i] = selfStatusEffect[i];
    }

}

bool AddPikomon(PiPointer pPikomons, DataQuantity dataQuantities, char *name, Element element, char iconImg[7][19], int BaseHP, int BaseDefense, int BaseMagicDefense, int BaseAcurracy, int BaseAttack, int BaseElementalAcurracy, int BaseMagicAttack, int BaseSpeed){
    //Se o memset estiver errado ele estara apagando memoria de outras variaveis;
    if(pPikomons == NULL){
        perror("ERRO, \"pPikomons\" não pode ser NULL em \"AddPikomon\"");
        return false;
    }
    if(name == NULL){
        perror("ERRO, \"name\" não pode ser NULL em \"AddPikomon\"");
        return false;
    }
    else if(strlen(name) > 19){
        perror("ERRO, \"name\" não pode ter mais de 19 caracteres em \"AddPikomon\"");
        return false;
    }
    if(iconImg == NULL){
        perror("ERRO, \"icoImg\" não pode ser NULL em \"AddPikomon\"");
        return false;
    }
    if(BaseHP == 0){
        perror("ERRO, \"BaseHP\" não pode ser 0 em \"AddPikomon\"");
        return false;
    }
    if(BaseDefense == 0){
        perror("ERRO, \"BaseDefense\" não pode ser 0 em \"AddPikomon\"");
        return false;
    }
    if(BaseMagicDefense == 0){
        perror("ERRO, \"BaseMagicDefense\" não pode ser 0 em \"AddPikomon\"");
        return false;
    }
    if(BaseAcurracy == 0){
        perror("ERRO, \"BaseAcurracy\" não pode ser 0 em \"AddPikomon\"");
        return false;
    }
    if(BaseAttack == 0){
        perror("ERRO, \"BaseAttack\" não pode ser 0 em \"AddPikomon\"");
        return false;
    }
    if(BaseElementalAcurracy == 0){
        perror("ERRO, \"BaseElementalAcurracy\" não pode ser 0 em \"AddPikomon\"");
        return false;
    }
    if(BaseMagicAttack == 0){
        perror("ERRO, \"BaseMagicAttack\" não pode ser 0 em \"AddPikomon\"");
        return false;
    }
    if(BaseSpeed == 0){
        perror("ERRO, \"BaseSpeed\" não pode ser 0 em \"AddPikomon\"");
        return false;
    }


    dataQuantities.Pikomon++;
    pPikomons = (PiPointer)realloc(pPikomons, dataQuantities.Pikomon * sizeof(Pikomon));
    if(pPikomons == NULL){
        perror("ERRO na realocação de memoria em \"AddPikomon\"");
        return false;
    }
    memset(&pPikomons[dataQuantities.Pikomon-1], 0, sizeof(Pikomon));
    strcpy(pPikomons[dataQuantities.Pikomon-1].Name, name);
    pPikomons[dataQuantities.Pikomon-1].Element = element;
    int i;
    for(i = 0; i < 7; i++){
        strcpy(pPikomons[dataQuantities.Pikomon-1].IconImg[i], iconImg[i]);
    }
    strcpy(pPikomons[dataQuantities.Pikomon-1].CurrentHP.Name, "CurrentHP");
    pPikomons[dataQuantities.Pikomon-1].CurrentHP.Base = 0;
    strcpy(pPikomons[dataQuantities.Pikomon-1].Atributes[0].Name, "HP");
    pPikomons[dataQuantities.Pikomon-1].Atributes[0].Base = BaseHP;
    strcpy(pPikomons[dataQuantities.Pikomon-1].Atributes[1].Name, "Defense");
    pPikomons[dataQuantities.Pikomon-1].Atributes[1].Base = BaseDefense;
    strcpy(pPikomons[dataQuantities.Pikomon-1].Atributes[2].Name, "MagicDefense");
    pPikomons[dataQuantities.Pikomon-1].Atributes[2].Base = BaseMagicDefense;
    strcpy(pPikomons[dataQuantities.Pikomon-1].Atributes[3].Name, "Acurracy");
    pPikomons[dataQuantities.Pikomon-1].Atributes[3].Base = BaseAcurracy;
    strcpy(pPikomons[dataQuantities.Pikomon-1].Atributes[4].Name, "Attack");
    pPikomons[dataQuantities.Pikomon-1].Atributes[4].Base = BaseAttack;
    strcpy(pPikomons[dataQuantities.Pikomon-1].Atributes[5].Name, "ElementalAcurracy");
    pPikomons[dataQuantities.Pikomon-1].Atributes[5].Base = BaseElementalAcurracy;
    strcpy(pPikomons[dataQuantities.Pikomon-1].Atributes[6].Name, "MagicAttack");
    pPikomons[dataQuantities.Pikomon-1].Atributes[6].Base = BaseMagicAttack;
    strcpy(pPikomons[dataQuantities.Pikomon-1].Atributes[7].Name, "Speed");
    pPikomons[dataQuantities.Pikomon-1].Atributes[7].Base = BaseSpeed;
    return true;
}

bool AddPlayer(PlPointer pPlayers, DataQuantity dataQuantities, char *name, char *pass){
    //Se o memset estiver errado ele estara apagando memoria de outras variaveis;
    if(pPlayers == NULL){
        perror("ERRO, \"pPlayers\" não pode ser NULL em \"AddPlayers\"");
        return false;
    }
    if(pass == NULL){
        perror("ERRO, \"pass\" não pode ser NULL em \"AddPlayers\"");
        return false;
    }
    else if(strlen(pass) != 6){
        perror("ERRO, \"pass\" não pode ter um tamanho diferente de 6 caracteres em \"AddPlayers\"");
        return false;
    }
    if(name == NULL){
        perror("ERRO, \"name\" não pode ser NULL em \"AddPlayers\"");
        return false;
    }
    else if (strlen(name) > 19){
        perror("ERRO, \"name\" não pode ter mais de 19 caracteres em \"AddPlayers\"");
        return false;
    }
    
    dataQuantities.Player++;
    pPlayers = (PlPointer)realloc(pPlayers, dataQuantities.Player * sizeof(Player));
    if(pPlayers == NULL){
        perror("ERRO na realocação de memoria em \"AddPlayers\"");
        return false;
    }
    memset(&pPlayers[dataQuantities.Player-1], 0, sizeof(Player));
    strcpy(pPlayers[dataQuantities.Player-1].Name, name);
    strcpy(pPlayers[dataQuantities.Player-1].Pass, pass);
    pPlayers[dataQuantities.Player-1].Pikocoins = 50;
    return true;
}

bool AddItemPlayerBag(PlPointer pPlayers, int playerIndex, ItPointer pItems, int itemIndex){
    if(pPlayers == NULL){
        perror("ERRO, \"pPlayers\" não pode ser NULL em \"AddItemPlayerBag\"");
        return false;
    }
    if(pItems == NULL){
        perror("ERRO, \"pItems\" não pode ser NULL em \"AddItemPlayerBag\"");
        return false;
    }
    if(playerIndex < 0){
        perror("ERRO, \"playerIndex\" não pode ser menor do que zero em \"AddItemPlayerBag\"");
        return false;
    }
    if(itemIndex < 0){
        perror("ERRO, \"itemIndex\" não pode ser menor do que zero em \"AddItemPlayerBag\"");
        return false;
    }

    pPlayers[playerIndex].BagCurrentSize++;
    pPlayers[playerIndex].Bag = (ItPointer)realloc(pPlayers[playerIndex].Bag, pPlayers[playerIndex].BagCurrentSize * sizeof(Item));
    if(pPlayers[playerIndex].Bag == NULL){
        perror("ERRO na realocação de memoria em \"AddItemPlayerBag\"");
        return false;
    }
    pPlayers[playerIndex].Bag[pPlayers[playerIndex].BagCurrentSize-1] = pItems[itemIndex];
}

bool StorePikomonPlayer(PlPointer pPlayers, int playerIndex, int storagePikomonPlacementIndex, PiPointer pPikomons, int pikomonIndex, DataQuantity dataQuantities){
    //n precisa uasar esse
    if(pPlayers == NULL){
        perror("ERRO, \"pPlayers\" não pode ser NULL em \"StorePikomonPlayer\"");
        return false;
    }
    if(playerIndex < 0){
        perror("ERRO, \"playerIndex\" não pode ser menor que zero em \"StorePikomonPlayer\"");
        return false;
    }
    else if(playerIndex >= dataQuantities.Player){
        perror("ERRO, \"playerIndex\" tem que ser menor que \"dataQuantities.Player\" em \"StorePikomonPlayer\"");
        return false;
    }
    if(storagePikomonPlacementIndex < 0){
        perror("ERRO, \"storagePikomonPlacementIndex\" não pode ser menor que zero em \"StorePikomonPlayer\"");
        return false;
    }
    else if(storagePikomonPlacementIndex >= 12){
        perror("ERRO, \"storagePikomonPlacementIndex\" tem que ser menor que 12 em \"StorePikomonPlayer\"");
        return false;
    }
    if(pPikomons == NULL){
        perror("ERRO, \"pPikomons\" não pode ser NULL em \"StorePikomonPlayer\"");
        return false;
    }
    if(pikomonIndex < 0){
        perror("ERRO, \"pikomonIndex\" não pode ser menor que zero em \"StorePikomonPlayer\"");
        return false;
    }
    else if(pikomonIndex >= dataQuantities.Pikomon){
        perror("ERRO, \"pikomonIndex\" tem que ser menor que \"dataQuantities.Pikomon\" em \"StorePikomonPlayer\"");
        return false;
    }
    pPlayers[playerIndex].PikomonsStorage[storagePikomonPlacementIndex] = pPikomons[pikomonIndex];
}

bool RemoveSkill(SkPointer pSkills, DataQuantity dataQuantities, int indexRemove){
    if(indexRemove < 0){
        perror("ERRO, \"indexRemove\" não pode ser menor que zero em \"RemoveSkill\"");
        return false;
    }
    else if(indexRemove >= dataQuantities.Skill){
        perror("ERRO, \"indexRemove\" tem que ser menor que \"dataQuantities.Skill\" em \"RemoveSkill\"");
        return false;
    }
    SkPointer tempSkills;
    tempSkills = (SkPointer)calloc(dataQuantities.Skill-1, sizeof(Skill));
    int i, j = 0;
    for(i = 0; i < dataQuantities.Skill; i++){
        if(i != indexRemove){
            tempSkills[j] = pSkills[i];
            j++;
        }
    }
    dataQuantities.Skill--;
    pSkills = (SkPointer)realloc(pSkills, dataQuantities.Skill *sizeof(Skill));
    for(i = 0; i < dataQuantities.Skill; i++){
        pSkills[i] = tempSkills[i];
    }
    free(tempSkills);
}

bool RemoveItem(ItPointer pItems, DataQuantity dataQuantities, int indexRemove){
    if(indexRemove < 0){
        perror("ERRO, \"indexRemove\" não pode ser menor que zero em \"RemoveItem\"");
        return false;
    }
    else if(indexRemove >= dataQuantities.Item){
        perror("ERRO, \"indexRemove\" tem que ser menor que \"dataQuantities.Item\" em \"RemoveItem\"");
        return false;
    }
    ItPointer tempItems;
    tempItems = (ItPointer)calloc(dataQuantities.Item-1, sizeof(Item));
    int i, j = 0;
    for(i = 0; i < dataQuantities.Item; i++){
        if(i != indexRemove){
            tempItems[j] = pItems[i];
            j++;
        }
    }
    dataQuantities.Item--;
    pItems = (ItPointer)realloc(pItems, dataQuantities.Item *sizeof(Item));
    for(i = 0; i < dataQuantities.Item; i++){
        pItems[i] = tempItems[i];
    }
    free(tempItems);
}

bool RemovePikomon(PiPointer pPikomons, DataQuantity dataQuantities, int indexRemove){
    if(indexRemove < 0){
        perror("ERRO, \"indexRemove\" não pode ser menor que zero em \"RemovePikomon\"");
        return false;
    }
    else if(indexRemove >= dataQuantities.Pikomon){
        perror("ERRO, \"indexRemove\" tem que ser menor que \"dataQuantities.Pikomon\" em \"RemovePikomon\"");
        return false;
    }
    PiPointer tempPikomons;
    tempPikomons = (PiPointer)calloc(dataQuantities.Pikomon-1, sizeof(Pikomon));
    int i, j = 0;
    for(i = 0; i < dataQuantities.Pikomon; i++){
        if(i != indexRemove){
            tempPikomons[j] = pPikomons[i];
            j++;
        }
    }
    dataQuantities.Pikomon--;
    pPikomons = (PiPointer)realloc(pPikomons, dataQuantities.Pikomon * sizeof(Pikomon));
    for(i = 0; i < dataQuantities.Pikomon; i++){
        pPikomons[i] = tempPikomons[i];
    }
    free(tempPikomons);
}

bool SellItemPlayerBag(PlPointer pPlayers, int playerIndex, int bagSellIndex){ 
    if(bagSellIndex >= pPlayers[playerIndex].BagCurrentSize){
        perror("ERRO, \"bagSellIndex\" tem que ser menor que \"pPlayers[playerIndex].BagCurrentSize\" em \"SellItemPlayerBag\"");
        return false;
    }
    else if(bagSellIndex < 0){
        perror("ERRO, \"bagSellIndex\" não pode ser menor que zero em \"SellItemPlayerBag\"");
        return false;
    }
    if(pPlayers[playerIndex].BagCurrentSize == 0){
        perror("ERRO, \"pPlayers[playerIndex].BagCurrentSize\" não pode ser zero em \"SellItemPlayerBag\"");
        return false;
    }
    
    ItPointer tempItems;
    tempItems = (ItPointer)calloc(pPlayers[playerIndex].BagCurrentSize-1, sizeof(Item));
    int i, j = 0;
    for(i = 0; i < pPlayers[playerIndex].BagCurrentSize; i++){
        if(i != bagSellIndex){
            tempItems[j] = pPlayers[playerIndex].Bag[i];
            j++; 
        }
        else{
            pPlayers[playerIndex].Pikocoins += pPlayers[playerIndex].Bag[i].Value;
        }
    }
    pPlayers[playerIndex].BagCurrentSize--;
    pPlayers[playerIndex].Bag = (ItPointer)realloc(pPlayers[playerIndex].Bag, pPlayers[playerIndex].BagCurrentSize * sizeof(Item));
    for(i = 0; i < pPlayers[playerIndex].BagCurrentSize; i++){
        pPlayers[playerIndex].Bag[i] = tempItems[i];
    }
    free(tempItems);
}
//------------------------------------------------------------------------------//



/**Battle functions**/
//------------------------------------------------------------------------------//
void CalcNextTurn(Pikomon selfPikomon, Pikomon enemyPikomon, char calcNextTurn[7]){ 
    //calcNextTurn vai ser a resposta a ser gerada

    calcNextTurn[6] = '\0';
    bool b;
    int i = 0, turnCost, selfSpeedCharged = 0, enemySpeedCharge = 0;
    if(selfPikomon.Atributes[7].Total > enemyPikomon.Atributes[7].Total) turnCost = enemyPikomon.Atributes[7].Total, b = true;
    else if(selfPikomon.Atributes[7].Total < enemyPikomon.Atributes[7].Total) turnCost = selfPikomon.Atributes[7].Total, b = false;

    if(selfPikomon.Atributes[7].Total == enemyPikomon.Atributes[7].Total){
        if((rand() % 100 + 1) > 50) b = true;
        else b = false;
        turnCost = selfPikomon.Atributes[7].Total;
    }
    while(i < 6){
        if(b){
            selfSpeedCharged += selfPikomon.Atributes[7].Total;
            while(selfSpeedCharged - turnCost >= 0){
                selfSpeedCharged -= turnCost;
                calcNextTurn[i] = '1';
                i++;
                if(i >= 6) break;
            }
            b = !b;
        }
        else{
            enemySpeedCharge += enemyPikomon.Atributes[7].Total;
            while(enemySpeedCharge - turnCost >= 0){
                enemySpeedCharge -= turnCost;
                calcNextTurn[i] = '2';
                i++;
                if(i >= 6) break;
            }
            b = !b;
        }
    }
}

void Batle(PlPointer pPlayers, int playerOneIndex, int playerTwoIndex){
    bool playerOneTurn, reset, nextTurnReset, battleIsOver;
    int i = 0, turnCost;
    PiPointer selectedPlayerOnePicomon = &pPlayers[playerOneIndex].BatlePikomons[pPlayers[playerOneIndex].SelectedPikomonIndex], selectedPlayerTwoPicomon = &pPlayers[playerTwoIndex].BatlePikomons[pPlayers[playerTwoIndex].SelectedPikomonIndex];
    reset = true;
    battleIsOver = false;
    while(!battleIsOver){
        if(nextTurnReset){
            nextTurnReset = false;
            reset = true;
        }
        //Recalcula os turnos toda a vez que tiver um reset, tipo quando a velocidade alterar
        if(reset){
            if(selectedPlayerOnePicomon[0].Atributes[7].Total == selectedPlayerTwoPicomon[0].Atributes[7].Total){
                if((rand() % 100 + 1) > 50) playerOneTurn = true;
                else playerOneTurn = false;
            turnCost = selectedPlayerOnePicomon[0].Atributes[7].Total;
            }
            else if(selectedPlayerOnePicomon[0].Atributes[7].Total > selectedPlayerTwoPicomon[0].Atributes[7].Total) turnCost = selectedPlayerTwoPicomon[0].Atributes[7].Total, playerOneTurn = true;
            else if(selectedPlayerOnePicomon[0].Atributes[7].Total < selectedPlayerTwoPicomon[0].Atributes[7].Total) turnCost = selectedPlayerOnePicomon[0].Atributes[7].Total, playerOneTurn = false;
        }


        if(playerOneTurn){
            selectedPlayerOnePicomon[0].ChargedSpeed += selectedPlayerOnePicomon[0].Atributes[7].Total;
            while(selectedPlayerOnePicomon[0].ChargedSpeed - turnCost >= 0){
                selectedPlayerOnePicomon[0].ChargedSpeed -= turnCost;

                /* 
                    ações do player1
                */

            }
            playerOneTurn = false;
        }
        else{
            selectedPlayerTwoPicomon[0].ChargedSpeed += selectedPlayerTwoPicomon[0].Atributes[7].Total;
            while(selectedPlayerTwoPicomon[0].ChargedSpeed - turnCost >= 0){
                selectedPlayerTwoPicomon[0].ChargedSpeed -= turnCost;

                /* 
                    ações do player2
                */

            }
            playerOneTurn = true;
        }
    }
}

void CalcSkill(PiPointer atacker, int skillIndex, PiPointer defenser){
    
    
    double ElementEffectChance;
    Element Element;

    char Target;
    int  AttackBase;
    double AttackScale;
    int MagicBase;
    double MagicAttackScale;
    double CritChance;

    //Isso daqui é expecificamente da skill, sem contar o elemento
    char EffectTarget;
    //Target pode ser 'S' para self, 'E' para enemy, e 'B' para both
    double EnemyEffectChance;
    Effect EnemyEffect[8];

    double SelfEffectChance;
    Effect SelfEffect[8];
    



    double physicalDamageReduction, magicDamageReduction;
    physicalDamageReduction = 1.0 - ((log10(defenser[0].Atributes[1].Total)/log10(2)) * 0.11);
    magicDamageReduction = 1.0 - ((log10(defenser[0].Atributes[2].Total)/log10(2)) * 0.11);
    SkPointer usedSkill = &atacker[0].Skills[skillIndex];
    defenser[0].CurrentHP.Total -= 
}

void UseItem(){

}
//------------------------------------------------------------------------------//


bool Login(PlPointer pPlayers, int playersQuantity, bool *login1, bool *login2, char *nomeUsuario1, char *nomeUsuario2) {
    char usernameEntrada[20];
    char passEntrada[7];
    int userNumero = 1;
    int indexPlayerLoop;
    bool usernameAchado, senhaAchado;

    *login1 = false;
    *login2 = false;

    while (!(*login1) || !(*login2)) {
        usernameAchado = false;
        senhaAchado = false;

        if (!(*login1)) {
            userNumero = 1;
        } else {
            userNumero = 2;
        }

        MenuLogin(userNumero);
        printf("                    Insira seu Username: ");
        scanf("%19s", usernameEntrada); 
        printf("                    Insira sua Senha   : ");
        scanf("%6s", passEntrada);

        for (indexPlayerLoop = 0; indexPlayerLoop < playersQuantity; indexPlayerLoop++) {
            if (strcmp(pPlayers[indexPlayerLoop].Name, usernameEntrada) == 0 &&
                strcmp(pPlayers[indexPlayerLoop].Pass, passEntrada) == 0) {
                usernameAchado = true;
                senhaAchado = true;
                break;
            }
        }

        if (usernameAchado && senhaAchado) {
            if (!(*login1)) {
                strcpy(nomeUsuario1, usernameEntrada);
                *login1 = true;
            } else {
                strcpy(nomeUsuario2, usernameEntrada);
                *login2 = true;
            }
            printf("Bem-vindo, %s! Você está logado!\n", usernameEntrada);
        } else {
            printf("Usuário ou senha incorretos. Tente novamente.\n");
        }
    }

    return true;
}