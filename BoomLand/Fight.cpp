#include "Fight.h"
#include <stdio.h>
#include "utilities.h"

Fight::Fight() : Game(){
    this->iTurn = 0;
    this->nRealTank[0][0] = 0;
    this->nRealTank[1][0] = 0;
}

Fight::Fight(const Game &game) : Game(game){
    this->iTurn = 0;
    this->nRealTank[0][0] = 0;
    this->nRealTank[1][0] = 0;
}

Fight::~Fight(){

}

bool Fight::ReadData(PRES_START_FIGHT pres){
    // Reading from fight.data and store it into TankOfPlayerX[0], battle[0] - preparing step
    __FILE filePath = __strcat(this->game, "\\IO\\fight.data");
    FILE * f = fopen(filePath, "r");
    delete[] filePath;

    if (f == nullptr){
        // if it can't open file
        // just exit program with bSuccess = false
        if (pres != nullptr)
            pres->bSuccess = false;
        return false;
    }   

    fscanf(f, "%d %d", &this->TankOfPlayer[0][0].n, &this->TankOfPlayer[1][0].n);
    this->TankOfPlayer[0][0].create(this->TankOfPlayer[0][0].n + 1);
    this->TankOfPlayer[1][0].create(this->TankOfPlayer[1][0].n + 1);

    for (int i = 1; i <= this->TankOfPlayer[0][0].n; i++){
        fscanf(f, "%d %d %d", 
            &this->TankOfPlayer[0][0].tArray[i].armor,
            &this->TankOfPlayer[0][0].tArray[i].range,
            &this->TankOfPlayer[0][0].tArray[i].strength);
    }

    
    for (int i = 1; i <= this->TankOfPlayer[1][0].n; i++){
        fscanf(f, "%d %d %d", 
            &this->TankOfPlayer[1][0].tArray[i].armor,
            &this->TankOfPlayer[1][0].tArray[i].range,
            &this->TankOfPlayer[1][0].tArray[i].strength);
    }

    for (int i = 1; i <= 8; i++)
        for (int j = 1; j <= 8; j++)
            fscanf(f, "%d", &this->battle[0][i][j]);

    fclose(f);
    // END READING FIGHT.DATA
    return true;
}

void Fight::start(void * res){
    PRES_START_FIGHT pres = (PRES_START_FIGHT) res;

    if (!this->ReadData(pres))
        return;

    // Setting up for first turn
    this->pos[0][0].n = this->TankOfPlayer[0][0].n;
    this->pos[1][0].n = this->TankOfPlayer[1][0].n;
    
    this->pos[0][0].create(this->pos[0][0].n + 1);
    this->pos[1][0].create(this->pos[1][0].n + 1);

    for (int i = 1; i <= this->pos[0][0].n; i++){
        this->pos[0][0].tArray[i].x = -1;
        this->pos[0][0].tArray[i].y = -1;
    }

    for (int i = 1; i <= this->pos[1][0].n; i++){
        this->pos[1][0].tArray[i].x = -1;
        this->pos[1][0].tArray[i].y = -1;
    }
    this->nRealTank[0][0] = this->TankOfPlayer[0][0].n;
    this->nRealTank[1][0] = this->TankOfPlayer[1][0].n;
      
    for (int i = 1; i <= 8; i++)
        for (int j = 1; j <= 8; j++)
            if (this->battle[0][i][j] != 0){
                if (this->battle[0][i][j] > 0){
                    int id = this->battle[0][i][j];
                    this->pos[0][0].tArray[id].x = i;
                    this->pos[0][0].tArray[id].y = j;
                }
                else{
                    int id = -this->battle[0][i][j];
                    this->pos[1][0].tArray[id].x = i;
                    this->pos[1][0].tArray[id].y = j;
                }
            }
    
    // WRITING TO FIGHT.INP
    // fight.inp has following contruction:
    //      + first line is id of player
    //      + all remain lines is completely same to fight.data
    this->WriteInputForPlayer(0);
    this->WriteInputForPlayer(1);

    this->iTurn = 1;
    
    if (pres != nullptr){
        pres->bSuccess = true;
        for (int i = 1; i <= 8; i++)
            for (int j = 1; j <= 8; j++)
                pres->battle[i][j] = this->battle[0][i][j];

        pres->tank1.n = this->TankOfPlayer[0][0].n;
        pres->tank2.n = this->TankOfPlayer[1][0].n;
        pres->tank1.create(pres->tank1.n + 1);
        pres->tank2.create(pres->tank2.n + 1);

        for (int i = 1; i <= pres->tank1.n; i++){
            pres->tank1.tArray[i] = this->TankOfPlayer[0][0].tArray[i];
        }
        
        for (int i = 1; i <= pres->tank2.n; i++){
            pres->tank2.tArray[i] = this->TankOfPlayer[1][0].tArray[i];
        }
    }
}

