#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <time.h>

typedef struct Personality
{
    char Name[20];

    // Definir os modificadores em double como porcentagem, Exemplo 1.2 é 120%, 0.85 é 85%

    double BaseHPModifier;
    double BaseDefenseModifier;
    double BaseMagicDefenseModifier;
    double BaseAttackModifier;
    double BaseMagicAttackModifier;
    double BaseSpeedModifier;

} Personality;

typedef struct Effect
{

    int Quantity;
    int Timer;

} Effect;

typedef struct Atribute
{

    char Name[20];
    int Base;
    int Total;
    int BonusQuantity;
    int *Bonus;
    int *BonusTimer;

} Atribute;

typedef struct Element
{
    char Name[20];

    double StatusEffectChance;
    Effect StatusEffect[8];

} Element;

typedef struct DataQuantity
{

    int Skill;
    int Item;
    int Pikomon;
    int Player;

} DataQuantity;

typedef struct Skill
{

    char Name[20];
    char Target;
    // Target pode ser 'S' para self, 'E' para enemy, e 'B' para both
    bool LearnablePersonalities[10];
    bool LearnableElements[10];
    double ElementEffectChance;
    Element Element;

    int AttackBase;
    double AttackScale;
    int MagicBase;
    double MagicAttackScale;
    double CritChance;

    // Isso daqui é expecificamente da skill, sem contar o elemento
    char EffectTarget;
    // Target pode ser 'S' para self, 'E' para enemy, e 'B' para both
    double EnemyEffectChance;
    Effect EnemyEffect[8];

    double SelfEffectChance;
    Effect SelfEffect[8];

} Skill, *SkPointer;

typedef struct Item
{
    bool EffectEnded;
    char Name[20];
    char Type[20];
    char Description[3][255];
    int Value;

    char EffectCurrentHPTarget;
    // Target pode ser 'S' para self, 'E' para enemy, e 'B' para both
    Effect EnemyEffectCurrentHP;
    Effect SelfEffectCurrentHP;
    // Esse valor é usado pra definir o quanto uma poção de cura cura e em quanto tempo, ou um veneno. O outro serve para alterar os status da vida maxima

    char EffectTarget;
    // Target pode ser 'S' para self, 'E' para enemy, e 'B' para both
    double EnemyStatusEffectChance;
    Effect EnemyStatusEffect[8];

    double SelfStatusEffectChance;
    Effect SelfStatusEffect[8];

} Item, *ItPointer;

typedef struct Pikomon
{

    char Name[10];
    char IconImg[7][19];
    Element Element;
    Personality Personality;

    Atribute CurrentHP;
    Atribute Atributes[8];
    // Atributes[0].Name, "HP"
    // Atributes[1].Name, "Defense"
    // Atributes[2].Name, "MagicDefense"
    // Atributes[3].Name, "Acurracy"
    // Atributes[4].Name, "Attack"
    // Atributes[5].Name, "ElementalAcurracy"
    // Atributes[6].Name, "MagicAttack"
    // Atributes[7].Name, "Speed"

    int ChargedSpeed;
    // isso é para o calculo de turno, é a velocidade que ele acumula pra poder ter mais de um turno

    Skill Skills[4];

} Pikomon, *PiPointer;

typedef struct Player
{

    char Name[20];
    char Pass[7];

    int Pikocoins;
    int BagCurrentSize;
    int SelectedPikomonIndex;

    Pikomon BatlePikomons[6];
    Pikomon PikomonsStorage[12];

    ItPointer Bag;

} Player, *PlPointer;

// funções da main
int DefineIndexOfElement(char *NomeElemento);
bool DebugPlayers(PlPointer pPlayers, int index, int playersQuantity);
bool DebugPikomons(PiPointer pPikomon, int index, int pikomonsQuantity);
bool DebugItems(ItPointer pItems, int index, int ItemsQuantity);
bool DebugSkills(SkPointer pSkills, int index, int skillsQuantity);
bool DebugPersonality(Personality *personalities, int index);
bool SavePersonalities(Personality allPersonalities[10], const char *destino);
bool SaveElements(Element allElements[10], const char *destino);
bool SaveSkills(SkPointer pSkills, int skillsQuantity, const char *destino);
bool SaveItems(ItPointer pItems, int ItemsQuantity, const char *destino);
bool SavePikomons(PiPointer pPikomons, int pikomonsQuantity, const char *destino);
bool SavePlayers(PlPointer pPlayers, int playersQuantity, const char *destino);

