#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

typedef struct Effect
{

    int Bonus;
    int Timer;

}Effect;


typedef struct Atribute
{

    char Name[20];
    int TotalBonus;
    int BonusQuantity;
    int *Bonus;
    int *BonusTimer;

}Atribute;

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
    char Target;
    char Element[10];
    //Target pode ser 'S' para self, 'E' para enemy, e 'B' para both
    
    bool ElementLearn[10];

    int  AttackBase;
    double AttackScale;
    int MagicBase;
    double MagicAttackScale;

    double CritChance;

    char EffectTarget;
    //Target pode ser 'S' para self, 'E' para enemy, e 'B' para both
    double EnemyEffectChance; 
    Effect EnemyEffect[6];

    double SelfEffectChance;
    Effect SelfEffect[6];

}Skill, *SkPointer;

typedef struct Item
{

    char Name[10];
    char Type[10];
    char Target;
    //Target pode ser 'S' para self, 'E' para enemy, e 'B' para both
    char EffectTarget;
    //Target pode ser 'S' para self, 'E' para enemy, e 'B' para both

    Effect EffectPoints;
    //Esse valor é usado pra definir o quanto uma poção de cura cura e em quanto tempo, ou um veneno. O outro serve para alterar os status da vida maxima 
    
    double EnemyEffectChance;
    Effect EnemyEffect[6];
    //esse "[6]" é referente a cada status que o pikomon tem, tipo o [0] é o status de HP, colocando bonus nele voce aumenta a vida maxima por um periodo de tempo

    double SelfEffectChance;
    Effect SelfEffect[6];
    //esse "[6]" é referente a cada status que o pikomon tem, tipo o [1] que é o status de defesa, colocando bonus nele voce a defesa de um alvo por um periodo de tempo

}Item, *ItPointer;

typedef struct Pikomon
{

    char Name[10];
    char IconImg[7][19];
    char Element[10];

    int HPCurrent;
    int BaseHP;
    int BaseDefense;
    int BaseMagicDefense;
    int BaseAttack;
    int BaseMagicAttack;
    int BaseSpeed;

    Atribute Atributes[6];
    //Esses são os valores mutaveis. Cada um dos seis é respectivo dos status originais sendo o Atributes[0] = BaseHP, Atributes[1] = BaseDefesa e assim por diante

    Skill Skills[4];
    
}Pikomon, *PiPointer;


typedef struct Player
{

    char Name[20];
    char Pass[7];
    
    int Pikocoins;
    int BagCurrentSize;
    int SelectedPikomonIndex;

    Pikomon Pikomons[6];
    Pikomon PikomonsStored[12];

    ItPointer Bag;

}Player, *PlPointer;


//funções da main
int DefineIndexOfElement(char* NomeElemento);
bool DebugPlayers(PlPointer pPlayers, int index, int playersQuantity);
bool DebugPikomons(PiPointer pPikomon, int index, int pikomonsQuantity);
bool DebugItems(ItPointer pItems, int index, int ItemsQuantity);
bool DebugSkills(SkPointer pSkills, int index, int skillsQuantity);
bool SaveDataQuantity(DataQuantity dataQuantities, char* destino);
bool SavePlayers(PlPointer pPlayers, int playersQuantity, char* destino);
bool SavePikomons(PiPointer pPikomons, int pikomonsQuantity, char* destino);
bool SaveItems(ItPointer pItems, int ItemsQuantity, char* destino);
bool SaveSkills(SkPointer pSkills, int skillsQuantity, char* destino);
void LimparTerminal();
void Menu();
void MenuLogin(int userNumero);


