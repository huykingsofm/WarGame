// Author : Le Ngoc Huy - UIT 2017
// Github : https://github.com/huykingsofm
//
// Class Game for BoomLand game
// 
// Include folder and file of Game
// This is only abtract class which is inherited by 4 Class 1. Auction  2. Seletion  3. Embattle   4. Fight

#pragma once
#include "ARRAY.h"

typedef const char * __FOLDER;
typedef const char * __FILE;

typedef struct{
    int price;
    int armor;
    int range;
    int strength;
} TANK, *PTANK;


class Game{
public:
    Game();
    Game(__FOLDER, __FOLDER, __FOLDER, __FILE);

    Game(const Game&);

public:
    int setPlayerFolder(__FOLDER, __FOLDER);
    int setGameFolder(__FOLDER);
    int setLogFile(__FILE);

public:
    virtual void start(void *) = 0;
    virtual void play(void *)  = 0;
    virtual void end(void *)   = 0;

public:
    ~Game();

protected:
    __FOLDER player[2];
//    __FOLDER player2;
    __FOLDER game;

    __FILE log;

    bool isAuctionCompleted;
    bool isEmbattleCompleted;
    bool isFightingCompleted;
};