int main()
{
    /**Declarações**/
    //------------------------------------------------------------------------------------------------------------------//
    setlocale(LC_ALL, "portuguese");
    setenv("TZ", "America/Sao_Paulo", 1);
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
    // dB é de "data base"

    int playerOneIndex = -1, playerTwoIndex = -1;

    Personality allPersonalities[10];
    memset(allPersonalities, 0, sizeof(allPersonalities));

    Element allElements[10];
    memset(allElements, 0, sizeof(allElements));

    SkPointer pSkills = NULL;

    ItPointer pItems = NULL;

    PiPointer pPikomons = NULL;

    PlPointer pPlayers = NULL;
    //------------------------------------------------------------------------------------------------------------------//

    /**Loads**/
    //------------------------------------------------------------------------------------------------------------------//
    dBDataQuantity = fopen(dataQuantity, "r");
    if (dBDataQuantity == NULL)
    {
        perror("Falha ao abrir \"dataQuantity\"");
        return 1;
    }
    fgets(readLine, 256, dBDataQuantity);
    fgets(readLine, 256, dBDataQuantity);
    sscanf(readLine, "%d,%d,%d,%d,%d", dataQuantities.Player, dataQuantities.Pikomon, dataQuantities.Item, dataQuantities.Skill);
    fclose(dBDataQuantity);
    if (dataQuantities.Player == -1 || dataQuantities.Pikomon == -1 || dataQuantities.Item == -1 || dataQuantities.Skill == -1)
    {
        perror("Falha ao aderir dados");
        return 3;
    }

    pSkills = (SkPointer)calloc(dataQuantities.Skill, sizeof(Skill));
    if (pSkills == NULL)
    {
        perror("Falha ao allocar \"pSkills\"");
        free(pPlayers);
        free(pPikomons);
        free(pItems);
        free(pSkills);
        return 2;
    }
    dBSkills = fopen(skills, "rb");
    if (dBSkills == NULL)
    {
        perror("Falha ao abrir \"skills\"");
        free(pPlayers);
        free(pPikomons);
        free(pItems);
        free(pSkills);
        return 1;
    }
    fread(pSkills, sizeof(Skill), dataQuantities.Skill, dBSkills);
    fclose(dBSkills);

    pItems = (ItPointer)calloc(dataQuantities.Item, sizeof(Item));
    if (pItems == NULL)
    {
        perror("Falha ao allocar \"pItems\"");
        free(pPlayers);
        free(pPikomons);
        free(pItems);
        return 2;
    }
    dBItens = fopen(items, "rb");
    if (dBItens == NULL)
    {
        perror("Falha ao abrir \"items\"");
        free(pPlayers);
        free(pPikomons);
        free(pItems);
        return 1;
    }
    fread(pItems, sizeof(Item), dataQuantities.Item, dBItens);
    fclose(dBItens);

    pPikomons = (PiPointer)calloc(dataQuantities.Pikomon, sizeof(Pikomon));
    if (pPikomons == NULL)
    {
        perror("Falha ao allocar \"pPikomons\"");
        free(pPlayers);
        free(pPikomons);
        return 2;
    }
    dBPikomons = fopen(pikomoms, "rb");
    if (dBPikomons == NULL)
    {
        perror("Falha ao abrir \"players\"");
        free(pPlayers);
        free(pPikomons);
        return 1;
    }
    fread(pPikomons, sizeof(Pikomon), dataQuantities.Pikomon, dBPikomons);
    fclose(dBPikomons);

    pPlayers = (PlPointer)calloc(dataQuantities.Player, sizeof(Player));
    if (dBPlayers == NULL)
    {
        perror("Falha ao allocar \"pPlayers\"");
        free(pPlayers);
        return 2;
    }
    dBPlayers = fopen(players, "rb");
    if (dBPlayers == NULL)
    {
        perror("Falha ao abrir \"players\"");
        free(pPlayers);
        return 1;
    }
    fread(pPlayers, sizeof(Player), dataQuantities.Player, dBPlayers);
    fclose(dBPlayers);
    //------------------------------------------------------------------------------------------------------------------//

    /**Principal Do Usuário**/
    //------------------------------------------------------------------------------------------------------------------//
    // login
    // menu
    //------------------------------------------------------------------------------------------------------------------//
    // toda a vez que acaba uma batalha tem que usar essa função
    FreeAllHeapMemoryAndSaveEverything(pSkills, pItems, pPikomons, pPlayers, dataQuantities, dataQuantity, skills, items, pikomoms, players);
}

