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
    int HitChance;
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

    char Name[20];
    char IconImg[7][25];
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
void FreeAllHeapMemoryAndSaveEverything(SkPointer pSkills, ItPointer pItems, PiPointer pPikomons, PlPointer *pPlayers, DataQuantity dataquantities, const char *dataQuantity, const char *skills, const char *items, const char *pikomoms, const char *players);
bool AddSkill(SkPointer *pSkills, DataQuantity *dataQuantities, char *name, char *type, char description[3][255], char *active, char activeDescription[3][255], bool learnablePersonalities[13], bool LearnableElements[10], int elementEffectHitChance, Element element, char target, int hitChance, int  attackBase, int attackScale, int magicBase, int magicAttackScale, int critChance, char effectTarget, int enemyEffectHitChance, Effect enemyEffect[8], int selfEffectHitChance, Effect selfEffect[8]);
bool AddItem(ItPointer *pItems, DataQuantity *dataQuantities, char *name, char *type, char description[3][255], char *active, char activeDescription[3][255], int value, bool currentHPDamageIsPysic, char effectCurrentHPTarget, Effect EffectCurrentHP, char effectTarget, int StatusEffectChance, Effect StatusEffect[8]);
bool AddPikomon(PiPointer *pPikomons, DataQuantity *dataQuantities, char *name, Element element, char iconImg[7][25], char passive[20], char passiveDescription[3][255], int value, int BaseHP, int BaseDefense, int BaseMagicDefense, int BaseAccuracy, int BaseAttack, int BaseElementalAccuracy, int BaseMagicAttack, int BaseSpeed);
bool AddPlayer(PlPointer *pPlayers, DataQuantity *dataQuantities, char *name, char *pass);
bool AddItemPlayerBag(PlPointer pPlayers, int playerIndex, ItPointer pItems, int itemIndex);;
bool StorePikomonPlayer(PlPointer *pPlayers, int playerIndex, int storagePikomonPlacementIndex, PiPointer pPikomons, int pikomonIndex, DataQuantity dataQuantities);
bool RemoveSkill(SkPointer *pSkills, DataQuantity *dataQuantities, int indexRemove);
bool RemoveItem(ItPointer *pItems, DataQuantity *dataQuantities, int indexRemove);
bool RemovePikomon(PiPointer *pPikomons, DataQuantity *dataQuantities, int indexRemove);
bool SellItemPlayerBag(PlPointer *pPlayers, int playerIndex, int bagSellIndex);
void SelectBattlePikomons(Player *player);
void CalcNextTurn(Pikomon selfPikomon, Pikomon enemyPikomon, char calcNextTurn[7]);
void Batle(PlPointer pPlayers, int playerOneIndex, int playerTwoIndex);
void LimparTerminal();
void Menu();
void MenuLogin(int userNumero);
void MenuBattle(Pikomon epPikomon, Pikomon ppPikomon, char *Turnos);
bool Login(PlPointer pPlayers, int playersQuantity, bool *login1, bool *login2, int *indexUs1, int *indexUs2);
bool ShopPikomon(PlPointer *players, int playerAtualIndex, PiPointer pPikomon, DataQuantity pikomonQuantidade, Personality* personalities);
void MenuShopMP();
double DefenseReductionCalc(double value);
double LNfalso(double x);
bool ShopItems(PlPointer players, int playerAtualIndex, ItPointer pItems, DataQuantity itemQuantidade);
void PrintPikomonEffects(PiPointer pikomon);
void PassPikomonTurnTime(PiPointer pikomon);
bool ShowSkill(SkPointer skill);
void CalcSkill(Element allElements[10], PiPointer atacker, int skillIndex, PiPointer defenser, bool *elementalEffectHit, bool *skillHit, bool *critHit, bool *selfEffectHit, bool *enemyEffectHit, int *selfDamage, int *enemyDamage);
bool ShowItems(ItPointer pItem);
void UseItem(PlPointer selfPlayer, PlPointer enemyPlayer, int itemUsedIndex, bool *usedItemStatusHit);
bool ShowPikomon(PiPointer pPikomon);
Pikomon GerarPikomon(PiPointer pPikomons, int indexPikomon, Personality *personalities);
void EscolherSkills(Player *player);


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
    /*
        strcpy(allPersonalities[0].Name, "Cabeca-Quente");
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
    strcpy(allElements[0].Name, "Comum"); 
    strcpy(allElements[0].Acronym, "Drm");
    allElements[0].Effectiveness[0] = 100; 
    allElements[0].Effectiveness[1] = 100;
    allElements[0].Effectiveness[2] = 100;
    allElements[0].Effectiveness[3] = 100;
    allElements[0].Effectiveness[4] = 100;
    allElements[0].Effectiveness[5] = 100;
    allElements[0].Effectiveness[6] = 100;
    allElements[0].Effectiveness[7] = 100;
    allElements[0].Effectiveness[8] = 100;
    allElements[0].Effectiveness[9] = 100;
    //------------------//
    allElements[0].ElementalDamageScale = 0;
    allElements[0].SelfElementIndex = 0;
    //------------------//
    strcpy(allElements[0].CurrentHPEffect.Acronym, "HP");
    allElements[0].CurrentHPEffect.Quantity = 0;
    allElements[0].CurrentHPEffect.Timer = 0;
    //------------------//
    strcpy(allElements[0].StatusEffect[0].Acronym, "Def");
    allElements[0].StatusEffect[0].Quantity = 0;
    allElements[0].StatusEffect[0].Timer = 0;
    strcpy(allElements[0].StatusEffect[1].Acronym, "MaD");
    allElements[0].StatusEffect[1].Quantity = 0;
    allElements[0].StatusEffect[1].Timer = 0;
    strcpy(allElements[0].StatusEffect[2].Acronym, "Acc");
    allElements[0].StatusEffect[2].Quantity = 0;
    allElements[0].StatusEffect[2].Timer = 0;
    strcpy(allElements[0].StatusEffect[3].Acronym, "Atk");
    allElements[0].StatusEffect[3].Quantity = 0;
    allElements[0].StatusEffect[3].Timer = 0; 
    strcpy(allElements[0].StatusEffect[4].Acronym, "ElA");
    allElements[0].StatusEffect[4].Quantity = 0;
    allElements[0].StatusEffect[4].Timer = 0;
    strcpy(allElements[0].StatusEffect[5].Acronym, "MaA");
    allElements[0].StatusEffect[5].Quantity = 0;
    allElements[0].StatusEffect[5].Timer = 0;
    strcpy(allElements[0].StatusEffect[6].Acronym, "Spd");
    allElements[0].StatusEffect[6].Quantity = -100;
    allElements[0].StatusEffect[6].Timer = 3;

    strcpy(allElements[1].Name, "Flamejante"); 
    strcpy(allElements[1].Acronym, "Qmd");
    allElements[1].Effectiveness[0] = 100; //Comum
    allElements[1].Effectiveness[1] = 50;  //Flamejante
    allElements[1].Effectiveness[2] = 50;  //Aquático
    allElements[1].Effectiveness[3] = 200; //Natural
    allElements[1].Effectiveness[4] = 100; //Elétrico
    allElements[1].Effectiveness[5] = 100; //Misterioso
    allElements[1].Effectiveness[6] = 100; //Luminoso
    allElements[1].Effectiveness[7] = 200; //Gélido
    allElements[1].Effectiveness[8] = 50;  //Mineral
    allElements[1].Effectiveness[9] = 100; //Tóxico
    //------------------//
    allElements[1].ElementalDamageScale = -15;
    allElements[1].SelfElementIndex = 1;
    //------------------//
    strcpy(allElements[1].CurrentHPEffect.Acronym, "HP");
    allElements[1].CurrentHPEffect.Quantity = -5;
    allElements[1].CurrentHPEffect.Timer = 4;
    //------------------//
    strcpy(allElements[1].StatusEffect[0].Acronym, "Def");
    allElements[1].StatusEffect[0].Quantity = 0;
    allElements[1].StatusEffect[0].Timer = 0;
    strcpy(allElements[1].StatusEffect[1].Acronym, "MaD");
    allElements[1].StatusEffect[1].Quantity = 0;
    allElements[1].StatusEffect[1].Timer = 0;
    strcpy(allElements[1].StatusEffect[2].Acronym, "Acc");
    allElements[1].StatusEffect[2].Quantity = 0;
    allElements[1].StatusEffect[2].Timer = 0;
    strcpy(allElements[1].StatusEffect[3].Acronym, "Atk");
    allElements[1].StatusEffect[3].Quantity = -15;
    allElements[1].StatusEffect[3].Timer = 4; 
    strcpy(allElements[1].StatusEffect[4].Acronym, "EAc");
    allElements[1].StatusEffect[4].Quantity = 0;
    allElements[1].StatusEffect[4].Timer = 0;
    strcpy(allElements[1].StatusEffect[5].Acronym, "MaA");
    allElements[1].StatusEffect[5].Quantity = 0;
    allElements[1].StatusEffect[5].Timer = 0;
    strcpy(allElements[1].StatusEffect[6].Acronym, "Spd");
    allElements[1].StatusEffect[6].Quantity = 0;
    allElements[1].StatusEffect[6].Timer = 0;

    strcpy(allElements[2].Name, "Aquático"); 
    strcpy(allElements[2].Acronym, "Mol");
    allElements[2].Effectiveness[0] = 100; //Comum
    allElements[2].Effectiveness[1] = 200;  //Flamejante
    allElements[2].Effectiveness[2] = 50;  //Aquático
    allElements[2].Effectiveness[3] = 50; //Natural
    allElements[2].Effectiveness[4] = 100; //Elétrico
    allElements[2].Effectiveness[5] = 100; //Misterioso
    allElements[2].Effectiveness[6] = 100; //Luminoso
    allElements[2].Effectiveness[7] = 50; //Gélido
    allElements[2].Effectiveness[8] = 200;  //Mineral
    allElements[2].Effectiveness[9] = 100; //Tóxico
    //------------------//
    allElements[2].ElementalDamageScale = 0;
    allElements[2].SelfElementIndex = 2;
    //------------------//
    strcpy(allElements[2].CurrentHPEffect.Acronym, "HP");
    allElements[2].CurrentHPEffect.Quantity = 0;
    allElements[2].CurrentHPEffect.Timer = 0;
    //------------------//
    strcpy(allElements[2].StatusEffect[0].Acronym, "Def");
    allElements[2].StatusEffect[0].Quantity = -15;
    allElements[2].StatusEffect[0].Timer = 3;
    strcpy(allElements[2].StatusEffect[1].Acronym, "MaD");
    allElements[2].StatusEffect[1].Quantity = -15;
    allElements[2].StatusEffect[1].Timer = 3;
    strcpy(allElements[2].StatusEffect[2].Acronym, "Acc");
    allElements[2].StatusEffect[2].Quantity = 0;
    allElements[2].StatusEffect[2].Timer = 0;
    strcpy(allElements[2].StatusEffect[3].Acronym, "Atk");
    allElements[2].StatusEffect[3].Quantity = 0;
    allElements[2].StatusEffect[3].Timer = 0; 
    strcpy(allElements[2].StatusEffect[4].Acronym, "EAc");
    allElements[2].StatusEffect[4].Quantity = 0;
    allElements[2].StatusEffect[4].Timer = 0;
    strcpy(allElements[2].StatusEffect[5].Acronym, "MaA");
    allElements[2].StatusEffect[5].Quantity = 0;
    allElements[2].StatusEffect[5].Timer = 0;
    strcpy(allElements[2].StatusEffect[6].Acronym, "Spd");
    allElements[2].StatusEffect[6].Quantity = 0;
    allElements[2].StatusEffect[6].Timer = 0;
 
    strcpy(allElements[3].Name, "Natural"); 
    strcpy(allElements[3].Acronym, "Erz");
    allElements[3].Effectiveness[0] = 100; //Comum
    allElements[3].Effectiveness[1] = 50;  //Flamejante
    allElements[3].Effectiveness[2] = 200;  //Aquático
    allElements[3].Effectiveness[3] = 50; //Natural
    allElements[3].Effectiveness[4] = 100; //Elétrico
    allElements[3].Effectiveness[5] = 100; //Misterioso
    allElements[3].Effectiveness[6] = 100; //Luminoso
    allElements[3].Effectiveness[7] = 50; //Gélido
    allElements[3].Effectiveness[8] = 200;  //Mineral
    allElements[3].Effectiveness[9] = 50; //Tóxico
    //------------------//
    allElements[3].ElementalDamageScale = 0;
    allElements[3].SelfElementIndex = 3;
    //------------------//
    strcpy(allElements[3].CurrentHPEffect.Acronym, "HP");
    allElements[3].CurrentHPEffect.Quantity = 0;
    allElements[3].CurrentHPEffect.Timer = 0;
    //------------------//
    strcpy(allElements[3].StatusEffect[0].Acronym, "Def");
    allElements[3].StatusEffect[0].Quantity = 0;
    allElements[3].StatusEffect[0].Timer = 0;
    strcpy(allElements[3].StatusEffect[1].Acronym, "MaD");
    allElements[3].StatusEffect[1].Quantity = 0;
    allElements[3].StatusEffect[1].Timer = 0;
    strcpy(allElements[3].StatusEffect[2].Acronym, "Acc");
    allElements[3].StatusEffect[2].Quantity = 0;
    allElements[3].StatusEffect[2].Timer = 0;
    strcpy(allElements[3].StatusEffect[3].Acronym, "Atk");
    allElements[3].StatusEffect[3].Quantity = -10;
    allElements[3].StatusEffect[3].Timer = 4; 
    strcpy(allElements[3].StatusEffect[4].Acronym, "EAc");
    allElements[3].StatusEffect[4].Quantity = 0;
    allElements[3].StatusEffect[4].Timer = 0;
    strcpy(allElements[3].StatusEffect[5].Acronym, "MaA");
    allElements[3].StatusEffect[5].Quantity = -10;
    allElements[3].StatusEffect[5].Timer = 0;
    strcpy(allElements[3].StatusEffect[6].Acronym, "Spd");
    allElements[3].StatusEffect[6].Quantity = -15;
    allElements[3].StatusEffect[6].Timer = 3;

    strcpy(allElements[4].Name, "Elétrico"); 
    strcpy(allElements[4].Acronym, "Prz");
    allElements[4].Effectiveness[0] = 100; //Comum
    allElements[4].Effectiveness[1] = 100;  //Flamejante
    allElements[4].Effectiveness[2] = 200;  //Aquático
    allElements[4].Effectiveness[3] = 50; //Natural
    allElements[4].Effectiveness[4] = 50; //Elétrico
    allElements[4].Effectiveness[5] = 100; //Misterioso
    allElements[4].Effectiveness[6] = 100; //Luminoso
    allElements[4].Effectiveness[7] = 50; //Gélido
    allElements[4].Effectiveness[8] = 50;  //Mineral
    allElements[4].Effectiveness[9] = 100; //Tóxico
    //------------------//
    allElements[4].ElementalDamageScale = -15;
    allElements[4].SelfElementIndex = 4;
    //------------------//
    strcpy(allElements[4].CurrentHPEffect.Acronym, "HP");
    allElements[4].CurrentHPEffect.Quantity = -5;
    allElements[4].CurrentHPEffect.Timer = 3;
    //------------------//
    strcpy(allElements[4].StatusEffect[0].Acronym, "Def");
    allElements[4].StatusEffect[0].Quantity = 0;
    allElements[4].StatusEffect[0].Timer = 0;
    strcpy(allElements[4].StatusEffect[1].Acronym, "MaD");
    allElements[4].StatusEffect[1].Quantity = 0;
    allElements[4].StatusEffect[1].Timer = 0;
    strcpy(allElements[4].StatusEffect[2].Acronym, "Acc");
    allElements[4].StatusEffect[2].Quantity = 0;
    allElements[4].StatusEffect[2].Timer = 0;
    strcpy(allElements[4].StatusEffect[3].Acronym, "Atk");
    allElements[4].StatusEffect[3].Quantity = 0;
    allElements[4].StatusEffect[3].Timer = 0; 
    strcpy(allElements[4].StatusEffect[4].Acronym, "EAc");
    allElements[4].StatusEffect[4].Quantity = 0;
    allElements[4].StatusEffect[4].Timer = 0;
    strcpy(allElements[4].StatusEffect[5].Acronym, "MaA");
    allElements[4].StatusEffect[5].Quantity = 0;
    allElements[4].StatusEffect[5].Timer = 0;
    strcpy(allElements[4].StatusEffect[6].Acronym, "Spd");
    allElements[4].StatusEffect[6].Quantity = -15;
    allElements[4].StatusEffect[6].Timer = 3;

    strcpy(allElements[5].Name, "Misterioso"); 
    strcpy(allElements[5].Acronym, "Asb");
    allElements[5].Effectiveness[0] = 50; //Comum
    allElements[5].Effectiveness[1] = 100;  //Flamejante
    allElements[5].Effectiveness[2] = 100;  //Aquático
    allElements[5].Effectiveness[3] = 100; //Natural
    allElements[5].Effectiveness[4] = 100; //Elétrico
    allElements[5].Effectiveness[5] = 200; //Misterioso
    allElements[5].Effectiveness[6] = 200; //Luminoso
    allElements[5].Effectiveness[7] = 100; //Gélido
    allElements[5].Effectiveness[8] = 100;  //Mineral
    allElements[5].Effectiveness[9] = 100; //Tóxico
    //------------------//
    allElements[5].ElementalDamageScale = -10;
    allElements[5].SelfElementIndex = 5;
    //------------------//
    strcpy(allElements[5].CurrentHPEffect.Acronym, "HP");
    allElements[5].CurrentHPEffect.Quantity = -4;
    allElements[5].CurrentHPEffect.Timer = 5;
    //------------------//
    strcpy(allElements[5].StatusEffect[0].Acronym, "Def");
    allElements[5].StatusEffect[0].Quantity = 0;
    allElements[5].StatusEffect[0].Timer = 0;
    strcpy(allElements[5].StatusEffect[1].Acronym, "MaD");
    allElements[5].StatusEffect[1].Quantity = 0;
    allElements[5].StatusEffect[1].Timer = 0;
    strcpy(allElements[5].StatusEffect[2].Acronym, "Acc");
    allElements[5].StatusEffect[2].Quantity = 0;
    allElements[5].StatusEffect[2].Timer = 0;
    strcpy(allElements[5].StatusEffect[3].Acronym, "Atk");
    allElements[5].StatusEffect[3].Quantity = 0;
    allElements[5].StatusEffect[3].Timer = 0; 
    strcpy(allElements[5].StatusEffect[4].Acronym, "EAc");
    allElements[5].StatusEffect[4].Quantity = 0;
    allElements[5].StatusEffect[4].Timer = 0;
    strcpy(allElements[5].StatusEffect[5].Acronym, "MaA");
    allElements[5].StatusEffect[5].Quantity = -15;
    allElements[5].StatusEffect[5].Timer = 5;
    strcpy(allElements[5].StatusEffect[6].Acronym, "Spd");
    allElements[5].StatusEffect[6].Quantity = 0;
    allElements[5].StatusEffect[6].Timer = 0;

    strcpy(allElements[6].Name, "Luminoso"); 
    strcpy(allElements[6].Acronym, "Clr");
    allElements[6].Effectiveness[0] = 100; //Comum
    allElements[6].Effectiveness[1] = 50;  //Flamejante
    allElements[6].Effectiveness[2] = 100;  //Aquático
    allElements[6].Effectiveness[3] = 100; //Natural
    allElements[6].Effectiveness[4] = 50; //Elétrico
    allElements[6].Effectiveness[5] = 200; //Misterioso
    allElements[6].Effectiveness[6] = 100; //Luminoso
    allElements[6].Effectiveness[7] = 100; //Gélido
    allElements[6].Effectiveness[8] = 100;  //Mineral
    allElements[6].Effectiveness[9] = 100; //Tóxico
    //------------------//
    allElements[6].ElementalDamageScale = 0;
    allElements[6].SelfElementIndex = 6;
    //------------------//
    strcpy(allElements[6].CurrentHPEffect.Acronym, "HP");
    allElements[6].CurrentHPEffect.Quantity = 0;
    allElements[6].CurrentHPEffect.Timer = 0;
    //------------------//
    strcpy(allElements[6].StatusEffect[0].Acronym, "Def");
    allElements[6].StatusEffect[0].Quantity = 0;
    allElements[6].StatusEffect[0].Timer = 0;
    strcpy(allElements[6].StatusEffect[1].Acronym, "MaD");
    allElements[6].StatusEffect[1].Quantity = 0;
    allElements[6].StatusEffect[1].Timer = 0;
    strcpy(allElements[6].StatusEffect[2].Acronym, "Acc");
    allElements[6].StatusEffect[2].Quantity = -20;
    allElements[6].StatusEffect[2].Timer = 2;
    strcpy(allElements[6].StatusEffect[3].Acronym, "Atk");
    allElements[6].StatusEffect[3].Quantity = 0;
    allElements[6].StatusEffect[3].Timer = 0; 
    strcpy(allElements[6].StatusEffect[4].Acronym, "EAc");
    allElements[6].StatusEffect[4].Quantity = -20;
    allElements[6].StatusEffect[4].Timer = 2;
    strcpy(allElements[6].StatusEffect[5].Acronym, "MaA");
    allElements[6].StatusEffect[5].Quantity = -15;
    allElements[6].StatusEffect[5].Timer = 5;
    strcpy(allElements[6].StatusEffect[6].Acronym, "Spd");
    allElements[6].StatusEffect[6].Quantity = 0;
    allElements[6].StatusEffect[6].Timer = 0;

    strcpy(allElements[7].Name, "Gélido"); 
    strcpy(allElements[7].Acronym, "Cal");
    allElements[7].Effectiveness[0] = 100; //Comum
    allElements[7].Effectiveness[1] = 50;  //Flamejante
    allElements[7].Effectiveness[2] = 50;  //Aquático
    allElements[7].Effectiveness[3] = 200; //Natural
    allElements[7].Effectiveness[4] = 100; //Elétrico
    allElements[7].Effectiveness[5] = 100; //Misterioso
    allElements[7].Effectiveness[6] = 100; //Luminoso
    allElements[7].Effectiveness[7] = 50; //Gélido
    allElements[7].Effectiveness[8] = 200;  //Mineral
    allElements[7].Effectiveness[9] = 100; //Tóxico
    //------------------//
    allElements[7].ElementalDamageScale = -10;
    allElements[7].SelfElementIndex = 7;
    //------------------//
    strcpy(allElements[7].CurrentHPEffect.Acronym, "HP");
    allElements[7].CurrentHPEffect.Quantity = -10;
    allElements[7].CurrentHPEffect.Timer = 2;
    //------------------//
    strcpy(allElements[7].StatusEffect[0].Acronym, "Def");
    allElements[7].StatusEffect[0].Quantity = 0;
    allElements[7].StatusEffect[0].Timer = 0;
    strcpy(allElements[7].StatusEffect[1].Acronym, "MaD");
    allElements[7].StatusEffect[1].Quantity = 0;
    allElements[7].StatusEffect[1].Timer = 0;
    strcpy(allElements[7].StatusEffect[2].Acronym, "Acc");
    allElements[7].StatusEffect[2].Quantity = 0;
    allElements[7].StatusEffect[2].Timer = 0;
    strcpy(allElements[7].StatusEffect[3].Acronym, "Atk");
    allElements[7].StatusEffect[3].Quantity = 0;
    allElements[7].StatusEffect[3].Timer = 0; 
    strcpy(allElements[7].StatusEffect[4].Acronym, "EAc");
    allElements[7].StatusEffect[4].Quantity = 0;
    allElements[7].StatusEffect[4].Timer = 0;
    strcpy(allElements[7].StatusEffect[5].Acronym, "MaA");
    allElements[7].StatusEffect[5].Quantity = 0;
    allElements[7].StatusEffect[5].Timer = 0;
    strcpy(allElements[7].StatusEffect[6].Acronym, "Spd");
    allElements[7].StatusEffect[6].Quantity = -30;
    allElements[7].StatusEffect[6].Timer = 3;

    strcpy(allElements[8].Name, "Mineral"); 
    strcpy(allElements[8].Acronym, "Sot");
    allElements[8].Effectiveness[0] = 100; //Comum
    allElements[8].Effectiveness[1] = 200;  //Flamejante
    allElements[8].Effectiveness[2] = 100;  //Aquático
    allElements[8].Effectiveness[3] = 50; //Natural
    allElements[8].Effectiveness[4] = 200; //Elétrico
    allElements[8].Effectiveness[5] = 100; //Misterioso
    allElements[8].Effectiveness[6] = 100; //Luminoso
    allElements[8].Effectiveness[7] = 50; //Gélido
    allElements[8].Effectiveness[8] = 100;  //Mineral
    allElements[8].Effectiveness[9] = 200; //Tóxico
    //------------------//
    allElements[8].ElementalDamageScale = 0;
    allElements[8].SelfElementIndex = 8;
    //------------------//
    strcpy(allElements[8].CurrentHPEffect.Acronym, "HP");
    allElements[8].CurrentHPEffect.Quantity = 0;
    allElements[8].CurrentHPEffect.Timer = 0;
    //------------------//
    strcpy(allElements[8].StatusEffect[0].Acronym, "Def");
    allElements[8].StatusEffect[0].Quantity = 0;
    allElements[8].StatusEffect[0].Timer = 0;
    strcpy(allElements[8].StatusEffect[1].Acronym, "MaD");
    allElements[8].StatusEffect[1].Quantity = 0;
    allElements[8].StatusEffect[1].Timer = 0;
    strcpy(allElements[8].StatusEffect[2].Acronym, "Acc");
    allElements[8].StatusEffect[2].Quantity = 0;
    allElements[8].StatusEffect[2].Timer = 0;
    strcpy(allElements[8].StatusEffect[3].Acronym, "Atk");
    allElements[8].StatusEffect[3].Quantity = 0;
    allElements[8].StatusEffect[3].Timer = 0; 
    strcpy(allElements[8].StatusEffect[4].Acronym, "EAc");
    allElements[8].StatusEffect[4].Quantity = 0;
    allElements[8].StatusEffect[4].Timer = 0;
    strcpy(allElements[8].StatusEffect[5].Acronym, "MaA");
    allElements[8].StatusEffect[5].Quantity = 0;
    allElements[8].StatusEffect[5].Timer = 0;
    strcpy(allElements[8].StatusEffect[6].Acronym, "Spd");
    allElements[8].StatusEffect[6].Quantity = -30;
    allElements[8].StatusEffect[6].Timer = 2;

    strcpy(allElements[9].Name, "Venenoso"); 
    strcpy(allElements[9].Acronym, "Ven");
    allElements[9].Effectiveness[0] = 100; //Comum
    allElements[9].Effectiveness[1] = 100;  //Flamejante
    allElements[9].Effectiveness[2] = 100;  //Aquático
    allElements[9].Effectiveness[3] = 200; //Natural
    allElements[9].Effectiveness[4] = 100; //Elétrico
    allElements[9].Effectiveness[5] = 100; //Misterioso
    allElements[9].Effectiveness[6] = 100; //Luminoso
    allElements[9].Effectiveness[7] = 100; //Gélido
    allElements[9].Effectiveness[8] = 50; //Mineral
    allElements[9].Effectiveness[9] = 50; //Tóxico
    //------------------//
    allElements[9].ElementalDamageScale = -30;
    allElements[9].SelfElementIndex = 9;
    //------------------//
    strcpy(allElements[9].CurrentHPEffect.Acronym, "HP");
    allElements[9].CurrentHPEffect.Quantity = -15;
    allElements[9].CurrentHPEffect.Timer = 5;
    //------------------//
    strcpy(allElements[9].StatusEffect[0].Acronym, "Def");
    allElements[9].StatusEffect[0].Quantity = 0;
    allElements[9].StatusEffect[0].Timer = 0;
    strcpy(allElements[9].StatusEffect[1].Acronym, "MaD");
    allElements[9].StatusEffect[1].Quantity = 0;
    allElements[9].StatusEffect[1].Timer = 0;
    strcpy(allElements[9].StatusEffect[2].Acronym, "Acc");
    allElements[9].StatusEffect[2].Quantity = 0;
    allElements[9].StatusEffect[2].Timer = 0;
    strcpy(allElements[9].StatusEffect[3].Acronym, "Atk");
    allElements[9].StatusEffect[3].Quantity = 0;
    allElements[9].StatusEffect[3].Timer = 0; 
    strcpy(allElements[9].StatusEffect[4].Acronym, "EAc");
    allElements[9].StatusEffect[4].Quantity = 0;
    allElements[9].StatusEffect[4].Timer = 0;
    strcpy(allElements[9].StatusEffect[5].Acronym, "MaA");
    allElements[9].StatusEffect[5].Quantity = 0;
    allElements[9].StatusEffect[5].Timer = 0;
    strcpy(allElements[9].StatusEffect[6].Acronym, "Spd");
    allElements[9].StatusEffect[6].Quantity = 0;
    allElements[9].StatusEffect[6].Timer = 0;
    SaveElements(allElements, elements);
    */

    //Itens//
    


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
        perror("Falha ao abrir \"pikomoms\"");
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


