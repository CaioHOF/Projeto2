# 📋 Introdução

Bem vindo ao Simulador de Batalhas Pikomon! Nosso segundo projeto é um pequeno jogo em C, um simulador de batalhas de turno! A experiência é recomendada com dois jogadores.

# 🔧 Uso do Programa

O programa pode ser executado tanto em IDEs instalados em uma máquina quanto em IDEs online. Caso o programa que você está usando não tenha embutido, certifique-se de instalar a extensão de C/C++. Caso você esteja usando o Replit, retire todos os arquivos da pasta "[filler]" ao descompactar o arquivo .zip. Ao executar o arquivo "main.c", uma tela de login será aberta. Crie seu perfil e você pode começar a jogar!

# 🕹️ O Jogo em Si

Cada conta começa com 50 PikoCoins, e após selecionar a opção de batalha, você decide como irá gastá-las. Sua "party" consiste de, no máximo, 6 Pikomons, e você pode usar quantos itens conseguir comprar.
Cada Pikomon possui os seguintes Atributos:

```
HP: A vida do Pikomon. Quando chegar a zero, ele está derrotado e não poderá mais batalhar. Derrote todos os do seu oponente para ganhar!
Def: A defesa do Pikomon, indica o quão resiliente ele é a golpes físicos;
MaD: A defesa mágica do Pikomon, indica o quão resiliente ele é a golpes mágicos;
Acc: A precisão dos golpes físicos do seu Pikomon;
Atk: A força que os golpes físicos do seu Pikomon serão disparados;
EAc: A precisão dos golpes mágicos do seu Pikomon;
MaA: A força que os golpes mágicos do seu Pikomon serão disparados;
Spd: Determina quem ataca antes, e determina quantas vezes cada jogador ataca.
```

Cada Pikomon possui um Elemento, que determina os golpes aos quais ele tem acesso e o dano que ele recebe de golpes de dados elementos. Segue a tabela:
```
Comum                                           |   | Misterioso
Fraco Contra: Nada                              |   | Fraco Contra: Misterioso, Luminoso
Resiste: Nada                                   |   | Resiste: Nada
================================================+===+=======================================================
Flamejante                                      |   | Luminoso
Fraco Contra: Aquático, Mineral                 |   | Fraco Contra: Misterioso, 
Resiste: Flamejante, Natural, Luminoso, Gélido  |   | Resiste: Nada
================================================+===+=======================================================
Aquático                                        |   | Gélido
Fraco Contra: Elétrico, Natural                 |   | Fraco Contra: Flamejante
Resiste: Flamejante, Aquático, Gélido           |   | Resiste: Gélido, Aquático, Natural, Elétrico, Mineral
================================================+===+=======================================================
Natural                                         |   | Mineral
Fraco Contra: Flamejante, Gélido, Venenoso      |   | Fraco Contra: Natural, Aquático, Gélido
Resiste: Natural, Aquático, Mineral, Elétrico   |   | Resiste: Venenoso, Flamejante, Elétrico
================================================+===+=======================================================
Elétrico                                        |   | Venenoso
Fraco Contra: Mineral                           |   | Fraco Contra: Mineral 
Resiste: Elétrico, Luminoso                     |   | Resiste: Venenoso, Natural
```

Com isso, você está pronto para se tornar um Mestre Pikomon! Aproveite e divirta-se!
# Participantes:

* Caio Henrique de Oliveira Fonseca
* RA: 24.124.066-2
* Davi Smuczek Loureiro
* RA: 24.124.016-7
* Lucas Rossi Dotzlaw
* RA: 24.124.025-8