/**Debug/Print Functions**/
//------------------------------------------------------------------------------//
bool DebugPlayers(Player *pPlayers, int index, int playersQuantity)
{
    int j, k;

    //-1 para pegar todos os players
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
                        printf("| Item Name:| %20s | |,| Type:| %20s | |,| Target:| %3c | |;\n",
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
                        printf("| Nome:| %20s | |,| Element:| %10s | |,| HP:| %3d |,| Atk:| %3lf |,| Def:| %3lf |,,| SpA:| %3lf |,| SpD:| %3lf |,| Spd:| %3lf |,| Acu:| %3lf |,| Eac:| %3lf | | \n",
                               pPlayers[index].PikomonsStorage[j].Name,
                               pPlayers[index].PikomonsStorage[j].Element.Name,
                               pPlayers[index].PikomonsStorage[j].Atributes[0],
                               pPlayers[index].PikomonsStorage[j].Atributes[4],
                               pPlayers[index].PikomonsStorage[j].Atributes[1],
                               pPlayers[index].PikomonsStorage[j].Atributes[6],
                               pPlayers[index].PikomonsStorage[j].Atributes[2],
                               pPlayers[index].PikomonsStorage[j].Atributes[7],
                               pPlayers[index].PikomonsStorage[j].Atributes[3],
                               pPlayers[index].PikomonsStorage[j].Atributes[5]);

                        // arrumar numero depois        -----------------------------------------------------------------------------------------------
                        // Exibir habilidades do pokemon
                        for (k = 0; k < 4; k++)
                        {
                            if (strlen(pPlayers[index].PikomonsStorage[j].Skills[k].Name) > 0)
                            {
                                printf("| Skill Name:| %20s | |,| Target:| %c | |,| AttackScale:| %1.2lf | |,| MagicAttackScale:| %1.2lf | |;\n",
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
    else if (index > -1 && index < playersQuantity)
    {
        // Exibir apenas um jogador
        if (strlen(pPlayers[index].Name) > 0)
        {
            printf("| Name:| %20s | |,| Pass:| %20s | |,| Pikocoins:| %3d | |,| BagCurrentSize:| %2d | |;\n",
                   pPlayers[index].Name,
                   pPlayers[index].Pass,
                   pPlayers[index].Pikocoins);

            // Exibir itens da bolsa
            for (j = 0; j < pPlayers[index].BagCurrentSize; j++)
            {
                if (strlen(pPlayers[index].Bag[j].Name) > 0)
                {
                    printf("| Item Name:| %20s | |,| Type:| %20s | |,| Target:| %3c | |;\n",
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
                    printf("| Nome:| %20s | |,| Element:| %10s | |,| HP:| %3d |,| Atk:| %3lf |,| Def:| %3lf |,| SpA:| %3lf |,| SpD:| %3lf |,| Spd:| %3lf |,| Acu:| %3lf |,| Eac:| %3lf | |\n",
                           pPlayers[index].PikomonsStorage[j].Name,
                           pPlayers[index].PikomonsStorage[j].Element.Name,
                           pPlayers[index].PikomonsStorage[j].Atributes[0],
                           pPlayers[index].PikomonsStorage[j].Atributes[4],
                           pPlayers[index].PikomonsStorage[j].Atributes[1],
                           pPlayers[index].PikomonsStorage[j].Atributes[6],
                           pPlayers[index].PikomonsStorage[j].Atributes[2],
                           pPlayers[index].PikomonsStorage[j].Atributes[7],
                           pPlayers[index].PikomonsStorage[j].Atributes[3],
                           pPlayers[index].PikomonsStorage[j].Atributes[5]);

                    // Exibir habilidades do pokemon
                    for (k = 0; k < 4; k++)
                    {
                        if (strlen(pPlayers[index].PikomonsStorage[j].Skills[k].Name) > 0)
                        {
                            printf("| Skill Name:| %3s | |,| Target:| %c | |,| AttackScale:| %1.2lf | |,| MagicAttackScale:| %1.2lf | | ;\n",
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
        perror("index tá estranho");
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
                printf("| Nome:| %10s | |,| Element:| %10s | |,| HP:| %3d | |,| Atk:| %3lf | |,| Def:| %3lf | |,| SpA:| %3lf | |,| SpD:| %3lf | |,| Spd:| %3lf |,| Acu:| %3lf |,| Eac:| %3lf | |\n",
                       pPikomon[index].Name,
                       pPikomon[index].Element.Name,
                       pPikomon[index].Atributes[0].Total,
                       pPikomon[index].Atributes[4].Total,
                       pPikomon[index].Atributes[1].Total,
                       pPikomon[index].Atributes[6].Total,
                       pPikomon[index].Atributes[2].Total,
                       pPikomon[index].Atributes[7].Total,
                       pPikomon[index].Atributes[3].Total,
                       pPikomon[index].Atributes[5]);

                // Exibir habilidades do Pikomon
                for (k = 0; k < 4; k++)
                {
                    if (strlen(pPikomon[index].Skills[k].Name) > 0)
                    {
                        printf("| Skill Name:| %20s | |,| Target:| %c | |,| AttackScale:| %1.2lf | |,| MagicAttackScale:| %1.2lf | |;\n",
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
            printf("| Nome:| %10s | |,| Element:| %10s | |,| HP:| %3d | |,| Atk:| %3lf | |,| Def:| %3lf | |,| SpA:| %3lf | |,| SpD:| %3lf | |,| Spd:| %3lf |,| Acu:| %3lf |,| Eac:| %3lf | |\n",
                   pPikomon[index].Name,
                   pPikomon[index].Element.Name,
                   pPikomon[index].Atributes[0].Total,
                   pPikomon[index].Atributes[4].Total,
                   pPikomon[index].Atributes[1].Total,
                   pPikomon[index].Atributes[6].Total,
                   pPikomon[index].Atributes[2].Total,
                   pPikomon[index].Atributes[7].Total,
                   pPikomon[index].Atributes[3].Total,
                   pPikomon[index].Atributes[5]);

            // Exibir habilidades do Pikomon
            for (k = 0; k < 4; k++)
            {
                if (strlen(pPikomon[index].Skills[k].Name) > 0)
                {
                    printf("| Skill Name:| %20s | |,| Target:| %c | |,| AttackScale:| %1.2lf | |,| MagicAttackScale:| %1.2lf | |;\n",
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
        perror("Vê esse indice ai man");
        return false;
    }
    return true;
}

bool DebugItems(ItPointer pItems, int index, int ItemsQuantity)
{
    if(index == -1){
        for(index = 0; index < ItemsQuantity; index++){
            printf("| Name:| %10s | |,| Type:| %10s | |,| Target:| %c | |;\n", 
            pItems[index].Name, 
            pItems[index].Type, 
            pItems[index].EffectTarget);
        }
    }
    else if(index > -1 && index < ItemsQuantity){
            printf("| Name:| %10s | |,| Type:| %10s | |,| Target:| %c | |;\n", 
            pItems[index].Name, 
            pItems[index].Type, 
            pItems[index].EffectTarget);
    }
    else{
        perror("index ruim mano");
        return false;
    }
    return true;
}

bool DebugSkills(SkPointer pSkills, int index, int skillsQuantity)
{
    if(index == -1){
        int i;
        for(i = 0; i < skillsQuantity; i++){
            printf("| Nome:| %20s | |,| Target:| %c | |,| AttackScale:| %1.2lf | |,| MagicAttackScale:| %1.2lf | |;\n", 
                pSkills[index].Name, 
                pSkills[index].Target, 
                pSkills[index].AttackScale, 
                pSkills[index].MagicAttackScale);
            printf("| Element[0]:| %d | |,| Element[1]:| %d | |,| Element[2]:| %d | |,| Element[3]:| %d | |,| Element[4]:| %d | |;\n| Element[5]:| %d | |,| Element[6]:| %d | |,| Element[7]:| %d | |,| Element[8]:| %d | |,| Element[9]:| %d | |;\n\n",pSkills[index].Element[0],
                pSkills[index].Element[1],
                pSkills[index].Element[2],
                pSkills[index].Element[3],
                pSkills[index].Element[4],
                pSkills[index].Element[5],
                pSkills[index].Element[6],
                pSkills[index].Element[7],
                pSkills[index].Element[8],
                pSkills[index].Element[9]);
        }
    }
    else if(index > -1 && index < skillsQuantity){
            printf("| Nome:| %20s | |,| Target:| %c | |,| AttackScale:| %1.2lf | |,| MagicAttackScale:| %1.2lf | |;\n", 
                pSkills[index].Name, 
                pSkills[index].Target, 
                pSkills[index].AttackScale, 
                pSkills[index].MagicAttackScale);
            printf("| Element[0]:| %d | |,| Element[1]:| %d | |,| Element[2]:| %d | |,| Element[3]:| %d | |,| Element[4]:| %d | |;\n| Element[5]:| %d | |,| Element[6]:| %d | |,| Element[7]:| %d | |,| Element[8]:| %d | |,| Element[9]:| %d | |;\n\n",pSkills[index].Element[0],
                pSkills[index].Element[1],
                pSkills[index].Element[2],
                pSkills[index].Element[3],
                pSkills[index].Element[4],
                pSkills[index].Element[5],
                pSkills[index].Element[6],
                pSkills[index].Element[7],
                pSkills[index].Element[8],
                pSkills[index].Element[9]);
    }
    else{
        perror("index usado na função \"DebugSkills\" não permitido");
        return false;
    }
    return true;
}

bool DebugPersonality(Personality *personalities, int index) {
    if (index == -1) {
        // Exibe todos as personalidades
        for (int i = 0; i < 13; i++) {
            if (strlen(personalities[i].Name) > 0) {
                printf("| Name:| %20s |,| BaseHPModifier:| %.2lf |,| BaseDefenseModifier:| %.2lf |,| BaseMagicDefenseModifier:| %.2lf |\n",
                       personalities[i].Name,
                       personalities[i].BaseHPModifier,
                       personalities[i].BaseDefenseModifier,
                       personalities[i].BaseMagicDefenseModifier);
                printf("| BaseAttackModifier:| %.2lf |,| BaseMagicAttackModifier:| %.2lf |,| BaseSpeedModifier:| %.2lf |\n",
                       personalities[i].BaseAttackModifier,
                       personalities[i].BaseMagicAttackModifier,
                       personalities[i].BaseSpeedModifier);
            }
        }
    } else if (index >= 0 && index < 13) {
        // Exibe apenas uma personalidade especificada pelo índice
        if (strlen(personalities[index].Name) > 0) {
            printf("| Name:| %20s |,| BaseHPModifier:| %.2lf |,| BaseDefenseModifier:| %.2lf |,| BaseMagicDefenseModifier:| %.2lf |\n",
                   personalities[index].Name,
                   personalities[index].BaseHPModifier,
                   personalities[index].BaseDefenseModifier,
                   personalities[index].BaseMagicDefenseModifier);
            printf("| BaseAttackModifier:| %.2lf |,| BaseMagicAttackModifier:| %.2lf |,| BaseSpeedModifier:| %.2lf |\n",
                   personalities[index].BaseAttackModifier,
                   personalities[index].BaseMagicAttackModifier,
                   personalities[index].BaseSpeedModifier);
        }
    } else {
        perror("Índice inválido para personalidade.");
        return false;
    }
    return true;
}




//------------------------------------------------------------------------------//

/**Save Functions**/
//------------------------------------------------------------------------------//
bool SavePersonalities(Personality allPersonalities[10], const char *destino)
{
    FILE *dBPersonalities;
    dBPersonalities = fopen(destino, "wb");
    if (dBPersonalities == NULL)
    {
        perror("falha ao abrir \"dBPersonalities\" na função \"SavePersonalities\"");
        return false;
    }
    fwrite(allPersonalities, sizeof(Personality), 10, dBPersonalities);
    fclose(dBPersonalities);
    return true;
}

bool SaveElements(Element allElements[10], const char *destino)
{
    FILE *dBElements;
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

bool SaveDataQuantity(DataQuantity dataQuantities, const char *destino)
{
    FILE *dBDataQuantity;
    dBDataQuantity = fopen(destino, "w");
    if (dBDataQuantity == NULL)
    {
        perror("falha ao abrir \"dBDataQuantity\" na função \"SaveDataQuantity\"");
        return false;
    }
    fprintf(dBDataQuantity, "Players,Pikomons,Itens,Skills\n%d,%d,%d,%d", dataQuantities.Player, dataQuantities.Pikomon, dataQuantities.Item, dataQuantities.Skill);
    fclose(dBDataQuantity);
    return true;
}

bool SaveSkills(SkPointer pSkills, int skillsQuantity, const char *destino)
{
    FILE *dBSkills;
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

bool SaveItems(ItPointer pItems, int ItemsQuantity, const char *destino)
{
    FILE *dBItems;
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

bool SavePikomons(PiPointer pPikomons, int pikomonsQuantity, const char *destino)
{
    FILE *dBPikomons;
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

bool SavePlayers(PlPointer pPlayers, int playersQuantity, const char *destino)
{
    FILE *dBPlayers;
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

// favor usar isso quando as batalhas acabarem para liberar as memorias dinamicas
void FreeAllHeapMemoryAndSaveEverything(SkPointer pSkills, ItPointer pItems, PiPointer pPikomons, PlPointer pPlayers, DataQuantity dataquantities, const char *dataQuantity, const char *skills, const char *items, const char *pikomoms, const char *players)
{
    // pode ter um memset cagado aqui
    if (pSkills == NULL)
    {
        perror("ERRO, \"pSkills\" é NULL em \"FreeAllHeapMemory\"");
    }
    if (pItems == NULL)
    {
        perror("ERRO, \"pItems\" é NULL em \"FreeAllHeapMemory\"");
    }
    if (pPikomons == NULL)
    {
        perror("\"pPikomons\" é NULL em \"FreeAllHeapMemory\"");
    }
    if (pPlayers == NULL)
    {
        perror("ERRO, \"pPlayers\" não pode ser NULL em \"FreeAllHeapMemory\"");
        return false;
    }
    SaveDataQuantity(dataquantities, dataQuantity);
    SaveSkills(pSkills, dataquantities.Skill, skills);
    free(pSkills);
    SaveItems(pItems, dataquantities.Item, items);
    free(pItems);
    SavePikomons(pPikomons, dataquantities.Pikomon, pikomoms);
    free(pPikomons);
    int i, j, k;
    for (i = 0; i < dataquantities.Player; i++)
    {
        for (j = 0; j < pPlayers[i].BagCurrentSize; j++)
        {
            pPlayers[i].Pikocoins += pPlayers[i].Bag[j].Value;
        }
        pPlayers[i].BagCurrentSize = 0;
        free(pPlayers[i].Bag);
        for (j = 0; j < 6; j++)
        {
            for (k = 0; k < 8; k++)
            {
                free(pPlayers[i].BatlePikomons[j].Atributes[k].Bonus);
                free(pPlayers[i].BatlePikomons[j].Atributes[k].BonusTimer);
                pPlayers[i].BatlePikomons[j].Atributes[k].BonusQuantity = 0;
            }
        }
        // esse cara aqui pode dar ruim
        memset(pPlayers[i].BatlePikomons, 0, 6 * sizeof(Pikomon));
    }
    SavePlayers(pPlayers, dataquantities.Player, players);
    free(pPlayers);
}

//------------------------------------------------------------------------------//

/**Manage Memory Functions**/
//------------------------------------------------------------------------------//
bool AddSkill(SkPointer pSkills, DataQuantity dataQuantities, char *name, char target, bool learnablePersonalities[10], bool LearnableElements[10], double elementEffectChance, Element element, int attackBase, double attackScale, int magicBase, double magicAttackScale, double critChance, char effectTarget, double enemyEffectChance, Effect enemyEffect[8], double selfEffectChance, Effect selfEffect[8])
{
    // Se o memset estiver errado ele estara apagando memoria de outras variaveis;
    if (pSkills == NULL)
    {
        perror("ERRO, \"pSkills\" não pode ser NULL em \"AddSkill\"");
        return false;
    }
    if (name == NULL)
    {
        perror("ERRO, \"name\" não pode ser NULL em \"AddSkill\"");
        return false;
    }
    if (target != 'S' && target != 'E' && target != 'B')
    {
        perror("ERRO, \"target\" não pode ser diferente de 'S', 'E' ou 'B' em \"AddSkill\"");
        return false;
    }
    if (elementEffectChance < 0)
    {
        perror("ERRO, \"elementEffectChance\" não pode ser menor que zero em \"AddSkill\"");
        return false;
    }
    if (critChance < 0)
    {
        perror("ERRO, \"critChance\" não pode ser menor que zero em \"AddSkill\"");
        return false;
    }
    if (effectTarget != 'S' && effectTarget != 'E' && effectTarget != 'B')
    {
        perror("ERRO, \"effectTarget\" não pode ser diferente de 'S', 'E' ou 'B' em \"AddSkill\"");
        return false;
    }
    if (enemyEffectChance < 0)
    {
        perror("ERRO, \"enemyEffectChance\" não pode ser menor que zero em \"AddSkill\"");
        return false;
    }
    if (selfEffectChance < 0)
    {
        perror("ERRO, \"selfEffectChance\" não pode ser menor que zero em \"AddSkill\"");
        return false;
    }

    dataQuantities.Skill++;
    pSkills = (SkPointer)realloc(pSkills, dataQuantities.Skill * sizeof(Skill));
    if (pSkills == NULL)
    {
        perror("ERRO na realocação de memoria em \"AddSkill\"");
        return false;
    }
    memset(&pSkills[dataQuantities.Skill - 1], 0, sizeof(Skill));
    strcpy(pSkills[dataQuantities.Skill - 1].Name, name);
    pSkills[dataQuantities.Skill - 1].Target = target;
    int i;
    for (i = 0; i < 10; i++)
    {
        pSkills[dataQuantities.Skill - 1].LearnablePersonalities[i] = learnablePersonalities[i];
        pSkills[dataQuantities.Skill - 1].LearnableElements[i] = LearnableElements[i];
    }
    pSkills[dataQuantities.Skill - 1].ElementEffectChance = elementEffectChance;
    pSkills[dataQuantities.Skill - 1].Element = element;
    pSkills[dataQuantities.Skill - 1].AttackBase = attackBase;
    pSkills[dataQuantities.Skill - 1].AttackScale = attackScale;
    pSkills[dataQuantities.Skill - 1].MagicBase = magicBase;
    pSkills[dataQuantities.Skill - 1].MagicAttackScale = magicAttackScale;
    pSkills[dataQuantities.Skill - 1].CritChance = critChance;
    pSkills[dataQuantities.Skill - 1].EffectTarget = effectTarget;
    pSkills[dataQuantities.Skill - 1].EnemyEffectChance = enemyEffectChance;
    for (i = 0; i < 8; i++)
    {
        pSkills[dataQuantities.Skill - 1].EnemyEffect[i] = enemyEffect[i];
    }
    pSkills[dataQuantities.Skill - 1].SelfEffectChance = selfEffectChance;
    for (i = 0; i < 8; i++)
    {
        pSkills[dataQuantities.Skill - 1].SelfEffect[i] = selfEffect[i];
    }
}

bool AddItem(ItPointer pItems, DataQuantity dataQuantities, char *name, char *type, char *description[3], int value, char effectCurrentHPTarget, Effect enemyEffectCurrentHP, Effect selfEffectCurrentHP, char effectTarget, double enemyStatusEffectChance, Effect enemyStatusEffect[8], double selfStatusEffectChance, Effect selfStatusEffect[8])
{
    // Se o memset estiver errado ele estara apagando memoria de outras variaveis;
    if (pItems == NULL)
    {
        perror("ERRO, \"pItems\" não pode ser NULL em \"AddItem\"");
        return false;
    }
    if (name == NULL)
    {
        perror("ERRO, \"name\" não pode ser NULL em \"AddItem\"");
        return false;
    }
    else if (strlen(name) > 19)
    {
        perror("ERRO, \"name\" não pode ter mais que 19 caracteres em \"AddItem\"");
        return false;
    }
    if (type == NULL)
    {
        perror("ERRO, \"name\" não pode ser NULL em \"AddItem\"");
        return false;
    }
    int i;
    for (i = 0; i < 3; i++)
    {
        if (strlen(description[i]) > 254)
        {
            perror("ERRO, nenhuma das 3 strings na variavel \"*description[3]\" pode ser maior que 254 caracteres em \"AddItem\"");
            return false;
        }
    }
    if (effectCurrentHPTarget == NULL)
    {
        perror("ERRO, \"effectCurrentHPTarget\" não pode ser NULL em \"AddItem\"");
        return false;
    }
    if (effectTarget == NULL)
    {
        perror("ERRO, \"effectTarget\" não pode ser NULL em \"AddItem\"");
        return false;
    }

    dataQuantities.Item++;
    pItems = (ItPointer)realloc(pItems, dataQuantities.Item * sizeof(Item));
    if (pItems == NULL)
    {
        perror("ERRO na realocação de memoria em \"AddItem\"");
        return false;
    }
    memset(&pItems[dataQuantities.Item - 1], 0, sizeof(Item));
    strcpy(pItems[dataQuantities.Item - 1].Name, name);
    strcpy(pItems[dataQuantities.Item - 1].Type, type);
    for (i = 0; i < 3; i++)
    {
        strcpy(pItems[dataQuantities.Item - 1].Description[i], description[i]);
    }
    pItems[dataQuantities.Item - 1].Value = value;
    pItems[dataQuantities.Item - 1].EffectCurrentHPTarget = effectCurrentHPTarget;
    pItems[dataQuantities.Item - 1].EnemyEffectCurrentHP = enemyEffectCurrentHP;
    pItems[dataQuantities.Item - 1].SelfEffectCurrentHP = selfEffectCurrentHP;
    pItems[dataQuantities.Item - 1].EffectTarget = effectTarget;
    pItems[dataQuantities.Item - 1].EnemyStatusEffectChance = enemyStatusEffectChance;
    for (i = 0; i < 8; i++)
    {
        pItems[dataQuantities.Item - 1].EnemyStatusEffect[i] = enemyStatusEffect[i];
    }
    pItems[dataQuantities.Item - 1].SelfStatusEffectChance = selfStatusEffectChance;
    for (i = 0; i < 8; i++)
    {
        pItems[dataQuantities.Item - 1].SelfStatusEffect[i] = selfStatusEffect[i];
    }
}

bool AddPikomon(PiPointer pPikomons, DataQuantity dataQuantities, char *name, Element element, char iconImg[7][19], int BaseHP, int BaseDefense, int BaseMagicDefense, int BaseAcurracy, int BaseAttack, int BaseElementalAcurracy, int BaseMagicAttack, int BaseSpeed)
{
    // Se o memset estiver errado ele estara apagando memoria de outras variaveis;
    if (pPikomons == NULL)
    {
        perror("ERRO, \"pPikomons\" não pode ser NULL em \"AddPikomon\"");
        return false;
    }
    if (name == NULL)
    {
        perror("ERRO, \"name\" não pode ser NULL em \"AddPikomon\"");
        return false;
    }
    else if (strlen(name) > 19)
    {
        perror("ERRO, \"name\" não pode ter mais de 19 caracteres em \"AddPikomon\"");
        return false;
    }
    if (iconImg == NULL)
    {
        perror("ERRO, \"icoImg\" não pode ser NULL em \"AddPikomon\"");
        return false;
    }
    if (BaseHP == 0)
    {
        perror("ERRO, \"BaseHP\" não pode ser 0 em \"AddPikomon\"");
        return false;
    }
    if (BaseDefense == 0)
    {
        perror("ERRO, \"BaseDefense\" não pode ser 0 em \"AddPikomon\"");
        return false;
    }
    if (BaseMagicDefense == 0)
    {
        perror("ERRO, \"BaseMagicDefense\" não pode ser 0 em \"AddPikomon\"");
        return false;
    }
    if (BaseAcurracy == 0)
    {
        perror("ERRO, \"BaseAcurracy\" não pode ser 0 em \"AddPikomon\"");
        return false;
    }
    if (BaseAttack == 0)
    {
        perror("ERRO, \"BaseAttack\" não pode ser 0 em \"AddPikomon\"");
        return false;
    }
    if (BaseElementalAcurracy == 0)
    {
        perror("ERRO, \"BaseElementalAcurracy\" não pode ser 0 em \"AddPikomon\"");
        return false;
    }
    if (BaseMagicAttack == 0)
    {
        perror("ERRO, \"BaseMagicAttack\" não pode ser 0 em \"AddPikomon\"");
        return false;
    }
    if (BaseSpeed == 0)
    {
        perror("ERRO, \"BaseSpeed\" não pode ser 0 em \"AddPikomon\"");
        return false;
    }

    dataQuantities.Pikomon++;
    pPikomons = (PiPointer)realloc(pPikomons, dataQuantities.Pikomon * sizeof(Pikomon));
    if (pPikomons == NULL)
    {
        perror("ERRO na realocação de memoria em \"AddPikomon\"");
        return false;
    }
    memset(&pPikomons[dataQuantities.Pikomon - 1], 0, sizeof(Pikomon));
    strcpy(pPikomons[dataQuantities.Pikomon - 1].Name, name);
    pPikomons[dataQuantities.Pikomon - 1].Element = element;
    int i;
    for (i = 0; i < 7; i++)
    {
        strcpy(pPikomons[dataQuantities.Pikomon - 1].IconImg[i], iconImg[i]);
    }
    strcpy(pPikomons[dataQuantities.Pikomon - 1].CurrentHP.Name, "CurrentHP");
    pPikomons[dataQuantities.Pikomon - 1].CurrentHP.Base = 0;
    strcpy(pPikomons[dataQuantities.Pikomon - 1].Atributes[0].Name, "HP");
    pPikomons[dataQuantities.Pikomon - 1].Atributes[0].Base = BaseHP;
    strcpy(pPikomons[dataQuantities.Pikomon - 1].Atributes[1].Name, "Defense");
    pPikomons[dataQuantities.Pikomon - 1].Atributes[1].Base = BaseDefense;
    strcpy(pPikomons[dataQuantities.Pikomon - 1].Atributes[2].Name, "MagicDefense");
    pPikomons[dataQuantities.Pikomon - 1].Atributes[2].Base = BaseMagicDefense;
    strcpy(pPikomons[dataQuantities.Pikomon - 1].Atributes[3].Name, "Acurracy");
    pPikomons[dataQuantities.Pikomon - 1].Atributes[3].Base = BaseAcurracy;
    strcpy(pPikomons[dataQuantities.Pikomon - 1].Atributes[4].Name, "Attack");
    pPikomons[dataQuantities.Pikomon - 1].Atributes[4].Base = BaseAttack;
    strcpy(pPikomons[dataQuantities.Pikomon - 1].Atributes[5].Name, "ElementalAcurracy");
    pPikomons[dataQuantities.Pikomon - 1].Atributes[5].Base = BaseElementalAcurracy;
    strcpy(pPikomons[dataQuantities.Pikomon - 1].Atributes[6].Name, "MagicAttack");
    pPikomons[dataQuantities.Pikomon - 1].Atributes[6].Base = BaseMagicAttack;
    strcpy(pPikomons[dataQuantities.Pikomon - 1].Atributes[7].Name, "Speed");
    pPikomons[dataQuantities.Pikomon - 1].Atributes[7].Base = BaseSpeed;
    return true;
}

bool AddPlayer(PlPointer pPlayers, DataQuantity dataQuantities, char *name, char *pass)
{
    // Se o memset estiver errado ele estara apagando memoria de outras variaveis;
    if (pPlayers == NULL)
    {
        perror("ERRO, \"pPlayers\" não pode ser NULL em \"AddPlayers\"");
        return false;
    }
    if (pass == NULL)
    {
        perror("ERRO, \"pass\" não pode ser NULL em \"AddPlayers\"");
        return false;
    }
    else if (strlen(pass) != 6)
    {
        perror("ERRO, \"pass\" não pode ter um tamanho diferente de 6 caracteres em \"AddPlayers\"");
        return false;
    }
    if (name == NULL)
    {
        perror("ERRO, \"name\" não pode ser NULL em \"AddPlayers\"");
        return false;
    }
    else if (strlen(name) > 19)
    {
        perror("ERRO, \"name\" não pode ter mais de 19 caracteres em \"AddPlayers\"");
        return false;
    }

    dataQuantities.Player++;
    pPlayers = (PlPointer)realloc(pPlayers, dataQuantities.Player * sizeof(Player));
    if (pPlayers == NULL)
    {
        perror("ERRO na realocação de memoria em \"AddPlayers\"");
        return false;
    }
    memset(&pPlayers[dataQuantities.Player - 1], 0, sizeof(Player));
    strcpy(pPlayers[dataQuantities.Player - 1].Name, name);
    strcpy(pPlayers[dataQuantities.Player - 1].Pass, pass);
    pPlayers[dataQuantities.Player - 1].Pikocoins = 50;
    return true;
}

bool AddItemPlayerBag(PlPointer pPlayers, int playerIndex, ItPointer pItems, int itemIndex)
{
    if (pPlayers == NULL)
    {
        perror("ERRO, \"pPlayers\" não pode ser NULL em \"AddItemPlayerBag\"");
        return false;
    }
    if (pItems == NULL)
    {
        perror("ERRO, \"pItems\" não pode ser NULL em \"AddItemPlayerBag\"");
        return false;
    }
    if (playerIndex < 0)
    {
        perror("ERRO, \"playerIndex\" não pode ser menor do que zero em \"AddItemPlayerBag\"");
        return false;
    }
    if (itemIndex < 0)
    {
        perror("ERRO, \"itemIndex\" não pode ser menor do que zero em \"AddItemPlayerBag\"");
        return false;
    }

    pPlayers[playerIndex].BagCurrentSize++;
    pPlayers[playerIndex].Bag = (ItPointer)realloc(pPlayers[playerIndex].Bag, pPlayers[playerIndex].BagCurrentSize * sizeof(Item));
    if (pPlayers[playerIndex].Bag == NULL)
    {
        perror("ERRO na realocação de memoria em \"AddItemPlayerBag\"");
        return false;
    }
    pPlayers[playerIndex].Bag[pPlayers[playerIndex].BagCurrentSize - 1] = pItems[itemIndex];
}

bool StorePikomonPlayer(PlPointer pPlayers, int playerIndex, int storagePikomonPlacementIndex, PiPointer pPikomons, int pikomonIndex, DataQuantity dataQuantities)
{
    // n precisa uasar esse
    if (pPlayers == NULL)
    {
        perror("ERRO, \"pPlayers\" não pode ser NULL em \"StorePikomonPlayer\"");
        return false;
    }
    if (playerIndex < 0)
    {
        perror("ERRO, \"playerIndex\" não pode ser menor que zero em \"StorePikomonPlayer\"");
        return false;
    }
    else if (playerIndex >= dataQuantities.Player)
    {
        perror("ERRO, \"playerIndex\" tem que ser menor que \"dataQuantities.Player\" em \"StorePikomonPlayer\"");
        return false;
    }
    if (storagePikomonPlacementIndex < 0)
    {
        perror("ERRO, \"storagePikomonPlacementIndex\" não pode ser menor que zero em \"StorePikomonPlayer\"");
        return false;
    }
    else if (storagePikomonPlacementIndex >= 12)
    {
        perror("ERRO, \"storagePikomonPlacementIndex\" tem que ser menor que 12 em \"StorePikomonPlayer\"");
        return false;
    }
    if (pPikomons == NULL)
    {
        perror("ERRO, \"pPikomons\" não pode ser NULL em \"StorePikomonPlayer\"");
        return false;
    }
    if (pikomonIndex < 0)
    {
        perror("ERRO, \"pikomonIndex\" não pode ser menor que zero em \"StorePikomonPlayer\"");
        return false;
    }
    else if (pikomonIndex >= dataQuantities.Pikomon)
    {
        perror("ERRO, \"pikomonIndex\" tem que ser menor que \"dataQuantities.Pikomon\" em \"StorePikomonPlayer\"");
        return false;
    }
    pPlayers[playerIndex].PikomonsStorage[storagePikomonPlacementIndex] = pPikomons[pikomonIndex];
}

bool RemoveSkill(SkPointer pSkills, DataQuantity dataQuantities, int indexRemove)
{
    /*
    if(indexRemove < 0){
        perror("ERRO, \"indexRemove\" não pode ser menor que zero em \"RemoveSkill\"");
        return false;
    }
    else if(indexRemove >= dataQuantities.Skill){
        perror("ERRO, \"indexRemove\" tem que ser menor que \"dataQuantities.Skill\" em \"RemoveSkill\"");
        return false;
    }

    Skill tempSkills[dataQuantities.Skill-1];
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
    */
}

bool RemoveItem(ItPointer pItems, DataQuantity dataQuantities, int indexRemove)
{
    /*
    if(indexRemove < 0){
        perror("ERRO, \"indexRemove\" não pode ser menor que zero em \"RemoveItem\"");
        return false;
    }
    else if(indexRemove >= dataQuantities.Item){
        perror("ERRO, \"indexRemove\" tem que ser menor que \"dataQuantities.Item\" em \"RemoveItem\"");
        return false;
    }

    Item tempItems[dataQuantities.Item-1];
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
    */
}

bool RemovePikomon(PiPointer pPikomons, DataQuantity dataQuantities, int indexRemove)
{
    /*
    if(indexRemove < 0){
        perror("ERRO, \"indexRemove\" não pode ser menor que zero em \"RemovePikomon\"");
        return false;
    }
    else if(indexRemove >= dataQuantities.Pikomon){
        perror("ERRO, \"indexRemove\" tem que ser menor que \"dataQuantities.Pikomon\" em \"RemovePikomon\"");
        return false;
    }

    Pikomon tempPikomons[dataQuantities.Pikomon-1];
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
    */
}

bool SellItemPlayerBag(PlPointer pPlayers, int playerIndex, int bagSellIndex)
{
    /*
    if(bagSellIndex >= pPlayers[playerIndex].BagCurrentSize){
        perror("ERRO, \"bagSellIndex\" tem que ser menor que \"pPlayers[playerIndex].BagCurrentSize\" em \"SellItemPlayerBag\"");
        return false;
    }
    else if(bagSellIndex < 0){
        perror("ERRO, \"bagSellIndex\" não pode ser menor que zero em \"SellItemPlayerBag\"");
        return false;
    }

    Item tempItems[pPlayers[playerIndex].BagCurrentSize-1];
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
    */
}
//------------------------------------------------------------------------------//

/**Battle functions**/
//------------------------------------------------------------------------------//

void CalcNextTurn(Pikomon selfPikomon, Pikomon enemyPikomon, char calcNextTurn[7])
{
    // calcNextTurn vai ser a resposta a ser gerada

    calcNextTurn[6] = '\0';
    bool b;
    int i = 0, turnCost, selfSpeedCharged, enemySpeedCharge;

    if (selfPikomon.Atributes[7].Total > enemyPikomon.Atributes[7].Total)
        turnCost = enemyPikomon.Atributes[7].Total, b = true;
    else
        turnCost = selfPikomon.Atributes[7].Total, b = false;

    if (selfPikomon.Atributes[7].Total == enemyPikomon.Atributes[7].Total)
    {
        if ((rand() % 100 + 1) > 50)
            b = true;
        else
            b = false;
        turnCost = selfPikomon.Atributes[7].Total;
    }
    while (i < 6)
    {
        if (b)
        {
            selfSpeedCharged += selfPikomon.Atributes[7].Total;
            while (selfSpeedCharged - turnCost >= 0)
            {
                selfSpeedCharged -= turnCost;
                calcNextTurn[i] = 'S';
                i++;
                if (i >= 6)
                    break;
            }
            b = !b;
        }
        else
        {
            enemySpeedCharge += enemyPikomon.Atributes[7].Total;
            while (enemySpeedCharge - turnCost >= 0)
            {
                enemySpeedCharge -= turnCost;
                calcNextTurn[i] = 'E';
                i++;
                if (i >= 6)
                    break;
            }
            b = !b;
        }
    }
}

void Batle(PlPointer pPlayers, int playerOneIndex, int playerTwoIndex)
{
    /*
    bool playerOneTurn;
    int i = 0, turnCost;
    PiPointer selectedPlayerOnePicomon = &pPlayers[playerOneIndex].BatlePikomons[pPlayers[playerOneIndex].SelectedPikomonIndex], selectedPlayerTwoPicomon;
    if(selectedPlayerOnePicomon[0].Atributes[7].Total > pPlayers[playerOneIndex].BatlePikomons[pPlayers[playerOneIndex].SelectedPikomonIndex].Atributes[7].Total) turnCost = enemyPikomon.Atributes[7].Total, b = true;
    else turnCost = selfPikomon.Atributes[7].Total, b = false;

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
                calcNextTurn[i] = 'S';
                i++;
                if(i >= 6) break;
            }
            b = !b;
        }
        else{
            enemySpeedCharge += enemyPikomon.Atributes[7].Total;
            while(enemySpeedCharge - turnCost >= 0){
                enemySpeedCharge -= turnCost;
                calcNextTurn[i] = 'E';
                i++;
                if(i >= 6) break;
            }
            b = !b;
        }
    }
}
*/
    //------------------------------------------------------------------------------//