//define pikomons

/*
    char descfds[3][255];
    strcpy(descfds[0], "");
    strcpy(descfds[1], "");
    strcpy(descfds[2], "");
    char icoImg[7][25];
    strcpy(icoImg[0], "####__________####");
    strcpy(icoImg[1], "###|          |###");
    strcpy(icoImg[2], "###| .      . |###");
    strcpy(icoImg[3], "###|   -__-   |###");
    strcpy(icoImg[4], "###|          |###");
    strcpy(icoImg[5], "###|          |###");
    strcpy(icoImg[6], "###|__________|###");

    AddPikomon(&pPikomons, &dataQuantities, "Moldew", allElements[0], icoImg, "", descfds, 5, 25,25,25,100,25,100,25,35);

    char icoImg1[7][25];
    strcpy(icoImg1[0], "   ===      ===   ");
    strcpy(icoImg1[1], "  [###]    [###]  ");
    strcpy(icoImg1[2], " |#####====#####| ");
    strcpy(icoImg1[3], "  [##0######0##]  ");
    strcpy(icoImg1[4], "   \\__\\####/__/   ");
    strcpy(icoImg1[5], "       \\##/       ");
    strcpy(icoImg1[6], "        \\/        ");

    AddPikomon(&pPikomons, &dataQuantities, "Ratton", allElements[0], icoImg1, "", descfds, 5, 50,20,35,100,30,100,20,65);

    char icoImg2[7][25];
    strcpy(icoImg2[0], "   /\\        /\\  ");
    strcpy(icoImg2[1], "  /##\\      /##\\  ");
    strcpy(icoImg2[2], " [#####======###\\ ");
    strcpy(icoImg2[3], "[######O######O##]");
    strcpy(icoImg2[4], "[|\\|\\|\\(___/\\_)##|");
    strcpy(icoImg2[5], "/====/##########| ");
    strcpy(icoImg2[6], "====/##########|  ");

    AddPikomon(&pPikomons, &dataQuantities, "Bichano", allElements[0], icoImg2, "", descfds, 5, 50,50,35,100,50,100,20,60);

    char icoImg3[7][25];
    strcpy(icoImg3[0], "        /\\        ");
    strcpy(icoImg3[1], "       /##\\       ");
    strcpy(icoImg3[2], "      /####\\      ");
    strcpy(icoImg3[3], "==================");
    strcpy(icoImg3[4], "     |*####*|     ");
    strcpy(icoImg3[5], "     v^^ww^^v     ");
    strcpy(icoImg3[6], "      wwvvww      ");

    AddPikomon(&pPikomons, &dataQuantities, "Mago", allElements[5], icoImg3, "", descfds, 5, 60,45,50,100,15,100,75,35);

    char icoImg4[7][25];
    strcpy(icoImg4[0], "   ___=====__     ");
    strcpy(icoImg4[1], " </##########\\__  ");
    strcpy(icoImg4[2], "</#######ò####*#} ");
    strcpy(icoImg4[3], "<#########/vvvvv  ");
    strcpy(icoImg4[4], "/#########\\^^^^^} ");
    strcpy(icoImg4[5], "#########/‾‾‾‾‾‾  ");
    strcpy(icoImg4[6], "########|         ");

    AddPikomon(&pPikomons, &dataQuantities, "Calangora", allElements[9], icoImg4, "", descfds, 5, 60,45,20,100,75,100,10,45);

    char icoImg5[7][25];
    strcpy(icoImg5[0], "######__/\\__######");
    strcpy(icoImg5[1], "#####[      ]#####");
    strcpy(icoImg5[2], "####|<>    <>|####");
    strcpy(icoImg5[3], "####| |====| |####");
    strcpy(icoImg5[4], "#####[______]#####");
    strcpy(icoImg5[5], "####/|010101|\\####");
    strcpy(icoImg5[6], "###| |101010| |###");

    AddPikomon(&pPikomons, &dataQuantities, "Roboto", allElements[4], icoImg5, "", descfds, 5, 60,80,80,100,20,100,20,35);

    char icoImg6[7][25];
    strcpy(icoImg6[0], "########^#######/|");
    strcpy(icoImg6[1], "#####__/#\\#####/ }");
    strcpy(icoImg6[2], "###==o    ==__/ }#");
    strcpy(icoImg6[3], "##>           <###");
    strcpy(icoImg6[4], "###===\\|__==‾‾\\###");
    strcpy(icoImg6[5], "###############‾##");
    strcpy(icoImg6[6], "##################");

    AddPikomon(&pPikomons, &dataQuantities, "Peixe Secreto", allElements[2], icoImg6, "", descfds, 5, 50,45,45,100,45,100,45,45);

    char icoImg7[7][25];
    strcpy(icoImg7[0], "##__##_===_##__###");
    strcpy(icoImg7[1], "#{  }{ o o }{  }##");
    strcpy(icoImg7[2], "#[. \\|  ^  |/; ]>#");
    strcpy(icoImg7[3], "#<\\__    .  __/###");
    strcpy(icoImg7[4], "###v#|v    |#,####");
    strcpy(icoImg7[5], "####,|   <>|######");
    strcpy(icoImg7[6], "#####|,    |>#####");

    AddPikomon(&pPikomons, &dataQuantities, "Cacto", allElements[3], icoImg7, "", descfds, 5, 70,75,60,100,10,100,65,35);

    char icoImg8[7][25];
    strcpy(icoImg8[0], " ==_   ^==^   _== ");
    strcpy(icoImg8[1], "{WVW{ {*\\/*} }WVW}");
    strcpy(icoImg8[2], "|{\\|v\\_/##\\_/v|/}|");
    strcpy(icoImg8[3], " [v\\V\\{####}/V/v] ");
    strcpy(icoImg8[4], "  \\V\\v\\{##}/v/V/  ");
    strcpy(icoImg8[5], "      /WwwW\\      ");
    strcpy(icoImg8[6], "    <v/V/\\V\\v>    ");

    AddPikomon(&pPikomons, &dataQuantities, "Fênix", allElements[1], icoImg8, "", descfds, 5, 50,20,20,100,70,100,75,65);

    char icoImg9[7][25];
    strcpy(icoImg9[0], "#######====#######");
    strcpy(icoImg9[1], "######{wVVw}######");
    strcpy(icoImg9[2], "#####[o ^^ o]#####");
    strcpy(icoImg9[3], "####[ T{  }T ]####");
    strcpy(icoImg9[4], "###{  J || L  }###");
    strcpy(icoImg9[5], "####{  /LJ\\  }####");
    strcpy(icoImg9[6], "####{m}u##u{m}####");

    AddPikomon(&pPikomons, &dataQuantities, "Mamute", allElements[7], icoImg9, "", descfds, 5, 80,20,20,100,60,100,20,45);

    char icoImg10[7][25];
    strcpy(icoImg10[0], "######_===_#######");
    strcpy(icoImg10[1], "#####/0 o /#######");
    strcpy(icoImg10[2], "####[ ^ _[######oO");
    strcpy(icoImg10[3], "#{‾‾ {__|#########");
    strcpy(icoImg10[4], "##‾‾{   {######O##");
    strcpy(icoImg10[5], "#####=_  \\###o####");
    strcpy(icoImg10[6], "########\\_\\~######");

    AddPikomon(&pPikomons, &dataQuantities, "Vulto", allElements[5], icoImg10, "", descfds, 5, 50,80,10,100,80,100,10,45);

    char icoImg11[7][25];
    strcpy(icoImg11[0], "  \\\\          //  ");
    strcpy(icoImg11[1], "   \\\\ _====_ //   ");
    strcpy(icoImg11[2], "    \\{O=O=O=}/    ");
    strcpy(icoImg11[3], "===={O=O=O=O=}====");
    strcpy(icoImg11[4], "    /{O=O=O=}\\    ");
    strcpy(icoImg11[5], "   // ‾====‾ \\\\   ");
    strcpy(icoImg11[6], "  //          \\\\  ");

    AddPikomon(&pPikomons, &dataQuantities, "AlienSolar", allElements[6], icoImg11, "", descfds, 5, 70,40,80,100,10,100,75,35);

    char icoImg12[7][25];
    strcpy(icoImg12[0], "#######/\\#########");
    strcpy(icoImg12[1], "######/__\\########");
    strcpy(icoImg12[2], "#####[T T ]#######");
    strcpy(icoImg12[3], "####|  ^  o|######");
    strcpy(icoImg12[4], "###|.   <>  |#####");
    strcpy(icoImg12[5], "#.[  ^    *  ]#.##");
    strcpy(icoImg12[6], "==================");

    AddPikomon(&pPikomons, &dataQuantities, "Speleotema", allElements[8], icoImg12, "", descfds, 5, 50,100,30,100,40,100,35,35);

    SavePikomons(pPikomons, dataQuantities.Pikomon, pikomoms);
    SaveDataQuantity(dataQuantities, dataQuantity);*/

    //Itens//
    char DescAtiva[3][255];
    strcpy(DescAtiva[0], "");
    strcpy(DescAtiva[1], "");
    strcpy(DescAtiva[2], "");

    char descItens[3][255];
    strcpy(descItens[0], "Uma poção que cura 10 de HP.");
    strcpy(descItens[1], "");
    strcpy(descItens[2], "");

    Effect Heal;
    strcpy(Heal.Acronym, "HP");
    Heal.Quantity = 10;
    Heal.Timer = 1;
    Effect Stats[8];
    strcpy(Stats[0].Acronym, "HP");
    strcpy(Stats[0].Acronym, "HP");
    strcpy(Stats[1].Acronym, "Def");
    strcpy(Stats[2].Acronym, "MaD");
    strcpy(Stats[3].Acronym, "Acc");
    strcpy(Stats[4].Acronym, "Atk");
    strcpy(Stats[5].Acronym, "MAc");
    strcpy(Stats[6].Acronym, "MaA");
    strcpy(Stats[7].Acronym, "Spd");
    Stats[0].Quantity = 0;
    Stats[0].Timer = 0;
    Stats[1].Quantity = 0;
    Stats[1].Timer = 0;
    Stats[2].Quantity = 0;
    Stats[2].Timer = 0;
    Stats[3].Quantity = 0;
    Stats[3].Timer = 0;
    Stats[4].Quantity = 0;
    Stats[4].Timer = 0;
    Stats[5].Quantity = 0;
    Stats[5].Timer = 0;
    Stats[6].Quantity = 0;
    Stats[6].Timer = 0;
    Stats[7].Quantity = 0;
    Stats[7].Timer = 0;

    AddItem(&pItems, &dataQuantities, "Poção Básica", "Cura", descItens, "", DescAtiva, 5, false, 'S', Heal, 'S', 100, Stats);

    char descItens1[3][255];
    strcpy(descItens1[0], "Uma poção que cura 25 de HP.");
    strcpy(descItens1[1], "");
    strcpy(descItens1[2], "");

    Effect Heal1;
    strcpy(Heal1.Acronym, "HP");
    Heal1.Quantity = 25;
    Heal1.Timer = 1;
    Effect Stats1[8];
    strcpy(Stats1[0].Acronym, "HP");
    strcpy(Stats1[1].Acronym, "Def");
    strcpy(Stats1[2].Acronym, "MaD");
    strcpy(Stats1[3].Acronym, "Acc");
    strcpy(Stats1[4].Acronym, "Atk");
    strcpy(Stats1[5].Acronym, "MAc");
    strcpy(Stats1[6].Acronym, "MaA");
    strcpy(Stats1[7].Acronym, "Spd");
    Stats1[0].Quantity = 0;
    Stats1[0].Timer = 0;
    Stats1[1].Quantity = 0;
    Stats1[1].Timer = 0;
    Stats1[2].Quantity = 0;
    Stats1[2].Timer = 0;
    Stats1[3].Quantity = 0;
    Stats1[3].Timer = 0;
    Stats1[4].Quantity = 0;
    Stats1[4].Timer = 0;
    Stats1[5].Quantity = 0;
    Stats1[5].Timer = 0;
    Stats1[6].Quantity = 0;
    Stats1[6].Timer = 0;
    Stats1[7].Quantity = 0;
    Stats1[7].Timer = 0;

    AddItem(&pItems, &dataQuantities, "Poção Média", "Cura", descItens1, "", DescAtiva, 5, false, 'S', Heal1, 'S', 100, Stats1);

    char descItens2[3][255];
    strcpy(descItens2[0], "Uma poção que cura 50 de HP.");
    strcpy(descItens2[1], "");
    strcpy(descItens2[2], "");

    Effect Heal2;
    strcpy(Heal2.Acronym, "HP");
    Heal2.Quantity = 50;
    Heal2.Timer = 1;
    Effect Stats2[8];
    strcpy(Stats2[0].Acronym, "HP");
    strcpy(Stats2[1].Acronym, "Def");
    strcpy(Stats2[2].Acronym, "MaD");
    strcpy(Stats2[3].Acronym, "Acc");
    strcpy(Stats2[4].Acronym, "Atk");
    strcpy(Stats2[5].Acronym, "MAc");
    strcpy(Stats2[6].Acronym, "MaA");
    strcpy(Stats2[7].Acronym, "Spd");
    Stats2[0].Quantity = 0;
    Stats2[0].Timer = 0;
    Stats2[1].Quantity = 0;
    Stats2[1].Timer = 0;
    Stats2[2].Quantity = 0;
    Stats2[2].Timer = 0;
    Stats2[3].Quantity = 0;
    Stats2[3].Timer = 0;
    Stats2[4].Quantity = 0;
    Stats2[4].Timer = 0;
    Stats2[5].Quantity = 0;
    Stats2[5].Timer = 0;
    Stats2[6].Quantity = 0;
    Stats2[6].Timer = 0;
    Stats2[7].Quantity = 0;
    Stats2[7].Timer = 0;

    AddItem(&pItems, &dataQuantities, "Poção Máxima", "Cura", descItens1, "", DescAtiva, 5, false, 'S', Heal2, 'S', 100, Stats2);

    char descItens3[3][255];
    strcpy(descItens3[0], "Um dispositivo que cura todos");
    strcpy(descItens3[1], "os Pikomons em campo, curando");
    strcpy(descItens3[2], "50 de HP ao longo de 3 turnos.");

    Effect Heal3;
    strcpy(Heal3.Acronym, "HP");
    Heal3.Quantity = 60;
    Heal3.Timer = 3;
    Effect Stats3[8];
    strcpy(Stats3[0].Acronym, "HP");
    strcpy(Stats3[1].Acronym, "Def");
    strcpy(Stats3[2].Acronym, "MaD");
    strcpy(Stats3[3].Acronym, "Acc");
    strcpy(Stats3[4].Acronym, "Atk");
    strcpy(Stats3[5].Acronym, "MAc");
    strcpy(Stats3[6].Acronym, "MaA");
    strcpy(Stats3[7].Acronym, "Spd");
    Stats3[0].Quantity = 0;
    Stats3[0].Timer = 0;
    Stats3[1].Quantity = 0;
    Stats3[1].Timer = 0;
    Stats3[2].Quantity = 0;
    Stats3[2].Timer = 0;
    Stats3[3].Quantity = 0;
    Stats3[3].Timer = 0;
    Stats3[4].Quantity = 0;
    Stats3[4].Timer = 0;
    Stats3[5].Quantity = 0;
    Stats3[5].Timer = 0;
    Stats3[6].Quantity = 0;
    Stats3[6].Timer = 0;
    Stats3[7].Quantity = 0;
    Stats3[7].Timer = 0;

    AddItem(&pItems, &dataQuantities, "Granada de Cura", "Cura", descItens3, "", DescAtiva, 5, false, 'B', Heal3, 'B', 100, Stats3);

    char descItens4[3][255];
    strcpy(descItens4[0], "Uma seringa que coloca o oponente");
    strcpy(descItens4[1], "em um estado de Sono por 3 turnos.");
    strcpy(descItens4[2], "");

    Effect Sleep;
    strcpy(Sleep.Acronym, "HP");
    Sleep.Quantity = 0;
    Sleep.Timer = 0;
    Effect SleepStats[8];
    strcpy(SleepStats[0].Acronym, "HP");
    strcpy(SleepStats[0].Acronym, "HP");
    strcpy(SleepStats[1].Acronym, "Def");
    strcpy(SleepStats[2].Acronym, "MaD");
    strcpy(SleepStats[3].Acronym, "Acc");
    strcpy(SleepStats[4].Acronym, "Atk");
    strcpy(SleepStats[5].Acronym, "MAc");
    strcpy(SleepStats[6].Acronym, "MaA");
    strcpy(SleepStats[7].Acronym, "Spd");
    SleepStats[0].Quantity = 0;
    SleepStats[0].Timer = 0;
    SleepStats[1].Quantity = 0;
    SleepStats[1].Timer = 0;
    SleepStats[2].Quantity = 0;
    SleepStats[2].Timer = 0;
    SleepStats[3].Quantity = 0;
    SleepStats[3].Timer = 0;
    SleepStats[4].Quantity = 0;
    SleepStats[4].Timer = 0;
    SleepStats[5].Quantity = 0;
    SleepStats[5].Timer = 0;
    SleepStats[6].Quantity = 0;
    SleepStats[6].Timer = 0;
    SleepStats[7].Quantity = -100;
    SleepStats[7].Timer = 3;

    AddItem(&pItems, &dataQuantities, "Sonífero", "Condições", descItens4, "", DescAtiva, 30, false, 'E', Sleep, 'E', 100, SleepStats);

    char descItens5[3][255];
    strcpy(descItens5[0], "Um pequeno dispositivo que");
    strcpy(descItens5[1], "diminui as Precisões dos dois");
    strcpy(descItens5[2], "Pikomons em campo em 25.");

    Effect Smoke;
    strcpy(Smoke.Acronym, "HP");
    Smoke.Quantity = 0;
    Smoke.Timer = 0;
    Effect SmokeStats[8];
    strcpy(SmokeStats[0].Acronym, "HP");
    strcpy(SmokeStats[1].Acronym, "Def");
    strcpy(SmokeStats[2].Acronym, "MaD");
    strcpy(SmokeStats[3].Acronym, "Acc");
    strcpy(SmokeStats[4].Acronym, "Atk");
    strcpy(SmokeStats[5].Acronym, "MAc");
    strcpy(SmokeStats[6].Acronym, "MaA");
    strcpy(SmokeStats[7].Acronym, "Spd");
    SmokeStats[0].Quantity = 0;
    SmokeStats[0].Timer = 0;
    SmokeStats[1].Quantity = 0;
    SmokeStats[1].Timer = 0;
    SmokeStats[2].Quantity = 0;
    SmokeStats[2].Timer = 0;
    SmokeStats[3].Quantity = -25;
    SmokeStats[3].Timer = 3;
    SmokeStats[4].Quantity = 0;
    SmokeStats[4].Timer = 0;
    SmokeStats[5].Quantity = -25;
    SmokeStats[5].Timer = 3;
    SmokeStats[6].Quantity = 0;
    SmokeStats[6].Timer = 0;
    SmokeStats[7].Quantity = 0;
    SmokeStats[7].Timer = 0;

    AddItem(&pItems, &dataQuantities, "Bomba de Fumaça", "Atributos", descItens5, "", DescAtiva, 5, false, 'B', Smoke, 'B', 100, SmokeStats);

    char descItens6[3][255];
    strcpy(descItens6[0], "Uma bebida que aumenta a Velocidade");
    strcpy(descItens6[1], "do seu Pikomon em 20.");
    strcpy(descItens6[2], "");

    Effect Drink;
    strcpy(Drink.Acronym, "HP");
    Drink.Quantity = 0;
    Drink.Timer = 0;
    Effect DrinkStats[8];
    strcpy(DrinkStats[0].Acronym, "HP");
    strcpy(DrinkStats[1].Acronym, "Def");
    strcpy(DrinkStats[2].Acronym, "MaD");
    strcpy(DrinkStats[3].Acronym, "Acc");
    strcpy(DrinkStats[4].Acronym, "Atk");
    strcpy(DrinkStats[5].Acronym, "MAc");
    strcpy(DrinkStats[6].Acronym, "MaA");
    strcpy(DrinkStats[7].Acronym, "Spd");
    DrinkStats[0].Quantity = 0;
    DrinkStats[0].Timer = 0;
    DrinkStats[1].Quantity = 0;
    DrinkStats[1].Timer = 0;
    DrinkStats[2].Quantity = 0;
    DrinkStats[2].Timer = 0;
    DrinkStats[3].Quantity = 0;
    DrinkStats[3].Timer = 0;
    DrinkStats[4].Quantity = 0;
    DrinkStats[4].Timer = 0;
    DrinkStats[5].Quantity = 0;
    DrinkStats[5].Timer = 0;
    DrinkStats[6].Quantity = 0;
    DrinkStats[6].Timer = 0;
    DrinkStats[7].Quantity = 20;
    DrinkStats[7].Timer = 3;

    AddItem(&pItems, &dataQuantities, "Energético", "Atributos", descItens6, "", DescAtiva, 5, false, 'S', Drink, 'S', 100, DrinkStats);

    char descItens7[3][255];
    strcpy(descItens7[0], "Uma bebida que aumenta o Ataque");
    strcpy(descItens7[1], "Mágico do seu Pikomon em 20.");
    strcpy(descItens7[2], "");

    Effect Coffee;
    strcpy(Coffee.Acronym, "HP");
    Coffee.Quantity = 0;
    Coffee.Timer = 0;
    Effect CoffeeStats[8];
    strcpy(CoffeeStats[0].Acronym, "HP");
    strcpy(CoffeeStats[1].Acronym, "Def");
    strcpy(CoffeeStats[2].Acronym, "MaD");
    strcpy(CoffeeStats[3].Acronym, "Acc");
    strcpy(CoffeeStats[4].Acronym, "Atk");
    strcpy(CoffeeStats[5].Acronym, "MAc");
    strcpy(CoffeeStats[6].Acronym, "MaA");
    strcpy(CoffeeStats[7].Acronym, "Spd");
    CoffeeStats[0].Quantity = 0;
    CoffeeStats[0].Timer = 0;
    CoffeeStats[1].Quantity = 0;
    CoffeeStats[1].Timer = 0;
    CoffeeStats[2].Quantity = 0;
    CoffeeStats[2].Timer = 0;
    CoffeeStats[3].Quantity = 0;
    CoffeeStats[3].Timer = 0;
    CoffeeStats[4].Quantity = 0;
    CoffeeStats[4].Timer = 0;
    CoffeeStats[5].Quantity = 0;
    CoffeeStats[5].Timer = 0;
    CoffeeStats[6].Quantity = 20;
    CoffeeStats[6].Timer = 3;
    CoffeeStats[7].Quantity = 0;
    CoffeeStats[7].Timer = 0;

    AddItem(&pItems, &dataQuantities, "Bebida Cafeinada", "Atributos", descItens7, "", DescAtiva, 5, false, 'S', Coffee, 'S', 100, CoffeeStats);

    char descItens8[3][255];
    strcpy(descItens8[0], "Uma armadura que aumenta a");
    strcpy(descItens8[1], "Defesa do seu Pikomon em 20.");
    strcpy(descItens8[2], "");

    Effect Armor;
    strcpy(Armor.Acronym, "HP");
    Armor.Quantity = 0;
    Armor.Timer = 0;
    Effect ArmorStats[8];
    strcpy(ArmorStats[0].Acronym, "HP");
    strcpy(ArmorStats[1].Acronym, "Def");
    strcpy(ArmorStats[2].Acronym, "MaD");
    strcpy(ArmorStats[3].Acronym, "Acc");
    strcpy(ArmorStats[4].Acronym, "Atk");
    strcpy(ArmorStats[5].Acronym, "MAc");
    strcpy(ArmorStats[6].Acronym, "MaA");
    strcpy(ArmorStats[7].Acronym, "Spd");
    ArmorStats[0].Quantity = 0;
    ArmorStats[0].Timer = 0;
    ArmorStats[1].Quantity = 20;
    ArmorStats[1].Timer = 3;
    ArmorStats[2].Quantity = 0;
    ArmorStats[2].Timer = 0;
    ArmorStats[3].Quantity = 0;
    ArmorStats[3].Timer = 0;
    ArmorStats[4].Quantity = 0;
    ArmorStats[4].Timer = 0;
    ArmorStats[5].Quantity = 0;
    ArmorStats[5].Timer = 0;
    ArmorStats[6].Quantity = 0;
    ArmorStats[6].Timer = 0;
    ArmorStats[7].Quantity = 0;
    ArmorStats[7].Timer = 0;

    AddItem(&pItems, &dataQuantities, "Armadura", "Atributos", descItens8, "", DescAtiva, 5, false, 'S', Armor, 'S', 100, ArmorStats);

    char descItens9[3][255];
    strcpy(descItens9[0], "Um talismã que aumenta a");
    strcpy(descItens9[1], "Defesa Mágica do seu Pikomon");
    strcpy(descItens9[2], "em 20.");

    Effect Talisman;
    strcpy(Armor.Acronym, "HP");
    Talisman.Quantity = 0;
    Talisman.Timer = 0;
    Effect TalismanStats[8];
    strcpy(TalismanStats[0].Acronym, "HP");
    strcpy(TalismanStats[1].Acronym, "Def");
    strcpy(TalismanStats[2].Acronym, "MaD");
    strcpy(TalismanStats[3].Acronym, "Acc");
    strcpy(TalismanStats[4].Acronym, "Atk");
    strcpy(TalismanStats[5].Acronym, "MAc");
    strcpy(TalismanStats[6].Acronym, "MaA");
    strcpy(TalismanStats[7].Acronym, "Spd");
    TalismanStats[0].Quantity = 0;
    TalismanStats[0].Timer = 0;
    TalismanStats[1].Quantity = 0;
    TalismanStats[1].Timer = 0;
    TalismanStats[2].Quantity = 20;
    TalismanStats[2].Timer = 3;
    TalismanStats[3].Quantity = 0;
    TalismanStats[3].Timer = 0;
    TalismanStats[4].Quantity = 0;
    TalismanStats[4].Timer = 0;
    TalismanStats[5].Quantity = 0;
    TalismanStats[5].Timer = 0;
    TalismanStats[6].Quantity = 0;
    TalismanStats[6].Timer = 0;
    TalismanStats[7].Quantity = 0;
    TalismanStats[7].Timer = 0;

    AddItem(&pItems, &dataQuantities, "Talisman", "Atributos", descItens9, "", DescAtiva, 5, false, 'S', Talisman, 'S', 100, TalismanStats);

    SaveItems(pItems, dataQuantities.Item, items);
    SaveDataQuantity(dataQuantities, dataQuantity);

    //Skills//

    char DescAtiva[3][255];
    strcpy(DescAtiva[0], "");
    strcpy(DescAtiva[1], "");
    strcpy(DescAtiva[2], "");

    bool LearnElementsComum[10];
    LearnElementsComum[0] = true;
    LearnElementsComum[1] = true;
    LearnElementsComum[2] = true;
    LearnElementsComum[3] = true;
    LearnElementsComum[4] = true;
    LearnElementsComum[5] = true;
    LearnElementsComum[6] = true;
    LearnElementsComum[7] = true;
    LearnElementsComum[8] = true;
    LearnElementsComum[9] = true;

    bool LearnElementsFire[10];
    LearnElementsFire[0] = true;
    LearnElementsFire[1] = true;
    LearnElementsFire[2] = false;
    LearnElementsFire[3] = false;
    LearnElementsFire[4] = false;
    LearnElementsFire[5] = false;
    LearnElementsFire[6] = true;
    LearnElementsFire[7] = false;
    LearnElementsFire[8] = false;
    LearnElementsFire[9] = false;

    bool LearnElementsWater[10];
    LearnElementsWater[0] = true;
    LearnElementsWater[1] = false;
    LearnElementsWater[2] = true;
    LearnElementsWater[3] = false;
    LearnElementsWater[4] = false;
    LearnElementsWater[5] = false;
    LearnElementsWater[6] = false;
    LearnElementsWater[7] = true;
    LearnElementsWater[8] = false;
    LearnElementsWater[9] = false;

    bool LearnElementsPlant[10];
    LearnElementsPlant[0] = true;
    LearnElementsPlant[1] = false;
    LearnElementsPlant[2] = false;
    LearnElementsPlant[3] = true;
    LearnElementsPlant[4] = false;
    LearnElementsPlant[5] = false;
    LearnElementsPlant[6] = false;
    LearnElementsPlant[7] = false;
    LearnElementsPlant[8] = false;
    LearnElementsPlant[9] = true;

    bool LearnElementsElec[10];
    LearnElementsElec[0] = true;
    LearnElementsElec[1] = false;
    LearnElementsElec[2] = false;
    LearnElementsElec[3] = false;
    LearnElementsElec[4] = true;
    LearnElementsElec[5] = false;
    LearnElementsElec[6] = true;
    LearnElementsElec[7] = false;
    LearnElementsElec[8] = false;
    LearnElementsElec[9] = false;

    bool LearnElementsSpook[10];
    LearnElementsSpook[0] = true;
    LearnElementsSpook[1] = false;
    LearnElementsSpook[2] = false;
    LearnElementsSpook[3] = false;
    LearnElementsSpook[4] = false;
    LearnElementsSpook[5] = true;
    LearnElementsSpook[6] = false;
    LearnElementsSpook[7] = false;
    LearnElementsSpook[8] = false;
    LearnElementsSpook[9] = false;

    bool LearnElementsLight[10];
    LearnElementsLight[0] = true;
    LearnElementsLight[1] = false;
    LearnElementsLight[2] = false;
    LearnElementsLight[3] = false;
    LearnElementsLight[4] = true;
    LearnElementsLight[5] = false;
    LearnElementsLight[6] = true;
    LearnElementsLight[7] = false;
    LearnElementsLight[8] = false;
    LearnElementsLight[9] = false;

    bool LearnElementsIce[10];
    LearnElementsIce[0] = true;
    LearnElementsIce[1] = false;
    LearnElementsIce[2] = true;
    LearnElementsIce[3] = false;
    LearnElementsIce[4] = false;
    LearnElementsIce[5] = false;
    LearnElementsIce[6] = false;
    LearnElementsIce[7] = true;
    LearnElementsIce[8] = false;
    LearnElementsIce[9] = false;

    bool LearnElementsStone[10];
    LearnElementsIce[0] = true;
    LearnElementsIce[1] = true;
    LearnElementsIce[2] = false;
    LearnElementsIce[3] = false;
    LearnElementsIce[4] = false;
    LearnElementsIce[5] = false;
    LearnElementsIce[6] = false;
    LearnElementsIce[7] = false;
    LearnElementsIce[8] = true;
    LearnElementsIce[9] = false;

    bool LearnElementsTox[10];
    LearnElementsTox[0] = true;
    LearnElementsTox[1] = false;
    LearnElementsTox[2] = false;
    LearnElementsTox[3] = false;
    LearnElementsTox[4] = false;
    LearnElementsTox[5] = false;
    LearnElementsTox[6] = false;
    LearnElementsTox[7] = false;
    LearnElementsTox[8] = false;
    LearnElementsTox[9] = true;

    bool LearnPersonalities[13];
    LearnPersonalities[0] = true;
    LearnPersonalities[1] = true;
    LearnPersonalities[2] = true;
    LearnPersonalities[3] = true;
    LearnPersonalities[4] = true;
    LearnPersonalities[5] = true;
    LearnPersonalities[6] = true;
    LearnPersonalities[7] = true;
    LearnPersonalities[8] = true;
    LearnPersonalities[9] = true;
    LearnPersonalities[10] = true;
    LearnPersonalities[11] = true;
    LearnPersonalities[12] = true;

    bool LearnPersonalitiesBasic[13];
    LearnPersonalities[0] = true;
    LearnPersonalities[1] = true;
    LearnPersonalities[2] = true;
    LearnPersonalities[3] = true;
    LearnPersonalities[4] = true;
    LearnPersonalities[5] = true;
    LearnPersonalities[6] = true;
    LearnPersonalities[7] = true;
    LearnPersonalities[8] = true;
    LearnPersonalities[9] = true;
    LearnPersonalities[10] = true;
    LearnPersonalities[11] = true;
    LearnPersonalities[12] = true;

    bool LearnPersonalitiesPhys[13];
    LearnPersonalitiesPhys[0] = true;
    LearnPersonalitiesPhys[1] = false;
    LearnPersonalitiesPhys[2] = false;
    LearnPersonalitiesPhys[3] = false;
    LearnPersonalitiesPhys[4] = false;
    LearnPersonalitiesPhys[5] = true;
    LearnPersonalitiesPhys[6] = false;
    LearnPersonalitiesPhys[7] = false;
    LearnPersonalitiesPhys[8] = false;
    LearnPersonalitiesPhys[9] = false;
    LearnPersonalitiesPhys[10] = false;
    LearnPersonalitiesPhys[11] = false;
    LearnPersonalitiesPhys[12] = true;

    bool LearnPersonalitiesMagic[13];
    LearnPersonalitiesMagic[0] = true;
    LearnPersonalitiesMagic[1] = false;
    LearnPersonalitiesMagic[2] = false;
    LearnPersonalitiesMagic[3] = false;
    LearnPersonalitiesMagic[4] = true;
    LearnPersonalitiesMagic[5] = false;
    LearnPersonalitiesMagic[6] = false;
    LearnPersonalitiesMagic[7] = false;
    LearnPersonalitiesMagic[8] = false;
    LearnPersonalitiesMagic[9] = false;
    LearnPersonalitiesMagic[10] = true;
    LearnPersonalitiesMagic[11] = false;
    LearnPersonalitiesMagic[12] = false;

    bool LearnPersonalitiesStatus[13];
    LearnPersonalitiesMagic[0] = false;
    LearnPersonalitiesMagic[1] = true;
    LearnPersonalitiesMagic[2] = true;
    LearnPersonalitiesMagic[3] = true;
    LearnPersonalitiesMagic[4] = false;
    LearnPersonalitiesMagic[5] = false;
    LearnPersonalitiesMagic[6] = true;
    LearnPersonalitiesMagic[7] = false;
    LearnPersonalitiesMagic[8] = true;
    LearnPersonalitiesMagic[9] = true;
    LearnPersonalitiesMagic[10] = false;
    LearnPersonalitiesMagic[11] = false;
    LearnPersonalitiesMagic[12] = false;

    Effect EffNull[8];
    strcpy(EffNull[0].Acronym, "HP");
    EffNull[0].Quantity = 0; //HP
    EffNull[0].Timer = 0;
    strcpy(EffNull[1].Acronym, "Def");
    EffNull[1].Quantity = 0; //Def
    EffNull[1].Timer = 0;
    strcpy(EffNull[2].Acronym, "MaD");
    EffNull[2].Quantity = 0; //MaD
    EffNull[2].Timer = 0;
    strcpy(EffNull[3].Acronym, "Acc");
    EffNull[3].Quantity = 0; //Acc
    EffNull[3].Timer = 0;
    strcpy(EffNull[4].Acronym, "Atk");
    EffNull[4].Quantity = 0; //Atk
    EffNull[4].Timer = 0;
    strcpy(EffNull[5].Acronym, "EAc");
    EffNull[5].Quantity = 0; //EAc
    EffNull[5].Timer = 0;
    strcpy(EffNull[6].Acronym, "MaA");
    EffNull[6].Quantity = 0; //MaA
    EffNull[6].Timer = 0;
    strcpy(EffNull[7].Acronym, "Spd");
    EffNull[7].Quantity = 0; //Spd
    EffNull[7].Timer = 0;

    Effect Eff1[8];
    strcpy(Eff1[0].Acronym, "HP");
    Eff1[0].Quantity = 0;
    Eff1[0].Timer = 0;
    strcpy(Eff1[1].Acronym, "Def");
    Eff1[1].Quantity = 0;
    Eff1[1].Timer = 0;
    strcpy(Eff1[2].Acronym, "MaD");
    Eff1[2].Quantity = 0;
    Eff1[2].Timer = 0;
    strcpy(Eff1[3].Acronym, "Acc");
    Eff1[3].Quantity = 0;
    Eff1[3].Timer = 0;
    strcpy(Eff1[4].Acronym, "Atk");
    Eff1[4].Quantity = 0;
    Eff1[4].Timer = 0;
    strcpy(Eff1[5].Acronym, "EAc");
    Eff1[5].Quantity = 0;
    Eff1[5].Timer = 0;
    strcpy(Eff1[6].Acronym, "MaD");
    Eff1[6].Quantity = 0;
    Eff1[6].Timer = 0;
    strcpy(Eff1[7].Acronym, "Spd");
    Eff1[7].Quantity = 10;
    Eff1[7].Timer = 4;

    Effect Eff2[8];
    strcpy(Eff2[0].Acronym, "HP");
    Eff2[0].Quantity = 0; //HP
    Eff2[0].Timer = 0;
    strcpy(Eff2[1].Acronym, "Def");
    Eff2[1].Quantity = 0; //Def
    Eff2[1].Timer = 0;
    strcpy(Eff2[2].Acronym, "MaD");
    Eff2[2].Quantity = 0; //MaD
    Eff2[2].Timer = 0;
    strcpy(Eff2[3].Acronym, "Acc");
    Eff2[3].Quantity = 0; //Acc
    Eff2[3].Timer = 0;
    strcpy(Eff2[4].Acronym, "Atk");
    Eff2[4].Quantity = 15; //Atk
    Eff2[4].Timer = 3;
    strcpy(Eff2[5].Acronym, "EAc");
    Eff2[5].Quantity = 0; //EAc
    Eff2[5].Timer = 0;
    strcpy(Eff2[6].Acronym, "MaA");
    Eff2[6].Quantity = 15; //MaA
    Eff2[6].Timer = 3;
    strcpy(Eff2[7].Acronym, "Spd");
    Eff2[7].Quantity = 10; //Spd
    Eff2[7].Timer = 3;

    Effect Eff3[8];
    strcpy(Eff3[0].Acronym, "HP");
    Eff3[0].Quantity = 10; //HP
    Eff3[0].Timer = 1;
    strcpy(Eff3[1].Acronym, "Def");
    Eff3[1].Quantity = 0; //Def
    Eff3[1].Timer = 0;
    strcpy(Eff3[2].Acronym, "MaD");
    Eff3[2].Quantity = 0; //MaD
    Eff3[2].Timer = 0;
    strcpy(Eff3[3].Acronym, "Acc");
    Eff3[3].Quantity = 0; //Acc
    Eff3[3].Timer = 0;
    strcpy(Eff3[4].Acronym, "Atk");
    Eff3[4].Quantity = 0; //Atk
    Eff3[4].Timer = 0;
    strcpy(Eff3[5].Acronym, "EAc");
    Eff3[5].Quantity = 0; //EAc
    Eff3[5].Timer = 0;
    strcpy(Eff3[6].Acronym, "MaA");
    Eff3[6].Quantity = 0; //MaA
    Eff3[6].Timer = 0;
    strcpy(Eff3[7].Acronym, "Spd");
    Eff3[7].Quantity = 0; //Spd
    Eff3[7].Timer = 0;

    Effect Eff4[8];
    strcpy(Eff4[0].Acronym, "HP");
    Eff4[0].Quantity = 0; //HP
    Eff4[0].Timer = 0;
    strcpy(Eff4[1].Acronym, "Def");
    Eff4[1].Quantity = 0; //Def
    Eff4[1].Timer = 0;
    strcpy(Eff4[2].Acronym, "MaD");
    Eff4[2].Quantity = 0; //MaD
    Eff4[2].Timer = 0;
    strcpy(Eff4[3].Acronym, "Acc");
    Eff4[3].Quantity = 0; //Acc
    Eff4[3].Timer = 0;
    strcpy(Eff4[4].Acronym, "Atk");
    Eff4[4].Quantity = 0; //Atk
    Eff4[4].Timer = 0;
    strcpy(Eff4[5].Acronym, "EAc");
    Eff4[5].Quantity = 0; //EAc
    Eff4[5].Timer = 0;
    strcpy(Eff4[6].Acronym, "MaA");
    Eff4[6].Quantity = 0; //MaA
    Eff4[6].Timer = 0;
    strcpy(Eff4[7].Acronym, "Spd");
    Eff4[7].Quantity = -15; //Spd
    Eff4[7].Timer = 3;

    Effect Eff5[8];
    strcpy(Eff5[0].Acronym, "HP");
    Eff5[0].Quantity = 0; //HP
    Eff5[0].Timer = 0;
    strcpy(Eff5[1].Acronym, "Def");
    Eff5[1].Quantity = 0; //Def
    Eff5[1].Timer = 0;
    strcpy(Eff5[2].Acronym, "MaD");
    Eff5[2].Quantity = 0; //MaD
    Eff5[2].Timer = 0;
    strcpy(Eff5[3].Acronym, "Acc");
    Eff5[3].Quantity = 0; //Acc
    Eff5[3].Timer = 0;
    strcpy(Eff5[4].Acronym, "Atk");
    Eff5[4].Quantity = -15; //Atk
    Eff5[4].Timer = 3;
    strcpy(Eff5[5].Acronym, "EAc");
    Eff5[5].Quantity = 0; //EAc
    Eff5[5].Timer = 0;
    strcpy(Eff5[6].Acronym, "MaA");
    Eff5[6].Quantity = -15; //MaA
    Eff5[6].Timer = 3;
    strcpy(Eff5[7].Acronym, "Spd");
    Eff5[7].Quantity = 0; //Spd
    Eff5[7].Timer = 0;

    Effect Eff6[8];
    strcpy(Eff6[0].Acronym, "HP");
    Eff6[0].Quantity = 0; //HP
    Eff6[0].Timer = 0;
    strcpy(Eff6[1].Acronym, "Def");
    Eff6[1].Quantity = 0; //Def
    Eff6[1].Timer = 0;
    strcpy(Eff6[2].Acronym, "MaD");
    Eff6[2].Quantity = 0; //MaD
    Eff6[2].Timer = 0;
    strcpy(Eff6[3].Acronym, "Acc");
    Eff6[3].Quantity = 0; //Acc
    Eff6[3].Timer = 0;
    strcpy(Eff6[4].Acronym, "Atk");
    Eff6[4].Quantity = 0; //Atk
    Eff6[4].Timer = 0;
    strcpy(Eff6[5].Acronym, "EAc");
    Eff6[5].Quantity = 0; //EAc
    Eff6[5].Timer = 0;
    strcpy(Eff6[6].Acronym, "MaA");
    Eff6[6].Quantity = 0; //MaA
    Eff6[6].Timer = 0;
    strcpy(Eff6[7].Acronym, "Spd");
    Eff6[7].Quantity = 25; //Spd
    Eff6[7].Timer = 1;

    Effect Eff7[8];
    strcpy(Eff7[0].Acronym, "HP");
    Eff7[0].Quantity = 0; //HP
    Eff7[0].Timer = 0;
    strcpy(Eff7[1].Acronym, "Def");
    Eff7[1].Quantity = 0; //Def
    Eff7[1].Timer = 0;
    strcpy(Eff7[2].Acronym, "MaD");
    Eff7[2].Quantity = 0; //MaD
    Eff7[2].Timer = 0;
    strcpy(Eff7[3].Acronym, "Acc");
    Eff7[3].Quantity = -10; //Acc
    Eff7[3].Timer = 2;
    strcpy(Eff7[4].Acronym, "Atk");
    Eff7[4].Quantity = 0; //Atk
    Eff7[4].Timer = 0;
    strcpy(Eff7[5].Acronym, "EAc");
    Eff7[5].Quantity = -10; //EAc
    Eff7[5].Timer = 2;
    strcpy(Eff7[6].Acronym, "MaA");
    Eff7[6].Quantity = 0; //MaA
    Eff7[6].Timer = 0;
    strcpy(Eff7[7].Acronym, "Spd");
    Eff7[7].Quantity = 0; //Spd
    Eff7[7].Timer = 0;

    Effect Eff8[8];
    strcpy(Eff8[0].Acronym, "HP");
    Eff8[0].Quantity = 0; //HP
    Eff8[0].Timer = 0;
    strcpy(Eff8[1].Acronym, "Def");
    Eff8[1].Quantity = -15; //Def
    Eff8[1].Timer = 3;
    strcpy(Eff8[2].Acronym, "MaD");
    Eff8[2].Quantity = -15; //MaD
    Eff8[2].Timer = 3;
    strcpy(Eff8[3].Acronym, "Acc");
    Eff8[3].Quantity = -10; //Acc
    Eff8[3].Timer = 2;
    strcpy(Eff8[4].Acronym, "Atk");
    Eff8[4].Quantity = 0; //Atk
    Eff8[4].Timer = 0;
    strcpy(Eff8[5].Acronym, "EAc");
    Eff8[5].Quantity = 0; //EAc
    Eff8[5].Timer = 0;
    strcpy(Eff8[6].Acronym, "MaA");
    Eff8[6].Quantity = 0; //MaA
    Eff8[6].Timer = 0;
    strcpy(Eff8[7].Acronym, "Spd");
    Eff8[7].Quantity = 0; //Spd
    Eff8[7].Timer = 0;

    Effect EffKABOOM[8];
    strcpy(EffKABOOM[0].Acronym, "HP");
    EffKABOOM[0].Quantity = -100; //HP
    EffKABOOM[0].Timer = 1;
    strcpy(EffKABOOM[1].Acronym, "Def");
    EffKABOOM[1].Quantity = 0; //Def
    EffKABOOM[1].Timer = 0;
    strcpy(EffKABOOM[2].Acronym, "MaD");
    EffKABOOM[2].Quantity = 0; //MaD
    EffKABOOM[2].Timer = 0;
    strcpy(EffKABOOM[3].Acronym, "Acc");
    EffKABOOM[3].Quantity = 0; //Acc
    EffKABOOM[3].Timer = 0;
    strcpy(EffKABOOM[4].Acronym, "Atk");
    EffKABOOM[4].Quantity = 0; //Atk
    EffKABOOM[4].Timer = 0;
    strcpy(EffKABOOM[5].Acronym, "EAc");
    EffKABOOM[5].Quantity = 0; //EAc
    EffKABOOM[5].Timer = 0;
    strcpy(EffKABOOM[6].Acronym, "MaA");
    EffKABOOM[6].Quantity = 0; //MaA
    EffKABOOM[6].Timer = 0;
    strcpy(EffKABOOM[7].Acronym, "Spd");
    EffKABOOM[7].Quantity = 0; //Spd
    EffKABOOM[7].Timer = 0;

    char Descri1[3][255];
    strcpy(Descri1[0], "Um ataque básico.");
    strcpy(Descri1[1], "");
    strcpy(Descri1[2], "Comum, Físico");

    AddSkill(&pSkills, &dataQuantities, "Mordida", "Físico", Descri1, "", DescAtiva, LearnPersonalitiesBasic, LearnElementsComum, 0, allElements[0], 'E', 100, 10, 10, 0, 0, 15, 'E', 0, EffNull, 0, EffNull);
                                            //bool AddSkill(SkPointer *pSkills, DataQuantity *dataQuantities, char *name, char *type, char description[3][255], 
                                            // char *active, char activeDescription[3][255], bool learnablePersonalities[13], bool LearnableElements[10], 
                                            // int elementEffectHitChance, Element element, char target, int hitChance, int  attackBase, int attackScale, 
                                            // int magicBase, int magicAttackScale, int critChance, char effectTarget, int enemyEffectHitChance, Effect enemyEffect[8], 
                                            // int selfEffectHitChance, Effect selfEffect[8]);


    char Descri2[3][255];
    strcpy(Descri2[0], "Um ataque que dá dano ao usuário");
    strcpy(Descri2[1], "e ao oponente.");
    strcpy(Descri2[2], "Comum, Físico");

    AddSkill(&pSkills, &dataQuantities, "Último Recurso", "Físico", Descri2, "", DescAtiva, LearnPersonalitiesBasic, LearnElementsComum, 0, allElements[0], 'E', 100, 10, 30, 0, 0, 15, 'E', 0, EffNull , 0, EffNull);

    char Descri3[3][255];
    strcpy(Descri3[0], "Um ataque básico.");
    strcpy(Descri3[1], "");
    strcpy(Descri3[2], "Comum, Mágico");

    AddSkill(&pSkills, &dataQuantities, "Berrante", "Mágico", Descri3, "", DescAtiva, LearnPersonalitiesBasic, LearnElementsComum, 0, allElements[0], 'E', 100, 0, 0, 15, 0, 15, 'E', 0, EffNull, 0, EffNull);


    char Descri4[3][255];
    strcpy(Descri4[0], "Um ataque que aumenta a");
    strcpy(Descri4[1], "Velocidade do usuário.");
    strcpy(Descri4[2], "Comum, Físico");

    AddSkill(&pSkills, &dataQuantities, "Ataque Rápido", "Físico", Descri4, "", DescAtiva, LearnPersonalitiesBasic, LearnElementsComum, 0, allElements[0], 'E', 100, 5, 0, 0, 0, 15, 'S', 0, EffNull, 100, Eff1);

    char Descri5[3][255];
    strcpy(Descri5[0], "Um ataque que pode queimar");
    strcpy(Descri5[1], "o oponente.");
    strcpy(Descri5[2], "Flamejante, Físico");

    AddSkill(&pSkills, &dataQuantities, "Baque Ardente", "Físico", Descri5, "", DescAtiva, LearnPersonalitiesPhys, LearnElementsFire, 0, allElements[1], 'E', 100, 5, 30, 0, 0, 15, 'E', 20, allElements[1].StatusEffect, 0, EffNull);

    char Descri6[3][255];
    strcpy(Descri6[0], "Um ataque que pode queimar");
    strcpy(Descri6[1], "o oponente.");
    strcpy(Descri6[2], "Flamejante, Mágico");

    AddSkill(&pSkills, &dataQuantities, "Onda de Calor", "Mágico", Descri6, "", DescAtiva, LearnPersonalitiesMagic, LearnElementsFire, 0, allElements[1], 'E', 100, 0, 0, 5, 30, 15, 'E', 20, allElements[1].StatusEffect, 0, EffNull);

    char Descri7[3][255];
    strcpy(Descri7[0], "Um ataque que pode queimar");
    strcpy(Descri7[1], "o oponente. Não dá dano.");
    strcpy(Descri7[2], "Flamejante, Status");

    AddSkill(&pSkills, &dataQuantities, "Faísca", "Status", Descri7, "", DescAtiva, LearnPersonalitiesStatus, LearnElementsFire, 0, allElements[1], 'E', 100, 0, 0, 0, 0, 0, 'E', 100, allElements[1].StatusEffect, 0, EffNull);

    char Descri8[3][255];
    strcpy(Descri8[0], "Um ataque que pode dá grande");
    strcpy(Descri8[1], "dano ao usuário e o oponente. Pode queimar.");
    strcpy(Descri8[2], "Flamejante, Mágico");

    AddSkill(&pSkills, &dataQuantities, "Terra Arrasada", "Mágico", Descri8, "", DescAtiva, LearnPersonalitiesMagic, LearnElementsFire, 0, allElements[1], 'B', 100, 0, 0, 30, 0, 0, 'B', 50, allElements[1].StatusEffect, 50, allElements[1].StatusEffect);

    char Descri9[3][255];
    strcpy(Descri9[0], "Um ataque básico.");
    strcpy(Descri9[1], "");
    strcpy(Descri9[2], "Aquático, Mágico");

    AddSkill(&pSkills, &dataQuantities, "Jato de Água", "Mágico", Descri9, "", DescAtiva, LearnPersonalitiesMagic, LearnElementsWater, 0, allElements[2], 'E', 100, 0, 0, 10, 15, 15, 'E', 0, EffNull, 0, EffNull);

    char Descri10[3][255];
    strcpy(Descri10[0], "Um ataque que aumenta ambos os");
    strcpy(Descri10[1], "Ataques e Velocidade do usuário.");
    strcpy(Descri10[2], "Aquático, Status");

    AddSkill(&pSkills, &dataQuantities, "Salto Carpado", "Status", Descri10, "", DescAtiva, LearnPersonalitiesStatus, LearnElementsWater, 0, allElements[2], 'S', 100, 0, 0, 0, 0, 0, 'S', 0, EffNull, 100, Eff2);

    char Descri11[3][255];
    strcpy(Descri11[0], "Um ataque que Encharca o");
    strcpy(Descri11[1], "oponente.");
    strcpy(Descri11[2], "Aquático, Status");

    AddSkill(&pSkills, &dataQuantities, "Encharcar", "Status", Descri11, "", DescAtiva, LearnPersonalitiesStatus, LearnElementsWater, 0, allElements[2], 'E', 100, 0, 0, 0, 0, 0, 'E', 100, allElements[2].StatusEffect, 0, EffNull);

    char Descri12[3][255];
    strcpy(Descri12[0], "Um ataque que possui maior");
    strcpy(Descri12[1], "chance de ser Crítico.");
    strcpy(Descri12[2], "Aquático, Físico");

    AddSkill(&pSkills, &dataQuantities, "Mergulho Torpedo", "Físico'", Descri12, "", DescAtiva, LearnPersonalitiesPhys, LearnElementsWater, 0, allElements[2], 'E', 100, 10, 10, 0, 0, 50, 'E', 0, EffNull, 0, EffNull);

    char Descri13[3][255];
    strcpy(Descri13[0], "Um ataque comum.");
    strcpy(Descri13[1], "");
    strcpy(Descri13[2], "Natural, Físico");

    AddSkill(&pSkills, &dataQuantities, "Galho Espinhoso", "Físico", Descri13, "", DescAtiva, LearnPersonalitiesMagic, LearnElementsPlant, 0, allElements[3], 'E', 100, 7, 35, 0, 0, 15, 'E', 0, EffNull, 0, EffNull);

    char Descri14[3][255];
    strcpy(Descri14[0], "Um ataque que retorna ao usuário");
    strcpy(Descri14[1], "parte do dano dado.");
    strcpy(Descri14[2], "Natural, Mágico");

    AddSkill(&pSkills, &dataQuantities, "Raíz Drenante", "Mágico", Descri14, "", DescAtiva, LearnPersonalitiesMagic, LearnElementsPlant, 0, allElements[3],  'E', 100, 0, 0, 10, 15, 15, 'S', 0, EffNull, 100, Eff3);

    char Descri15[3][255];
    strcpy(Descri15[0], "Um ataque que Enraíza");
    strcpy(Descri15[1], "o oponente.");
    strcpy(Descri15[2], "Natural, Status");

    AddSkill(&pSkills, &dataQuantities, "Enraizar", "Status", Descri15, "", DescAtiva, LearnPersonalitiesStatus, LearnElementsPlant, 0, allElements[3], 'E', 100, 0, 0, 0, 0, 0, 'E', 100, allElements[2].StatusEffect, 0, EffNull);

    char Descri16[3][255];
    strcpy(Descri16[0], "Um ataque que diminui a");
    strcpy(Descri16[1], "Velocidade do oponente.");
    strcpy(Descri16[2], "Natural, Mágico");

    AddSkill(&pSkills, &dataQuantities, "Pólen Atordoante", "Status", Descri16, "", DescAtiva, LearnPersonalitiesMagic, LearnElementsPlant, 0, allElements[3], 'E', 100, 5, 0, 0, 0, 15, 'E', 100, Eff4, 0, EffNull);    

    char Descri17[3][255];
    strcpy(Descri17[0], "Um ataque poderosíssimo");
    strcpy(Descri17[1], "que nocauteia o usuário.");
    strcpy(Descri17[2], "Elétrico, Mágico");

    AddSkill(&pSkills, &dataQuantities, "Curto Circuito", "Mágico", Descri17, "", DescAtiva, LearnPersonalitiesMagic, LearnElementsElec, 0, allElements[4], 'E', 100, 0, 0, 40, 0, 0, 'S', 0, EffNull, 100, EffKABOOM);

    char Descri18[3][255];
    strcpy(Descri18[0], "Um ataque fraco que");
    strcpy(Descri18[1], "pode Paralisar.");
    strcpy(Descri18[2], "Elétrico, Mágico");

    AddSkill(&pSkills, &dataQuantities, "Choque Fraco", "Mágico", Descri18, "", DescAtiva, LearnPersonalitiesMagic, LearnElementsElec, 0, allElements[4], 'E', 100, 0, 0, 5, 15, 15, 'E', 0, EffNull, 30, allElements[4].StatusEffect);

    char Descri19[3][255];
    strcpy(Descri19[0], "Um ataque comum.");
    strcpy(Descri19[1], "");
    strcpy(Descri19[2], "Misterioso, Mágico");

    AddSkill(&pSkills, &dataQuantities, "Pó Mágico", "Mágico", Descri19, "", DescAtiva, LearnPersonalitiesMagic, LearnElementsSpook, 0, allElements[5], 'E', 100, 0, 0, 10, 20, 15, 'E', 0, EffNull, 0, EffNull);

    char Descri20[3][255];
    strcpy(Descri20[0], "Um ataque comum.");
    strcpy(Descri20[1], "");
    strcpy(Descri20[2], "Misterioso, Físico");

    AddSkill(&pSkills, &dataQuantities, "Tapa Dimensional", "Mágico", Descri20, "", DescAtiva, LearnPersonalitiesPhys, LearnElementsSpook, 0, allElements[5], 'E', 100, 10, 20, 0, 0, 15, 'E', 0, EffNull, 0, EffNull);

    char Descri21[3][255];
    strcpy(Descri21[0], "Um ataque que Assombra");
    strcpy(Descri21[1], "o oponente.");
    strcpy(Descri21[2], "Misterioso, Status");

    AddSkill(&pSkills, &dataQuantities, "Assombração", "Status", Descri21, "", DescAtiva, LearnPersonalitiesStatus, LearnElementsSpook, 0, allElements[5], 'E', 0, 0, 0, 0, 0, 0, 'E', 100, allElements[5].StatusEffect, 0, EffNull);

    char Descri22[3][255];
    strcpy(Descri22[0], "Um ataque que retorna ao usuário");
    strcpy(Descri22[1], "parte do dano aplicado.");
    strcpy(Descri22[2], "Misterioso, Físico");

    AddSkill(&pSkills, &dataQuantities, "Drenagem de Vida", "Mágico", Descri22, "", DescAtiva, LearnPersonalitiesMagic, LearnElementsSpook, 0, allElements[5], 'E', 100, 0, 0, 10, 15, 15, 'S', 0, EffNull, 100, Eff3);

    char Descri23[3][255];
    strcpy(Descri23[0], "Um ataque que Cega o oponente.");
    strcpy(Descri23[1], "");
    strcpy(Descri23[2], "Luminoso, Status");

    AddSkill(&pSkills, &dataQuantities, "Flashbang", "Status", Descri23, "", DescAtiva, LearnPersonalitiesStatus, LearnElementsLight, 0, allElements[6], 'E', 100, 0, 0, 0, 0, 0, 'E', 0, EffNull, 100, allElements[6].StatusEffect);

    char Descri24[3][255];
    strcpy(Descri24[0], "Um golpe básico.");
    strcpy(Descri24[1], "");
    strcpy(Descri24[2], "Luminoso, Físico");

    AddSkill(&pSkills, &dataQuantities, "Carga Iluminada", "Físico", Descri24, "", DescAtiva, LearnPersonalitiesPhys, LearnElementsLight, 0, allElements[6], 'E', 100, 10, 30, 0, 0, 15, 'E', 0, EffNull, 0, EffNull);

    char Descri25[3][255];
    strcpy(Descri25[0], "Um golpe que diminui");
    strcpy(Descri25[1], "ambos os ataques do oponente.");
    strcpy(Descri25[2], "Luminoso, Status");

    AddSkill(&pSkills, &dataQuantities, "Olhar Desconsertante", "Status", Descri25, "", DescAtiva, LearnPersonalitiesMagic, LearnElementsLight, 0, allElements[6], 'E', 100, 0, 0, 0, 0, 0, 'E', 100, Eff5, 0, EffNull);
        
    char Descri26[3][255];
    strcpy(Descri26[0], "Um ataque poderosíssimo");
    strcpy(Descri26[1], "que nocauteia o usuário.");
    strcpy(Descri26[2], "Luminoso, Mágico");

    AddSkill(&pSkills, &dataQuantities, "Explosão Solar", "Mágico", Descri26, "", DescAtiva, LearnPersonalitiesMagic, LearnElementsLight, 0, allElements[6], 'E', 100, 0, 0, 40, 0, 0, 'S', 0, EffNull, 100, EffKABOOM);
        
    char Descri27[3][255];
    strcpy(Descri27[0], "Um golpe soterra o");
    strcpy(Descri27[1], "oponente.");
    strcpy(Descri27[2], "Mineral, Status");

    AddSkill(&pSkills, &dataQuantities, "Soterragem", "Status", Descri27, "", DescAtiva, LearnPersonalitiesStatus, LearnElementsStone, 0, allElements[8], 'E', 100, 0, 0, 0, 0, 0, 'E', 100, allElements[7].StatusEffect, 0, EffNull);    
        
    char Descri28[3][255];
    strcpy(Descri28[0], "Um golpe que possui");
    strcpy(Descri28[1], "mais chances de Critar.");
    strcpy(Descri28[2], "Mineral, Físico");

    AddSkill(&pSkills, &dataQuantities, "Cacos de Pedra", "Físico", Descri28, "", DescAtiva, LearnPersonalitiesPhys, LearnElementsStone, 0, allElements[8], 'E', 100, 10, 15, 0, 0, 50, 'E', 0, EffNull, 0, EffNull);    
        
    char Descri29[3][255];
    strcpy(Descri29[0], "Um golpe que possui");
    strcpy(Descri29[1], "mais chances de Critar");
    strcpy(Descri29[2], "Mineral, Mágico");

    AddSkill(&pSkills, &dataQuantities, "Cacos de Pedra", "Mágico", Descri29, "", DescAtiva, LearnPersonalitiesMagic, LearnElementsStone, 0, allElements[8], 'E', 100, 0, 0, 10, 15, 50, 'E', 0, EffNull, 0, EffNull);    
        
    char Descri30[3][255];
    strcpy(Descri30[0], "Um golpe que aumenta a");
    strcpy(Descri30[1], "Velocidade do usuário.");
    strcpy(Descri30[2], "Mineral, Status");

    AddSkill(&pSkills, &dataQuantities, "Cacos de Pedra", "Status", Descri30, "", DescAtiva, LearnPersonalitiesStatus, LearnElementsStone, 0, allElements[8], 'E', 100, 0, 0, 0, 0, 0, 'S', 0, EffNull, 100, Eff6);  
        
    char Descri31[3][255];
    strcpy(Descri31[0], "Um golpe básico.");
    strcpy(Descri31[1], "");
    strcpy(Descri31[2], "Gélido, Físico");

    AddSkill(&pSkills, &dataQuantities, "Frente Fria", "Físico", Descri31, "", DescAtiva, LearnPersonalitiesPhys, LearnElementsIce, 0, allElements[7], 'E', 100, 10, 20, 0, 0, 15, 'E', 0, EffNull, 0, EffNull);  

    char Descri32[3][255];
    strcpy(Descri32[0], "Um golpe que diminui as");
    strcpy(Descri32[1], "Precisões do oponente.");
    strcpy(Descri32[2], "Gélido, Mágico");

    AddSkill(&pSkills, &dataQuantities, "Granizo", "Mágico", Descri32, "", DescAtiva, LearnPersonalitiesMagic, LearnElementsIce, 0, allElements[7], 'E', 100, 0, 0, 10, 20, 15, 'E', 100, Eff7, 0, EffNull);  

    char Descri33[3][255];
    strcpy(Descri33[0], "Um golpe que dá Calafrios");
    strcpy(Descri33[1], "ao oponente.");
    strcpy(Descri33[2], "Gélido, Status");

    AddSkill(&pSkills, &dataQuantities, "Hipotermizar", "Status", Descri33, "", DescAtiva, LearnPersonalitiesStatus, LearnElementsIce, 0, allElements[7], 'E', 100, 0, 0, 0, 0, 0, 'E', 100, allElements[7].StatusEffect, 0, EffNull); 

    char Descri34[3][255];
    strcpy(Descri34[0], "Um golpe que pode Envenenar");
    strcpy(Descri34[1], "o oponente.");
    strcpy(Descri34[2], "Venenoso, Físico");

    AddSkill(&pSkills, &dataQuantities, "Mordida Podre", "Físico", Descri34, "", DescAtiva, LearnPersonalitiesPhys, LearnElementsTox, 0, allElements[9], 'E', 100, 10, 10, 0, 0, 15, 'E', 50, allElements[9].StatusEffect, 0, EffNull); 
        
    char Descri35[3][255];
    strcpy(Descri35[0], "Um golpe que pode Envenenar");
    strcpy(Descri35[1], "o oponente.");
    strcpy(Descri35[2], "Venenoso, Mágico");

    AddSkill(&pSkills, &dataQuantities, "Cuspe Tóxico", "Mágico", Descri35, "", DescAtiva, LearnPersonalitiesMagic, LearnElementsTox, 0, allElements[9], 'E', 100, 0, 0, 10, 10, 15, 'E', 50, allElements[9].StatusEffect, 0, EffNull);     
        
    char Descri36[3][255];
    strcpy(Descri36[0], "Um golpe que diminui as");
    strcpy(Descri36[1], "Defesas do oponente.");
    strcpy(Descri36[2], "Venenoso, Mágico");

    AddSkill(&pSkills, &dataQuantities, "Sopro Neurotóxico", "Status", Descri36, "", DescAtiva, LearnPersonalitiesStatus, LearnElementsTox, 0, allElements[9], 'E', 100, 0, 0, 0, 0, 0, 'E', 100, Eff8, 0, EffNull);   

    SaveSkills(pSkills, dataQuantities.Skill, skills);
    SaveDataQuantity(dataQuantities, dataQuantity);

    //------------------------------------------------------------------------------------------------------------------//


    //Principal Do Usuário
    //------------------------------------------------------------------------------------------------------------------//
    int respostaUserMP;
    bool login1 = false, login2 = false;
    char respostaCadastro;
    int indexUs1, indexUs2;

    printf("Gostaria de cadastrar-se?(S/n): ");

    scanf(" %c", &respostaCadastro);

    if (respostaCadastro == 's' || respostaCadastro == 'S')
    {

        char name[20];
        char pass[7];
        printf("Digite o nome do jogador (até 19 caracteres): ");
        scanf("%19s", name);
        printf("Digite a senha do jogador (6 caracteres): ");
        scanf("%6s", pass);

        if (AddPlayer(&pPlayers, &dataQuantities, name, pass))
        {
            if (SavePlayers(pPlayers, dataQuantities.Player, "Players.bin"))
            {
                SaveDataQuantity(dataQuantities, "DataQuantity.txt");
                printf("Jogador cadastrado e salvo com sucesso!(Press Enter)\n");
                getchar();
                getchar();
            }
            else
            {
                printf("Falha ao salvar os jogadores.(Press Enter)\n");
                getchar();
                getchar();
            }
        }
        else
        {
            printf("Falha ao cadastrar o jogador.(Press Enter)\n");
            getchar();
            getchar();
        }
    }
    while (true)
    {
        LimparTerminal();
        Menu();
        printf("Sua escolha?: ");
        scanf(" %d", &respostaUserMP);
        if (respostaUserMP == 1)
        {
            LimparTerminal();
            Login(pPlayers, dataQuantities.Player, &login1, &login2, &indexUs1, &indexUs2);
            playerOneIndex = indexUs1;
            playerTwoIndex = indexUs2;
            bool shopping = true;
            bool oPrimeiroFoi = false;
            int turnoShop, jogador;
            while (shopping)
            {
                if (!oPrimeiroFoi)
                {
                    turnoShop = indexUs1;
                    jogador = 1;
                }
                else
                {
                    turnoShop = indexUs2;
                    jogador = 2;
                }
                LimparTerminal();
                MenuShopMP();
                printf("Jogador %d, escolha sua ação de compra: ", jogador);
                scanf(" %d", &respostaUserMP);

                switch (respostaUserMP)
                {
                case 1: 
                    if (!ShopPikomon(&pPlayers, turnoShop, pPikomons, dataQuantities, allPersonalities))
                    {
                        printf("Não foi possível comprar Pikomon. (Pressione Enter para continuar)\n");
                        getchar();
                        getchar();
                    }
                    break;
                case 2: 
                    if (!ShopItems(pPlayers, turnoShop, pItems, dataQuantities))
                    {
                        printf("Não foi possível comprar o item. (Pressione Enter para continuar)\n");
                        getchar();
                        getchar();
                    }
                    break;
                case 3:
                    if(!oPrimeiroFoi){
                        oPrimeiroFoi = true;
                    }
                    else{
                        shopping = false;
                    }
                    break;
                default:
                    printf("Opção inválida. Tente novamente. (Pressione Enter)\n");
                    getchar();
                    getchar();
                    break;
                }
            }
            SelectBattlePikomons(&pPlayers[playerOneIndex]);
            for(int x = 0; x < 6; x++){
                for(int k = 0; k < 8; k++){
                    pPlayers[playerOneIndex].PikomonsStorage[x].Atributes[k].Total = pPlayers[playerOneIndex].PikomonsStorage[x].Atributes[k].Base;
                }
                pPlayers[playerOneIndex].PikomonsStorage[x].CurrentHP.Total = pPlayers[playerOneIndex].PikomonsStorage[x].Atributes[0].Total;
            }
            SelectBattlePikomons(&pPlayers[playerTwoIndex]);
            for(int x = 0; x < 6; x++){
                for(int k = 0; k < 8; k++){
                    pPlayers[playerTwoIndex].PikomonsStorage[x].Atributes[k].Total = pPlayers[playerOneIndex].PikomonsStorage[x].Atributes[k].Base;
                }
                pPlayers[playerTwoIndex].PikomonsStorage[x].CurrentHP.Total = pPlayers[playerOneIndex].PikomonsStorage[x].Atributes[0].Total;
            }

            //EscolherSkills(&pPlayers[playerOneIndex]);
            //getchar();
            //getchar();
            //EscolherSkills(&pPlayers[playerTwoIndex]);
            //getchar();
            //getchar();
            //Variaveis calcSkill
            bool SkillWasUsed = false;
            bool elementalEffectHit, skillHit, critHit, selfEffectHit, enemyEffectHit, usedItemStatusHit;
            int selfDamage, enemyDamage, usedSkillIndex;
            //useitem
            bool ItemWasUsed = false;
            //Variaveis Battle
            char calcNextTurn[7], userResponse;
            bool playerOneTurn, reset, nextTurnReset = false, battleIsOver, playerOneVictory, respostaValida, respostaValida2;
            int i = 0, turnCost;
            PiPointer selectedPlayerOnePicomon = &pPlayers[playerOneIndex].BatlePikomons[pPlayers[playerOneIndex].SelectedPikomonIndex], selectedPlayerTwoPicomon = &pPlayers[playerTwoIndex].BatlePikomons[pPlayers[playerTwoIndex].SelectedPikomonIndex];
            PlPointer PlayerOne = &pPlayers[playerOneIndex], PlayerTwo = &pPlayers[playerTwoIndex];
            reset = true;
            battleIsOver = false;
            while(!battleIsOver){
                //Recalcula os turnos toda a vez que tiver um reset, tipo quando a velocidade alterar
                if(SkillWasUsed){
                    SkillWasUsed = false;
                    if (skillHit)
                    {
                        if (critHit)
                        {
                            printf("O pikomon acertou um critico! Causando %d ao pokemon inimigo e causando %d a si mesmo", enemyDamage, selfDamage);
                        }
                        else
                        {
                            printf("O pikomon acertou causando %d ao pokemon inimigo e causando %d a si mesmo", enemyDamage, selfDamage);
                        }
                    }
                    else
                    {
                        printf("O pikomon errou...");
                    }
                    getchar();
                    getchar();
                }
                if(ItemWasUsed){
                    ItemWasUsed = false;
                    if(selfEffectHit){
                        printf("O efeito do item funcionou");
                    }
                    else{
                        printf("O efeito do item não funcionou");
                    } 
                    getchar();
                    getchar();
                }
                
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
                        if(SkillWasUsed){
                            SkillWasUsed = false;
                            if (skillHit)
                                {
                                    if (critHit)
                                    {
                                        printf("O pikomon acertou um critico! Causando %d ao pokemon inimigo e causando %d a si mesmo", enemyDamage, selfDamage);
                                    }
                                    else
                                    {
                                        printf("O pikomon acertou causando %d ao pokemon inimigo e causando %d a si mesmo", enemyDamage, selfDamage);
                                    }
                                }
                                else
                                {
                                    printf("O pikomon errou...");
                                }
                            getchar();
                            getchar();
                        }
                        if(ItemWasUsed){
                            ItemWasUsed = false;
                            if(selfEffectHit){
                                printf("O efeito do item funcionou");
                            }
                            else{
                                printf("O efeito do item não funcionou");
                            } 
                            getchar();
                            getchar();
                        }
                        
                        PrintPikomonEffects(&pPlayers[playerOneIndex].BatlePikomons[pPlayers[playerOneIndex].SelectedPikomonIndex]);
                        getchar();
                        getchar();
                        //acoes do player1
                        if(pPlayers[playerOneIndex].BatlePikomons[pPlayers[playerOneIndex].SelectedPikomonIndex].CurrentHP.Total > 0){
                            PassPikomonTurnTime(&pPlayers[playerOneIndex].BatlePikomons[pPlayers[playerOneIndex].SelectedPikomonIndex]);
                            CalcNextTurn(pPlayers[playerOneIndex].BatlePikomons[pPlayers[playerOneIndex].SelectedPikomonIndex], pPlayers[playerTwoIndex].BatlePikomons[pPlayers[playerTwoIndex].SelectedPikomonIndex], calcNextTurn);
                            MenuBattle(pPlayers[playerTwoIndex].BatlePikomons[pPlayers[playerTwoIndex].SelectedPikomonIndex], pPlayers[playerOneIndex].BatlePikomons[pPlayers[playerOneIndex].SelectedPikomonIndex], calcNextTurn);
                            respostaValida = false;
                            while (!respostaValida){
                                scanf(" %c", &userResponse);
                                if(/*escolheu usa skill*/userResponse == '1'){
                                    respostaValida = true;
                                    printf("Skill(1):\n");
                                    ShowSkill(&pPlayers[playerOneIndex].BatlePikomons[pPlayers[playerOneIndex].SelectedPikomonIndex].Skills[0]);
                                    printf("Skill(2):\n");
                                    ShowSkill(&pPlayers[playerOneIndex].BatlePikomons[pPlayers[playerOneIndex].SelectedPikomonIndex].Skills[1]);
                                    printf("Skill(3):\n");
                                    ShowSkill(&pPlayers[playerOneIndex].BatlePikomons[pPlayers[playerOneIndex].SelectedPikomonIndex].Skills[2]);
                                    printf("Skill(4):\n");
                                    ShowSkill(&pPlayers[playerOneIndex].BatlePikomons[pPlayers[playerOneIndex].SelectedPikomonIndex].Skills[3]);
                                    respostaValida2 = false;
                                    while(!respostaValida2){
                                        printf("Escolha uma skill\n");
                                        scanf(" %c", &userResponse);
                                        if(userResponse == '1'){
                                            respostaValida2 = true;
                                            SkillWasUsed = true;
                                            CalcSkill(allElements, selectedPlayerOnePicomon, 0, selectedPlayerTwoPicomon, &elementalEffectHit, &skillHit, &critHit, &selfEffectHit, &enemyEffectHit, &selfDamage, &enemyDamage);
                                        }
                                        else if(userResponse == '2'){
                                            respostaValida2 = true;
                                            SkillWasUsed = true;
                                            CalcSkill(allElements, selectedPlayerOnePicomon, 1, selectedPlayerTwoPicomon, &elementalEffectHit, &skillHit, &critHit, &selfEffectHit, &enemyEffectHit, &selfDamage, &enemyDamage);

                                        }
                                        else if(userResponse == '3'){
                                            respostaValida2 = true;
                                            SkillWasUsed = true;
                                            CalcSkill(allElements, selectedPlayerOnePicomon, 2, selectedPlayerTwoPicomon, &elementalEffectHit, &skillHit, &critHit, &selfEffectHit, &enemyEffectHit, &selfDamage, &enemyDamage);

                                        }
                                        else if(userResponse == '4'){
                                            respostaValida2 = true;
                                            SkillWasUsed = true;
                                            CalcSkill(allElements, selectedPlayerOnePicomon, 3, selectedPlayerTwoPicomon, &elementalEffectHit, &skillHit, &critHit, &selfEffectHit, &enemyEffectHit, &selfDamage, &enemyDamage);

                                        }
                                        else{
                                            printf("Comando digitado invalido, digite novamente");
                                            respostaValida2 = false;
                                        }
                                    }
                                }
                                else if(/*escolheu usa Item*/userResponse == '2'){
                                    respostaValida = true;
                                    int flamengo;
                                    for(flamengo = 0; flamengo < pPlayers[playerOneIndex].BagCurrentSize; flamengo++){
                                        printf("Item(%d):\n", flamengo);
                                        ShowItems(&pPlayers[playerOneIndex].Bag[flamengo]);
                                    }
                                    respostaValida2 = false;
                                    while (!respostaValida2){
                                        printf("Selecione um item");
                                        scanf(" %c", &userResponse);
                                        if((int)userResponse - (int)'0' > -1 && (int)userResponse - (int)'0' < pPlayers[playerOneIndex].BagCurrentSize){
                                            ItemWasUsed = true;
                                            UseItem(PlayerOne, PlayerTwo, (int)userResponse - (int)'0', &usedItemStatusHit);
                                            pPlayers[playerOneIndex].Pikocoins -= pPlayers[playerOneIndex].Bag[(int)userResponse - (int)'0'].Value;
                                            SellItemPlayerBag(&pPlayers, playerOneIndex, (int)userResponse - (int)'0');
                                        }
                                        else{
                                            printf("Comando digitado invalido, digite novamente");
                                            respostaValida2 = false;
                                        }
                                    }
                                }
                                else if(/*escolheu troca pikomon*/userResponse == '3'){
                                    respostaValida = true;
                                    reset = true;
                                    int quant;
                                    for (quant = 0; quant < 6; quant++){
                                        printf("Pikomon(%d):\n", quant);
                                        ShowPikomon(&pPlayers[playerOneIndex].BatlePikomons[quant]);
                                    }
                                    respostaValida2 = false;
                                    while (!respostaValida2){
                                        printf("Selecione um pikomon ou digite D para desistir da batalha");
                                        scanf(" %c", &userResponse);
                                        if((int)userResponse - (int)'0' > -1 && (int)userResponse - (int)'0' < 6){
                                            if(pPlayers[playerOneIndex].BatlePikomons[(int)userResponse - (int)'0'].CurrentHP.Total > 0){
                                                respostaValida2 = true;
                                                pPlayers[playerOneIndex].SelectedPikomonIndex = (int)userResponse - (int)'0';
                                            }
                                            else{
                                                printf("este pikomon não consegue mais batalhar, digite novamente");
                                                respostaValida2 = false;
                                            }
                                        }
                                        else if(userResponse == 'D'){
                                            respostaValida = true;
                                            playerOneVictory = false;
                                            battleIsOver = true;
                                        }
                                        else{
                                            printf("Comando digitado invalido, digite novamente");
                                            respostaValida2 = false;
                                        }
                                    }
                                    selectedPlayerOnePicomon = &pPlayers[playerOneIndex].BatlePikomons[pPlayers[playerOneIndex].SelectedPikomonIndex];
                                    selectedPlayerTwoPicomon = &pPlayers[playerTwoIndex].BatlePikomons[pPlayers[playerTwoIndex].SelectedPikomonIndex];
                                    break;
                                }
                                else if(/*escolheu desistir*/userResponse == '4'){
                                    respostaValida = true;
                                    playerOneVictory = false;
                                    battleIsOver = true;
                                    
                                    break;
                                }
                                else{
                                    printf("Comando digitado invalido, digite novamente");
                                    respostaValida = false;
                                }
                            }
                            
                        }
                        else{
                            CalcNextTurn(pPlayers[playerOneIndex].BatlePikomons[pPlayers[playerOneIndex].SelectedPikomonIndex], pPlayers[playerTwoIndex].BatlePikomons[pPlayers[playerTwoIndex].SelectedPikomonIndex], calcNextTurn);
                            MenuBattle(pPlayers[playerTwoIndex].BatlePikomons[pPlayers[playerTwoIndex].SelectedPikomonIndex], pPlayers[playerOneIndex].BatlePikomons[pPlayers[playerOneIndex].SelectedPikomonIndex], calcNextTurn);
                            respostaValida = false;
                            while (!respostaValida){
                                printf("Seu pikomon não consegue mais batalhar, troque o ou desista\n");
                                scanf(" %c", &userResponse);
                                if(/*escolheu troca pikomon*/userResponse == '3'){
                                    respostaValida = true;
                                    reset = true;
                                    int quant;
                                    for (quant = 0; quant < 6; quant++){
                                        printf("Pikomon(%d):\n", quant);
                                        ShowPikomon(&pPlayers[playerOneIndex].BatlePikomons[quant]);
                                    }
                                    respostaValida2 = false;
                                    while (!respostaValida2){
                                        printf("Selecione um pikomon ou digite D para desistir da batalha");
                                        scanf(" %c", &userResponse);
                                        if((int)userResponse - (int)'0' > -1 && (int)userResponse - (int)'0' < 6){
                                            if(pPlayers[playerOneIndex].BatlePikomons[(int)userResponse - (int)'0'].CurrentHP.Total > 0){
                                                respostaValida2 = true;
                                                pPlayers[playerOneIndex].SelectedPikomonIndex = (int)userResponse - (int)'0';
                                            }
                                            else{
                                                printf("este pikomon não consegue mais batalhar, digite novamente");
                                                respostaValida2 = false;
                                            }
                                        }
                                        else if(userResponse == 'D'){
                                            respostaValida = true;
                                            playerOneVictory = false;
                                            battleIsOver = true;
                                        }
                                        else{
                                            printf("Comando digitado invalido, digite novamente");
                                            respostaValida2 = false;
                                        }
                                    }
                                    selectedPlayerOnePicomon = &pPlayers[playerOneIndex].BatlePikomons[pPlayers[playerOneIndex].SelectedPikomonIndex];
                                    selectedPlayerTwoPicomon = &pPlayers[playerTwoIndex].BatlePikomons[pPlayers[playerTwoIndex].SelectedPikomonIndex];
                                    break;
                                }
                                else if(/*escolheu desistir*/userResponse == '4'){
                                    respostaValida = true;
                                    playerOneVictory = false;
                                    battleIsOver = true;
                                    
                                    break;
                                }
                                else{
                                    printf("Comando digitado invalido, digite novamente");
                                    respostaValida = false;
                                }
                            }
                        }

                    }
                    playerOneTurn = false;
                }
                else{
                    selectedPlayerTwoPicomon[0].ChargedSpeed += selectedPlayerTwoPicomon[0].Atributes[7].Total;
                    while(selectedPlayerTwoPicomon[0].ChargedSpeed - turnCost >= 0){
                        selectedPlayerTwoPicomon[0].ChargedSpeed -= turnCost;
                        if(SkillWasUsed){
                            SkillWasUsed = false;
                            if (skillHit)
                            {
                                if (critHit)
                                {
                                    printf("O pikomon acertou um critico! Causando %d ao pokemon inimigo e causando %d a si mesmo", enemyDamage, selfDamage);
                                }
                                else
                                {
                                    printf("O pikomon acertou causando %d ao pokemon inimigo e causando %d a si mesmo", enemyDamage, selfDamage);
                                }
                            }
                            else
                            {
                                printf("O pikomon errou...");
                            }
                            getchar();
                            getchar();                        
                        }
                        if(ItemWasUsed){
                            ItemWasUsed = false;
                            if(selfEffectHit){
                                printf("O efeito do item funcionou");
                            }
                            else{
                                printf("O efeito do item não funcionou");
                            } 
                            getchar();
                            getchar();
                        }
                        PrintPikomonEffects(&pPlayers[playerTwoIndex].BatlePikomons[pPlayers[playerTwoIndex].SelectedPikomonIndex]);
                        getchar();
                        getchar();
                        //acoes do player2
                        if(pPlayers[playerTwoIndex].BatlePikomons[pPlayers[playerTwoIndex].SelectedPikomonIndex].CurrentHP.Total > 0){
                            PassPikomonTurnTime(&pPlayers[playerTwoIndex].BatlePikomons[pPlayers[playerTwoIndex].SelectedPikomonIndex]);
                            CalcNextTurn(pPlayers[playerTwoIndex].BatlePikomons[pPlayers[playerTwoIndex].SelectedPikomonIndex], pPlayers[playerOneIndex].BatlePikomons[pPlayers[playerOneIndex].SelectedPikomonIndex], calcNextTurn);
                            MenuBattle(pPlayers[playerOneIndex].BatlePikomons[pPlayers[playerOneIndex].SelectedPikomonIndex], pPlayers[playerTwoIndex].BatlePikomons[pPlayers[playerTwoIndex].SelectedPikomonIndex], calcNextTurn);
                            respostaValida = false;
                            while (!respostaValida){
                                scanf(" %c", &userResponse);
                                if(/*escolheu usa skill*/userResponse == '1'){
                                    respostaValida = true;
                                    printf("Skill(1):\n");
                                    ShowSkill(&pPlayers[playerTwoIndex].BatlePikomons[pPlayers[playerTwoIndex].SelectedPikomonIndex].Skills[0]);
                                    printf("Skill(2):\n");
                                    ShowSkill(&pPlayers[playerTwoIndex].BatlePikomons[pPlayers[playerTwoIndex].SelectedPikomonIndex].Skills[1]);
                                    printf("Skill(3):\n");
                                    ShowSkill(&pPlayers[playerTwoIndex].BatlePikomons[pPlayers[playerTwoIndex].SelectedPikomonIndex].Skills[2]);
                                    printf("Skill(4):\n");
                                    ShowSkill(&pPlayers[playerTwoIndex].BatlePikomons[pPlayers[playerTwoIndex].SelectedPikomonIndex].Skills[3]);
                                    respostaValida2 = false;
                                    while(!respostaValida2){
                                        printf("Escolha uma skill\n");
                                        scanf(" %c", &userResponse);
                                        if(userResponse == '1'){
                                            respostaValida2 = true;
                                            SkillWasUsed = true;
                                            CalcSkill(allElements, selectedPlayerTwoPicomon, 0, selectedPlayerOnePicomon, &elementalEffectHit, &skillHit, &critHit, &selfEffectHit, &enemyEffectHit, &selfDamage, &enemyDamage);
                                        }
                                        else if(userResponse == '2'){
                                            respostaValida2 = true;
                                            SkillWasUsed = true;
                                            CalcSkill(allElements, selectedPlayerTwoPicomon, 1, selectedPlayerOnePicomon, &elementalEffectHit, &skillHit, &critHit, &selfEffectHit, &enemyEffectHit, &selfDamage, &enemyDamage);

                                        }
                                        else if(userResponse == '3'){
                                            respostaValida2 = true;
                                            SkillWasUsed = true;
                                            CalcSkill(allElements, selectedPlayerTwoPicomon, 2, selectedPlayerOnePicomon, &elementalEffectHit, &skillHit, &critHit, &selfEffectHit, &enemyEffectHit, &selfDamage, &enemyDamage);

                                        }
                                        else if(userResponse == '4'){
                                            respostaValida2 = true;
                                            SkillWasUsed = true;
                                            CalcSkill(allElements, selectedPlayerTwoPicomon, 3, selectedPlayerOnePicomon, &elementalEffectHit, &skillHit, &critHit, &selfEffectHit, &enemyEffectHit, &selfDamage, &enemyDamage);

                                        }
                                        else{
                                            printf("Comando digitado invalido, digite novamente");
                                            respostaValida2 = false;
                                        }
                                    }
                                }
                                else if(/*escolheu usa item*/userResponse == '2'){
                                    respostaValida = true;
                                    int flamengo;
                                    for(flamengo = 0; flamengo < pPlayers[playerTwoIndex].BagCurrentSize; flamengo++){
                                        printf("Item(%d):\n", flamengo);
                                        ShowItems(&pPlayers[playerTwoIndex].Bag[flamengo]);
                                    }
                                    respostaValida2 = false;
                                    while (!respostaValida2){
                                        printf("Selecione um item");
                                        scanf(" %c", &userResponse);
                                        if((int)userResponse - (int)'0' > -1 && (int)userResponse - (int)'0' < pPlayers[playerTwoIndex].BagCurrentSize){
                                            ItemWasUsed = true;
                                            UseItem(&pPlayers[playerTwoIndex], &pPlayers[playerOneIndex], (int)userResponse - (int)'0', &usedItemStatusHit);
                                            pPlayers[playerTwoIndex].Pikocoins -= pPlayers[playerTwoIndex].Bag[(int)userResponse - (int)'0'].Value;
                                            SellItemPlayerBag(&pPlayers, playerTwoIndex, (int)userResponse - (int)'0');
                                        }
                                        else{
                                            printf("Comando digitado invalido, digite novamente");
                                            respostaValida2 = false;
                                        }
                                    }
                                }
                                else if(/*escolheu troca pikomon*/userResponse == '3'){
                                    respostaValida = true;
                                    reset = true;
                                    int quant;
                                    for (quant = 0; quant < 6; quant++){
                                        printf("Pikomon(%d):\n", quant);
                                        ShowPikomon(&pPlayers[playerTwoIndex].BatlePikomons[quant]);
                                    }
                                    respostaValida2 = false;
                                    while (!respostaValida2){
                                        printf("Selecione um pikomon ou digite D para desistir da batalha");
                                        scanf(" %c", &userResponse);
                                        if((int)userResponse - (int)'0' > -1 && (int)userResponse - (int)'0' < 6){
                                            if(pPlayers[playerTwoIndex].BatlePikomons[(int)userResponse - (int)'0'].CurrentHP.Total > 0){
                                                respostaValida2 = true;
                                                pPlayers[playerTwoIndex].SelectedPikomonIndex = (int)userResponse - (int)'0';
                                            }
                                            else{
                                                printf("este pikomon não consegue mais batalhar, digite novamente");
                                                respostaValida2 = false;
                                            }
                                        }
                                        else if(userResponse == 'D'){
                                            respostaValida = true;
                                            playerOneVictory = true;
                                            battleIsOver = true;
                                        }
                                        else{
                                            printf("Comando digitado invalido, digite novamente");
                                            respostaValida2 = false;
                                        }
                                    }
                                    selectedPlayerOnePicomon = &pPlayers[playerOneIndex].BatlePikomons[pPlayers[playerOneIndex].SelectedPikomonIndex];
                                    selectedPlayerTwoPicomon = &pPlayers[playerTwoIndex].BatlePikomons[pPlayers[playerTwoIndex].SelectedPikomonIndex];
                                    break;
                                }
                                else if(/*escolheu desistir*/userResponse == '4'){
                                    respostaValida = true;
                                    playerOneVictory = true;
                                    battleIsOver = true;
                                    
                                    break;
                                }
                                else{
                                    printf("Comando digitado invalido, digite novamente");
                                    respostaValida = false;
                                }
                            }
                        }
                        else{
                            CalcNextTurn(pPlayers[playerTwoIndex].BatlePikomons[pPlayers[playerTwoIndex].SelectedPikomonIndex], pPlayers[playerOneIndex].BatlePikomons[pPlayers[playerOneIndex].SelectedPikomonIndex], calcNextTurn);
                            MenuBattle(pPlayers[playerOneIndex].BatlePikomons[pPlayers[playerOneIndex].SelectedPikomonIndex], pPlayers[playerTwoIndex].BatlePikomons[pPlayers[playerTwoIndex].SelectedPikomonIndex], calcNextTurn);
                            respostaValida = false;
                            while (!respostaValida){
                                printf("Seu pikomon não consegue mais batalhar, troque o ou desista\n");
                                scanf(" %c", &userResponse);
                                if(/*escolheu troca pikomon*/userResponse == '3'){
                                    respostaValida = true;
                                    reset = true;
                                    int quant;
                                    for (quant = 0; quant < 6; quant++){
                                        printf("Pikomon(%d):\n", quant);
                                        ShowPikomon(&pPlayers[playerTwoIndex].BatlePikomons[quant]);
                                    }
                                    respostaValida2 = false;
                                    while (!respostaValida2){
                                        printf("Selecione um pikomon ou digite D para desistir da batalha");
                                        scanf(" %c", &userResponse);
                                        if((int)userResponse - (int)'0' > -1 && (int)userResponse - (int)'0' < 6){
                                            if(pPlayers[playerTwoIndex].BatlePikomons[(int)userResponse - (int)'0'].CurrentHP.Total > 0){
                                                respostaValida2 = true;
                                                pPlayers[playerTwoIndex].SelectedPikomonIndex = (int)userResponse - (int)'0';
                                            }
                                            else{
                                                printf("este pikomon não consegue mais batalhar, digite novamente");
                                                respostaValida2 = false;
                                            }
                                        }
                                        else if(userResponse == 'D'){
                                            respostaValida = true;
                                            playerOneVictory = true;
                                            battleIsOver = true;
                                        }
                                        else{
                                            printf("Comando digitado invalido, digite novamente");
                                            respostaValida2 = false;
                                        }
                                    }
                                    selectedPlayerOnePicomon = &pPlayers[playerOneIndex].BatlePikomons[pPlayers[playerOneIndex].SelectedPikomonIndex];
                                    selectedPlayerTwoPicomon = &pPlayers[playerTwoIndex].BatlePikomons[pPlayers[playerTwoIndex].SelectedPikomonIndex];
                                    break;
                                }
                                else if(/*escolheu desistir*/userResponse == '4'){
                                    respostaValida = true;
                                    playerOneVictory = true;
                                    battleIsOver = true;
                                    
                                    break;
                                }
                                else{
                                    printf("Comando digitado invalido, digite novamente");
                                    respostaValida = false;
                                }
                            }
                        }

                    }
                    playerOneTurn = true;
                }
            }
            if(playerOneVictory){
                pPlayers[playerOneIndex].Pikocoins += 100;

            }
            else{
                pPlayers[playerTwoIndex].Pikocoins += 100;

            }

        }
        if (respostaUserMP == 2)
        {
            char EntradaPlayerDebug[20];
            int indexPlayerLoop;
            printf("Qual o username do jogador: ");
            scanf(" %19s", EntradaPlayerDebug);
            for (indexPlayerLoop = 0; indexPlayerLoop < dataQuantities.Player; indexPlayerLoop++)
            {
                if (strcmp(pPlayers[indexPlayerLoop].Name, EntradaPlayerDebug) == 0)
                {
                    break;
                }
            }
            DebugPlayers(pPlayers,indexPlayerLoop,dataQuantities.Player);
            printf("Press Enter");
            getchar();
            getchar();
        }
        if (respostaUserMP == 3)
        {
            DebugPikomons(pPikomons, -1, dataQuantities.Pikomon);
            printf("Press Enter");
            getchar();  
            getchar();
        }
        if (respostaUserMP == 4)
        {
            break;
        }
    }
    //------------------------------------------------------------------------------------------------------------------//
    //toda a vez que acaba uma batalha tem que usar essa funcao
    FreeAllHeapMemoryAndSaveEverything(pSkills,pItems,pPikomons,&pPlayers,dataQuantities,dataQuantity,skills,items,pikomoms,players);
    return 0;
}