void Fight::InitializationForNewTurn(){
    if (this->iTurn > 0){
        for (int iPlayer = 0; iPlayer <= 1; iPlayer++){
            this->nRealTank[iPlayer][this->iTurn] = this->nRealTank[iPlayer][this->iTurn - 1];

            this->pos[iPlayer][this->iTurn].n = this->pos[iPlayer][this->iTurn - 1].n;
            this->TankOfPlayer[iPlayer][this->iTurn].n = this->TankOfPlayer[iPlayer][this->iTurn - 1].n;
            
            this->pos[iPlayer][this->iTurn].create(this->pos[iPlayer][this->iTurn].n + 1);
            this->TankOfPlayer[iPlayer][this->iTurn].create(this->TankOfPlayer[iPlayer][this->iTurn].n + 1);

            for (int i = 1; i <= this->pos[iPlayer][this->iTurn].n; i++){
                this->pos[iPlayer][this->iTurn].tArray[i] = 
                    this->pos[iPlayer][this->iTurn - 1].tArray[i];
                
                this->TankOfPlayer[iPlayer][this->iTurn].tArray[i] = 
                    this->TankOfPlayer[iPlayer][this->iTurn - 1].tArray[i];
            }
        }
        for (int i = 1; i <= 8; i++)
            for (int j = 1; j <= 8; j++)
                this->battle[this->iTurn][i][j] = 0;
    }
}

int Fight::ReadOutputFromPlayer(int iPlayer, int temp[]){
    // File fight.out has following contruction:
    //      + first line is iTurn - current turn
    //      + second line is four values, which is type, idTank, x and y
    //          where:
    //              type is 1(moving) or 2(shooting), or 0 (if no-move)
    //              idTank is id of tank of corresponding player
    //              x, y is coordinates of move of this turn
    __FILE filePath = __strcat(this->player[iPlayer], "\\IO\\fight.out");

    FILE * f = fopen(filePath, "r");
    delete[] filePath;

    if (f == nullptr)
        return READ_INVALID;


    // Reading data from file
    int n = 0;
    while (!feof(f)){
        fscanf(f, "%d ", &temp[n]);
        n++;
        if (n > 5)
            break;
    }
    fclose(f);

    if (n > 5){
        return READ_INVALID;
    }

    if (temp[0] != this->iTurn)
        return READ_INVALID;

    // a move has to be in three values (0, 1, 2)
    if (temp[1] != 1 && temp[1] != 2)
        return READ_INVALID;

    // id of tank has to exist in tanks of corresponding player
    int nTank = this->TankOfPlayer[iPlayer][this->iTurn - 1].n;
    if (temp[2] < 1 || temp[2] > nTank)
        return READ_INVALID;

    if (this->pos[iPlayer][this->iTurn].tArray[temp[2]].x == -1 ||
        this->pos[iPlayer][this->iTurn].tArray[temp[2]].y == -1)
        return READ_INVALID;

    // Calculating distance between new pos and old pos
    int dx = this->pos[iPlayer][this->iTurn - 1].tArray[temp[2]].x - temp[3];
    int dy = this->pos[iPlayer][this->iTurn - 1].tArray[temp[2]].y - temp[4];

   int distance = dx * dx + dy * dy;

    // the coordinates has to be in chessboard area
    if (__OutOfChessBoard(temp[3], temp[4]))
        if (temp[1] == 1 && distance == 1)        
            return READ_OUT_OF_RANGE;
        else 
            return READ_INVALID;

    if (temp[1] == 1){ 
        // If it is a moving step  
        // Then distance must be less than or equal to 1
        if (distance > 1)
            return READ_INVALID;
    }// END of solving moving step

    if (temp[1] == 2){
        // If it is shooting step
        // Then distance must be less than or equal to range of the tank
        int iRange = this->TankOfPlayer[iPlayer][this->iTurn].tArray[temp[2]].range;

        if (distance > iRange * iRange)
            return READ_INVALID;
    }//--> End of solving shooting step
    return READ_VALID;
}// End of Read Output From Player

