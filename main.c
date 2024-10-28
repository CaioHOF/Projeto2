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
    int BaseAccuracyModifier;
    int BaseAttackModifier;
    int BaseElementalAccuracyModifier;
    int BaseMagicAttackModifier;
    int BaseSpeedModifier;

}Personality;

typedef struct Effect
{

    char Acronym[4];
    int Quantity;
    int Timer;

}Effect;

typedef struct Atribute
{

    char Name[20];
    int Base;
    int Total;
    int BonusQuantity;
    char **acronym;
    int *Bonus;
    int *BonusTimer;

}Atribute;

typedef struct Element
{

    char Name[20];
    char Acronym[4];
    int Effectiveness[10];
    int ElementalDamageScale;
    int SelfElementIndex;
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
    int ElementEffectHitChance;
    Element Element;

    char Target;
    int hitChance;
    int AttackBase;
    int AttackScale;
    int MagicBase;
    int MagicAttackScale;
    int CritChance;

    //Isso daqui é expecificamente da skill, sem contar o elemento
    char EffectTarget;
    //Target pode ser 'S' para self, 'E' para enemy, e 'B' para both
    int EnemyEffectHitChance;
    Effect EnemyEffect[8];

    int SelfEffectHitChance;
    Effect SelfEffect[8];

}Skill, *SkPointer;

typedef struct Item
{

    char Name[20];
    char Type[20];
    char Description[3][255];
    char Active[20];
    char ActiveDescription[3][255];
    int Value;

    bool CurrentHPDamageIsPhysic;
    char EffectCurrentHPTarget;
    //Target pode ser 'S' para self, 'E' para enemy, e 'B' para both
    Effect EffectCurrentHP;
    //Esse valor é usado pra definir o quanto uma pocao de cura cura e em quanto tempo, ou um veneno. O outro serve para alterar os status da vida maxima 
    
    char EffectTarget;
    //Target pode ser 'S' para self, 'E' para enemy, e 'B' para both
    int StatusEffectChance;
    Effect StatusEffect[8];

}Item, *ItPointer;