//Debug/Print/Base Functions
//------------------------------------------------------------------------------//
bool DebugPlayers(Player *pPlayers, int index, int playersQuantity){
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
                               pPlayers[index].PikomonsStorage[j].Atributes[0].Base,
                               pPlayers[index].PikomonsStorage[j].Atributes[4].Base,
                               pPlayers[index].PikomonsStorage[j].Atributes[1].Base,
                               pPlayers[index].PikomonsStorage[j].Atributes[6].Base,
                               pPlayers[index].PikomonsStorage[j].Atributes[2].Base,
                               pPlayers[index].PikomonsStorage[j].Atributes[7].Base,
                               pPlayers[index].PikomonsStorage[j].Atributes[3].Base,
                               pPlayers[index].PikomonsStorage[j].Atributes[5].Base);

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
                           pPlayers[index].PikomonsStorage[j].Atributes[0].Base,
                           pPlayers[index].PikomonsStorage[j].Atributes[4].Base,
                           pPlayers[index].PikomonsStorage[j].Atributes[1].Base,
                           pPlayers[index].PikomonsStorage[j].Atributes[6].Base,
                           pPlayers[index].PikomonsStorage[j].Atributes[2].Base,
                           pPlayers[index].PikomonsStorage[j].Atributes[7].Base,
                           pPlayers[index].PikomonsStorage[j].Atributes[3].Base,
                           pPlayers[index].PikomonsStorage[j].Atributes[5].Base);

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