void Fight::WriteInputForPlayer(int iPlayer){

    // WRITING TO FIGHT.inp for next turn
    // WRITING TO FIGHT.INP
    // fight.inp has following contruction:
    //      + first line is id of player
    //      + all remain lines is completely same to fight.data

    __FILE filePath = __strcat(this->player[iPlayer], "\\IO\\fight.inp");
    FILE * f = fopen(filePath, "w");
    delete[] filePath;
    
    fprintf(f, "%d\n", this->iTurn + 1);        // id of turn
    fprintf(f, "%d\n", iPlayer + 1);                  // id of player
    fprintf(f, "%d %d\n", 
        this->TankOfPlayer[0][this->iTurn].n, 
        this->TankOfPlayer[1][this->iTurn].n);
    
    for (int i = 1; i <= this->TankOfPlayer[0][this->iTurn].n; i++){
        fprintf(f, "%d %d %d\n", 
            this->TankOfPlayer[0][this->iTurn].tArray[i].armor,
            this->TankOfPlayer[0][this->iTurn].tArray[i].range,
            this->TankOfPlayer[0][this->iTurn].tArray[i].strength);
    }
    for (int i = 1; i <= this->TankOfPlayer[1][this->iTurn].n; i++){
        fprintf(f, "%d %d %d\n", 
            this->TankOfPlayer[1][this->iTurn].tArray[i].armor,
            this->TankOfPlayer[1][this->iTurn].tArray[i].range,
            this->TankOfPlayer[1][this->iTurn].tArray[i].strength);
    }
    
    for (int i = 1; i <= 8; i++){
        for (int j = 1; j <= 8; j++)
            fprintf(f, "%d ", this->battle[this->iTurn][i][j]);
        fprintf(f, "\n");
    }

    fclose(f);
    // END OF WRITING
}

void Fight::ConfigureMove(int iPlayer){
    //Read fight.out of each player
    int temp[10];
    int isValid = this->ReadOutputFromPlayer(iPlayer, temp);
    
    if (isValid){
        this->move[iPlayer][this->iTurn].type  = temp[1];
        this->move[iPlayer][this->iTurn].id    = temp[2];
        this->move[iPlayer][this->iTurn].pos.x = temp[3];
        this->move[iPlayer][this->iTurn].pos.y = temp[4];
        
        int x = this->pos[iPlayer][this->iTurn - 1].tArray[temp[2]].x;
        int y = this->pos[iPlayer][this->iTurn - 1].tArray[temp[2]].y;

        if (this->move[iPlayer][this->iTurn].type == 1)
            this->battle[this->iTurn][x][y] = 0;
    }
    else{
        this->move[iPlayer][this->iTurn].type  = 0;
        this->move[iPlayer][this->iTurn].id    = 0;
        this->move[iPlayer][this->iTurn].pos.x = 0;
        this->move[iPlayer][this->iTurn].pos.y = 0;
    }
}

void Fight::ContructChessBoard(){
    for (int iPlayer = 0; iPlayer < 2; iPlayer++){
        // go thround 2 player
        for (int i = 1; i <= this->pos[iPlayer][this->iTurn].n; i++){
            //go through all tanks of each player
            int x = this->pos[iPlayer][this->iTurn].tArray[i].x;
            int y = this->pos[iPlayer][this->iTurn].tArray[i].y;

            if (!__OutOfChessBoard(x, y)){
                // if iPlayer = 0 -> Signal = 1
                //    iPlayer = 1 -> Signal = -1
                int Signal = iPlayer == 0 ? 1 : -1;
                this->battle[this->iTurn][x][y] = Signal * i;
            }
        }// End of loop (all tank)
    }// End of loop (all player)
}



