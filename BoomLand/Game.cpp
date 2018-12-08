#include "Game.h"
#include "utilities.h"

Game::Game(){
    this->player[0] = nullptr;
    this->player[1] = nullptr;
    this->game = nullptr;
    this->log = nullptr;

    this->isAuctionCompleted = false;
    this->isEmbattleCompleted = false;
    this->isFightingCompleted = false;
}

Game::Game(__FOLDER player1, __FOLDER player2, __FOLDER game, __FILE log)
    : Game(){
    this->player[0] = player1;
    this->player[1] = player2;
    this->game = game;
    this->log = log;
}

// Copy constructor
Game::Game(const Game &game) : Game(){
    this->player[0] = __strcpy(game.player[0]);
    this->player[1] = __strcpy(game.player[1]);
    this->game = __strcpy(game.game);
    this->log = __strcpy(game.log);

    this->isAuctionCompleted = game.isAuctionCompleted;
    this->isEmbattleCompleted = game.isEmbattleCompleted;
    this->isFightingCompleted = game.isFightingCompleted;
}

Game::~Game(){
    delete[] this->player[0];
    delete[] this->player[1];
    delete[] this->game;
    delete[] this->log;

    this->player[0] = nullptr;
    this->player[1] = nullptr;
    this->game = nullptr;
    this->log = nullptr;
}

int Game::setPlayerFolder(__FOLDER player1, __FOLDER player2){
    
    delete[] this->player[0];
    delete[] this->player[1];
   
    if (player1 == nullptr || player2 == nullptr){
        this->player[0] = nullptr;
        this->player[1] = nullptr;
        return 1;
    }
       
    this->player[0] = __strcpy(player1);
    this->player[1] = __strcpy(player2);

    return 0;
}

int Game::setGameFolder(__FOLDER game){
    delete[] this->game;

    if (game == nullptr){
        this->game = nullptr;
        return 1;
    }

    this->game = __strcpy(game);
    return 0;
}

int Game::setLogFile(__FILE log){
    delete[] this->log;

    if (log == nullptr){
        this->log = nullptr;
        return 1;
    }

    this->log = __strcpy(log);
    return 0;
}