bool DebugPikomons(PiPointer pPikomon, int index, int pikomonsQuantity){
    int j, k;

    if (index == -1){
        for (index = 0; index < pikomonsQuantity; index++){
            /*char Name[20];
            char IconImg[7][25];
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

            Skill Skills[4];*/
            printf("IconImg:\n");
            for (j = 0; j < 7; j++){
                printf("| %s |\n", pPikomon[index].IconImg[j]);
            }
            printf("\n");
            printf("|Index:| %2d | |,| Name:| %10s | |\n",index,pPikomon[index].Name);
            printf("|Passive:| %20s | |\n | PassiveDescription: |\n %s\n%s\n%s\n",pPikomon[index].Passive,pPikomon[index].PassiveDescription[0],pPikomon[index].PassiveDescription[1],pPikomon[index].PassiveDescription[2]);
            printf("| Value: | %3d | |\n",pPikomon[index].Value);
            printf("| Element: |\n");
            printf("| name: | %s | |,| Acronym: | %4s | |\n",pPikomon[index].Element.Name,pPikomon[index].Element.Acronym);
            for (int p = 0; p < 10; p++){
                printf("| Effectiveness[%d]: | %d | | ",p,pPikomon[index].Element.Effectiveness[p]);
            }
            printf("\n");
            printf("|ElementalDamageScale: | %2d | |\n",pPikomon[index].Element.ElementalDamageScale);
            printf("|SelfElementIndex: | %2d | |\n",pPikomon[index].Element.SelfElementIndex);
            printf("|CurrentHPEffect|\n");
            printf("| Acronym: | %4s | |,| quantity: | %2d | |,| Timer: | | %2d |",pPikomon[index].Element.CurrentHPEffect.Acronym,pPikomon[index].Element.CurrentHPEffect.Quantity,pPikomon[index].Element.CurrentHPEffect.Timer);
            for (int p = 0; p < 8; p++){
            printf("|StatusEffect[%2d]|\n",p);
            printf("| Acronym: | %s | |,| quantity: | %2d | |,| Timer: | | %2d |",pPikomon[index].Element.StatusEffect[p].Acronym,pPikomon[index].Element.StatusEffect[p].Quantity,pPikomon[index].Element.StatusEffect[p].Timer);
            }
            DebugPersonality(&pPikomon[index].Personality, 0);
            printf("|CurrentHP|\n");
            printf("| Name: | %20s | |,| Base: | %2d | |,| Total: | | %2d |,| BonusQuantity: | | %2d |",pPikomon[index].CurrentHP.Name,pPikomon[index].CurrentHP.Base,pPikomon[index].CurrentHP.Total,pPikomon[index].CurrentHP.BonusQuantity);
            for (int p = 0; p < 8; p++){
            printf("|Atributes[%2d]|\n",p);
            printf("| Name: | %20s | |,| Base: | %2d | |,| Total: | | %2d |,| BonusQuantity: | | %2d |",pPikomon[index].Atributes[p].Name,pPikomon[index].Atributes[p].Base,pPikomon[index].Atributes[p].Total,pPikomon[index].Atributes[p].BonusQuantity);
            }


            /*
            printf("| Index:| %2d | |,| Nome:| %10s | |,| Element:| %10s | |,| HP:| %3d | |,| Atk:| %3d | |,| Def:| %3d | |,| SpA:| %3d | |,| SpD:| %3d | |,| Spd:| %3d |,| Acu:| %3d |,| Eac:| %3d | |\n",
                    index,
                    pPikomon[index].Name,
                    pPikomon[index].Element.Name,
                    pPikomon[index].Atributes[0].Base,
                    pPikomon[index].Atributes[4].Base,
                    pPikomon[index].Atributes[1].Base,
                    pPikomon[index].Atributes[6].Base,
                    pPikomon[index].Atributes[2].Base,
                    pPikomon[index].Atributes[7].Base,
                    pPikomon[index].Atributes[3].Base,
                    pPikomon[index].Atributes[5].Base);


            // Exibir IconImg do Pikomon


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
            }*/
        }
    }
    else if (index >= 0 && index < pikomonsQuantity){
        // Exibir apenas um Pikomon
        if (strlen(pPikomon[index].Name) > 0)
        {
            printf("IconImg:\n");
            for (j = 0; j < 7; j++){
                printf("| %s |\n", pPikomon[index].IconImg[j]);
            }
            printf("\n");
            printf("|Index:| %2d | |,| Name:| %10s | |\n",index,pPikomon[index].Name);
            printf("|Passive:| %20s | |\n | PassiveDescription: |\n %s\n%s\n%s\n",pPikomon[index].Passive,pPikomon[index].PassiveDescription[0],pPikomon[index].PassiveDescription[1],pPikomon[index].PassiveDescription[2]);
            printf("| Value: | %3d | |\n",pPikomon[index].Value);
            printf("| Element: |\n");
            printf("| name: | %s | |,| Acronym: | %4s | |\n",pPikomon[index].Element.Name,pPikomon[index].Element.Acronym);
            for (int p = 0; p < 10; p++){
                printf("| Effectiveness[%d]: | %d | | ",p,pPikomon[index].Element.Effectiveness[p]);
            }
            printf("\n");
            printf("|ElementalDamageScale: | %2d | |\n",pPikomon[index].Element.ElementalDamageScale);
            printf("|SelfElementIndex: | %2d | |\n",pPikomon[index].Element.SelfElementIndex);
            printf("|CurrentHPEffect|\n");
            printf("| Acronym: | %4s | |,| quantity: | %2d | |,| Timer: | | %2d |",pPikomon[index].Element.CurrentHPEffect.Acronym,pPikomon[index].Element.CurrentHPEffect.Quantity,pPikomon[index].Element.CurrentHPEffect.Timer);
            for (int p = 0; p < 8; p++){
            printf("|StatusEffect[%2d]|\n",p);
            printf("| Acronym: | %s | |,| quantity: | %2d | |,| Timer: | | %2d |",pPikomon[index].Element.StatusEffect[p].Acronym,pPikomon[index].Element.StatusEffect[p].Quantity,pPikomon[index].Element.StatusEffect[p].Timer);
            }
            DebugPersonality(&pPikomon[index].Personality, 0);
            printf("|CurrentHP|\n");
            printf("| Name: | %20s | |,| Base: | %2d | |,| Total: | | %2d |,| BonusQuantity: | | %2d |",pPikomon[index].CurrentHP.Name,pPikomon[index].CurrentHP.Base,pPikomon[index].CurrentHP.Total,pPikomon[index].CurrentHP.BonusQuantity);
            for (int p = 0; p < 8; p++){
            printf("|Atributes[%2d]|\n",p);
            printf("| Name: | %20s | |,| Base: | %2d | |,| Total: | | %2d |,| BonusQuantity: | | %2d |",pPikomon[index].Atributes[p].Name,pPikomon[index].Atributes[p].Base,pPikomon[index].Atributes[p].Total,pPikomon[index].Atributes[p].BonusQuantity);
            }
            /*
            printf("| Nome:| %10s | |,| Element:| %10s | |,| HP:| %3d | |,| Atk:| %3d | |,| Def:| %3d | |,| SpA:| %3d | |,| SpD:| %3d | |,| Spd:| %3d |,| Acu:| %3d |,| Eac:| %3d | |\n",
                   pPikomon[index].Name,
                   pPikomon[index].Element.Name,
                   pPikomon[index].Atributes[0].Base,
                   pPikomon[index].Atributes[4].Base,
                   pPikomon[index].Atributes[1].Base,
                   pPikomon[index].Atributes[6].Base,
                   pPikomon[index].Atributes[2].Base,
                   pPikomon[index].Atributes[7].Base,
                   pPikomon[index].Atributes[3].Base,
                   pPikomon[index].Atributes[5].Base);



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
            }*/
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

bool ShowSkill(SkPointer skill){
    if (skill == NULL) {
        perror("Skill inválida.");
        return false;
    }

    printf("Skill:\n");
    printf("Name: %s | Type: %s | Target: %c | ElementEffectChance: %d | AttackBase: %d | AttackScale: %d | MagicBase: %d | MagicAttackScale: %d | CritChance: %d | EffectTarget: %c | EnemyEffectHitChance: %d | SelfEffectHitChance: %d\n",
           skill[0].Name,
           skill[0].Type,
           skill[0].Target,
           skill[0].ElementEffectHitChance,
           skill[0].AttackBase,
           skill[0].AttackScale,
           skill[0].MagicBase,
           skill[0].MagicAttackScale,
           skill[0].CritChance,
           skill[0].EffectTarget,
           skill[0].EnemyEffectHitChance,
           skill[0].SelfEffectHitChance);


    for (int j = 0; j < 3; j++)
    {
        printf("| Description[%d]: %s |\n", j, skill[0].Description[j]);
    }


    printf("| Active: %s |\n", skill[0].Active);
    for (int j = 0; j < 3; j++)
    {
        printf("| ActiveDescription[%d]: %s |\n", j, skill[0].ActiveDescription[j]);
    }


    printf("| Enemy Effects:\n");
    for (int k = 0; k < 8; k++)
    {
        if (skill[0].EnemyEffect[k].Quantity > 0)
        {
            printf("| Enemy Effect[%d] | Quantity: %d | Timer: %d |\n",
                   k,
                   skill[0].EnemyEffect[k].Quantity,
                   skill[0].EnemyEffect[k].Timer);
        }
    }


    printf("| Self Effects:\n");
    for (int k = 0; k < 8; k++)
    {
        if (skill[0].SelfEffect[k].Quantity > 0)
        {
            printf("| Self Effect[%d] | Quantity: %d | Timer: %d |\n",
                   k,
                   skill[0].SelfEffect[k].Quantity,
                   skill[0].SelfEffect[k].Timer);
        }
    }

    return true;
}

bool ShowItems(ItPointer pItem){
    if (pItem == NULL) {
        perror("Item inválido.");
        return false;
    }

    printf("| Item | Name: %s | Type: %s | Value: %d |\n",
           pItem[0].Name,
           pItem[0].Type,
           pItem[0].Value);

    for (int j = 0; j < 3; j++)
    {
        printf("| Description[%d]: %s |\n", j, pItem[0].Description[j]);
    }

    printf("| Active: %s |\n", pItem[0].Active);
    for (int j = 0; j < 3; j++)
    {
        printf("| ActiveDescription[%d]: %s |\n", j, pItem[0].ActiveDescription[j]);
    }

    printf("| Current HP Damage Is Physic: %s |\n", pItem[0].CurrentHPDamageIsPhysic ? "true" : "false");
    printf("| EffectCurrentHPTarget: %c |\n", pItem[0].EffectCurrentHPTarget);
    printf("| EffectCurrentHP | Quantity: %d | Timer: %d |\n",
           pItem[0].EffectCurrentHP.Quantity,
           pItem[0].EffectCurrentHP.Timer);

    printf("| EffectTarget: %c |\n", pItem[0].EffectTarget);
    printf("| Status Effect Chance: %d |\n", pItem[0].StatusEffectChance);

    for (int k = 0; k < 8; k++)
    {
        if (pItem[0].StatusEffect[k].Quantity > 0)
        {
            printf("| Status Effect[%d] | Quantity: %d | Timer: %d |\n",
                   k,
                   pItem[0].StatusEffect[k].Quantity,
                   pItem[0].StatusEffect[k].Timer);
        }
    }

    return true;
}

bool ShowPikomon(PiPointer pPikomon){
    int j, k;

    if (pPikomon == NULL || strlen(pPikomon[0].Name) == 0)
    {
        printf("Pikomon inválido.\n");
        return false;
    }

    printf("| Nome:| %10s | |,| Element:| %10s | |,| HP:| %3d | |,| Atk:| %3d | |,| Def:| %3d | |,| SpA:| %3d | |,| SpD:| %3d | |,| Spd:| %3d |,| Acu:| %3d |,| Eac:| %3d | |\n",
           pPikomon[0].Name,
           pPikomon[0].Element.Name,
           pPikomon[0].Atributes[0].Base,
           pPikomon[0].Atributes[4].Base,
           pPikomon[0].Atributes[1].Base,
           pPikomon[0].Atributes[6].Base,
           pPikomon[0].Atributes[2].Base,
           pPikomon[0].Atributes[7].Base,
           pPikomon[0].Atributes[3].Base,
           pPikomon[0].Atributes[5].Base);

    printf("IconImg:\n");
    for (j = 0; j < 7; j++)
    {
        printf("| %s |\n", pPikomon[0].IconImg[j]);
    }
    printf("\n");

    for (k = 0; k < 4; k++)
    {
        if (strlen(pPikomon[0].Skills[k].Name) > 0)
        {
            printf("| Skill Name:| %20s | |,| Target:| %c | |,| AttackScale:| %d | |,| MagicAttackScale:| %d | |;\n",
                   pPikomon[0].Skills[k].Name,
                   pPikomon[0].Skills[k].Target,
                   pPikomon[0].Skills[k].AttackScale,
                   pPikomon[0].Skills[k].MagicAttackScale);
        }
    }

    return true;
}
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
void FreeAllHeapMemoryAndSaveEverything(SkPointer pSkills, ItPointer pItems, PiPointer pPikomons, PlPointer *pPlayers, DataQuantity dataquantities, const char *dataQuantity, const char *skills, const char *items, const char *pikomoms, const char *players){
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
        for(j = 0; j < (*pPlayers)[i].BagCurrentSize; j++){
            (*pPlayers)[i].Pikocoins += (*pPlayers)[i].Bag[j].Value;
        }
        (*pPlayers)[i].BagCurrentSize = 0;
        free((*pPlayers)[i].Bag);
        for(j = 0; j < 6; j++){
            for(k = 0; k < 8; k++){
                for(I = 0; I < (*pPlayers)[i].BatlePikomons[j].Atributes[k].BonusQuantity; I++){
                    free((*pPlayers)[i].BatlePikomons[j].Atributes[k].acronym[I]);
                    (*pPlayers)[i].BatlePikomons[j].Atributes[k].acronym[I] = NULL;
                }
                free((*pPlayers)[i].BatlePikomons[j].Atributes[k].Bonus);
                (*pPlayers)[i].BatlePikomons[j].Atributes[k].Bonus = NULL;
                free((*pPlayers)[i].BatlePikomons[j].Atributes[k].BonusTimer);
                (*pPlayers)[i].BatlePikomons[j].Atributes[k].BonusTimer = NULL;
                (*pPlayers)[i].BatlePikomons[j].Atributes[k].BonusQuantity = 0;
            }
        }
        //esse cara aqui pode dar ruim
        memset((*pPlayers)[i].BatlePikomons, 0, 6 * sizeof(Pikomon));
    }
    SavePlayers((*pPlayers), dataquantities.Player, players);
    free((*pPlayers));
}
//------------------------------------------------------------------------------//



//Manage Memory Functions
//------------------------------------------------------------------------------//
bool AddSkill(SkPointer *pSkills, DataQuantity *dataQuantities, char *name, char *type, char description[3][255], char *active, char activeDescription[3][255], bool learnablePersonalities[13], bool LearnableElements[10], int elementEffectHitChance, Element element, char target, int hitChance, int  attackBase, int attackScale, int magicBase, int magicAttackScale, int critChance, char effectTarget, int enemyEffectHitChance, Effect enemyEffect[8], int selfEffectHitChance, Effect selfEffect[8]){
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
    if(elementEffectHitChance < 0){
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
    if(enemyEffectHitChance < 0){
        perror("ERRO, \"enemyEffectChance\" não pode ser menor que zero em \"AddSkill\"");
        return false;
    }
    if(selfEffectHitChance < 0){
        perror("ERRO, \"selfEffectChance\" não pode ser menor que zero em \"AddSkill\"");
        return false;
    }

    dataQuantities[0].Skill++;
    (*pSkills) = (SkPointer)realloc((*pSkills), dataQuantities[0].Skill * sizeof(Skill));
    if(pSkills == NULL){
        perror("ERRO na realocacao de memoria em \"AddSkill\"");
        return false;
    }
    memset(&(*pSkills)[dataQuantities[0].Skill-1], 0, sizeof(Skill));
    strcpy((*pSkills)[dataQuantities[0].Skill-1].Name, name);
    strcpy((*pSkills)[dataQuantities[0].Skill-1].Type, type);
    int i;
    for(i = 0; i < 3; i++){
        strcpy((*pSkills)[dataQuantities[0].Skill-1].Description[i], description[i]);
    }
    strcpy((*pSkills)[dataQuantities[0].Skill-1].Active, active);
    for(i = 0; i < 3; i++){
        strcpy((*pSkills)[dataQuantities[0].Skill-1].ActiveDescription[i], activeDescription[i]);
    }
    for(i = 0; i < 13; i++){
        (*pSkills)[dataQuantities[0].Skill-1].LearnablePersonalities[i] = learnablePersonalities[i];
    }
    for(i = 0; i < 10; i++){
        (*pSkills)[dataQuantities[0].Skill-1].LearnableElements[i] = LearnableElements[i];
    }
    (*pSkills)[dataQuantities[0].Skill-1].ElementEffectHitChance = elementEffectHitChance;
    (*pSkills)[dataQuantities[0].Skill-1].Element = element;
    (*pSkills)[dataQuantities[0].Skill-1].Target = target;
    (*pSkills)[dataQuantities[0].Skill-1].AttackBase = attackBase;
    (*pSkills)[dataQuantities[0].Skill-1].AttackScale = attackScale;
    (*pSkills)[dataQuantities[0].Skill-1].MagicBase = magicBase;
    (*pSkills)[dataQuantities[0].Skill-1].MagicAttackScale = magicAttackScale;
    (*pSkills)[dataQuantities[0].Skill-1].CritChance = critChance;
    (*pSkills)[dataQuantities[0].Skill-1].EffectTarget = effectTarget;
    (*pSkills)[dataQuantities[0].Skill-1].EnemyEffectHitChance = enemyEffectHitChance;
    for(i = 0; i < 8; i++){
        (*pSkills)[dataQuantities[0].Skill-1].EnemyEffect[i] = enemyEffect[i];
    }
    (*pSkills)[dataQuantities[0].Skill-1].SelfEffectHitChance = selfEffectHitChance;
    for(i = 0; i < 8; i++){
        (*pSkills)[dataQuantities[0].Skill-1].SelfEffect[i] = selfEffect[i];
    }
    return true;
}

bool AddItem(ItPointer *pItems, DataQuantity *dataQuantities, char *name, char *type, char description[3][255], char *active, char activeDescription[3][255], int value, bool currentHPDamageIsPysic, char effectCurrentHPTarget, Effect EffectCurrentHP, char effectTarget, int StatusEffectChance, Effect StatusEffect[8]){
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

    /*char Name[20];
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
    Effect StatusEffect[8];*/
    dataQuantities[0].Item++;
    (*pItems) = (ItPointer)realloc((*pItems), dataQuantities[0].Item * sizeof(Item));
    if(pItems == NULL){
        perror("ERRO na realocacao de memoria em \"AddItem\"");
        return false;
    }
    memset(&(*pItems)[dataQuantities[0].Item-1], 0, sizeof(Item));
    strcpy((*pItems)[dataQuantities[0].Item-1].Name, name);
    strcpy((*pItems)[dataQuantities[0].Item-1].Type, type);
    for(i = 0; i < 3; i++){
        strcpy((*pItems)[dataQuantities[0].Item-1].Description[i], description[i]);
    }
    strcpy((*pItems)[dataQuantities[0].Item-1].Active, active);
    for(i = 0; i < 3; i++){
        strcpy((*pItems)[dataQuantities[0].Item-1].ActiveDescription[i], activeDescription[i]);
    }
    (*pItems)[dataQuantities[0].Item-1].Value = value;
    (*pItems)[dataQuantities[0].Item-1].CurrentHPDamageIsPhysic = currentHPDamageIsPysic;
    (*pItems)[dataQuantities[0].Item-1].EffectCurrentHPTarget = effectCurrentHPTarget;
    (*pItems)[dataQuantities[0].Item-1].EffectCurrentHP = EffectCurrentHP;
    (*pItems)[dataQuantities[0].Item-1].EffectTarget = effectTarget;
    (*pItems)[dataQuantities[0].Item-1].StatusEffectChance = StatusEffectChance;
    for(i = 0; i < 8; i++){
        (*pItems)[dataQuantities[0].Item-1].StatusEffect[i] = StatusEffect[i];
    }
    return true;

}

bool AddPikomon(PiPointer *pPikomons, DataQuantity *dataQuantities, char *name, Element element, char iconImg[7][25], char passive[20], char passiveDescription[3][255], int value, int BaseHP, int BaseDefense, int BaseMagicDefense, int BaseAccuracy, int BaseAttack, int BaseElementalAccuracy, int BaseMagicAttack, int BaseSpeed){
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
    if(BaseAccuracy == 0){
        perror("ERRO, \"BaseAccuracy\" não pode ser 0 em \"AddPikomon\"");
        return false;
    }
    if (BaseAttack == 0) {
        perror("ERRO, \"BaseAttack\" não pode ser 0 em \"AddPikomon\"");
        return false;
    }
    if(BaseElementalAccuracy == 0){
        perror("ERRO, \"BaseElementalAccuracy\" não pode ser 0 em \"AddPikomon\"");
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

    dataQuantities[0].Pikomon++;
    (*pPikomons) = (PiPointer)realloc((*pPikomons), dataQuantities[0].Pikomon * sizeof(Pikomon));
    if(pPikomons == NULL){
        perror("ERRO na realocacao de memoria em \"AddPikomon\"");
        return false;
    }
    memset(&(*pPikomons)[dataQuantities[0].Pikomon-1], 0, sizeof(Pikomon));
    strcpy((*pPikomons)[dataQuantities[0].Pikomon-1].Name, name);
    (*pPikomons)[dataQuantities[0].Pikomon-1].Element = element;
    int i;
    for(i = 0; i < 7; i++){
        strcpy((*pPikomons)[dataQuantities[0].Pikomon-1].IconImg[i], iconImg[i]);
    }
    strcpy((*pPikomons)[dataQuantities[0].Pikomon-1].Passive, passive);
    for(i = 0; i < 3; i++){
        strcpy((*pPikomons)[dataQuantities[0].Pikomon-1].PassiveDescription[i], passiveDescription[i]);
    }
    (*pPikomons)[dataQuantities[0].Pikomon-1].Value = value;
    strcpy((*pPikomons)[dataQuantities[0].Pikomon-1].CurrentHP.Name, "CurrentHP");
    (*pPikomons)[dataQuantities[0].Pikomon-1].CurrentHP.Base = 0;
    (*pPikomons)[dataQuantities[0].Pikomon-1].CurrentHP.acronym = NULL;
    (*pPikomons)[dataQuantities[0].Pikomon-1].CurrentHP.Bonus = NULL;
    (*pPikomons)[dataQuantities[0].Pikomon-1].CurrentHP.BonusTimer = NULL;
    strcpy((*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[0].Name, "HP");
    (*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[0].Base = BaseHP;
    (*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[0].acronym = NULL;
    (*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[0].Bonus = NULL;
    (*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[0].BonusTimer = NULL;
    strcpy((*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[1].Name, "Defense");
    (*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[1].Base = BaseDefense;
    (*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[1].acronym = NULL;
    (*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[1].Bonus = NULL;
    (*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[1].BonusTimer = NULL;
    strcpy((*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[2].Name, "MagicDefense");
    (*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[2].Base = BaseMagicDefense;
    (*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[2].acronym = NULL;
    (*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[2].Bonus = NULL;
    (*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[2].BonusTimer = NULL;
    strcpy((*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[3].Name, "Accuracy");
    (*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[3].Base = BaseAccuracy;
    (*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[3].acronym = NULL;
    (*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[3].Bonus = NULL;
    (*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[3].BonusTimer = NULL;
    strcpy((*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[4].Name, "Attack");
    (*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[4].Base = BaseAttack;
    (*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[4].acronym = NULL;
    (*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[4].Bonus = NULL;
    (*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[4].BonusTimer = NULL;
    strcpy((*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[5].Name, "ElementalAccuracy");
    (*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[5].Base = BaseElementalAccuracy;
    (*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[5].acronym = NULL;
    (*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[5].Bonus = NULL;
    (*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[5].BonusTimer = NULL;
    strcpy((*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[6].Name, "MagicAttack");
    (*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[6].Base = BaseMagicAttack;
    (*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[6].acronym = NULL;
    (*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[6].Bonus = NULL;
    (*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[6].BonusTimer = NULL;
    strcpy((*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[7].Name, "Speed");
    (*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[7].Base = BaseSpeed;
    (*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[7].acronym = NULL;
    (*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[7].Bonus = NULL;
    (*pPikomons)[dataQuantities[0].Pikomon-1].Atributes[7].BonusTimer = NULL;
    return true;
}

bool AddPlayer(PlPointer *pPlayers, DataQuantity *dataQuantities, char *name, char *pass){
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
    
    dataQuantities[0].Player++;
    (*pPlayers) = (PlPointer)realloc((*pPlayers), dataQuantities[0].Player * sizeof(Player));
    if (*pPlayers == NULL) {
        perror("ERRO na realocação de memória em \"AddPlayers\"");
        return false;
    }
    memset(&(*pPlayers)[dataQuantities[0].Player - 1], 0, sizeof(Player));
    strcpy((*pPlayers)[dataQuantities[0].Player - 1].Name, name);
    strcpy((*pPlayers)[dataQuantities[0].Player - 1].Pass, pass);
    (*pPlayers)[dataQuantities[0].Player - 1].Pikocoins = 50;
    return true;
}

bool AddItemPlayerBag(PlPointer pPlayers, int playerIndex, ItPointer pItems, int itemIndex){
    if (pPlayers == NULL || pPlayers == NULL) {
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

    // Aumenta o tamanho atual da bag do jogador
    pPlayers[playerIndex].BagCurrentSize++;
    pPlayers[playerIndex].Bag = (ItPointer)realloc(pPlayers[playerIndex].Bag, pPlayers[playerIndex].BagCurrentSize * sizeof(Item));
    if (pPlayers[playerIndex].Bag == NULL) {
        perror("ERRO na realocação de memória em \"AddItemPlayerBag\"");
        return false;
    }

    // Adiciona o item à bag do jogador
    pPlayers[playerIndex].Bag[pPlayers[playerIndex].BagCurrentSize - 1] = pItems[itemIndex];
    return true; 
}

bool StorePikomonPlayer(PlPointer *pPlayers, int playerIndex, int storagePikomonPlacementIndex, PiPointer pPikomons, int pikomonIndex, DataQuantity dataQuantities){
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
    (*pPlayers)[playerIndex].PikomonsStorage[storagePikomonPlacementIndex] = pPikomons[pikomonIndex];
    return true;
}

bool RemoveSkill(SkPointer *pSkills, DataQuantity *dataQuantities, int indexRemove){
    if(indexRemove < 0){
        perror("ERRO, \"indexRemove\" não pode ser menor que zero em \"RemoveSkill\"");
        return false;
    }
    else if(indexRemove >= dataQuantities[0].Skill){
        perror("ERRO, \"indexRemove\" tem que ser menor que \"dataQuantities.Skill\" em \"RemoveSkill\"");
        return false;
    }
    SkPointer tempSkills;
    tempSkills = (SkPointer)calloc(dataQuantities[0].Skill-1, sizeof(Skill));
    int i, j = 0;
    for(i = 0; i < dataQuantities[0].Skill; i++){
        if(i != indexRemove){
            tempSkills[j] = (*pSkills)[i];
            j++;
        }
    }
    dataQuantities[0].Skill--;
    (*pSkills) = (SkPointer)realloc((*pSkills), dataQuantities[0].Skill * sizeof(Skill));
    for(i = 0; i < dataQuantities[0].Skill; i++){
        (*pSkills)[i] = tempSkills[i];
    }
    free(tempSkills);
    return true;
}

bool RemoveItem(ItPointer *pItems, DataQuantity *dataQuantities, int indexRemove){
    if(indexRemove < 0){
        perror("ERRO, \"indexRemove\" não pode ser menor que zero em \"RemoveItem\"");
        return false;
    }
    else if(indexRemove >= dataQuantities[0].Item){
        perror("ERRO, \"indexRemove\" tem que ser menor que \"dataQuantities.Item\" em \"RemoveItem\"");
        return false;
    }
    ItPointer tempItems;
    tempItems = (ItPointer)calloc(dataQuantities[0].Item-1, sizeof(Item));
    int i, j = 0;
    for(i = 0; i < dataQuantities[0].Item; i++){
        if(i != indexRemove){
            tempItems[j] = (*pItems)[i];
            j++;
        }
    }
    dataQuantities[0].Item--;
    (*pItems) = (ItPointer)realloc((*pItems), dataQuantities[0].Item * sizeof(Item));
    for(i = 0; i < dataQuantities[0].Item; i++){
        (*pItems)[i] = tempItems[i];
    }
    free(tempItems);
    return true;
}

bool RemovePikomon(PiPointer *pPikomons, DataQuantity *dataQuantities, int indexRemove){
    if(indexRemove < 0){
        perror("ERRO, \"indexRemove\" não pode ser menor que zero em \"RemovePikomon\"");
        return false;
    }
    else if(indexRemove >= dataQuantities[0].Pikomon){
        perror("ERRO, \"indexRemove\" tem que ser menor que \"dataQuantities.Pikomon\" em \"RemovePikomon\"");
        return false;
    }
    PiPointer tempPikomons;
    tempPikomons = (PiPointer)calloc(dataQuantities[0].Pikomon-1, sizeof(Pikomon));
    int i, j = 0;
    for(i = 0; i < dataQuantities[0].Pikomon; i++){
        if(i != indexRemove){
            tempPikomons[j] = (*pPikomons)[i];
            j++;
        }
    }
    dataQuantities[0].Pikomon--;
    (*pPikomons) = (PiPointer)realloc((*pPikomons), dataQuantities[0].Pikomon * sizeof(Pikomon));
    for(i = 0; i < dataQuantities[0].Pikomon; i++){
        (*pPikomons)[i] = tempPikomons[i];
    }
    free(tempPikomons);
    return true;
}

bool SellItemPlayerBag(PlPointer *pPlayers, int playerIndex, int bagSellIndex){ 
    if(bagSellIndex >= (*pPlayers)[playerIndex].BagCurrentSize){
        perror("ERRO, \"bagSellIndex\" tem que ser menor que \"pPlayers[playerIndex].BagCurrentSize\" em \"SellItemPlayerBag\"");
        return false;
    }
    else if(bagSellIndex < 0){
        perror("ERRO, \"bagSellIndex\" não pode ser menor que zero em \"SellItemPlayerBag\"");
        return false;
    }
    if((*pPlayers)[playerIndex].BagCurrentSize == 0){
        perror("ERRO, \"pPlayers[playerIndex].BagCurrentSize\" não pode ser zero em \"SellItemPlayerBag\"");
        return false;
    }
    
    ItPointer tempItems;
    tempItems = (ItPointer)calloc((*pPlayers)[playerIndex].BagCurrentSize-1, sizeof(Item));
    int i, j = 0;
    for(i = 0; i < (*pPlayers)[playerIndex].BagCurrentSize; i++){
        if(i != bagSellIndex){
            tempItems[j] = (*pPlayers)[playerIndex].Bag[i];
            j++; 
        }
        else{
            (*pPlayers)[playerIndex].Pikocoins += (*pPlayers)[playerIndex].Bag[i].Value;
        }
    }
    (*pPlayers)[playerIndex].BagCurrentSize--;
    (*pPlayers)[playerIndex].Bag = (ItPointer)realloc((*pPlayers)[playerIndex].Bag, (*pPlayers)[playerIndex].BagCurrentSize * sizeof(Item));
    for(i = 0; i < (*pPlayers)[playerIndex].BagCurrentSize; i++){
        (*pPlayers)[playerIndex].Bag[i] = tempItems[i];
    }
    free(tempItems);
    return true;
}

void SelectBattlePikomons(PlPointer player){
    LimparTerminal();
    int escolhidos[6] = {0}; 
    int countEscolhidos = 0;

    for (int k = 0; k < 6; k++) {
        LimparTerminal();
        printf("Escolha seu Pikomon para a batalha:\n");

        for (int i = 0; i < 12; i++) {
            bool jaSelecionado = false;
            for (int j = 0; j < countEscolhidos; j++) {
                if (escolhidos[j] == i) {
                    jaSelecionado = true;
                    break;
                }
            }
            if (strlen(player[0].PikomonsStorage[i].Name) > 0 && !jaSelecionado) {
                printf("%d. %s\n", i + 1, player[0].PikomonsStorage[i].Name);
            }
        }

        printf("Digite o número do Pikomon que deseja escolher (1 à 12, -1 para sair): ");
        int escolha;
        scanf("%d", &escolha);
        if (escolha == -1) {
            return;
        }

        if (escolha < 1 || escolha > 12 || strlen(player[0].PikomonsStorage[escolha - 1].Name) == 0) {
            printf("Escolha inválida. Tente novamente.\n");
            getchar(); 
            getchar();
            k--; 
        } else {
            DebugPlayers(player, 0, 2);
            getchar();
            getchar();
            player[0].BatlePikomons[k] = player[0].PikomonsStorage[escolha - 1];
            DebugPlayers(player, 0, 2);
            getchar();
            getchar();
            escolhidos[countEscolhidos++] = escolha - 1; 
            printf("Você escolheu %s para a batalha!\n", player[0].PikomonsStorage[escolha - 1].Name);
            getchar(); 
            getchar();
        }
    }
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

void CalcSkill(Element allElements[10], PiPointer atacker, int skillIndex, PiPointer defenser, bool *elementalEffectHit, bool *skillHit, bool *critHit, bool *selfEffectHit, bool *enemyEffectHit, int *selfDamage, int *enemyDamage){
    int I, J, bonusQuantity;
    double elementalEffectivness;
    double physicalDamageReduction, magicDamageReduction;
    SkPointer usedSkill = &atacker[0].Skills[skillIndex];


    if(((rand() % 100)+1) <= ((double)usedSkill[0].ElementEffectHitChance) * ((double)atacker[0].Atributes[5].Total)/100.0){
        *elementalEffectHit = true;
        physicalDamageReduction = 1.0 - (DefenseReductionCalc(defenser[0].Atributes[1].Total) * 0.11);
        magicDamageReduction = 1.0 - (DefenseReductionCalc(defenser[0].Atributes[2].Total) * 0.11);
        elementalEffectivness = (double)usedSkill[0].Element.Effectiveness[defenser[0].Element.SelfElementIndex] / 100.0;

        if(usedSkill[0].Element.CurrentHPEffect.Timer > 0){
            defenser[0].CurrentHP.BonusQuantity++;
            bonusQuantity = defenser[0].CurrentHP.BonusQuantity;
            defenser[0].CurrentHP.acronym = (char**)realloc(defenser[0].CurrentHP.acronym, bonusQuantity * sizeof(char*));
            defenser[0].CurrentHP.acronym[bonusQuantity-1] = (char *)calloc(4, sizeof(char));
            defenser[0].CurrentHP.Bonus = (int*)realloc(defenser[0].CurrentHP.Bonus, bonusQuantity * sizeof(int));
            defenser[0].CurrentHP.BonusTimer = (int*)realloc(defenser[0].CurrentHP.BonusTimer, bonusQuantity * sizeof(int));
            strcpy(defenser[0].CurrentHP.acronym[bonusQuantity-1], usedSkill[0].Element.Acronym);
            defenser[0].CurrentHP.Bonus[bonusQuantity-1] = (int)((usedSkill[0].Element.CurrentHPEffect.Quantity + (((double)usedSkill[0].Element.ElementalDamageScale) * ((double)atacker[0].Atributes[6].Total)/ 100.0)) * magicDamageReduction * elementalEffectivness);
            defenser[0].CurrentHP.BonusTimer[bonusQuantity-1] = usedSkill[0].Element.CurrentHPEffect.Timer;
        }
        
        for (I = 0; I < 8; I++){
            if(usedSkill[0].Element.StatusEffect[I].Timer > 0){
                defenser[0].Atributes[I].BonusQuantity++;
                bonusQuantity = defenser[0].Atributes[I].BonusQuantity;
                defenser[0].Atributes[I].acronym = (char**)realloc(defenser[0].Atributes[I].acronym, bonusQuantity * sizeof(char*));
                defenser[0].Atributes[I].acronym[bonusQuantity-1] = (char*)calloc(4, sizeof(char));
                defenser[0].Atributes[I].Bonus = (int*)realloc(defenser[0].Atributes[I].Bonus, bonusQuantity * sizeof(int));
                defenser[0].Atributes[I].BonusTimer = (int*)realloc(defenser[0].Atributes[I].BonusTimer, bonusQuantity * sizeof(int));
                strcpy(defenser[0].Atributes[I].acronym[bonusQuantity-1], usedSkill[0].Element.StatusEffect[I].Acronym);
                defenser[0].Atributes[I].Bonus[bonusQuantity-1] = usedSkill[0].Element.StatusEffect[I].Quantity;
                defenser[0].Atributes[I].Total += defenser[0].Atributes[I].Bonus[bonusQuantity-1];
                defenser[0].Atributes[I].BonusTimer[bonusQuantity-1] = usedSkill[0].Element.StatusEffect[I].Timer;
            }
        }
    }
    else *elementalEffectHit = false;


    if(((rand() % 100)+1) <= (double)atacker[0].Atributes[3].Total * (double)usedSkill[0].HitChance / 100.0){
        *skillHit = true;
        int magicDamage, physicalDamage;
        if((rand() % 100 +1) <= usedSkill[0].CritChance) *critHit = true;

        magicDamage = (int)(((double)atacker[0].Atributes[6].Total * (double)usedSkill[0].MagicAttackScale / 100.0) + usedSkill[0].MagicBase) * (*critHit) ? 2 : 1;
        physicalDamage = (int)(((double)atacker[0].Atributes[4].Total * (double)usedSkill[0].AttackScale / 100.0) + usedSkill[0].AttackBase) * (*critHit) ? 2 : 1;
        if(usedSkill[0].Target == 'S'){
            elementalEffectivness = (double)usedSkill[0].Element.Effectiveness[atacker[0].Element.SelfElementIndex] / 100.0;
            physicalDamageReduction = 1.0 - (DefenseReductionCalc(atacker[0].Atributes[1].Total) * 0.11);
            magicDamageReduction = 1.0 - (DefenseReductionCalc(atacker[0].Atributes[2].Total) * 0.11);
            *selfDamage = (physicalDamage * physicalDamageReduction + magicDamage * magicDamageReduction) * elementalEffectivness;

            atacker[0].CurrentHP.Total -= *selfDamage;

        }
        else if(usedSkill[0].Target == 'E'){
            elementalEffectivness = (double)usedSkill[0].Element.Effectiveness[defenser[0].Element.SelfElementIndex] / 100.0;
            physicalDamageReduction = 1.0 - (DefenseReductionCalc(defenser[0].Atributes[1].Total) * 0.11);
            magicDamageReduction = 1.0 - (DefenseReductionCalc(defenser[0].Atributes[2].Total) * 0.11);
            *enemyDamage = (physicalDamage * physicalDamageReduction + magicDamage * magicDamageReduction) * elementalEffectivness;

            defenser[0].CurrentHP.Total -= *enemyDamage;

        }
        else if(usedSkill[0].Target == 'B'){
            elementalEffectivness = (double)usedSkill[0].Element.Effectiveness[atacker[0].Element.SelfElementIndex] / 100.0;
            physicalDamageReduction = 1.0 - (DefenseReductionCalc(atacker[0].Atributes[1].Total) * 0.11);
            magicDamageReduction = 1.0 - (DefenseReductionCalc(atacker[0].Atributes[2].Total) * 0.11);
            *selfDamage = (physicalDamage * physicalDamageReduction + magicDamage * magicDamageReduction) * elementalEffectivness;

            atacker[0].CurrentHP.Total -= *selfDamage;


            elementalEffectivness = (double)usedSkill[0].Element.Effectiveness[defenser[0].Element.SelfElementIndex] / 100.0;
            physicalDamageReduction = 1.0 - (DefenseReductionCalc(defenser[0].Atributes[1].Total) * 0.11);
            magicDamageReduction = 1.0 - (DefenseReductionCalc(defenser[0].Atributes[2].Total) * 0.11);
            *enemyDamage = (physicalDamage * physicalDamageReduction + magicDamage * magicDamageReduction) * elementalEffectivness;

            defenser[0].CurrentHP.Total -= *enemyDamage;
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
                atacker[0].Atributes[I].BonusQuantity++;
                bonusQuantity = atacker[0].Atributes[I].BonusQuantity;
                atacker[0].Atributes[I].acronym = (char**)realloc(atacker[0].Atributes[I].acronym, bonusQuantity * sizeof(char*));
                atacker[0].Atributes[I].acronym[bonusQuantity-1] = (char*)calloc(4, sizeof(char));
                atacker[0].Atributes[I].Bonus = (int*)realloc(atacker[0].Atributes[I].Bonus, bonusQuantity * sizeof(int));
                atacker[0].Atributes[I].BonusTimer = (int*)realloc(atacker[0].Atributes[I].BonusTimer, bonusQuantity * sizeof(int));
                strcpy(atacker[0].Atributes[I].acronym[bonusQuantity-1], usedSkill[0].Element.StatusEffect[I].Acronym);
                atacker[0].Atributes[I].Bonus[bonusQuantity-1] = usedSkill[0].Element.StatusEffect[I].Quantity;
                atacker[0].Atributes[I].Total += atacker[0].Atributes[I].Bonus[bonusQuantity-1];
                atacker[0].Atributes[I].BonusTimer[bonusQuantity-1] = usedSkill[0].Element.StatusEffect[I].Timer;
            }
        }
    }
    else *selfEffectHit = false;


    if((rand() % 100 +1 <= usedSkill[0].EnemyEffectHitChance)){
        *enemyEffectHit = true;
        for (I = 0; I < 8; I++){
            if(usedSkill[0].Element.StatusEffect[I].Timer > 0){
                defenser[0].Atributes[I].BonusQuantity++;
                bonusQuantity = defenser[0].Atributes[I].BonusQuantity;
                defenser[0].Atributes[I].acronym = (char**)realloc(defenser[0].Atributes[I].acronym, bonusQuantity * sizeof(char*));
                defenser[0].Atributes[I].acronym[bonusQuantity-1] = (char*)calloc(4, sizeof(char));
                defenser[0].Atributes[I].Bonus = (int*)realloc(defenser[0].Atributes[I].Bonus, bonusQuantity * sizeof(int));
                defenser[0].Atributes[I].BonusTimer = (int*)realloc(defenser[0].Atributes[I].BonusTimer, bonusQuantity * sizeof(int));
                strcpy(defenser[0].Atributes[I].acronym[bonusQuantity-1], usedSkill[0].Element.StatusEffect[I].Acronym);
                defenser[0].Atributes[I].Bonus[bonusQuantity-1] = usedSkill[0].Element.StatusEffect[I].Quantity;
                defenser[0].Atributes[I].Total += defenser[0].Atributes[I].Bonus[bonusQuantity-1];
                defenser[0].Atributes[I].BonusTimer[bonusQuantity-1] = usedSkill[0].Element.StatusEffect[I].Timer;
            }
        }
    }
    else *enemyEffectHit = false;
}

void UseItem(PlPointer selfPlayer, PlPointer enemyPlayer, int itemUsedIndex, bool *usedItemStatusHit){
    int I, bonusQuantity;
    double pikomonDamageReduction, otherpikomonDamageReduction;
    ItPointer usedItem;
    PiPointer pikomon, otherPikomon;
    usedItem = &selfPlayer[0].Bag[itemUsedIndex];
    pikomon = &selfPlayer[0].BatlePikomons[selfPlayer[0].SelectedPikomonIndex];
    otherPikomon = &enemyPlayer[0].BatlePikomons[enemyPlayer[0].SelectedPikomonIndex];
    if(usedItem[0].EffectCurrentHP.Quantity >= 0){
        if(usedItem[0].CurrentHPDamageIsPhysic){
            pikomonDamageReduction = 1.0 - (DefenseReductionCalc(pikomon[0].Atributes[1].Total) * 0.11);
            otherpikomonDamageReduction = 1.0 - (DefenseReductionCalc(otherPikomon[0].Atributes[1].Total) * 0.11); 
        }
        else{
            pikomonDamageReduction = 1.0 - (DefenseReductionCalc(pikomon[0].Atributes[2].Total) * 0.11);
            otherpikomonDamageReduction = 1.0 - (DefenseReductionCalc(otherPikomon[0].Atributes[2].Total) * 0.11);
        }
    }
    else{
        pikomonDamageReduction = 1;
        otherpikomonDamageReduction = 1;
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
                otherPikomon[0].CurrentHP.Bonus[bonusQuantity-1] = (int)( usedItem[0].EffectCurrentHP.Quantity * otherpikomonDamageReduction);
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
                otherPikomon[0].CurrentHP.Bonus[bonusQuantity-1] = (int)( usedItem[0].EffectCurrentHP.Quantity * otherpikomonDamageReduction);
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

void PassPikomonTurnTime(PiPointer pikomon){
    //vai ter problema com o CurrentHp.acronym e os atribute.acronym. neles não se podem usar o realloc porque não libera os ponteiros individuais
    int I, J, K, indexToRemove;
    Effect *tempEffect;
    PiPointer pik;
    pik = pikomon;
    
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
        pik[0].Atributes[I].Total = pik[0].Atributes[I].Base;
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
    return LNfalso(value) / LNfalso(base);
    return 1.0;
}
double LNfalso(double x){
    if (x <= 0) {
        return 0;
    }
    double z = (x - 1) / (x + 1);
    double resultado = 0.0;
    int n;
    for (n = 1; n <= 20; n += 2) {
        resultado += (1.0 / n) * z;
        z *= (x - 1) / (x + 1) * (x - 1) / (x + 1);
    }
    return 2 * resultado;
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

Pikomon GerarPikomon(PiPointer pPikomons, int indexPikomon, Personality *personalities){
    // Isso aq inicializa o gerador de numeros
    srand(time(NULL));
    Pikomon novo;
    novo = pPikomons[indexPikomon];
    int totalRaridades = 0;
    for (int i = 0; i < 13; i++) {
        totalRaridades += personalities[i].rarity;
    }

    int valorAleatorio = rand() % totalRaridades;
    int raridadeSum = 0;
    for (int i = 0; i < 13; i++) {
        raridadeSum += personalities[i].rarity;
        if (valorAleatorio < raridadeSum) {
            novo.Personality = personalities[i];
            break;
        }
    }

    novo.Atributes[0].Base = (int)(pPikomons[indexPikomon].Atributes[0].Base * (double)novo.Personality.BaseHPModifier / 100.0);
    novo.Atributes[1].Base = (int)(pPikomons[indexPikomon].Atributes[1].Base * (double)novo.Personality.BaseDefenseModifier / 100.0);
    novo.Atributes[2].Base = (int)(pPikomons[indexPikomon].Atributes[2].Base * (double)novo.Personality.BaseMagicDefenseModifier / 100.0);
    novo.Atributes[3].Base = (int)(pPikomons[indexPikomon].Atributes[3].Base * (double)novo.Personality.BaseAccuracyModifier / 100.0);
    novo.Atributes[4].Base = (int)(pPikomons[indexPikomon].Atributes[4].Base * (double)novo.Personality.BaseAttackModifier / 100.0);
    novo.Atributes[5].Base = (int)(pPikomons[indexPikomon].Atributes[5].Base * (double)novo.Personality.BaseElementalAccuracyModifier / 100.0);
    novo.Atributes[6].Base = (int)(pPikomons[indexPikomon].Atributes[6].Base * (double)novo.Personality.BaseMagicAttackModifier / 100.0);
    novo.Atributes[7].Base = (int)(pPikomons[indexPikomon].Atributes[7].Base * (double)novo.Personality.BaseSpeedModifier / 100.0);
    return novo;
}


bool ShopPikomon(PlPointer *players, int playerAtualIndex, PiPointer pPikomon, DataQuantity pikomonQuantidade, Personality* personalities){ 
    Player* playerAtual = &(*players)[playerAtualIndex];

    printf("_____________________________________\n");
    printf("|                                   |\n");
    printf("|        !Você tem %d pikocoins!     |\n", playerAtual[0].Pikocoins);
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
        getchar();
        getchar();
        return false;
    }

    if (playerAtual[0].Pikocoins < 10) {
        printf("Você não tem Pikocoins suficientes para comprar este Pikomon.\n");
        getchar();
        getchar();
        return false;
    }
    int quantidadePikomonArmazenado = 0;
    for (int i = 0; i < 12; i++) {
        if (strlen(playerAtual[0].PikomonsStorage[i].Name) > 0) {
            quantidadePikomonArmazenado++;
        }
    }

    if (quantidadePikomonArmazenado >= 12) {
        printf("Você já possui o máximo de Pikomons permitido no armazenamento.\n");
        return false;
    }

    
    playerAtual[0].PikomonsStorage[quantidadePikomonArmazenado] = GerarPikomon(pPikomon, playerEscolha, personalities);
    playerAtual[0].Pikocoins -= playerAtual[0].PikomonsStorage[quantidadePikomonArmazenado].Value;

    printf("Você comprou %s! Agora você tem %d pikocoins restantes.\n", playerAtual[0].PikomonsStorage[quantidadePikomonArmazenado].Name, playerAtual[0].Pikocoins);
    getchar();
    getchar();
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

bool ShopItems(PlPointer players, int playerAtualIndex, ItPointer pItems, DataQuantity itemQuantidade){
    Player *playerAtual = &players[playerAtualIndex];

    printf("______________________________________\n");
    printf("|                                   |\n");
    printf("|     !Você tem %4d pikocoins!   |\n", playerAtual[0].Pikocoins);
    printf("|                                   |\n");
    printf("______________________________________\n");

    printf("Itens disponíveis para compra:\n");
    DebugItems(pItems, -1, itemQuantidade.Item);
    printf("Escolha um item para comprar (ou -1 para sair): ");

    int itemEscolha;
    scanf("%d", &itemEscolha);

    if (itemEscolha == -1)
    {
        return true;
    }

    if (itemEscolha < 0 || itemEscolha >= itemQuantidade.Item)
    {
        printf("Escolha inválida. Tente novamente.\n");
        getchar();
        getchar();
        return false;
    }

    if (playerAtual[0].Pikocoins < pItems[itemEscolha].Value)
    {
        printf("Você não tem Pikocoins suficientes para comprar este item.\n");
        getchar();
        getchar();
        return false;
    }

    if (!AddItemPlayerBag(players, playerAtualIndex, pItems, itemEscolha))
    {
        printf("Falha ao adicionar o item ao inventário. Tente novamente.\n");
        getchar();
        getchar();
        return false;
    }

    playerAtual[0].Pikocoins -= pItems[itemEscolha].Value;

    printf("Você comprou %s! Agora você tem %d pikocoins restantes.\n", pItems[itemEscolha].Name, playerAtual[0].Pikocoins);
    getchar();
    getchar();

    return true;
}

void PrintPikomonEffects(PiPointer pikomon){
    int i, j;
    PiPointer pik =pikomon;

    printf("Efeitos no HP Atual:\n");
    for (j = 0; j < pik[0].CurrentHP.BonusQuantity; j++)
    {
        printf("Efeito %s: Quantidade = %d, Timer = %d\n", 
               pik[0].CurrentHP.acronym[j], 
               pik[0].CurrentHP.Bonus[j], 
               pik[0].CurrentHP.BonusTimer[j]);
    }

    printf("\nEfeitos nos Atributos:\n");
    for (i = 0; i < 8; i++)
    {
        printf("Atributo %d:\n", i + 1);
        for (j = 0; j < pik[0].Atributes[i].BonusQuantity; j++)
        {
            printf("  Efeito %s: Quantidade = %d, Timer = %d\n",
                   pik[0].Atributes[i].acronym[j],
                   pik[0].Atributes[i].Bonus[j],
                   pik[0].Atributes[i].BonusTimer[j]);
        }
    }
}

void EscolherSkills(Player *player) {
    for (int k = 0; k < 6; k++) {
        printf("%s, escolha as habilidades para %s:\n", player[0].Name, player[0].BatlePikomons[k].Name);

        int skillCount = 0;

        for (int i = 0; i < 4; i++) {
            Skill currentSkill = player[0].BatlePikomons[k].Skills[i];
            bool canLearn = currentSkill.LearnablePersonalities[player[0].BatlePikomons[k].Personality.rarity] &&
                            currentSkill.LearnableElements[player[0].BatlePikomons[k].Element.SelfElementIndex];
            
            if (strlen(currentSkill.Name) > 0 && canLearn) {
                printf("%d. %s - Tipo: %s\n", i + 1, currentSkill.Name, currentSkill.Type);
                skillCount++;
            }
        }

        if (skillCount == 0) {
            printf("Nenhuma habilidade disponível para %s.\n", player[0].BatlePikomons[k].Name);
            continue; 
        }

        for (int j = 0; j < 4; j++) {
            printf("Digite o número da habilidade %d para %s: ", j + 1, player[0].BatlePikomons[k].Name);
            int escolha;
            scanf("%d", &escolha);

            if (escolha < 1 || escolha > 4) {
                printf("Escolha inválida. Tente novamente.\n");
                j--; 
            } else {
                Skill chosenSkill = player[0].BatlePikomons[k].Skills[escolha - 1];
                bool isValidSkill = chosenSkill.LearnablePersonalities[player[0].BatlePikomons[k].Personality.rarity] &&
                                    chosenSkill.LearnableElements[player[0].BatlePikomons[k].Element.SelfElementIndex];

                if (!isValidSkill) {
                    printf("Habilidade não aprendível por este Pikomon. Tente novamente.\n");
                    j--; 
                } else {
                    player[0].BatlePikomons[k].Skills[j] = chosenSkill; 
                    printf("Habilidade %s selecionada para %s!\n", chosenSkill.Name, player[0].BatlePikomons[k].Name);
                }
            }
        }
    }
}

void printPlayer(Player *p) {
    if (p == NULL) {
        printf("Jogador não encontrado.\n");
        return;
    }

    printf("Nome: %s\n", p->Name);
    printf("Pikocoins: %d\n", p->Pikocoins);
    printf("Índice do Pikomon Selecionado: %d\n", p->SelectedPikomonIndex);
    printf("Pikomon em Batalha:\n");
    
    for (int i = 0; i < 6; i++) {
        if (strlen(p->BatlePikomons[i].Name) > 0) { // Verifica se o Pikomon tem um nome
            printf("  Pikomon %d: %s\n", i + 1, p->BatlePikomons[i].Name);
        }
    }
    
    printf("Pikomon Armazenados:\n");
    for (int i = 0; i < 12; i++) {
        if (strlen(p->PikomonsStorage[i].Name) > 0) { // Verifica se o Pikomon tem um nome
            printf("  Pikomon %d: %s\n", i + 1, p->PikomonsStorage[i].Name);
        }
    }
    
    printf("Tamanho Atual da Bag: %d\n", p->BagCurrentSize);
}