void Fight::RemoveTank(int iPlayer, int iTank){
    // Remove tank from battle
    // Step 1 : Remove tank from chessboard
    // Step 2 : Set up position of tank is (-1, -1)
    // Step 3 : Set up tank armor is 0
    // Step 4 : Decrease nRealTank
    
    // Step 1
    __POS pos = this->pos[iPlayer][this->iTurn].tArray[iTank];
    if (pos.x != -1 && pos.y != -1)
        this->battle[this->iTurn][pos.x][pos.y] = 0;

    // Step 2
    pos.x = pos.y = -1;
    this->pos[iPlayer][this->iTurn].tArray[iTank] = pos;

    // Step 3
    this->TankOfPlayer[iPlayer][this->iTurn].tArray[iTank].armor = 0;

    // Step 4
    this->nRealTank[iPlayer][this->iTurn]--;
}

bool Fight::TankCrossOverEvent(__EVENT_ON_CHESSBOARD * pEvent, int &nevent){
    int idTank[2];
    __POS pOld[2], pNew[2];

    if (this->move[0][this->iTurn].type != 1 || this->move[1][this->iTurn].type != 1)
        return false;

    for (int iPlayer = 0; iPlayer < 2; iPlayer++){
        idTank[iPlayer] = this->move[iPlayer][this->iTurn].id;
        pOld[iPlayer] = this->pos[iPlayer][this->iTurn - 1].tArray[idTank[iPlayer]];
        pNew[iPlayer] = this->move[iPlayer][this->iTurn].pos;
    }

    if (pOld[0].x == pNew[1].x && pOld[0].y == pNew[1].y && 
        pOld[1].x == pNew[0].x && pOld[1].y == pNew[0].y)
    {
        this->RemoveTank(0, idTank[0]);
        this->RemoveTank(1, idTank[1]);

        pEvent[nevent++] = __CreateEventMoving(0, idTank[0], pOld[0], pNew[0]);
        pEvent[nevent++] = __CreateEventMoving(1, idTank[1], pOld[1], pNew[1]);
        pEvent[nevent++] = __CreateEventExploisive(pNew[0]);
        pEvent[nevent++] = __CreateEventExploisive(pNew[1]);
        pEvent[nevent++] = __CreateEventDisappear(0, idTank[0], pNew[0]);
        pEvent[nevent++] = __CreateEventDisappear(1, idTank[1], pNew[1]);
        return true;
    }
    return false;
}

void Fight::TankMovingEvent(int iPlayer, __EVENT_ON_CHESSBOARD * pEvent, int &nevent){
    if (this->move[iPlayer][this->iTurn].type != 1)
        return;
    
    int idTank = this->move[iPlayer][this->iTurn].id;
    __POS newPos = this->move[iPlayer][this->iTurn].pos;
    __POS oldPos = this->pos[iPlayer][this->iTurn - 1].tArray[idTank];

    pEvent[nevent++] = __CreateEventMoving(iPlayer, idTank, oldPos, newPos);

    if (__OutOfChessBoard(newPos)){
        this->RemoveTank(iPlayer, idTank);
        pEvent[nevent++] = __CreateEventExploisive(newPos);
        pEvent[nevent++] = __CreateEventDisappear(iPlayer, idTank, newPos);
    }
    else if(this->battle[this->iTurn][newPos.x][newPos.y] != 0){
        int iHitTank = this->battle[this->iTurn][newPos.x][newPos.y];
        int iHitPlayer = iHitTank > 0 ? 0 : 1;
        iHitTank = abs(iHitTank);

        this->RemoveTank(iPlayer, idTank);
        this->RemoveTank(iHitPlayer, iHitTank);

        pEvent[nevent++] = __CreateEventExploisive(newPos);
        pEvent[nevent++] = __CreateEventDisappear(iPlayer, idTank, newPos);
    }
    else{
        this->pos[iPlayer][this->iTurn].tArray[idTank] = newPos;

        int Signal = iPlayer == 0 ? 1 : -1;
        this->battle[this->iTurn][oldPos.x][oldPos.y] = 0;
        this->battle[this->iTurn][newPos.x][newPos.y] = Signal * idTank;
    }
}