typedef struct Pikomon
{

    char Name[10];
    char IconImg[7][20];
    char Passive[20];
    char PassiveDescription[3][255];
    int Value;
    Element Element;
    Personality Personality;

    Atribute CurrentHP;
    Atribute Atributes[8];
    //Atributes[0].Name, "HP"
    //Atributes[1].Name, "Defense"
    //Atributes[2].Name, "MagicDefense"
    //Atributes[3].Name, "Accuracy"
    //Atributes[4].Name, "Attack"
    //Atributes[5].Name, "ElementalAccuracy"
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



//funcoes da main
bool DebugPlayers(PlPointer pPlayers, int index, int playersQuantity);
bool DebugPikomons(PiPointer pPikomon, int index, int pikomonsQuantity);
bool DebugItems(ItPointer pItems, int index, int ItemsQuantity);
bool DebugSkills(SkPointer pSkills, int index, int skillsQuantity);
bool DebugPersonality(Personality *personalities, int index);
bool DebugElements(Element *elements, int index);
bool SavePersonalities(Personality allPersonalities[13], const char *destino);
bool SaveElements(Element allElements[10], const char *destino);
bool SaveDataQuantity(DataQuantity dataQuantities, const char *destino);
bool SaveSkills(SkPointer pSkills, int skillsQuantity, const char *destino);
bool SaveItems(ItPointer pItems, int ItemsQuantity, const char *destino);
bool SavePikomons(PiPointer pPikomons, int pikomonsQuantity, const char *destino);
bool SavePlayers(PlPointer pPlayers, int playersQuantity, const char *destino);
void FreeAllHeapMemoryAndSaveEverything(SkPointer pSkills, ItPointer pItems, PiPointer pPikomons, PlPointer pPlayers, DataQuantity dataquantities, const char *dataQuantity, const char *skills, const char *items, const char *pikomoms, const char *players);
bool AddSkill(SkPointer pSkills, DataQuantity dataQuantities, char *name, char target, bool learnablePersonalities[13], bool LearnableElements[10], int elementEffectChance, Element element, int  attackBase, int attackScale, int magicBase, int magicAttackScale, int critChance, char effectTarget, int enemyEffectChance, Effect enemyEffect[8], int selfEffectChance, Effect selfEffect[8]);
<<<<<<< HEAD
bool AddItem(ItPointer pItems, DataQuantity dataQuantities, char *name, char *type, char description[3][255], int value, char effectCurrentHPTarget, Effect EffectCurrentHP, char effectTarget, double StatusEffectChance, Effect StatusEffect[8]);
bool AddPikomon(PiPointer pPikomons, DataQuantity dataQuantities, char *name, Element element, char iconImg[7][19], int BaseHP, int BaseDefense, int BaseMagicDefense, int BaseAccuracy, int BaseAttack, int BaseElementalAccuracy, int BaseMagicAttack, int BaseSpeed);
bool AddPlayer(PlPointer pPlayers, DataQuantity dataQuantities, char *name, char *pass);
bool AddItemPlayerBag(PlPointer pPlayers, int playerIndex, ItPointer pItems, int itemIndex);
=======
bool AddItem(ItPointer pItems, DataQuantity dataQuantities, char *name, char *type, char *description[3], int value, char effectCurrentHPTarget, Effect enemyEffectCurrentHP, Effect selfEffectCurrentHP, char effectTarget, double enemyStatusEffectChance, Effect enemyStatusEffect[8], double selfStatusEffectChance, Effect selfStatusEffect[8]);
bool AddPikomon(PiPointer *pPikomons, DataQuantity *dataQuantities, char *name, Element element, char iconImg[7][19], int BaseHP, int BaseDefense, int BaseMagicDefense, int BaseAcurracy, int BaseAttack, int BaseElementalAcurracy, int BaseMagicAttack, int BaseSpeed);
bool AddPlayer(PlPointer *pPlayers, DataQuantity *dataQuantities, char *name, char *pass);
bool AddItemPlayerBag(PlPointer *pPlayers, int playerIndex, ItPointer pItems, int itemIndex);;
>>>>>>> origin/Shop
bool StorePikomonPlayer(PlPointer pPlayers, int playerIndex, int storagePikomonPlacementIndex, PiPointer pPikomons, int pikomonIndex, DataQuantity dataQuantities);
bool RemoveSkill(SkPointer pSkills, DataQuantity dataQuantities, int indexRemove);
bool RemoveItem(ItPointer pItems, DataQuantity dataQuantities, int indexRemove);
bool RemovePikomon(PiPointer pPikomons, DataQuantity dataQuantities, int indexRemove);
bool SellItemPlayerBag(PlPointer pPlayers, int playerIndex, int bagSellIndex);
void CalcNextTurn(Pikomon selfPikomon, Pikomon enemyPikomon, char calcNextTurn[7]);
void CalcSkill(Element allElements[10], PiPointer *atacker, int skillIndex, PiPointer *defenser, bool *elementalEffectHit, bool *skillHit, bool *critHit, bool *selfEffectHit, bool *enemyEffectHit, int *selfDamage, int *enemyDamage);
void Batle(PlPointer pPlayers, int playerOneIndex, int playerTwoIndex);
<<<<<<< HEAD
double DefenseReductionCalc(double value);
=======
void LimparTerminal();
void Menu();
void MenuLogin(int userNumero);
void MenuBattle(Pikomon epPikomon, Pikomon ppPikomon, char *Turnos);
bool Login(PlPointer pPlayers, int playersQuantity, bool *login1, bool *login2, int *indexUs1, int *indexUs2);
bool ShopPikomon(PlPointer players, int playerAtualIndex, PiPointer pPikomon, DataQuantity pikomonQuantidade, Personality* personalities);
void MenuShopMP();
>>>>>>> origin/Shop


int main(){ 
    //Declaracoes
    //------------------------------------------------------------------------------------------------------------------//
    setlocale(LC_ALL, "portuguese");
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
    //------------------------------------------------------------------------------------------------------------------//

    //Loads
    //------------------------------------------------------------------------------------------------------------------//
    //Define Personalities
    /*strcpy(allPersonalities[0].Name, "Cabeca-Quente");
    allPersonalities[0].rarity = 8;
    allPersonalities[0].BaseHPModifier = 100;
    allPersonalities[0].BaseDefenseModifier = 100;
    allPersonalities[0].BaseMagicDefenseModifier = 100;
    allPersonalities[0].BaseAccuracyModifier = 70;
    allPersonalities[0].BaseAttackModifier = 130;
    allPersonalities[0].BaseElementalAccuracyModifier = 70;
    allPersonalities[0].BaseMagicAttackModifier = 130;
    allPersonalities[0].BaseSpeedModifier = 100;

    strcpy(allPersonalities[1].Name, "Meigo");
    allPersonalities[1].rarity = 10;
    allPersonalities[1].BaseHPModifier = 100;
    allPersonalities[1].BaseDefenseModifier = 115;
    allPersonalities[1].BaseMagicDefenseModifier = 115;
    allPersonalities[1].BaseAccuracyModifier = 100;
    allPersonalities[1].BaseAttackModifier = 85;
    allPersonalities[1].BaseElementalAccuracyModifier = 100;
    allPersonalities[1].BaseMagicAttackModifier = 85;
    allPersonalities[1].BaseSpeedModifier = 100;

    strcpy(allPersonalities[2].Name, "Apressado");
    allPersonalities[2].rarity = 5;
    allPersonalities[2].BaseHPModifier = 100;
    allPersonalities[2].BaseDefenseModifier = 100;
    allPersonalities[2].BaseMagicDefenseModifier = 100;
    allPersonalities[2].BaseAccuracyModifier = 95;
    allPersonalities[2].BaseAttackModifier = 80;
    allPersonalities[2].BaseElementalAccuracyModifier = 95;
    allPersonalities[2].BaseMagicAttackModifier = 80;
    allPersonalities[2].BaseSpeedModifier = 130;

    strcpy(allPersonalities[3].Name, "Dorminhoco");
    allPersonalities[3].rarity = 11;
    allPersonalities[3].BaseHPModifier = 100;
    allPersonalities[3].BaseDefenseModifier = 100;
    allPersonalities[3].BaseMagicDefenseModifier = 130;
    allPersonalities[3].BaseAccuracyModifier = 100;
    allPersonalities[3].BaseAttackModifier = 100;
    allPersonalities[3].BaseElementalAccuracyModifier = 100;
    allPersonalities[3].BaseMagicAttackModifier = 80;
    allPersonalities[3].BaseSpeedModifier = 70;

    strcpy(allPersonalities[4].Name, "Místico");
    allPersonalities[4].rarity = 6;
    allPersonalities[4].BaseHPModifier = 100;
    allPersonalities[4].BaseDefenseModifier = 90;
    allPersonalities[4].BaseMagicDefenseModifier = 130;
    allPersonalities[4].BaseAccuracyModifier = 100;
    allPersonalities[4].BaseAttackModifier = 70;
    allPersonalities[4].BaseElementalAccuracyModifier = 100;
    allPersonalities[4].BaseMagicAttackModifier = 130;
    allPersonalities[4].BaseSpeedModifier = 100;

    strcpy(allPersonalities[5].Name, "Hercúleo");
    allPersonalities[5].rarity = 6;
    allPersonalities[5].BaseHPModifier = 100;
    allPersonalities[5].BaseDefenseModifier = 110;
    allPersonalities[5].BaseMagicDefenseModifier = 90;
    allPersonalities[5].BaseAccuracyModifier = 100;
    allPersonalities[5].BaseAttackModifier = 130;
    allPersonalities[5].BaseElementalAccuracyModifier = 100;
    allPersonalities[5].BaseMagicAttackModifier = 70;
    allPersonalities[5].BaseSpeedModifier = 100;

    strcpy(allPersonalities[6].Name, "Assustado");
    allPersonalities[6].rarity = 5;
    allPersonalities[6].BaseHPModifier = 100;
    allPersonalities[6].BaseDefenseModifier = 80;
    allPersonalities[6].BaseMagicDefenseModifier = 80;
    allPersonalities[6].BaseAccuracyModifier = 100;
    allPersonalities[6].BaseAttackModifier = 100;
    allPersonalities[6].BaseElementalAccuracyModifier = 100;
    allPersonalities[6].BaseMagicAttackModifier = 100;
    allPersonalities[6].BaseSpeedModifier = 130;

    strcpy(allPersonalities[7].Name, "Jovial");
    allPersonalities[7].rarity = 1;
    allPersonalities[7].BaseHPModifier = 100;
    allPersonalities[7].BaseDefenseModifier = 110;
    allPersonalities[7].BaseMagicDefenseModifier = 110;
    allPersonalities[7].BaseAccuracyModifier = 110;
    allPersonalities[7].BaseAttackModifier = 110;
    allPersonalities[7].BaseMagicAttackModifier = 110;
    allPersonalities[7].BaseSpeedModifier = 110;

    strcpy(allPersonalities[8].Name, "Afobado");
    allPersonalities[8].rarity = 5;
    allPersonalities[8].BaseHPModifier = 100;
    allPersonalities[8].BaseDefenseModifier = 100;
    allPersonalities[8].BaseMagicDefenseModifier = 100;
    allPersonalities[8].BaseAccuracyModifier = 80;
    allPersonalities[8].BaseAttackModifier = 100;
    allPersonalities[8].BaseElementalAccuracyModifier = 80;
    allPersonalities[8].BaseMagicAttackModifier = 100;
    allPersonalities[8].BaseSpeedModifier = 120;

    strcpy(allPersonalities[9].Name, "Teimoso");
    allPersonalities[9].rarity = 10;
    allPersonalities[9].BaseHPModifier = 100;
    allPersonalities[9].BaseDefenseModifier = 85;
    allPersonalities[9].BaseMagicDefenseModifier = 85;
    allPersonalities[9].BaseAccuracyModifier = 130;
    allPersonalities[9].BaseAttackModifier = 100;
    allPersonalities[9].BaseElementalAccuracyModifier = 130;
    allPersonalities[9].BaseMagicAttackModifier = 100;
    allPersonalities[9].BaseSpeedModifier = 100;

    strcpy(allPersonalities[10].Name, "Invejoso");
    allPersonalities[10].rarity = 11;
    allPersonalities[10].BaseHPModifier = 100;
    allPersonalities[10].BaseDefenseModifier = 100;
    allPersonalities[10].BaseMagicDefenseModifier = 120;
    allPersonalities[10].BaseAccuracyModifier = 100;
    allPersonalities[10].BaseAttackModifier = 100;
    allPersonalities[10].BaseElementalAccuracyModifier = 100;
    allPersonalities[10].BaseMagicAttackModifier = 120;
    allPersonalities[10].BaseSpeedModifier = 80;

    strcpy(allPersonalities[11].Name, "Resistente");
    allPersonalities[11].rarity = 11;
    allPersonalities[11].BaseHPModifier = 100;
    allPersonalities[11].BaseDefenseModifier = 130;
    allPersonalities[11].BaseMagicDefenseModifier = 100;
    allPersonalities[11].BaseAccuracyModifier = 100;
    allPersonalities[11].BaseElementalAccuracyModifier = 100;
    allPersonalities[11].BaseAttackModifier = 100;
    allPersonalities[11].BaseMagicAttackModifier = 100;
    allPersonalities[11].BaseSpeedModifier = 70;

    strcpy(allPersonalities[12].Name, "Decidido");
    allPersonalities[12].rarity = 11;
    allPersonalities[12].BaseHPModifier = 100;
    allPersonalities[12].BaseDefenseModifier = 120;
    allPersonalities[12].BaseMagicDefenseModifier = 100;
    allPersonalities[12].BaseAccuracyModifier = 100;
    allPersonalities[12].BaseAttackModifier = 120;
    allPersonalities[12].BaseElementalAccuracyModifier = 100;
    allPersonalities[12].BaseMagicAttackModifier = 100;
    allPersonalities[12].BaseSpeedModifier = 80;
    
    SavePersonalities(allPersonalities, personalities);*/

    //Define Elements
    /*
        
    */

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

    dBElements = fopen(elements, "rb");
    if(dBElements == NULL){
        perror("Falha ao abrir \"elements\"");
        return 1;
    }
    fread(allElements, sizeof(Element), 10, dBElements);
    if(allElements == NULL){
        perror("Falha ao ler os Elementos!");
        return 2;
    }
    fclose(dBElements);

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


    //Principal Do Usuário
    //------------------------------------------------------------------------------------------------------------------//
<<<<<<< HEAD
    //login
    //menu
    bool Battle = false;
=======
    int respostaUserMP;
    bool login1 = false, login2 = false;
    char respostaCadastro;
    int indexUs1,indexUs2;

    printf("Gostaria de cadastrar-se?(S/n): ");

    scanf(" %c", &respostaCadastro);

        if (respostaCadastro == 's' || respostaCadastro == 'S') {
            
            char name[20];
            char pass[7];
            printf("Digite o nome do jogador (até 19 caracteres): ");
            scanf("%19s", name); 
            printf("Digite a senha do jogador (6 caracteres): ");
            scanf("%6s", pass); 

            if (AddPlayer(&pPlayers, &dataQuantities, name, pass)) {
                if (SavePlayers(pPlayers, dataQuantities.Player, "Players.bin")) {
                    SaveDataQuantity(dataQuantities, "DataQuantity.txt");
                    printf("Jogador cadastrado e salvo com sucesso!(Press Enter)\n");
                    getchar();
                    getchar();
                } else {
                    printf("Falha ao salvar os jogadores.(Press Enter)\n");
                    getchar();
                    getchar();
                }
            } else {
                printf("Falha ao cadastrar o jogador.(Press Enter)\n");
                getchar();
                getchar();
            }

        }



     while (true){
        LimparTerminal();
        Menu();
        printf("Sua escolha?: ");
        scanf(" %d", &respostaUserMP);
        if(respostaUserMP == 1){
            LimparTerminal();
            Login(pPlayers, dataQuantities.Player, &login1, &login2, &indexUs1, &indexUs2);
            while(true){
                MenuShopMP();
                scanf(" %d", &respostaUserMP);
                //if(respostaUserMP == 1){
                    //ShopPikomon();
                //}
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


    bool Battle;
>>>>>>> origin/Shop
    if(Battle){
        //Variaveis calcSkill
        bool elementalEffectHit, skillHit, critHit, selfEffectHit, enemyEffectHit, usedItemStatusHit;
        int selfDamage, enemyDamage;
        
        
        //Variaveis Battle
        bool playerOneTurn, reset, nextTurnReset = false, battleIsOver;
        int i = 0, turnCost;
        PiPointer selectedPlayerOnePicomon = &pPlayers[playerOneIndex].BatlePikomons[pPlayers[playerOneIndex].SelectedPikomonIndex], selectedPlayerTwoPicomon = &pPlayers[playerTwoIndex].BatlePikomons[pPlayers[playerTwoIndex].SelectedPikomonIndex];
        reset = true;
        battleIsOver = false;
        
        
        while(!battleIsOver){
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
            if(nextTurnReset){
                nextTurnReset = false;
                reset = true;
            }


            if(playerOneTurn){
                selectedPlayerOnePicomon[0].ChargedSpeed += selectedPlayerOnePicomon[0].Atributes[7].Total;
                while(selectedPlayerOnePicomon[0].ChargedSpeed - turnCost >= 0){
                    selectedPlayerOnePicomon[0].ChargedSpeed -= turnCost;

                    
                        //acoes do player1
                    

                }
                playerOneTurn = false;
            }
            else{
                selectedPlayerTwoPicomon[0].ChargedSpeed += selectedPlayerTwoPicomon[0].Atributes[7].Total;
                while(selectedPlayerTwoPicomon[0].ChargedSpeed - turnCost >= 0){
                    selectedPlayerTwoPicomon[0].ChargedSpeed -= turnCost;

                     
                        //acoes do player2
                    

                }
                playerOneTurn = true;
            }
        }
    }
    //------------------------------------------------------------------------------------------------------------------//
    //toda a vez que acaba uma batalha tem que usar essa funcao
    FreeAllHeapMemoryAndSaveEverything(pSkills,pItems,pPikomons,pPlayers,dataQuantities,dataQuantity,skills,items,pikomoms,players);
}




//Debug/Print/Base Functions
//------------------------------------------------------------------------------//
<<<<<<< HEAD
bool DebugPlayers(PlPointer pPlayers, int index, int playersQuantity){
    return false;
}

bool DebugPikomons(PiPointer pPikomon, int index, int pikomonsQuantity){
    return false;
}

bool DebugItems(ItPointer pItems, int index, int ItemsQuantity){
    return false;
}

bool DebugSkills(SkPointer pSkills, int index, int skillsQuantity){
    return false;
}
=======
bool DebugPlayers(Player *pPlayers, int index, int playersQuantity)
{
    int j, k;

    // -1 para pegar todos os players
    if (index == -1)
    {
        for (index = 0; index < playersQuantity; index++)
        {
            if (strlen(pPlayers[index].Name) > 0)
            {
                printf("| Name:| %20s | |,| Pass:| %20s | |,| Pikocoins:| %3d |;\n",
                       pPlayers[index].Name,
                       pPlayers[index].Pass,
                       pPlayers[index].Pikocoins);

                // Exibir itens da bolsa
                for (j = 0; j < pPlayers[index].BagCurrentSize; j++)
                {
                    if (strlen(pPlayers[index].Bag[j].Name) > 0)
                    {
                        printf("| Item Name:| %20s | |,| Type:| %20s | |,| Target:| %3c |;\n",
                               pPlayers[index].Bag[j].Name,
                               pPlayers[index].Bag[j].Type,
                               pPlayers[index].Bag[j].EffectTarget);
                    }
                }

                // Exibir Pokémons
                for (j = 0; j < 12; j++)
                {
                    if (strlen(pPlayers[index].PikomonsStorage[j].Name) > 0)
                    {
                        printf("| Nome:| %20s | |,| Element:| %10s | |,| HP:| %3d |,| Atk:| %3d |,| Def:| %3d |,,| SpA:| %3d |,| SpD:| %3d |,| Spd:| %3d |,| Acu:| %3d |,| Eac:| %3d |\n",
                               pPlayers[index].PikomonsStorage[j].Name,
                               pPlayers[index].PikomonsStorage[j].Element.Name,
                               pPlayers[index].PikomonsStorage[j].Atributes[0].Total,
                               pPlayers[index].PikomonsStorage[j].Atributes[4].Total,
                               pPlayers[index].PikomonsStorage[j].Atributes[1].Total,
                               pPlayers[index].PikomonsStorage[j].Atributes[6].Total,
                               pPlayers[index].PikomonsStorage[j].Atributes[2].Total,
                               pPlayers[index].PikomonsStorage[j].Atributes[7].Total,
                               pPlayers[index].PikomonsStorage[j].Atributes[3].Total,
                               pPlayers[index].PikomonsStorage[j].Atributes[5].Total);

                        // Exibir habilidades do Pokémon
                        for (k = 0; k < 4; k++)
                        {
                            if (strlen(pPlayers[index].PikomonsStorage[j].Skills[k].Name) > 0)
                            {
                                printf("| Skill Name:| %20s | |,| Target:| %c | |,| AttackScale:| %d| |,| MagicAttackScale:| %d |;\n",
                                       pPlayers[index].PikomonsStorage[j].Skills[k].Name,
                                       pPlayers[index].PikomonsStorage[j].Skills[k].Target,
                                       pPlayers[index].PikomonsStorage[j].Skills[k].AttackScale,
                                       pPlayers[index].PikomonsStorage[j].Skills[k].MagicAttackScale);
                            }
                        }
                    }
                }
            }
        }
    }
    else if (index >= 0 && index < playersQuantity)
    {
        // Exibir apenas um jogador
        if (strlen(pPlayers[index].Name) > 0)
        {
            printf("| Name:| %20s | |,| Pass:| %20s | |,| Pikocoins:| %3d |;\n",
                   pPlayers[index].Name,
                   pPlayers[index].Pass,
                   pPlayers[index].Pikocoins);

            // Exibir itens da bolsa
            for (j = 0; j < pPlayers[index].BagCurrentSize; j++)
            {
                if (strlen(pPlayers[index].Bag[j].Name) > 0)
                {
                    printf("| Item Name:| %20s | |,| Type:| %20s | |,| Target:| %3c |;\n",
                           pPlayers[index].Bag[j].Name,
                           pPlayers[index].Bag[j].Type,
                           pPlayers[index].Bag[j].EffectTarget);
                }
            }

            // Exibir Pokémons
            for (j = 0; j < 12; j++)
            {
                if (strlen(pPlayers[index].PikomonsStorage[j].Name) > 0)
                {
                    printf("| Nome:| %20s | |,| Element:| %10s | |,| HP:| %3d |,| Atk:| %3d |,| Def:| %3d |,,| SpA:| %3d |,| SpD:| %3d |,| Spd:| %3d |,| Acu:| %3d |,| Eac:| %3d |\n",
                           pPlayers[index].PikomonsStorage[j].Name,
                           pPlayers[index].PikomonsStorage[j].Element.Name,
                           pPlayers[index].PikomonsStorage[j].Atributes[0].Total,
                           pPlayers[index].PikomonsStorage[j].Atributes[4].Total,
                           pPlayers[index].PikomonsStorage[j].Atributes[1].Total,
                           pPlayers[index].PikomonsStorage[j].Atributes[6].Total,
                           pPlayers[index].PikomonsStorage[j].Atributes[2].Total,
                           pPlayers[index].PikomonsStorage[j].Atributes[7].Total,
                           pPlayers[index].PikomonsStorage[j].Atributes[3].Total,
                           pPlayers[index].PikomonsStorage[j].Atributes[5].Total);

                    // Exibir habilidades do Pokémon
                    for (k = 0; k < 4; k++)
                    {
                        if (strlen(pPlayers[index].PikomonsStorage[j].Skills[k].Name) > 0)
                        {
                            printf("| Skill Name:| %20s | |,| Target:| %c | |,| AttackScale:| %d | |,| MagicAttackScale:| %d |;\n",
                                   pPlayers[index].PikomonsStorage[j].Skills[k].Name,
                                   pPlayers[index].PikomonsStorage[j].Skills[k].Target,
                                   pPlayers[index].PikomonsStorage[j].Skills[k].AttackScale,
                                   pPlayers[index].PikomonsStorage[j].Skills[k].MagicAttackScale);
                        }
                    }
                }
            }
        }
    }
    else
    {
        perror("Index inválido.");
        return false;
    }
    return true;
}

bool DebugPikomons(PiPointer pPikomon, int index, int pikomonsQuantity)
{
    int j, k;

    if (index == -1)
    {
        for (index = 0; index < pikomonsQuantity; index++)
        {
            if (strlen(pPikomon[index].Name) > 0)
            {
                // Exibe os atributos do pikomon
                printf("| Nome:| %10s | |,| Element:| %10s | |,| HP:| %3d | |,| Atk:| %3d | |,| Def:| %3d | |,| SpA:| %3d | |,| SpD:| %3d | |,| Spd:| %3d |,| Acu:| %3d |,| Eac:| %3d | |\n",
                       pPikomon[index].Name,
                       pPikomon[index].Element.Name,
                       pPikomon[index].Atributes[0].Total,
                       pPikomon[index].Atributes[4].Total,
                       pPikomon[index].Atributes[1].Total,
                       pPikomon[index].Atributes[6].Total,
                       pPikomon[index].Atributes[2].Total,
                       pPikomon[index].Atributes[7].Total,
                       pPikomon[index].Atributes[3].Total,
                       pPikomon[index].Atributes[5].Total);


                // Exibir IconImg do Pikomon
                printf("IconImg:\n");
                for (j = 0; j < 7; j++)
                {
                    printf("| %s |\n", pPikomon[index].IconImg[j]);
                }
                printf("\n");


                // Exibir habilidades do Pikomon
                for (k = 0; k < 4; k++)
                {
                    if (strlen(pPikomon[index].Skills[k].Name) > 0)
                    {
                        printf("| Skill Name:| %20s | |,| Target:| %c | |,| AttackScale:| %d | |,| MagicAttackScale:| %d | |;\n",
                               pPikomon[index].Skills[k].Name,
                               pPikomon[index].Skills[k].Target,
                               pPikomon[index].Skills[k].AttackScale,
                               pPikomon[index].Skills[k].MagicAttackScale);
                    }
                }
            }
        }
    }
    else if (index >= 0 && index < pikomonsQuantity)
    {
        // Exibir apenas um Pikomon
        if (strlen(pPikomon[index].Name) > 0)
        {
            printf("| Nome:| %10s | |,| Element:| %10s | |,| HP:| %3d | |,| Atk:| %3d | |,| Def:| %3d | |,| SpA:| %3d | |,| SpD:| %3d | |,| Spd:| %3d |,| Acu:| %3d |,| Eac:| %3d | |\n",
                   pPikomon[index].Name,
                   pPikomon[index].Element.Name,
                   pPikomon[index].Atributes[0].Total,
                   pPikomon[index].Atributes[4].Total,
                   pPikomon[index].Atributes[1].Total,
                   pPikomon[index].Atributes[6].Total,
                   pPikomon[index].Atributes[2].Total,
                   pPikomon[index].Atributes[7].Total,
                   pPikomon[index].Atributes[3].Total,
                   pPikomon[index].Atributes[5].Total);



            printf("IconImg:\n");
            for (j = 0; j < 7; j++)
            {
                printf("| %s |\n", pPikomon[index].IconImg[j]);
            }
            printf("\n");

            // Exibir habilidades do Pikomon
            for (k = 0; k < 4; k++)
            {
                if (strlen(pPikomon[index].Skills[k].Name) > 0)
                {
                    printf("| Skill Name:| %20s | |,| Target:| %c | |,| AttackScale:| %d | |,| MagicAttackScale:| %d | |;\n",
                           pPikomon[index].Skills[k].Name,
                           pPikomon[index].Skills[k].Target,
                           pPikomon[index].Skills[k].AttackScale,
                           pPikomon[index].Skills[k].MagicAttackScale);
                }
            }
        }
    }
    else
    {
        perror("Index inválido.");
        return false;
    }
    return true;
}

bool DebugItems(ItPointer pItems, int index, int ItemsQuantity) {
    // Se index for -1, debugar todos os itens
    if (index == -1) {
        for (index = 0; index < ItemsQuantity; index++) {
            // Exibir informações do item
            printf("| Item #%d | Name: %s | Type: %s | Value: %d |\n",
                   index + 1,
                   pItems[index].Name,
                   pItems[index].Type,
                   pItems[index].Value);

            for (int j = 0; j < 3; j++) {
                printf("| Description[%d]: %s |\n", j, pItems[index].Description[j]);
            }

            printf("| Active: %s |\n", pItems[index].Active);
            for (int j = 0; j < 3; j++) {
                printf("| ActiveDescription[%d]: %s |\n", j, pItems[index].ActiveDescription[j]);
            }

            printf("| Current HP Damage Is Physic: %s |\n", pItems[index].CurrentHPDamageIsPhysic ? "true" : "false");
            printf("| EffectCurrentHPTarget: %c |\n", pItems[index].EffectCurrentHPTarget);
            printf("| EffectCurrentHP | Quantity: %d | Timer: %d |\n",
                   pItems[index].EffectCurrentHP.Quantity,
                   pItems[index].EffectCurrentHP.Timer);

            printf("| EffectTarget: %c |\n", pItems[index].EffectTarget);
            printf("| Status Effect Chance: %d |\n", pItems[index].StatusEffectChance);
            for (int k = 0; k < 8; k++) {
                if (pItems[index].StatusEffect[k].Quantity > 0) {
                    printf("| Status Effect[%d] | Quantity: %d | Timer: %d |\n",
                           k,
                           pItems[index].StatusEffect[k].Quantity,
                           pItems[index].StatusEffect[k].Timer);
                }
            }

            printf("\n");
        }
    }
    // Exibir apenas um item específico
    else if (index >= 0 && index < ItemsQuantity) {
        printf("| Item #%d | Name: %s | Type: %s | Value: %d |\n",
               index + 1,
               pItems[index].Name,
               pItems[index].Type,
               pItems[index].Value);

        for (int j = 0; j < 3; j++) {
            printf("| Description[%d]: %s |\n", j, pItems[index].Description[j]);
        }

        printf("| Active: %s |\n", pItems[index].Active);
        for (int j = 0; j < 3; j++) {
            printf("| ActiveDescription[%d]: %s |\n", j, pItems[index].ActiveDescription[j]);
        }

        printf("| Current HP Damage Is Physic: %s |\n", pItems[index].CurrentHPDamageIsPhysic ? "true" : "false");
        printf("| EffectCurrentHPTarget: %c |\n", pItems[index].EffectCurrentHPTarget);
        printf("| EffectCurrentHP | Quantity: %d | Timer: %d |\n",
               pItems[index].EffectCurrentHP.Quantity,
               pItems[index].EffectCurrentHP.Timer);

        printf("| EffectTarget: %c |\n", pItems[index].EffectTarget);
        printf("| Status Effect Chance: %d |\n", pItems[index].StatusEffectChance);
        for (int k = 0; k < 8; k++) {
            if (pItems[index].StatusEffect[k].Quantity > 0) {
                printf("| Status Effect[%d] | Quantity: %d | Timer: %d |\n",
                       k,
                       pItems[index].StatusEffect[k].Quantity,
                       pItems[index].StatusEffect[k].Timer);
            }
        }
    }
    else {
        perror("Index inválido.");
        return false;
    }

    return true;
}


bool DebugSkills(SkPointer pSkills, int index, int skillsQuantity) {
    if (index == -1) {
        for (index = 0; index < skillsQuantity; index++) {
            printf("Skill #%d:\n", index + 1);
            printf("Name: %s | Type: %s | Target: %c | ElementEffectChance: %d | AttackBase: %d | AttackScale: %d | MagicBase: %d | MagicAttackScale: %d | CritChance: %d | EffectTarget: %c | EnemyEffectHitChance: %d | SelfEffectHitChance: %d\n",
                   pSkills[index].Name,
                   pSkills[index].Type,
                   pSkills[index].Target,
                   pSkills[index].ElementEffectHitChance,
                   pSkills[index].AttackBase,
                   pSkills[index].AttackScale,
                   pSkills[index].MagicBase,
                   pSkills[index].MagicAttackScale,
                   pSkills[index].CritChance,
                   pSkills[index].EffectTarget,
                   pSkills[index].EnemyEffectHitChance,
                   pSkills[index].SelfEffectHitChance);

            // Exibir descrições
            for (int j = 0; j < 3; j++) {
                printf("| Description[%d]: %s |\n", j, pSkills[index].Description[j]);
            }

            // Exibir Active e ActiveDescription
            printf("| Active: %s |\n", pSkills[index].Active);
            for (int j = 0; j < 3; j++) {
                printf("| ActiveDescription[%d]: %s |\n", j, pSkills[index].ActiveDescription[j]);
            }

            // Exibir Learnable Personalities
            printf("| Learnable Personalities: ");
            for (int j = 0; j < 13; j++) {
                printf("%d ", pSkills[index].LearnablePersonalities[j]);
            }
            printf("|\n");

            // Exibir Learnable Elements
            printf("| Learnable Elements: ");
            for (int j = 0; j < 10; j++) {
                printf("%d ", pSkills[index].LearnableElements[j]);
            }
            printf("|\n");

            // Exibir efeitos do inimigo
            printf("| Enemy Effects:\n");
            for (int k = 0; k < 8; k++) {
                if (pSkills[index].EnemyEffect[k].Quantity > 0) {
                    printf("| Enemy Effect[%d] | Quantity: %d | Timer: %d |\n",
                           k,
                           pSkills[index].EnemyEffect[k].Quantity,
                           pSkills[index].EnemyEffect[k].Timer);
                }
            }

            // Exibir efeitos próprios
            printf("| Self Effects:\n");
            for (int k = 0; k < 8; k++) {
                if (pSkills[index].SelfEffect[k].Quantity > 0) {
                    printf("| Self Effect[%d] | Quantity: %d | Timer: %d |\n",
                           k,
                           pSkills[index].SelfEffect[k].Quantity,
                           pSkills[index].SelfEffect[k].Timer);
                }
            }

            printf("\n");
        }
    } else if (index >= 0 && index < skillsQuantity) {
        printf("Skill #%d:\n", index + 1);
        printf("Name: %s | Type: %s | Target: %c | ElementEffectChance: %d | AttackBase: %d | AttackScale: %d | MagicBase: %d | MagicAttackScale: %d | CritChance: %d | EffectTarget: %c | EnemyEffectHitChance: %d | SelfEffectHitChance: %d\n",
               pSkills[index].Name,
               pSkills[index].Type,
               pSkills[index].Target,
               pSkills[index].ElementEffectHitChance,
               pSkills[index].AttackBase,
               pSkills[index].AttackScale,
               pSkills[index].MagicBase,
               pSkills[index].MagicAttackScale,
               pSkills[index].CritChance,
               pSkills[index].EffectTarget,
               pSkills[index].EnemyEffectHitChance,
               pSkills[index].SelfEffectHitChance);

        // Exibir descrições
        for (int j = 0; j < 3; j++) {
            printf("| Description[%d]: %s |\n", j, pSkills[index].Description[j]);
        }

        // Exibir Active e ActiveDescription
        printf("| Active: %s |\n", pSkills[index].Active);
        for (int j = 0; j < 3; j++) {
            printf("| ActiveDescription[%d]: %s |\n", j, pSkills[index].ActiveDescription[j]);
        }

        // Exibir Learnable Personalities
        printf("| Learnable Personalities: ");
        for (int j = 0; j < 13; j++) {
            printf("%d ", pSkills[index].LearnablePersonalities[j]);
        }
        printf("|\n");

        // Exibir Learnable Elements
        printf("| Learnable Elements: ");
        for (int j = 0; j < 10; j++) {
            printf("%d ", pSkills[index].LearnableElements[j]);
        }
        printf("|\n");

        // Exibir efeitos do inimigo
        printf("| Enemy Effects:\n");
        for (int k = 0; k < 8; k++) {
            if (pSkills[index].EnemyEffect[k].Quantity > 0) {
                printf("| Enemy Effect[%d] | Quantity: %d | Timer: %d |\n",
                       k,
                       pSkills[index].EnemyEffect[k].Quantity,
                       pSkills[index].EnemyEffect[k].Timer);
            }
        }

        // Exibir efeitos próprios
        printf("| Self Effects:\n");
        for (int k = 0; k < 8; k++) {
            if (pSkills[index].SelfEffect[k].Quantity > 0) {
                printf("| Self Effect[%d] | Quantity: %d | Timer: %d |\n",
                       k,
                       pSkills[index].SelfEffect[k].Quantity,
                       pSkills[index].SelfEffect[k].Timer);
            }
        }
    } else {
        perror("Index inválido.");
        return false;
    }

    return true;
}

bool DebugPersonality(Personality *personalities, int index) {
    if (index == -1) {
        // Exibe todos as personalidades
        for (int i = 0; i < 13; i++) {
            if (strlen(personalities[i].Name) > 0) {
                printf("| Name:| %20s |,| BaseHPModifier:| %.2d |,| BaseDefenseModifier:| %.2d |,| BaseMagicDefenseModifier:| %.2d |\n",
                       personalities[i].Name,
                       personalities[i].BaseHPModifier,
                       personalities[i].BaseDefenseModifier,
                       personalities[i].BaseMagicDefenseModifier);
                printf("| BaseAttackModifier:| %.2d |,| BaseMagicAttackModifier:| %.2d |,| BaseSpeedModifier:| %.2d |\n",
                       personalities[i].BaseAttackModifier,
                       personalities[i].BaseMagicAttackModifier,
                       personalities[i].BaseSpeedModifier);
            }
        }
    } else if (index >= 0 && index < 13) {
        // Exibe apenas uma personalidade especificada pelo índice
        if (strlen(personalities[index].Name) > 0) {
            printf("| Name:| %20s |,| BaseHPModifier:| %.2d |,| BaseDefenseModifier:| %.2d |,| BaseMagicDefenseModifier:| %.2d |\n",
                   personalities[index].Name,
                   personalities[index].BaseHPModifier,
                   personalities[index].BaseDefenseModifier,
                   personalities[index].BaseMagicDefenseModifier);
            printf("| BaseAttackModifier:| %.2d |,| BaseMagicAttackModifier:| %.2d |,| BaseSpeedModifier:| %.2d |\n",
                   personalities[index].BaseAttackModifier,
                   personalities[index].BaseMagicAttackModifier,
                   personalities[index].BaseSpeedModifier);
        }
    } else {
        perror("Index inválido.");
        return false;
    }
    return true;
}

bool DebugElements(Element *elements, int index) {
    if (index == -1) {
        // Exibir todos os elementos
        for (int i = 0; i < 10; i++) {
            printf("| Element Name:| %20s |,| Elemental Damage Scale:| %3d |\n", 
                   elements[i].Name, elements[i].ElementalDamageScale);
            printf("  Effectiveness:");
            for (int j = 0; j < 10; j++) {
                printf(" %d", elements[i].Effectiveness[j]);
            }
            printf("\n  Current HP Effect - Quantity: %d, Timer: %d\n",
                   elements[i].CurrentHPEffect.Quantity,
                   elements[i].CurrentHPEffect.Timer);

            // Exibir efeitos de status
            printf("  Status Effects:\n");
            for (int k = 0; k < 8; k++) {
                printf("    Status %d - Quantity: %d, Timer: %d\n",
                       k + 1,
                       elements[i].StatusEffect[k].Quantity,
                       elements[i].StatusEffect[k].Timer);
            }
            printf("\n");
        }
    } else if (index >= 0 && index < 10) {
        // Exibir um único elemento
        printf("| Element Name:| %20s |,| Elemental Damage Scale:| %3d |\n", 
               elements[index].Name, elements[index].ElementalDamageScale);
        printf("  Effectiveness:");
        for (int j = 0; j < 10; j++) {
            printf(" %d", elements[index].Effectiveness[j]);
        }
        printf("\n  Current HP Effect - Quantity: %d, Timer: %d\n",
               elements[index].CurrentHPEffect.Quantity,
               elements[index].CurrentHPEffect.Timer);

        // Exibir efeitos de status
        printf("  Status Effects:\n");
        for (int k = 0; k < 8; k++) {
            printf("    Status %d - Quantity: %d, Timer: %d\n",
                   k + 1,
                   elements[index].StatusEffect[k].Quantity,
                   elements[index].StatusEffect[k].Timer);
        }
        printf("\n");
    } else {
        perror("Index inválido.");
        return false;
    }
    return true;
}

>>>>>>> origin/Shop
//------------------------------------------------------------------------------//



//Save Functions
//------------------------------------------------------------------------------//
bool SavePersonalities(Personality allPersonalities[13], const char *destino){
    FILE* dBPersonalities;
    dBPersonalities = fopen(destino, "wb");
    if (dBPersonalities == NULL)
    {
        perror("falha ao abrir \"dBPersonalities\" na funcao \"SavePersonalities\"");
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
        perror("falha ao abrir \"dBElements\" na funcao \"SaveElements\"");
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
        perror("falha ao abrir \"dBDataQuantity\" na funcao \"SaveDataQuantity\"");
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
        perror("falha ao abrir \"dBSkills\" na funcao \"SaveSkills\"");
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
        perror("falha ao abrir \"dBItems\" na funcao \"SaveItems\"");
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
        perror("falha ao abrir \"dBPikomons\" na funcao \"SavePikomons\"");
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
        perror("falha ao abrir \"dBPlayers\" na funcao \"SavePlayers\"");
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
    int i, j, k, I;
    for(i = 0; i < dataquantities.Player; i++){
        for(j = 0; j < pPlayers[i].BagCurrentSize; j++){
            pPlayers[i].Pikocoins += pPlayers[i].Bag[j].Value;
        }
        pPlayers[i].BagCurrentSize = 0;
        free(pPlayers[i].Bag);
        for(j = 0; j < 6; j++){
            for(k = 0; k < 8; k++){
                for(I = 0; I < pPlayers[i].BatlePikomons[j].Atributes[k].BonusQuantity; I++){
                    free(pPlayers[i].BatlePikomons[j].Atributes[k].acronym[I]);
                    pPlayers[i].BatlePikomons[j].Atributes[k].acronym[I] = NULL;
                }
                free(pPlayers[i].BatlePikomons[j].Atributes[k].Bonus);
                pPlayers[i].BatlePikomons[j].Atributes[k].Bonus = NULL;
                free(pPlayers[i].BatlePikomons[j].Atributes[k].BonusTimer);
                pPlayers[i].BatlePikomons[j].Atributes[k].BonusTimer = NULL;
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



//Manage Memory Functions
//------------------------------------------------------------------------------//
<<<<<<< HEAD
bool AddSkill(SkPointer pSkills, DataQuantity dataQuantities, char *name, char target, bool learnablePersonalities[13], bool LearnableElements[10], int elementEffectChance, Element element, int  attackBase, int attackScale, int magicBase, int magicAttackScale, int critChance, char effectTarget, int enemyEffectChance, Effect enemyEffect[8], int selfEffectChance, Effect selfEffect[8]){
=======
bool AddSkill(SkPointer pSkills, DataQuantity dataQuantities, char *name, char target, bool *learnablePersonalities, bool *LearnableElements, int elementEffectChance, Element element, int  attackBase, int attackScale, int magicBase, int magicAttackScale, int critChance, char effectTarget, int enemyEffectChance, Effect *enemyEffect, int selfEffectChance, Effect *selfEffect){
   /*
>>>>>>> origin/Shop
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
        perror("ERRO na realocacao de memoria em \"AddSkill\"");
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
    pSkills[dataQuantities.Skill-1].ElementEffectHitChance = elementEffectChance;
    pSkills[dataQuantities.Skill-1].Element = element;
    pSkills[dataQuantities.Skill-1].AttackBase = attackBase;
    pSkills[dataQuantities.Skill-1].AttackScale = attackScale;
    pSkills[dataQuantities.Skill-1].MagicBase = magicBase;
    pSkills[dataQuantities.Skill-1].MagicAttackScale = magicAttackScale;
    pSkills[dataQuantities.Skill-1].CritChance = critChance;
    pSkills[dataQuantities.Skill-1].EffectTarget = effectTarget;
    pSkills[dataQuantities.Skill-1].EnemyEffectHitChance = enemyEffectChance;
    for(i = 0; i < 8; i++){
        pSkills[dataQuantities.Skill-1].EnemyEffect[i] = enemyEffect[i];
    }
    pSkills[dataQuantities.Skill-1].SelfEffectHitChance = selfEffectChance;
    for(i = 0; i < 8; i++){
        pSkills[dataQuantities.Skill-1].SelfEffect[i] = selfEffect[i];
    }
<<<<<<< HEAD
    return true;
}

bool AddItem(ItPointer pItems, DataQuantity dataQuantities, char *name, char *type, char description[3][255], int value, char effectCurrentHPTarget, Effect EffectCurrentHP, char effectTarget, double StatusEffectChance, Effect StatusEffect[8]){
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
        perror("ERRO na realocacao de memoria em \"AddItem\"");
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
    pItems[dataQuantities.Item-1].EffectCurrentHP = EffectCurrentHP;
    pItems[dataQuantities.Item-1].EffectTarget = effectTarget;
    pItems[dataQuantities.Item-1].StatusEffectChance = StatusEffectChance;
    for(i = 0; i < 8; i++){
        pItems[dataQuantities.Item-1].StatusEffect[i] = StatusEffect[i];
    }
    return true;

}

bool AddPikomon(PiPointer pPikomons, DataQuantity dataQuantities, char *name, Element element, char iconImg[7][19], int BaseHP, int BaseDefense, int BaseMagicDefense, int BaseAccuracy, int BaseAttack, int BaseElementalAccuracy, int BaseMagicAttack, int BaseSpeed){
    //Se o memset estiver errado ele estara apagando memoria de outras variaveis;
    if(pPikomons == NULL){
=======
    */
}

bool AddPikomon(PiPointer *pPikomons, DataQuantity *dataQuantities, char *name, Element element, char iconImg[7][19], int BaseHP, int BaseDefense, int BaseMagicDefense, int BaseAcurracy, int BaseAttack, int BaseElementalAcurracy, int BaseMagicAttack, int BaseSpeed) {
    // Se o memset estiver errado, ele estará apagando memória de outras variáveis;
    if (pPikomons == NULL || *pPikomons == NULL) {
>>>>>>> origin/Shop
        perror("ERRO, \"pPikomons\" não pode ser NULL em \"AddPikomon\"");
        return false;
    }
    if (name == NULL) {
        perror("ERRO, \"name\" não pode ser NULL em \"AddPikomon\"");
        return false;
    } else if (strlen(name) > 19) {
        perror("ERRO, \"name\" não pode ter mais de 19 caracteres em \"AddPikomon\"");
        return false;
    }
    if (iconImg == NULL) {
        perror("ERRO, \"icoImg\" não pode ser NULL em \"AddPikomon\"");
        return false;
    }
    if (BaseHP == 0) {
        perror("ERRO, \"BaseHP\" não pode ser 0 em \"AddPikomon\"");
        return false;
    }
    if (BaseDefense == 0) {
        perror("ERRO, \"BaseDefense\" não pode ser 0 em \"AddPikomon\"");
        return false;
    }
    if (BaseMagicDefense == 0) {
        perror("ERRO, \"BaseMagicDefense\" não pode ser 0 em \"AddPikomon\"");
        return false;
    }
<<<<<<< HEAD
    if(BaseAccuracy == 0){
        perror("ERRO, \"BaseAccuracy\" não pode ser 0 em \"AddPikomon\"");
=======
    if (BaseAcurracy == 0) {
        perror("ERRO, \"BaseAcurracy\" não pode ser 0 em \"AddPikomon\"");
>>>>>>> origin/Shop
        return false;
    }
    if (BaseAttack == 0) {
        perror("ERRO, \"BaseAttack\" não pode ser 0 em \"AddPikomon\"");
        return false;
    }
<<<<<<< HEAD
    if(BaseElementalAccuracy == 0){
        perror("ERRO, \"BaseElementalAccuracy\" não pode ser 0 em \"AddPikomon\"");
=======
    if (BaseElementalAcurracy == 0) {
        perror("ERRO, \"BaseElementalAcurracy\" não pode ser 0 em \"AddPikomon\"");
>>>>>>> origin/Shop
        return false;
    }
    if (BaseMagicAttack == 0) {
        perror("ERRO, \"BaseMagicAttack\" não pode ser 0 em \"AddPikomon\"");
        return false;
    }
    if (BaseSpeed == 0) {
        perror("ERRO, \"BaseSpeed\" não pode ser 0 em \"AddPikomon\"");
        return false;
    }

<<<<<<< HEAD

    dataQuantities.Pikomon++;
    pPikomons = (PiPointer)realloc(pPikomons, dataQuantities.Pikomon * sizeof(Pikomon));
    if(pPikomons == NULL){
        perror("ERRO na realocacao de memoria em \"AddPikomon\"");
=======
    dataQuantities->Pikomon++;
    *pPikomons = (PiPointer)realloc(*pPikomons, dataQuantities->Pikomon * sizeof(Pikomon));
    if (*pPikomons == NULL) {
        perror("ERRO na realocação de memória em \"AddPikomon\"");
>>>>>>> origin/Shop
        return false;
    }

    memset(&(*pPikomons)[dataQuantities->Pikomon - 1], 0, sizeof(Pikomon));
    strcpy((*pPikomons)[dataQuantities->Pikomon - 1].Name, name);
    (*pPikomons)[dataQuantities->Pikomon - 1].Element = element;

    for (int i = 0; i < 7; i++) {
        strcpy((*pPikomons)[dataQuantities->Pikomon - 1].IconImg[i], iconImg[i]);
    }
<<<<<<< HEAD
    strcpy(pPikomons[dataQuantities.Pikomon-1].CurrentHP.Name, "CurrentHP");
    pPikomons[dataQuantities.Pikomon-1].CurrentHP.Base = 0;
    pPikomons[dataQuantities.Pikomon-1].CurrentHP.acronym = NULL;
    pPikomons[dataQuantities.Pikomon-1].CurrentHP.Bonus = NULL;
    pPikomons[dataQuantities.Pikomon-1].CurrentHP.BonusTimer = NULL;
    strcpy(pPikomons[dataQuantities.Pikomon-1].Atributes[0].Name, "HP");
    pPikomons[dataQuantities.Pikomon-1].Atributes[0].Base = BaseHP;
    pPikomons[dataQuantities.Pikomon-1].Atributes[0].acronym = NULL;
    pPikomons[dataQuantities.Pikomon-1].Atributes[0].Bonus = NULL;
    pPikomons[dataQuantities.Pikomon-1].Atributes[0].BonusTimer = NULL;
    strcpy(pPikomons[dataQuantities.Pikomon-1].Atributes[1].Name, "Defense");
    pPikomons[dataQuantities.Pikomon-1].Atributes[1].Base = BaseDefense;
    pPikomons[dataQuantities.Pikomon-1].Atributes[1].acronym = NULL;
    pPikomons[dataQuantities.Pikomon-1].Atributes[1].Bonus = NULL;
    pPikomons[dataQuantities.Pikomon-1].Atributes[1].BonusTimer = NULL;
    strcpy(pPikomons[dataQuantities.Pikomon-1].Atributes[2].Name, "MagicDefense");
    pPikomons[dataQuantities.Pikomon-1].Atributes[2].Base = BaseMagicDefense;
    pPikomons[dataQuantities.Pikomon-1].Atributes[2].acronym = NULL;
    pPikomons[dataQuantities.Pikomon-1].Atributes[2].Bonus = NULL;
    pPikomons[dataQuantities.Pikomon-1].Atributes[2].BonusTimer = NULL;
    strcpy(pPikomons[dataQuantities.Pikomon-1].Atributes[3].Name, "Accuracy");
    pPikomons[dataQuantities.Pikomon-1].Atributes[3].Base = BaseAccuracy;
    pPikomons[dataQuantities.Pikomon-1].Atributes[3].acronym = NULL;
    pPikomons[dataQuantities.Pikomon-1].Atributes[3].Bonus = NULL;
    pPikomons[dataQuantities.Pikomon-1].Atributes[3].BonusTimer = NULL;
    strcpy(pPikomons[dataQuantities.Pikomon-1].Atributes[4].Name, "Attack");
    pPikomons[dataQuantities.Pikomon-1].Atributes[4].Base = BaseAttack;
    pPikomons[dataQuantities.Pikomon-1].Atributes[4].acronym = NULL;
    pPikomons[dataQuantities.Pikomon-1].Atributes[4].Bonus = NULL;
    pPikomons[dataQuantities.Pikomon-1].Atributes[4].BonusTimer = NULL;
    strcpy(pPikomons[dataQuantities.Pikomon-1].Atributes[5].Name, "ElementalAccuracy");
    pPikomons[dataQuantities.Pikomon-1].Atributes[5].Base = BaseElementalAccuracy;
    pPikomons[dataQuantities.Pikomon-1].Atributes[5].acronym = NULL;
    pPikomons[dataQuantities.Pikomon-1].Atributes[5].Bonus = NULL;
    pPikomons[dataQuantities.Pikomon-1].Atributes[5].BonusTimer = NULL;
    strcpy(pPikomons[dataQuantities.Pikomon-1].Atributes[6].Name, "MagicAttack");
    pPikomons[dataQuantities.Pikomon-1].Atributes[6].Base = BaseMagicAttack;
    pPikomons[dataQuantities.Pikomon-1].Atributes[6].acronym = NULL;
    pPikomons[dataQuantities.Pikomon-1].Atributes[6].Bonus = NULL;
    pPikomons[dataQuantities.Pikomon-1].Atributes[6].BonusTimer = NULL;
    strcpy(pPikomons[dataQuantities.Pikomon-1].Atributes[7].Name, "Speed");
    pPikomons[dataQuantities.Pikomon-1].Atributes[7].Base = BaseSpeed;
    pPikomons[dataQuantities.Pikomon-1].Atributes[7].acronym = NULL;
    pPikomons[dataQuantities.Pikomon-1].Atributes[7].Bonus = NULL;
    pPikomons[dataQuantities.Pikomon-1].Atributes[7].BonusTimer = NULL;
    return true;
=======

    strcpy((*pPikomons)[dataQuantities->Pikomon - 1].CurrentHP.Name, "CurrentHP");
    (*pPikomons)[dataQuantities->Pikomon - 1].CurrentHP.Base = 0;

    strcpy((*pPikomons)[dataQuantities->Pikomon - 1].Atributes[0].Name, "HP");
    (*pPikomons)[dataQuantities->Pikomon - 1].Atributes[0].Base = BaseHP;
    
    strcpy((*pPikomons)[dataQuantities->Pikomon - 1].Atributes[1].Name, "Defense");
    (*pPikomons)[dataQuantities->Pikomon - 1].Atributes[1].Base = BaseDefense;
    
    strcpy((*pPikomons)[dataQuantities->Pikomon - 1].Atributes[2].Name, "MagicDefense");
    (*pPikomons)[dataQuantities->Pikomon - 1].Atributes[2].Base = BaseMagicDefense;
    
    strcpy((*pPikomons)[dataQuantities->Pikomon - 1].Atributes[3].Name, "Acurracy");
    (*pPikomons)[dataQuantities->Pikomon - 1].Atributes[3].Base = BaseAcurracy;
    
    strcpy((*pPikomons)[dataQuantities->Pikomon - 1].Atributes[4].Name, "Attack");
    (*pPikomons)[dataQuantities->Pikomon - 1].Atributes[4].Base = BaseAttack;
    
    strcpy((*pPikomons)[dataQuantities->Pikomon - 1].Atributes[5].Name, "ElementalAcurracy");
    (*pPikomons)[dataQuantities->Pikomon - 1].Atributes[5].Base = BaseElementalAcurracy;
    
    strcpy((*pPikomons)[dataQuantities->Pikomon - 1].Atributes[6].Name, "MagicAttack");
    (*pPikomons)[dataQuantities->Pikomon - 1].Atributes[6].Base = BaseMagicAttack;
    
    strcpy((*pPikomons)[dataQuantities->Pikomon - 1].Atributes[7].Name, "Speed");
    (*pPikomons)[dataQuantities->Pikomon - 1].Atributes[7].Base = BaseSpeed;

    return true; 
>>>>>>> origin/Shop
}

bool AddPlayer(PlPointer *pPlayers, DataQuantity *dataQuantities, char *name, char *pass) {
    // Se o memset estiver errado, ele estará apagando memória de outras variáveis;
    if (pPlayers == NULL || *pPlayers == NULL) {
        perror("ERRO, \"pPlayers\" não pode ser NULL em \"AddPlayers\"");
        return false;
    }
    if (pass == NULL) {
        perror("ERRO, \"pass\" não pode ser NULL em \"AddPlayers\"");
        return false;
    } else if (strlen(pass) != 6) {
        perror("ERRO, \"pass\" não pode ter um tamanho diferente de 6 caracteres em \"AddPlayers\"");
        return false;
    }
    if (name == NULL) {
        perror("ERRO, \"name\" não pode ser NULL em \"AddPlayers\"");
        return false;
    } else if (strlen(name) > 19) {
        perror("ERRO, \"name\" não pode ter mais de 19 caracteres em \"AddPlayers\"");
        return false;
    }
<<<<<<< HEAD
    
    dataQuantities.Player++;
    pPlayers = (PlPointer)realloc(pPlayers, dataQuantities.Player * sizeof(Player));
    if(pPlayers == NULL){
        perror("ERRO na realocacao de memoria em \"AddPlayers\"");
=======

    dataQuantities->Player++;
    *pPlayers = (PlPointer)realloc(*pPlayers, dataQuantities->Player * sizeof(Player));
    if (*pPlayers == NULL) {
        perror("ERRO na realocação de memória em \"AddPlayers\"");
>>>>>>> origin/Shop
        return false;
    }
    memset(&(*pPlayers)[dataQuantities->Player - 1], 0, sizeof(Player));
    strcpy((*pPlayers)[dataQuantities->Player - 1].Name, name);
    strcpy((*pPlayers)[dataQuantities->Player - 1].Pass, pass);
    (*pPlayers)[dataQuantities->Player - 1].Pikocoins = 50;
    return true;
}

bool AddItemPlayerBag(PlPointer *pPlayers, int playerIndex, ItPointer pItems, int itemIndex) {
    if (pPlayers == NULL || *pPlayers == NULL) {
        perror("ERRO, \"pPlayers\" não pode ser NULL em \"AddItemPlayerBag\"");
        return false;
    }
    if (pItems == NULL) {
        perror("ERRO, \"pItems\" não pode ser NULL em \"AddItemPlayerBag\"");
        return false;
    }
    if (playerIndex < 0) {
        perror("ERRO, \"playerIndex\" não pode ser menor do que zero em \"AddItemPlayerBag\"");
        return false;
    }
    if (itemIndex < 0) {
        perror("ERRO, \"itemIndex\" não pode ser menor do que zero em \"AddItemPlayerBag\"");
        return false;
    }

<<<<<<< HEAD
    pPlayers[playerIndex].BagCurrentSize++;
    pPlayers[playerIndex].Bag = (ItPointer)realloc(pPlayers[playerIndex].Bag, pPlayers[playerIndex].BagCurrentSize * sizeof(Item));
    if(pPlayers[playerIndex].Bag == NULL){
        perror("ERRO na realocacao de memoria em \"AddItemPlayerBag\"");
        return false;
    }
    pPlayers[playerIndex].Bag[pPlayers[playerIndex].BagCurrentSize-1] = pItems[itemIndex];
    return true;
=======
    // Aumenta o tamanho atual da bag do jogador
    (*pPlayers)[playerIndex].BagCurrentSize++;
    (*pPlayers)[playerIndex].Bag = (ItPointer)realloc((*pPlayers)[playerIndex].Bag, (*pPlayers)[playerIndex].BagCurrentSize * sizeof(Item));
    if ((*pPlayers)[playerIndex].Bag == NULL) {
        perror("ERRO na realocação de memória em \"AddItemPlayerBag\"");
        return false;
    }

    // Adiciona o item à bag do jogador
    (*pPlayers)[playerIndex].Bag[(*pPlayers)[playerIndex].BagCurrentSize - 1] = pItems[itemIndex];
    return true; 
>>>>>>> origin/Shop
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
    return true;
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
    pSkills = (SkPointer)realloc(pSkills, dataQuantities.Skill * sizeof(Skill));
    for(i = 0; i < dataQuantities.Skill; i++){
        pSkills[i] = tempSkills[i];
    }
    free(tempSkills);
    return true;
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
    pItems = (ItPointer)realloc(pItems, dataQuantities.Item * sizeof(Item));
    for(i = 0; i < dataQuantities.Item; i++){
        pItems[i] = tempItems[i];
    }
    free(tempItems);
    return true;
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
    return true;
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
    return true;
}
//------------------------------------------------------------------------------//



//Battle functions
//------------------------------------------------------------------------------//
void CalcNextTurn(Pikomon selfPikomon, Pikomon enemyPikomon, char *calcNextTurn){ 
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

void CalcSkill(Element allElements[10], PiPointer *atacker, int skillIndex, PiPointer *defenser, bool *elementalEffectHit, bool *skillHit, bool *critHit, bool *selfEffectHit, bool *enemyEffectHit, int *selfDamage, int *enemyDamage){
/*
    int I, J, bonusQuantity;
    double elementalEffectivness;
    double physicalDamageReduction, magicDamageReduction;
    SkPointer usedSkill = &(*atacker)[0].Skills[skillIndex];


    if(((rand() % 100)+1) <= ((double)usedSkill[0].ElementEffectHitChance) * ((double)(*atacker)[0].Atributes[5].Total)/100.0){
        *elementalEffectHit = true;
        physicalDamageReduction = 1.0 - (DefenseReductionCalc((*defenser)[0].Atributes[1].Total) * 0.11);
        magicDamageReduction = 1.0 - (DefenseReductionCalc((*defenser)[0].Atributes[2].Total) * 0.11);
        elementalEffectivness = (double)usedSkill[0].Element.Effectiveness[(*defenser)[0].Element.SelfElementIndex] / 100.0;

        if(usedSkill[0].Element.CurrentHPEffect.Timer > 0){
            (*defenser)[0].CurrentHP.BonusQuantity++;
            bonusQuantity = (*defenser)[0].CurrentHP.BonusQuantity;
            (*defenser)[0].CurrentHP.acronym = (char**)realloc((*defenser)[0].CurrentHP.acronym, bonusQuantity * sizeof(char*));
            (*defenser)[0].CurrentHP.acronym[bonusQuantity-1] = (char *)calloc(4, sizeof(char));
            (*defenser)[0].CurrentHP.Bonus = (int*)realloc((*defenser)[0].CurrentHP.Bonus, bonusQuantity * sizeof(int));
            (*defenser)[0].CurrentHP.BonusTimer = (int*)realloc((*defenser)[0].CurrentHP.BonusTimer, bonusQuantity * sizeof(int));
            strcpy((*defenser)[0].CurrentHP.acronym[bonusQuantity-1], usedSkill[0].Element.Acronym);
            (*defenser)[0].CurrentHP.Bonus[bonusQuantity-1] = (int)((usedSkill[0].Element.CurrentHPEffect.Quantity + (((double)usedSkill[0].Element.ElementalDamageScale) * ((double)(*atacker)[0].Atributes[6].Total)/ 100.0)) * magicDamageReduction * elementalEffectivness);
            (*defenser)[0].CurrentHP.BonusTimer[bonusQuantity-1] = usedSkill[0].Element.CurrentHPEffect.Timer;
        }
        
        for (I = 0; I < 8; I++){
            if(usedSkill[0].Element.StatusEffect[I].Timer > 0){
                (*defenser)[0].Atributes[I].BonusQuantity++;
                bonusQuantity = (*defenser)[0].Atributes[I].BonusQuantity;
                (*defenser)[0].Atributes[I].acronym = (char**)realloc((*defenser)[0].Atributes[I].acronym, bonusQuantity * sizeof(char*));
                (*defenser)[0].Atributes[I].acronym[bonusQuantity-1] = (char*)calloc(4, sizeof(char));
                (*defenser)[0].Atributes[I].Bonus = (int*)realloc((*defenser)[0].Atributes[I].Bonus, bonusQuantity * sizeof(int));
                (*defenser)[0].Atributes[I].BonusTimer = (int*)realloc((*defenser)[0].Atributes[I].BonusTimer, bonusQuantity * sizeof(int));
                strcpy((*defenser)[0].Atributes[I].acronym[bonusQuantity-1], usedSkill[0].Element.StatusEffect[I].Acronym);
                (*defenser)[0].Atributes[I].Bonus[bonusQuantity-1] = usedSkill[0].Element.StatusEffect[I].Quantity;
                (*defenser)[0].Atributes[I].Total += (*defenser)[0].Atributes[I].Bonus[bonusQuantity-1];
                (*defenser)[0].Atributes[I].BonusTimer[bonusQuantity-1] = usedSkill[0].Element.StatusEffect[I].Timer;
            }
        }
    }
    else *elementalEffectHit = false;


    if(((rand() % 100)+1) <= (double)(*atacker)[0].Atributes[3].Total * (double)usedSkill[0].hitChance / 100.0){
        *skillHit = true;
        int magicDamage, physicalDamage;
        if((rand() % 100 +1) <= usedSkill[0].CritChance) *critHit = true;

        magicDamage = (int)(((double)(*atacker)[0].Atributes[6].Total * (double)usedSkill[0].MagicAttackScale / 100.0) + usedSkill[0].MagicBase) * (*critHit) ? 2 : 1;
        physicalDamage = (int)(((double)(*atacker)[0].Atributes[4].Total * (double)usedSkill[0].AttackScale / 100.0) + usedSkill[0].AttackBase) * (*critHit) ? 2 : 1;
        if(usedSkill[0].Target == 'S'){
            elementalEffectivness = (double)usedSkill[0].Element.Effectiveness[(*atacker)[0].Element.SelfElementIndex] / 100.0;
            physicalDamageReduction = 1.0 - (DefenseReductionCalc((*atacker)[0].Atributes[1].Total) * 0.11);
            magicDamageReduction = 1.0 - (DefenseReductionCalc((*atacker)[0].Atributes[2].Total) * 0.11);
            *selfDamage = (physicalDamage * physicalDamageReduction + magicDamage * magicDamageReduction) * elementalEffectivness;

            (*atacker)[0].CurrentHP.Total -= *selfDamage;

        }
        else if(usedSkill[0].Target == 'E'){
            elementalEffectivness = (double)usedSkill[0].Element.Effectiveness[(*defenser)[0].Element.SelfElementIndex] / 100.0;
            physicalDamageReduction = 1.0 - (DefenseReductionCalc((*defenser)[0].Atributes[1].Total) * 0.11);
            magicDamageReduction = 1.0 - (DefenseReductionCalc((*defenser)[0].Atributes[2].Total) * 0.11);
            *enemyDamage = (physicalDamage * physicalDamageReduction + magicDamage * magicDamageReduction) * elementalEffectivness;

            (*defenser)[0].CurrentHP.Total -= *enemyDamage;

        }
        else if(usedSkill[0].Target == 'B'){
            elementalEffectivness = (double)usedSkill[0].Element.Effectiveness[(*atacker)[0].Element.SelfElementIndex] / 100.0;
            physicalDamageReduction = 1.0 - (DefenseReductionCalc((*atacker)[0].Atributes[1].Total) * 0.11);
            magicDamageReduction = 1.0 - (DefenseReductionCalc((*atacker)[0].Atributes[2].Total) * 0.11);
            *selfDamage = (physicalDamage * physicalDamageReduction + magicDamage * magicDamageReduction) * elementalEffectivness;

            (*atacker)[0].CurrentHP.Total -= *selfDamage;


            elementalEffectivness = (double)usedSkill[0].Element.Effectiveness[(*defenser)[0].Element.SelfElementIndex] / 100.0;
            physicalDamageReduction = 1.0 - (DefenseReductionCalc((*defenser)[0].Atributes[1].Total) * 0.11);
            magicDamageReduction = 1.0 - (DefenseReductionCalc((*defenser)[0].Atributes[2].Total) * 0.11);
            *enemyDamage = (physicalDamage * physicalDamageReduction + magicDamage * magicDamageReduction) * elementalEffectivness;

            (*defenser)[0].CurrentHP.Total -= *enemyDamage;
        }
        else{
            perror("Alvo da skill usada não reconhecido");
        }
    }
    else *skillHit = false;


    if((rand() % 100 +1 <= usedSkill[0].SelfEffectHitChance)){
        *selfEffectHit = true;
        for (I = 0; I < 8; I++){
            if(usedSkill[0].Element.StatusEffect[I].Timer > 0){
                (*atacker)[0].Atributes[I].BonusQuantity++;
                bonusQuantity = (*atacker)[0].Atributes[I].BonusQuantity;
                (*atacker)[0].Atributes[I].acronym = (char**)realloc((*atacker)[0].Atributes[I].acronym, bonusQuantity * sizeof(char*));
                (*atacker)[0].Atributes[I].acronym[bonusQuantity-1] = (char*)calloc(4, sizeof(char));
                (*atacker)[0].Atributes[I].Bonus = (int*)realloc((*atacker)[0].Atributes[I].Bonus, bonusQuantity * sizeof(int));
                (*atacker)[0].Atributes[I].BonusTimer = (int*)realloc((*atacker)[0].Atributes[I].BonusTimer, bonusQuantity * sizeof(int));
                strcpy((*atacker)[0].Atributes[I].acronym[bonusQuantity-1], usedSkill[0].Element.StatusEffect[I].Acronym);
                (*atacker)[0].Atributes[I].Bonus[bonusQuantity-1] = usedSkill[0].Element.StatusEffect[I].Quantity;
                (*atacker)[0].Atributes[I].Total += (*atacker)[0].Atributes[I].Bonus[bonusQuantity-1];
                (*atacker)[0].Atributes[I].BonusTimer[bonusQuantity-1] = usedSkill[0].Element.StatusEffect[I].Timer;
            }
        }
    }
    else *selfEffectHit = false;


    if((rand() % 100 +1 <= usedSkill[0].EnemyEffectHitChance)){
        *enemyEffectHit = true;
        for (I = 0; I < 8; I++){
            if(usedSkill[0].Element.StatusEffect[I].Timer > 0){
                (*defenser)[0].Atributes[I].BonusQuantity++;
                bonusQuantity = (*defenser)[0].Atributes[I].BonusQuantity;
                (*defenser)[0].Atributes[I].acronym = (char**)realloc((*defenser)[0].Atributes[I].acronym, bonusQuantity * sizeof(char*));
                (*defenser)[0].Atributes[I].acronym[bonusQuantity-1] = (char*)calloc(4, sizeof(char));
                (*defenser)[0].Atributes[I].Bonus = (int*)realloc((*defenser)[0].Atributes[I].Bonus, bonusQuantity * sizeof(int));
                (*defenser)[0].Atributes[I].BonusTimer = (int*)realloc((*defenser)[0].Atributes[I].BonusTimer, bonusQuantity * sizeof(int));
                strcpy((*defenser)[0].Atributes[I].acronym[bonusQuantity-1], usedSkill[0].Element.StatusEffect[I].Acronym);
                (*defenser)[0].Atributes[I].Bonus[bonusQuantity-1] = usedSkill[0].Element.StatusEffect[I].Quantity;
                (*defenser)[0].Atributes[I].Total += (*defenser)[0].Atributes[I].Bonus[bonusQuantity-1];
                (*defenser)[0].Atributes[I].BonusTimer[bonusQuantity-1] = usedSkill[0].Element.StatusEffect[I].Timer;
            }
        }
    }
    else *enemyEffectHit = false;
 
 */
}

void UseItem(PlPointer *selfPlayer, PlPointer *enemyPlayer, int itemUsedIndex, bool *usedItemStatusHit){
    int I, bonusQuantity;
    double pikomonDamageReduction, otherpikomonDamageReduction;
    ItPointer usedItem;
    PiPointer pikomon, otherPikomon;
    usedItem = &(*selfPlayer)[0].Bag[itemUsedIndex];
    pikomon = &(*selfPlayer)[0].BatlePikomons[(*selfPlayer)[0].SelectedPikomonIndex];
    otherPikomon = &(*enemyPlayer)[0].BatlePikomons[(*enemyPlayer)[0].SelectedPikomonIndex];
    if(usedItem[0].CurrentHPDamageIsPhysic){
        pikomonDamageReduction = 1.0 - (DefenseReductionCalc(pikomon[0].Atributes[1].Total) * 0.11);
        otherpikomonDamageReduction = 1.0 - (DefenseReductionCalc(otherPikomon[0].Atributes[1].Total) * 0.11); 
    }
    else{
        pikomonDamageReduction = 1.0 - (DefenseReductionCalc(pikomon[0].Atributes[2].Total) * 0.11);
        otherpikomonDamageReduction = 1.0 - (DefenseReductionCalc(otherPikomon[0].Atributes[2].Total) * 0.11);
    }

    if(usedItem[0].EffectCurrentHPTarget == 'S'){
        if(rand() % 100 +1 <= usedItem[0].StatusEffectChance){
            *usedItemStatusHit = true;
            if(usedItem[0].EffectCurrentHP.Timer > 0){
                pikomon[0].CurrentHP.BonusQuantity++;
                bonusQuantity = pikomon[0].CurrentHP.BonusQuantity;
                pikomon[0].CurrentHP.acronym = (char**)realloc(pikomon[0].CurrentHP.acronym, bonusQuantity * sizeof(char*));
                pikomon[0].CurrentHP.acronym[bonusQuantity-1] = (char *)calloc(4, sizeof(char));
                pikomon[0].CurrentHP.Bonus = (int*)realloc(pikomon[0].CurrentHP.Bonus, bonusQuantity * sizeof(int));
                pikomon[0].CurrentHP.BonusTimer = (int*)realloc(pikomon[0].CurrentHP.BonusTimer, bonusQuantity * sizeof(int));
                strcpy(pikomon[0].CurrentHP.acronym[bonusQuantity-1], usedItem[0].EffectCurrentHP.Acronym);
                pikomon[0].CurrentHP.Bonus[bonusQuantity-1] = (int)( usedItem[0].EffectCurrentHP.Quantity * pikomonDamageReduction);
                pikomon[0].CurrentHP.BonusTimer[bonusQuantity-1] = usedItem[0].EffectCurrentHP.Timer;
            }
            
            for (I = 0; I < 8; I++){
                if(usedItem[0].StatusEffect[I].Timer > 0){
                    pikomon[0].Atributes[I].BonusQuantity++;
                    bonusQuantity = pikomon[0].Atributes[I].BonusQuantity;
                    pikomon[0].Atributes[I].acronym = (char**)realloc(pikomon[0].Atributes[I].acronym, bonusQuantity * sizeof(char*));
                    pikomon[0].Atributes[I].acronym[bonusQuantity-1] = (char*)calloc(4, sizeof(char));
                    pikomon[0].Atributes[I].Bonus = (int*)realloc(pikomon[0].Atributes[I].Bonus, bonusQuantity * sizeof(int));
                    pikomon[0].Atributes[I].BonusTimer = (int*)realloc(pikomon[0].Atributes[I].BonusTimer, bonusQuantity * sizeof(int));
                    strcpy(pikomon[0].Atributes[I].acronym[bonusQuantity-1], usedItem[0].StatusEffect[I].Acronym);
                    pikomon[0].Atributes[I].Bonus[bonusQuantity-1] = usedItem[0].StatusEffect[I].Quantity;
                    pikomon[0].Atributes[I].Total += pikomon[0].Atributes[I].Bonus[bonusQuantity-1];
                    pikomon[0].Atributes[I].BonusTimer[bonusQuantity-1] = usedItem[0].StatusEffect[I].Timer;
                }
            }
        }
        else *usedItemStatusHit = false;
    }
    else if(usedItem[0].EffectCurrentHPTarget == 'E'){
        if(rand() % 100 +1 <= usedItem[0].StatusEffectChance){
            *usedItemStatusHit = true;
            if(usedItem[0].EffectCurrentHP.Timer > 0){
                otherPikomon[0].CurrentHP.BonusQuantity++;
                bonusQuantity = otherPikomon[0].CurrentHP.BonusQuantity;
                otherPikomon[0].CurrentHP.acronym = (char**)realloc(otherPikomon[0].CurrentHP.acronym, bonusQuantity * sizeof(char*));
                otherPikomon[0].CurrentHP.acronym[bonusQuantity-1] = (char *)calloc(4, sizeof(char));
                otherPikomon[0].CurrentHP.Bonus = (int*)realloc(otherPikomon[0].CurrentHP.Bonus, bonusQuantity * sizeof(int));
                otherPikomon[0].CurrentHP.BonusTimer = (int*)realloc(otherPikomon[0].CurrentHP.BonusTimer, bonusQuantity * sizeof(int));
                strcpy(otherPikomon[0].CurrentHP.acronym[bonusQuantity-1], usedItem[0].EffectCurrentHP.Acronym);
                otherPikomon[0].CurrentHP.Bonus[bonusQuantity-1] = (int)( usedItem[0].EffectCurrentHP.Quantity * pikomonDamageReduction);
                otherPikomon[0].CurrentHP.BonusTimer[bonusQuantity-1] = usedItem[0].EffectCurrentHP.Timer;
            }
            
            for (I = 0; I < 8; I++){
                if(usedItem[0].StatusEffect[I].Timer > 0){
                    otherPikomon[0].Atributes[I].BonusQuantity++;
                    bonusQuantity = otherPikomon[0].Atributes[I].BonusQuantity;
                    otherPikomon[0].Atributes[I].acronym = (char**)realloc(otherPikomon[0].Atributes[I].acronym, bonusQuantity * sizeof(char*));
                    otherPikomon[0].Atributes[I].acronym[bonusQuantity-1] = (char*)calloc(4, sizeof(char));
                    otherPikomon[0].Atributes[I].Bonus = (int*)realloc(otherPikomon[0].Atributes[I].Bonus, bonusQuantity * sizeof(int));
                    otherPikomon[0].Atributes[I].BonusTimer = (int*)realloc(otherPikomon[0].Atributes[I].BonusTimer, bonusQuantity * sizeof(int));
                    strcpy(otherPikomon[0].Atributes[I].acronym[bonusQuantity-1], usedItem[0].StatusEffect[I].Acronym);
                    otherPikomon[0].Atributes[I].Bonus[bonusQuantity-1] = usedItem[0].StatusEffect[I].Quantity;
                    otherPikomon[0].Atributes[I].Total += otherPikomon[0].Atributes[I].Bonus[bonusQuantity-1];
                    otherPikomon[0].Atributes[I].BonusTimer[bonusQuantity-1] = usedItem[0].StatusEffect[I].Timer;
                }
            }
        }
        else *usedItemStatusHit = false;
    }
    else if(usedItem[0].EffectCurrentHPTarget == 'B'){
        if(rand() % 100 +1 <= usedItem[0].StatusEffectChance){
            *usedItemStatusHit = true;

            if(usedItem[0].EffectCurrentHP.Timer > 0){
                pikomon[0].CurrentHP.BonusQuantity++;
                bonusQuantity = pikomon[0].CurrentHP.BonusQuantity;
                pikomon[0].CurrentHP.acronym = (char**)realloc(pikomon[0].CurrentHP.acronym, bonusQuantity * sizeof(char*));
                pikomon[0].CurrentHP.acronym[bonusQuantity-1] = (char *)calloc(4, sizeof(char));
                pikomon[0].CurrentHP.Bonus = (int*)realloc(pikomon[0].CurrentHP.Bonus, bonusQuantity * sizeof(int));
                pikomon[0].CurrentHP.BonusTimer = (int*)realloc(pikomon[0].CurrentHP.BonusTimer, bonusQuantity * sizeof(int));
                strcpy(pikomon[0].CurrentHP.acronym[bonusQuantity-1], usedItem[0].EffectCurrentHP.Acronym);
                pikomon[0].CurrentHP.Bonus[bonusQuantity-1] = (int)( usedItem[0].EffectCurrentHP.Quantity * pikomonDamageReduction);
                pikomon[0].CurrentHP.BonusTimer[bonusQuantity-1] = usedItem[0].EffectCurrentHP.Timer;
            }
            
            for (I = 0; I < 8; I++){
                if(usedItem[0].StatusEffect[I].Timer > 0){
                    pikomon[0].Atributes[I].BonusQuantity++;
                    bonusQuantity = pikomon[0].Atributes[I].BonusQuantity;
                    pikomon[0].Atributes[I].acronym = (char**)realloc(pikomon[0].Atributes[I].acronym, bonusQuantity * sizeof(char*));
                    pikomon[0].Atributes[I].acronym[bonusQuantity-1] = (char*)calloc(4, sizeof(char));
                    pikomon[0].Atributes[I].Bonus = (int*)realloc(pikomon[0].Atributes[I].Bonus, bonusQuantity * sizeof(int));
                    pikomon[0].Atributes[I].BonusTimer = (int*)realloc(pikomon[0].Atributes[I].BonusTimer, bonusQuantity * sizeof(int));
                    strcpy(pikomon[0].Atributes[I].acronym[bonusQuantity-1], usedItem[0].StatusEffect[I].Acronym);
                    pikomon[0].Atributes[I].Bonus[bonusQuantity-1] = usedItem[0].StatusEffect[I].Quantity;
                    pikomon[0].Atributes[I].Total += pikomon[0].Atributes[I].Bonus[bonusQuantity-1];
                    pikomon[0].Atributes[I].BonusTimer[bonusQuantity-1] = usedItem[0].StatusEffect[I].Timer;
                }
            }

            if(usedItem[0].EffectCurrentHP.Timer > 0){
                otherPikomon[0].CurrentHP.BonusQuantity++;
                bonusQuantity = otherPikomon[0].CurrentHP.BonusQuantity;
                otherPikomon[0].CurrentHP.acronym = (char**)realloc(otherPikomon[0].CurrentHP.acronym, bonusQuantity * sizeof(char*));
                otherPikomon[0].CurrentHP.acronym[bonusQuantity-1] = (char *)calloc(4, sizeof(char));
                otherPikomon[0].CurrentHP.Bonus = (int*)realloc(otherPikomon[0].CurrentHP.Bonus, bonusQuantity * sizeof(int));
                otherPikomon[0].CurrentHP.BonusTimer = (int*)realloc(otherPikomon[0].CurrentHP.BonusTimer, bonusQuantity * sizeof(int));
                strcpy(otherPikomon[0].CurrentHP.acronym[bonusQuantity-1], usedItem[0].EffectCurrentHP.Acronym);
                otherPikomon[0].CurrentHP.Bonus[bonusQuantity-1] = (int)( usedItem[0].EffectCurrentHP.Quantity * pikomonDamageReduction);
                otherPikomon[0].CurrentHP.BonusTimer[bonusQuantity-1] = usedItem[0].EffectCurrentHP.Timer;
            }
            
            for (I = 0; I < 8; I++){
                if(usedItem[0].StatusEffect[I].Timer > 0){
                    otherPikomon[0].Atributes[I].BonusQuantity++;
                    bonusQuantity = otherPikomon[0].Atributes[I].BonusQuantity;
                    otherPikomon[0].Atributes[I].acronym = (char**)realloc(otherPikomon[0].Atributes[I].acronym, bonusQuantity * sizeof(char*));
                    otherPikomon[0].Atributes[I].acronym[bonusQuantity-1] = (char*)calloc(4, sizeof(char));
                    otherPikomon[0].Atributes[I].Bonus = (int*)realloc(otherPikomon[0].Atributes[I].Bonus, bonusQuantity * sizeof(int));
                    otherPikomon[0].Atributes[I].BonusTimer = (int*)realloc(otherPikomon[0].Atributes[I].BonusTimer, bonusQuantity * sizeof(int));
                    strcpy(otherPikomon[0].Atributes[I].acronym[bonusQuantity-1], usedItem[0].StatusEffect[I].Acronym);
                    otherPikomon[0].Atributes[I].Bonus[bonusQuantity-1] = usedItem[0].StatusEffect[I].Quantity;
                    otherPikomon[0].Atributes[I].Total += otherPikomon[0].Atributes[I].Bonus[bonusQuantity-1];
                    otherPikomon[0].Atributes[I].BonusTimer[bonusQuantity-1] = usedItem[0].StatusEffect[I].Timer;
                }
            }

        }
        else *usedItemStatusHit = false;
    }
    else{
        perror("usedItem[0].EffectCurrentHPTarget nao reconhecido");
    }
}

void PassPikomonTurnTime(PiPointer *pikomon){
    //vai ter problema com o CurrentHp.acronym e os atribute.acronym. neles não se podem usar o realloc porque não libera os ponteiros individuais
    int I, J, K, indexToRemove;
    Effect *tempEffect;
    PiPointer pik;
    pik = *pikomon;
    
    for(J = 0; J < pik[0].CurrentHP.BonusQuantity; J++){
        pik[0].CurrentHP.BonusTimer[J]--;
        if(pik[0].CurrentHP.BonusTimer[J] < 0){
            tempEffect = calloc(pik[0].CurrentHP.BonusQuantity-1, sizeof(Effect));
                for(K = 0; K < pik[0].CurrentHP.BonusQuantity; K++){
                    if(K != J){
                        strcpy(tempEffect[K].Acronym, pik[0].CurrentHP.acronym[K]);
                        tempEffect[K].Quantity = pik[0].CurrentHP.Bonus[K];
                        tempEffect[K].Timer = pik[0].CurrentHP.BonusTimer[K];
                    }
                }
                for(K = 0; K < pik[0].CurrentHP.BonusQuantity; K++){
                    free(pik[0].CurrentHP.acronym[K]);
                }
                pik[0].CurrentHP.BonusQuantity--;
                pik[0].CurrentHP.acronym = (char**)realloc(pik[0].CurrentHP.acronym, pik[0].CurrentHP.BonusQuantity * sizeof(char*));
                for(K = 0; K < pik[0].CurrentHP.BonusQuantity; K++){
                    pik[0].CurrentHP.acronym[K] = (char*)calloc(4, sizeof(char));
                }
                pik[0].CurrentHP.Bonus = (int*)realloc(pik[0].CurrentHP.Bonus, pik[0].CurrentHP.BonusQuantity * sizeof(int));
                pik[0].CurrentHP.BonusTimer = (int*)realloc(pik[0].CurrentHP.BonusTimer, pik[0].CurrentHP.BonusQuantity * sizeof(int));
                for(K = 0; K < pik[0].CurrentHP.BonusQuantity; K++){
                    strcpy(pik[0].CurrentHP.acronym[K], tempEffect[K].Acronym);
                    pik[0].CurrentHP.Bonus[K] = tempEffect[K].Quantity;
                    pik[0].CurrentHP.BonusTimer[K] = tempEffect[K].Timer;
                }
            free(tempEffect);
        }
    }

    for(I = 0; I < 8; I++){
        for(J = 0; J < pik[0].Atributes[I].BonusQuantity; J++){
            pik[0].Atributes[I].BonusTimer[J]--;
            if(pik[0].Atributes[I].BonusTimer[J] < 0){
                tempEffect = calloc(pik[0].Atributes[I].BonusQuantity-1, sizeof(Effect));
                for(K = 0; K < pik[0].Atributes[I].BonusQuantity; K++){
                    if(K != J){
                        strcpy(tempEffect[K].Acronym, pik[0].Atributes[I].acronym[K]);
                        tempEffect[K].Quantity = pik[0].Atributes[I].Bonus[K];
                        tempEffect[K].Timer = pik[0].Atributes[I].BonusTimer[K];
                    }
                }
                for(K = 0; K < pik[0].Atributes[I].BonusQuantity; K++){
                    free(pik[0].Atributes[I].acronym[K]);
                }
                pik[0].Atributes[I].BonusQuantity--;
                pik[0].Atributes[I].acronym = (char**)realloc(pik[0].Atributes[I].acronym, pik[0].Atributes[I].BonusQuantity * sizeof(char*));
                for(K = 0; K < pik[0].Atributes[I].BonusQuantity; K++){
                    pik[0].Atributes[I].acronym[K] = (char*)calloc(4, sizeof(char));
                }
                pik[0].Atributes[I].Bonus = (int*)realloc(pik[0].Atributes[I].Bonus, pik[0].Atributes[I].BonusQuantity * sizeof(int));
                pik[0].Atributes[I].BonusTimer = (int*)realloc(pik[0].Atributes[I].BonusTimer, pik[0].Atributes[I].BonusQuantity * sizeof(int));
                for(K = 0; K < pik[0].Atributes[I].BonusQuantity; K++){
                    strcpy(pik[0].Atributes[I].acronym[K], tempEffect[K].Acronym);
                    pik[0].Atributes[I].Bonus[K] = tempEffect[K].Quantity;
                    pik[0].Atributes[I].BonusTimer[K] = tempEffect[K].Timer;
                }
            free(tempEffect);
            }
        }
    }

    for(J = 0; J < pik[0].CurrentHP.BonusQuantity; J++){
        pik[0].CurrentHP.Total += pik[0].CurrentHP.Bonus[J];
    }

    for(I = 0; I < 8; I++){
        pik[0].Atributes[I].Total = 0;
        for(J = 0; J < pik[0].Atributes[I].BonusQuantity; J++){
            pik[0].Atributes[I].Total += pik[0].Atributes[I].Bonus[J];
        }
    }
}
//------------------------------------------------------------------------------//


//Log function
//------------------------------------------------------------------------------//
//a funcao log do math.h estava dando um conflito muito estranho toda a vez que chamava com alguma variavel dentro
double DefenseReductionCalc(double value){
    int base = 2;
    return log(value) / log(base);
    return 1.0;
}
//------------------------------------------------------------------------------//



bool Login(PlPointer pPlayers, int playersQuantity, bool *login1, bool *login2, int *indexUs1, int *indexUs2) {
    char usernameEntrada[20];
    char passEntrada[7];
    int userNumero = 1;
    int indexPlayerLoop;
    bool usernameAchado, senhaAchado;
    char nomeUsuario1[20];

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
        printf("\n                    Insira seu Username: ");
        scanf("%19s", usernameEntrada); 
        printf("\n                    Insira sua Senha   : ");
        scanf("%6s", passEntrada);
        if(*login1 && strcmp(usernameEntrada, nomeUsuario1) == 0){
            printf("Usuário já logado. Tente novamente.(Press Enter)\n");
            getchar();
            getchar();
            LimparTerminal();
        }
        else{
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
                *indexUs1 = indexPlayerLoop;
                *login1 = true;
            } else {
                *indexUs2 = indexPlayerLoop;
                *login2 = true;
            }
            printf("Bem-vindo, %s! Você está logado!(Press Enter)\n", usernameEntrada);
            getchar();
            getchar();
            LimparTerminal();
        } else {
            printf("Usuário ou senha incorretos. Tente novamente.(Press Enter)\n");
            getchar();
            getchar();
            LimparTerminal();
        }
    }
    }

    return true;
}


void Menu(){
    struct timespec tempo;
    tempo.tv_sec = 0;              
    tempo.tv_nsec = 200000000;




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


void MenuBattle(Pikomon epPikomon, Pikomon ppPikomon, char *Turnos) {

    LimparTerminal();  

    // Pikomon inimigo
    printf(" __________________________________________________________________________________________________________________________________________ \n");
    printf("|                    %-20s                                                                                                  |\n", epPikomon.Name);
    printf("|                    HP: %d/%d                                                                                                           |\n", epPikomon.CurrentHP.Total, epPikomon.Atributes[0].Total);
    printf("|%19s                                                                                                                       |\n", ppPikomon.IconImg[0]);
    printf("|%19s                                                                                                                       |\n", ppPikomon.IconImg[1]);
    printf("|%19s                                                                                                                       |\n", ppPikomon.IconImg[2]);
    printf("|%19s                                                                                                                       |\n", ppPikomon.IconImg[3]);
    printf("|%19s                                                                                                                       |\n", ppPikomon.IconImg[4]);
    printf("|%19s                                                                                                                       |\n", ppPikomon.IconImg[5]);
    printf("|%19s                                                                                                                       |\n", ppPikomon.IconImg[6]);
    printf("|                                                                                                                                          |\n");
    printf("|__________________________________________________________________________________________________________________________________________|\n\n");

    printf("%-s\n", Turnos);
    // Pikomon do jogador Turno
    printf(" __________________________________________________________________________________________________________________________________________ \n");
    printf("|                   |%-20s |                                                                                                 |\n", ppPikomon.Name);
    printf("|                   |HP: %-7d/%-7d |                                                                                                 |\n", ppPikomon.CurrentHP.Total, ppPikomon.Atributes[0].Total);
    printf("|%19s|ATK: %-15d|      1. A T A C A R                                         2. I T E N S                        |\n", ppPikomon.IconImg[0], ppPikomon.Atributes[4].Total);
    printf("|%19s|DEF: %-15d|         - - - - - -                                            - - - - -                        |\n", ppPikomon.IconImg[1], ppPikomon.Atributes[1].Total);
    printf("|%19s|SpA: %-15d|                                                                                                 |\n", ppPikomon.IconImg[2], ppPikomon.Atributes[6].Total);
    printf("|%19s|SpD: %-15d|                                                                                                 |\n", ppPikomon.IconImg[3], ppPikomon.Atributes[2].Total);
    printf("|%19s|Spd: %-15d|      3. T R O C A R                                         4. D E S I S T I R                  |\n", ppPikomon.IconImg[4], ppPikomon.Atributes[7].Total);
    printf("|%19s|                    |         - - - - - -                                            - - - - - - - -                  |\n", ppPikomon.IconImg[5]);
    printf("|%19s|                    |                                                                                                 |\n", ppPikomon.IconImg[6]);
    printf("|                   |                    |                                                                                                 |\n");
    printf("|___________________|____________________|_________________________________________________________________________________________________|\n\n");
}


bool GerarPikomon(Pikomon pPikomon, Personality *personalities, Element element, int BaseHP, int BaseDefense, int BaseMagicDefense, int BaseAcurracy, int BaseAttack, int BaseElementalAcurracy, int BaseMagicAttack, int BaseSpeed) {
    if (personalities == NULL || 13 <= 0) {
        return false;
    }

    // Isso aq inicializa o gerador de numeros
    srand(time(NULL));

    int totalRaridades = 0;
    for (int i = 0; i < 13; i++) {
        totalRaridades += personalities[i].rarity;
    }

    int valorAleatorio = rand() % totalRaridades;
    int raridadeSum = 0;
    for (int i = 0; i < 13; i++) {
        raridadeSum += personalities[i].rarity;
        if (valorAleatorio < raridadeSum) {
            pPikomon.Personality = personalities[i];
            break;
        }
    }

    pPikomon.Atributes[0].Base = (int)(BaseHP * pPikomon.Personality.BaseHPModifier / 100);
    pPikomon.Atributes[1].Base = (int)(BaseDefense * pPikomon.Personality.BaseDefenseModifier / 100);
    pPikomon.Atributes[2].Base = (int)(BaseMagicDefense * pPikomon.Personality.BaseMagicDefenseModifier / 100);
    pPikomon.Atributes[3].Base = (int)(BaseAcurracy * pPikomon.Personality.BaseAcurracyModifier / 100);
    pPikomon.Atributes[4].Base = (int)(BaseAttack * pPikomon.Personality.BaseAttackModifier / 100);
    pPikomon.Atributes[5].Base = (int)(BaseElementalAcurracy * pPikomon.Personality.BaseElementalAcurracyModifier / 100);
    pPikomon.Atributes[6].Base = (int)(BaseMagicAttack * pPikomon.Personality.BaseMagicAttackModifier / 100);
    pPikomon.Atributes[7].Base = (int)(BaseSpeed * pPikomon.Personality.BaseSpeedModifier / 100);

    pPikomon.Element = element;

    //PRECISA AINDA COLOCAR O ADDPIKOMON NO STORAGE DO PLAYER

    return true;
}


bool ShopPikomon(PlPointer players, int playerAtualIndex, PiPointer pPikomon, DataQuantity pikomonQuantidade, Personality* personalities) { 
    Player* playerAtual = &players[playerAtualIndex];

    printf("_____________________________________\n");
    printf("|                                   |\n");
    printf("|        !Você tem %d pikocoins!     |\n", playerAtual->Pikocoins);
    printf("|                                   |\n");
    printf("_____________________________________\n");

    printf("Pikomons disponíveis para compra:\n");
    DebugPikomons(pPikomon, -1, pikomonQuantidade.Pikomon);
    printf("Escolha um Pikomon para comprar (ou -1 para sair): ");
    
    int playerEscolha;
    scanf("%d", &playerEscolha);

    if (playerEscolha == -1) {
        return true;
    }

    if (playerEscolha < 0 || playerEscolha >= pikomonQuantidade.Pikomon) {
        printf("Escolha inválida. Tente novamente.\n");
        return false;
    }

    if (playerAtual->Pikocoins < 10) {
        printf("Você não tem Pikocoins suficientes para comprar este Pikomon.\n");
        return false;
    }

    Pikomon novoPikomon = pPikomon[playerEscolha];
    if (!GerarPikomon(novoPikomon, personalities, pPikomon[playerEscolha].Element, pPikomon[playerEscolha].Atributes[0].Total, pPikomon[playerEscolha].Atributes[1].Total, pPikomon[playerEscolha].Atributes[2].Total, pPikomon[playerEscolha].Atributes[3].Total, pPikomon[playerEscolha].Atributes[4].Total, pPikomon[playerEscolha].Atributes[5].Total, pPikomon[playerEscolha].Atributes[6].Total, pPikomon[playerEscolha].Atributes[7].Total)) {
        printf("Falha ao gerar Pikomon. Tente novamente.\n");
        return false;
    }

    int quantidadePikomonArmazenado = 0;
    for (int i = 0; i < 12; i++) {
        if (strlen(playerAtual->PikomonsStorage[i].Name) > 0) {
            quantidadePikomonArmazenado++;
        }
    }

    if (quantidadePikomonArmazenado >= 12) {
        printf("Você já possui o máximo de Pikomons permitido no armazenamento.\n");
        return false;
    }

    playerAtual->PikomonsStorage[quantidadePikomonArmazenado] = novoPikomon;
    playerAtual->Pikocoins -= 10;

    printf("Você comprou %s! Agora você tem %d pikocoins restantes.\n", novoPikomon.Name, playerAtual->Pikocoins);
    
    return true;
}

void MenuShopMP() {
    printf("                 _____________________________________\n");
    printf("                |                                     |\n");
    printf("                |                MENU DA LOJA         |\n");
    printf("                |                                     |\n");
    printf("                |    1. Pikomons                      |\n");
    printf("                |    2. Itens                         |\n");
    printf("                |_____________________________________|\n");
    printf("                | Escolha uma opção (3 para passar)   |\n");
    printf("                |_____________________________________|\n");
}