int main(){ 
    setlocale(LC_ALL, "portuguese");
    setenv("TZ","America/Sao_Paulo",1);
    tzset();
    srand(time(NULL));
    
    const char *dataQuantity = "DataQuantity.txt", *players = "Players.bin", *items = "Items.bin", *pikomons = "Pikomons.bin", *skills = "Skills.bin";
    char readLine[256];

    DataQuantity dataQuantities;
    dataQuantities.Skill = -1;
    dataQuantities.Item = -1;
    dataQuantities.Pikomon = -1;
    dataQuantities.Player = -1;

    FILE *dBDataQuantity, *dBPlayers, *dBItens, *dBPikomons, *dBSkills; 
    //dB é de "data base"

    PlPointer pPlayerOne = NULL, pPlayerTwo = NULL;
    int playerOneIndex = -1, playerTwoIndex = -1;
    int respostaUserMP;

    PlPointer pPlayers;

    PiPointer pPikomons;
    
    ItPointer pItems;

    SkPointer pSkills; 


    dBDataQuantity = fopen(dataQuantity, "r");
    if(dBDataQuantity == NULL){
        perror("Falha ao abrir \"quantidadeDados\"");
        return 1;
    } 
    fgets(readLine,256,dBDataQuantity);
    fgets(readLine,256,dBDataQuantity);
    sscanf(readLine, "%d,%d,%d,%d,%d", dataQuantities.Player, dataQuantities.Pikomon, dataQuantities.Item, dataQuantities.Skill);
    fclose(dBDataQuantity);
    if(dataQuantities.Player == -1 || dataQuantities.Pikomon == -1 || dataQuantities.Item == -1 || dataQuantities.Skill == -1){
        perror("Falha ao aderir dados");
        return 3;
    }

    pPlayers = (PlPointer)calloc(dataQuantities.Player, sizeof(Player));
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
    fread(pPlayers, sizeof(Player), dataQuantities.Player, dBPlayers);
    fclose(dBPlayers);


    pPikomons = (PiPointer)calloc(dataQuantities.Pikomon, sizeof(Pikomon));
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
    fread(pPikomons, sizeof(Pikomon), dataQuantities.Pikomon, dBPikomons);
    fclose(dBPikomons);


    pItems = (ItPointer)calloc(dataQuantities.Item, sizeof(Item));
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
    fread(pItems,sizeof(Item), dataQuantities.Item, dBItens);
    fclose(dBItens);

    pSkills = (SkPointer)calloc(dataQuantities.Skill, sizeof(Skill));
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
    fread(pSkills,sizeof(Skill), dataQuantities.Skill, dBSkills);
    fclose(dBSkills);

    //loginMenuPrincipal
    while (true){
        Menu();
        scanf("Sua escolha?: %d", &respostaUserMP);
        if(respostaUserMP == 1){
            char usernameEntrada[50];
            char passEntrada[50];
            bool login1 = false;
            bool login2 = false;
            int userNumero = 1;
            while(login1 && login2){
                //if(login1)
                //{
                    //userNumero = 2;
                //}
                MenuLogin(userNumero);
                printf("                    Insira seu Username: ");
                scanf("%s", usernameEntrada);
                printf("                    Insira sua Senha   : ");
                scanf("%s", passEntrada);
            }
        }
        if(respostaUserMP == 2){
            printf(":)");
        }
        if(respostaUserMP == 3){
            printf(":)");
        }
        if(respostaUserMP == 4){
            printf(":)");
        }


    }
    
}

int DefineIndexOfElement(char* NomeElemento){
    //retorna o index fixo de um determinado elemento, caso ele não encontre o elemento ele retorna -1;
    // é só continua a logica em baixo
    if(strcmp(NomeElemento, "sua mae")) return 0;
    else if(strcmp(NomeElemento, "teu pai")) return 1;
    else return -1;
}