void Fight::TankShootingEvent(int iPlayer, __EVENT_ON_CHESSBOARD * pEvent, int &nevent){
    if (this->move[iPlayer][this->iTurn].type != 2)
        return;

    int idTank = this->move[iPlayer][this->iTurn].id;
    __POS Pos = this->pos[iPlayer][this->iTurn].tArray[idTank];
    __POS ShootingPos = this->move[iPlayer][this->iTurn].pos;

    //if (Pos.x == -1 || Pos.y == -1)
     //   return;

    pEvent[nevent++] = __CreateEventShooting(iPlayer, idTank, Pos, ShootingPos);

    if (this->battle[this->iTurn][ShootingPos.x][ShootingPos.y] != 0){
        int idShootedTank = this->battle[this->iTurn][ShootingPos.x][ShootingPos.y];
        int iShootedPlayer = idShootedTank > 0 ? 0 : 1;
        idShootedTank = abs(idShootedTank);

        this->TankOfPlayer[iShootedPlayer][this->iTurn].tArray[idShootedTank].armor -=
            this->TankOfPlayer[iPlayer][this->iTurn].tArray[idTank].strength;

        if (this->TankOfPlayer[iShootedPlayer][this->iTurn].tArray[idShootedTank].armor <= 0){
            this->RemoveTank(iShootedPlayer, idShootedTank);

            pEvent[nevent++] = __CreateEventExploisive(ShootingPos);
            pEvent[nevent++] = __CreateEventDisappear(iShootedPlayer, idShootedTank, ShootingPos);
        }
    }
}

void Fight::SolvingEventOnChessBoard(__EVENT_ON_CHESSBOARD * pEvent, int &nevent){
    bool bCrossOver = this->TankCrossOverEvent(pEvent, nevent);
    if (!bCrossOver){
        this->TankMovingEvent(0, pEvent, nevent);
        this->TankMovingEvent(1, pEvent, nevent);
        this->TankShootingEvent(0, pEvent, nevent);
        this->TankShootingEvent(1, pEvent, nevent);
    }
}

void Fight::WriteResult(PRES_FIGHT pres, __EVENT_ON_CHESSBOARD event[], int nevent){
    if (pres != nullptr){
        pres->iTurn = this->iTurn;
        if (this->iTurn < MAX_TURN)
            if (this->nRealTank[0][this->iTurn] == 0)
                pres->exitCode = RES_FIGHT::PLAYER2_WIN;
            else if (this->nRealTank[1][this->iTurn] == 0)
                pres->exitCode = RES_FIGHT::PLAYER1_WIN;
            else
                pres->exitCode = RES_FIGHT::GO_NEXT_TURN;
        else
            pres->exitCode = RES_FIGHT::LAST_TURN;

        pres->iTurn = this->iTurn;
        for (int iPlayer = 0; iPlayer <= 1; iPlayer++){
            pres->tank[iPlayer].n = this->TankOfPlayer[iPlayer][this->iTurn].n;
            
            pres->tank[iPlayer].create(pres->tank[iPlayer].n + 1);

            for (int i = 1; i <= pres->tank[iPlayer].n; i++){    
                pres->tank[iPlayer].tArray[i] = 
                    this->TankOfPlayer[iPlayer][this->iTurn].tArray[i];
            }
        }
        for (int i = 1; i <= 8; i++)
            for (int j = 1; j <= 8; j++)
                pres->battle[i][j] = this->battle[this->iTurn][i][j];

        for (int i = 0; i<nevent; i++){
            pres->event[i] = event[i];
        }
        pres->nevent = nevent;
    }
}

void Fight::play(void * res){
    PRES_FIGHT pres = (PRES_FIGHT) res;

    if (this->iTurn > MAX_TURN){
        if (pres != nullptr)
            pres->exitCode = RES_FIGHT::CLOSED;
        return;
    }

    if (this->nRealTank[0][this->iTurn - 1] == 0){
        if (pres != nullptr)
            pres->exitCode = RES_FIGHT::PLAYER2_WIN;
        return;
    }
    else if (this->nRealTank[1][this->iTurn - 1] == 0){
        if (pres != nullptr)
            pres->exitCode = RES_FIGHT::PLAYER1_WIN;
        return;
    }

    // Launch 2 files fight.exe of each player to create a move
    HANDLE hEx[2];
    hEx[0] = CreateMarker(this->player[0], "fight.exe", 3);
    hEx[1] = CreateMarker(this->player[1], "fight.exe", 3);

    //Sleep(3000);
    WaitForSingleObject(hEx[0], 3000);
    WaitForSingleObject(hEx[1], 3000);

    CloseHandle(hEx[0]);
    CloseHandle(hEx[1]);

    // Initialization for battle
    // Creating a status battle same to previous status
    this->InitializationForNewTurn();

    int nevent = 0;
    __EVENT_ON_CHESSBOARD event[20];

    // Remove tanks which is not be placed in battle
    if (this->iTurn == 1){
        for (int iPlayer = 0; iPlayer <= 1; iPlayer++)
            for (int i = 1; i <= this->TankOfPlayer[iPlayer][1].n; i++){
                if (this->pos[iPlayer][1].tArray[i].x == -1){
                    this->RemoveTank(iPlayer, i);
                    event[nevent] = __CreateEventDisappear(iPlayer, i, {0, 0});
                    nevent++;
                }
            }
    }
    // Reading fight.out of each player 
    // Recording the move of 2 players
    // If it's moving step (opposite to shooting step), set up new position of tank 
    this->ConfigureMove(0);
    this->ConfigureMove(1);

    // SETUP BATTLE
    // 1. CONTRUCT CHESSBOARD after moving
    this->ContructChessBoard();

    this->SolvingEventOnChessBoard(event, nevent);

    // WRITING TO FIGHT.inp for next turn
    this->WriteInputForPlayer(0);
    this->WriteInputForPlayer(1);

    // Writing result
    this->WriteResult(pres, event, nevent);

    // INCREASING TURN
    this->iTurn += 1;
}

void Fight::end(void * res){
    FILE * hFile = fopen(this->log, "a");
    fprintf(hFile, "%d\n", this->iTurn - 1);

    for (int i = 1;  i < this->iTurn; i++){
        for (int iPlayer = 0; iPlayer < 2; iPlayer ++){
            for (int j = 1; j <= this->TankOfPlayer[iPlayer][i].n; j++){
                fprintf(hFile, "%d %d %d\n",
                    this->TankOfPlayer[iPlayer][i].tArray[j].armor,
                    this->TankOfPlayer[iPlayer][i].tArray[j].range,
                    this->TankOfPlayer[iPlayer][i].tArray[j].strength);
            }
            fprintf(hFile, "%d %d %d %d\n",
                this->move[iPlayer][i].type,
                this->move[iPlayer][i].id,
                this->move[iPlayer][i].pos.x,
                this->move[iPlayer][i].pos.y);
        }

        for (int x = 1; x <= 8; x++){
            for (int y = 1; y <= 8; y++)
                fprintf(hFile, "%d ", this->battle[i][x][y]);
            fprintf(hFile, "\n");
        }
    }
    fclose(hFile);
}

bool __OutOfChessBoard(__POS pos){
    if (pos.x <= 0 || pos.x >8 || pos.y <= 0 || pos.y >8)
        return true;
    return false;
}
bool __OutOfChessBoard(int x, int y){
    if (x <= 0 || x >8 || y <= 0 || y >8)
        return true;
    return false;
}


__EVENT_ON_CHESSBOARD __CreateEventMoving(int iPlayer, int idTank,__POS curpos, __POS newpos){
    __EVENT_ON_CHESSBOARD event;
    event.type = __EVENT_ON_CHESSBOARD::MOVING;
    event.idTank = iPlayer == 0 ? idTank : -idTank;
    event.pos1 = curpos;
    event.pos2 = newpos;

    return event;
}
__EVENT_ON_CHESSBOARD __CreateEventShooting(int iPlayer, int idTank, __POS curpos, __POS shootingpos){
    __EVENT_ON_CHESSBOARD event;
    event.type = __EVENT_ON_CHESSBOARD::SHOOTING;
    event.idTank = iPlayer == 0 ? idTank : -idTank;
    event.pos1 = curpos;
    event.pos2 = shootingpos;

    return event;
}
__EVENT_ON_CHESSBOARD __CreateEventExploisive(__POS pos){
    __EVENT_ON_CHESSBOARD event;
    event.type = __EVENT_ON_CHESSBOARD::EXPLOISIVE;
    event.idTank = 0;
    event.pos1 = pos;
    event.pos2 = {-1, -1};

    return event;
}
__EVENT_ON_CHESSBOARD __CreateEventDisappear(int iPlayer, int idTank, __POS pos){
    __EVENT_ON_CHESSBOARD event;
    event.type = __EVENT_ON_CHESSBOARD::DISAPPEAR;
    event.idTank = iPlayer == 0 ? idTank : -idTank;
    event.pos1 = pos;
    event.pos2 = {-1, -1};

    return event;
}