bool DebugPlayers(PlPointer pPlayers, int index, int playersQuantity){
    /*if(index == -1){
        int j, k;
        for(index = 0; index < playersQuantity; index++){
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
    if(index == -1){
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
    return true;
}

bool DebugSkills(SkPointer pSkills, int index, int skillsQuantity){
    if(index == -1){
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
    return true;
}

bool SaveDataQuantity(DataQuantity dataQuantities, char* destino){
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

bool SavePlayers(PlPointer pPlayers, int playersQuantity, char* destino){
    FILE* dBPlayers;
    dBPlayers = fopen(destino, 'wb');
    if (dBPlayers == NULL)
    {
        perror("falha ao abrir \"dBPlayers\" na função \"SavePlayers\"");
        return false;
    }
    fwrite(pPlayers, sizeof(Player), playersQuantity, dBPlayers);
    fclose(dBPlayers);
    return true;
}

bool SavePikomons(PiPointer pPikomons, int pikomonsQuantity, char* destino){
    FILE* dBPikomons;
    dBPikomons = fopen(destino, 'wb');
    if (dBPikomons == NULL)
    {
        perror("falha ao abrir \"dBPikomons\" na função \"SavePikomons\"");
        return false;
    }
    fwrite(pPikomons, sizeof(Pikomon), pikomonsQuantity, dBPikomons);
    fclose(dBPikomons);
    return true;
}

bool SaveItems(ItPointer pItems, int ItemsQuantity, char* destino){
    FILE* dBItems;
    dBItems = fopen(destino, 'wb');
    if (dBItems == NULL)
    {
        perror("falha ao abrir \"dBItems\" na função \"SaveItems\"");
        return false;
    }
    fwrite(pItems, sizeof(Item), ItemsQuantity, dBItems);
    fclose(dBItems);
    return true;
}

bool SaveSkills(SkPointer pSkills, int skillsQuantity, char* destino){
    FILE* dBSkills;
    dBSkills = fopen(destino, 'wb');
    if (dBSkills == NULL)
    {
        perror("falha ao abrir \"dBSkills\" na função \"SaveSkills\"");
        return false;
    }
    fwrite(pSkills, sizeof(Skill), skillsQuantity, dBSkills);
    fclose(dBSkills);
    return true;
}

bool AddPlayer(PlPointer pPlayers, DataQuantity dataQuantities, char *name, char *pass){
    //Se o memset estiver errado ele estara apagando memoria de outras variaveis;
    dataQuantities.Player++;
    pPlayers = (PlPointer)realloc(pPlayers, dataQuantities.Player * sizeof(Player));
    if(pPlayers == NULL){
        perror("ERRO na realocação de memoria em \"AddPlayers\"");
        return false;
    }
    memset(pPlayers + (dataQuantities.Player -2), 0, sizeof(Player));
    strcpy(pPlayers[dataQuantities.Player-1].Name, name);
    strcpy(pPlayers[dataQuantities.Player-1].Pass, pass);
    return true;
}

bool AddPikomon(PiPointer pPikomons, DataQuantity dataQuantities, char *name, char *element, char **iconImg, int HPCurrent,int BaseHP, int BaseDefense, int BaseMagicDefense, int BaseAttack, int BaseMagicAttack, int BaseSpeed){
    //Se o memset estiver errado ele estara apagando memoria de outras variaveis;
    dataQuantities.Pikomon++;
    pPikomons = (PiPointer)realloc(pPikomons, dataQuantities.Pikomon * sizeof(Pikomon));
    if(pPikomons == NULL){
        perror("ERRO na realocação de memoria em \"AddPikomon\"");
        return false;
    }
    memset(pPikomons + (dataQuantities.Pikomon -2), 0, sizeof(Pikomon));
    strcpy(pPikomons[dataQuantities.Pikomon-1].Name, name);
    strcpy(pPikomons[dataQuantities.Pikomon-1].Element, element);
    int i;
    for(i = 0; i < 7; i++){
        strcpy(pPikomons[dataQuantities.Pikomon-1].IconImg[i], iconImg[i]);
    }
    pPikomons[dataQuantities.Pikomon-1].HPCurrent = HPCurrent;
    pPikomons[dataQuantities.Pikomon-1].BaseHP = BaseHP;
    pPikomons[dataQuantities.Pikomon-1].BaseDefense = BaseDefense;
    pPikomons[dataQuantities.Pikomon-1].BaseMagicDefense = BaseMagicDefense;
    pPikomons[dataQuantities.Pikomon-1].BaseAttack = BaseAttack;
    pPikomons[dataQuantities.Pikomon-1].BaseMagicAttack = BaseMagicAttack;
    pPikomons[dataQuantities.Pikomon-1].BaseSpeed = BaseSpeed;
    return true;
}

bool AddItem(ItPointer pItems, DataQuantity dataQuantities, char *Name, char *Type, char Target){
    
}

bool AddSkill(SkPointer pSkills, DataQuantity dataQuantities, char *Name, char Target, bool Element[10], double AttackScale, double MagicAttackScale){
    
}

bool AddItemPlayerBag(){

}


void Menu(){
    struct timespec tempo;
    tempo.tv_sec = 0;              
    tempo.tv_nsec = 500000000;




printf("                *********************************\n");
nanosleep(&tempo, NULL);
printf("                *       B E M   V I N D O       *\n");
nanosleep(&tempo, NULL);
printf("                *********************************\n");
nanosleep(&tempo, NULL);
printf("\n");
nanosleep(&tempo, NULL);
printf("\n");
nanosleep(&tempo, NULL);
printf("     *********************************************************\n");
nanosleep(&tempo, NULL);
printf("     * 1. P L A Y                   3. A L L P I K O S       *\n");
nanosleep(&tempo, NULL);
printf("     *    - - - -                      - - - - - - - -       *\n");
nanosleep(&tempo, NULL);
printf("     *                                                       *\n");
nanosleep(&tempo, NULL);
printf("     *                                                       *\n");
nanosleep(&tempo, NULL);
printf("     * 2. P L A Y E R S I N F O     4. E X I T               *\n");
nanosleep(&tempo, NULL);
printf("     *    - - - - - - - - - - -        - - - -               *\n");
nanosleep(&tempo, NULL);
printf("     *********************************************************\n");
}


void MenuLogin(int userNumero) {
    printf("                ***************************************\n");
    printf("                *                 US%d                 *\n", userNumero);
    printf("                ***************************************\n");
    printf("\n");
}


void LimparTerminal() {
    printf("\033[H\033[J");
}