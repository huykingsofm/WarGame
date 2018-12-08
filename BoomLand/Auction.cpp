#include "Auction.h"
#include <stdio.h>
#include <windows.h>
#include "utilities.h"

Auction::Auction() : Game(){
    this->iTurn = 0;
}

Auction::Auction(const Game &game) : Game(game){
    this->iTurn = 0;
}

Auction::~Auction(){
}

bool Auction::ReadData(PRES_START_AUCTION pres){
    __FILE marketPath = __strcat(this->game, "\\IO\\market.data");
    FILE * fmarket = fopen(marketPath, "r");    
    delete[] marketPath;
    
    if (fmarket == nullptr){
        // if market.data does not exist or can't open
        // Just exit which failed result
       if (pres != nullptr){
            pres->bSuccess = false;
        }
        return false;
    }

    // FILE market.data has following contruction:
    // first line is n  : is number of tanks which is auctioning
    // second line is K : is money which is sent for each player
    // next n lines, line i has 4 element pi, ai, ri and si
    //      where pi : price of ith tank
    //            ai : armor of ith tank
    //            ri : range of ith tank
    //            si : strength of ith tank

    // Read n
    int n;
    fscanf(fmarket, "%d", &n);
    
    // Allocate all arrray
    this->AuctioningTank.n = n;
    this->MoneyOfPlayer[0].n        = this->MoneyOfPlayer[1].n        = n;    
    this->MoneyForTankOfPlayer[0].n = this->MoneyForTankOfPlayer[1].n = n;
    this->TankOfPlayer[0].n         = this->TankOfPlayer[1].n         = 0;

    this->AuctioningTank.create(n + 1);

    this->MoneyOfPlayer[0].create(n + 1);
    this->MoneyOfPlayer[1].create(n + 1);
    
    this->MoneyForTankOfPlayer[0].create(n + 1);
    this->MoneyForTankOfPlayer[1].create(n + 1);

    this->TankOfPlayer[0].create(n);
    this->TankOfPlayer[1].create(n);

    // Read money of each player
    int K;
    fscanf(fmarket, "%d", &K);

    // Set up money of each player
    this->MoneyOfPlayer[0].tArray[0] = K;
    this->MoneyOfPlayer[1].tArray[0] = K;

    // Read attribute of tank
    for (int i = 1; i<=n; i++){
        fscanf(fmarket, "%d %d %d %d", 
            &this->AuctioningTank.tArray[i].price,
            &this->AuctioningTank.tArray[i].armor,
            &this->AuctioningTank.tArray[i].range,
            &this->AuctioningTank.tArray[i].strength);
    }

    // Read market.data completely
    fclose(fmarket);
    return true;
}

void Auction::WriteInputForPlayer(int iPlayer){
    // Create file market.inp of each player, prepare for first auction
    // File market.inp has following contruction:
    //      first line is a integer n  : the number of tanks is auctioning
    //      third line is a interget K : the remain money responding of each player
    //      second line is a integer m : the index of tanks is being auctioned.
    //      next n lines, ith line has 4 integer pi ai ri si
    //      where pi : price of ith tank
    //            ai : armor of ith tank
    //            ri : range of ith tank
    //            si : strength of ith tank

    __FILE marketPath = __strcat(this->player[iPlayer], "\\IO\\market.inp");
    FILE * fmarket = fopen(marketPath, "w");
    delete[] marketPath;

    fprintf(fmarket, "%d\n%d\n%d\n", 
        this->AuctioningTank.n,                                 // n = AuctionTank.n
        this->MoneyOfPlayer[iPlayer].tArray[this->iTurn],       // K = first number of money of player
        this->iTurn + 1);                                       // the tank is being auctioned is first tank

    for (int i = 1; i<=this->AuctioningTank.n; i++){
        fprintf(fmarket, "%d %d %d %d\n",
            this->AuctioningTank.tArray[i].price,       // pi
            this->AuctioningTank.tArray[i].armor,       // ai
            this->AuctioningTank.tArray[i].range,       // ri
            this->AuctioningTank.tArray[i].strength);   // si
    }

    fclose(fmarket);
    // End of writing file market.inp of player
}

void Auction::start(void *res){
    
    // Transform void pointer to bool pointer
    // bSuccess will store result of start() of Auction
    PRES_START_AUCTION pres = (PRES_START_AUCTION) res;

    if (this->player[0] == nullptr || this->player[1] == nullptr 
    || this->game == nullptr || this ->log == nullptr){
        // If one of four paths is not provided, then game have not completely set up
        // Just exit program
        if (pres != nullptr){
            // If bSuccess is not null
            // It means res pointer is passed to received result of this method
            // so sending failed result to it. 
            pres->bSuccess = false;
        }
        return;
    }

    if (!this->ReadData(pres))
        return;

    this->WriteInputForPlayer(0);
    this->WriteInputForPlayer(1);

    if (pres != nullptr){
        // Program is done completely
        // Result is success
        pres->bSuccess = true;
        pres->MoneyOfPlayer1 = this->MoneyOfPlayer[0].tArray[0];
        pres->MoneyOfPlayer2 = this->MoneyOfPlayer[1].tArray[0];
    }   
    
    this->iTurn = 1; 
}

void Auction::ReadOutputFromPlayer(int iPlayer, int &MaxMoneyForTank, int &PlayerTakeTank){
    
    char * filePath = __strcat(this->player[iPlayer], "\\IO\\market.out");
    FILE * f = fopen(filePath, "r");
    delete[] filePath;

    this->MoneyForTankOfPlayer[iPlayer].tArray[this->iTurn] = 0;
    if (f != nullptr){
        // if file market.out exist
        // read all data from file
        int temp[3];
        int ntemp = 0;
        while(!feof(f) && ntemp < 3){
            fscanf(f, "%d ", &temp[ntemp]);
            ntemp++;
        }
        // reading id of tank 
        // and money of player for that tank
        int flag = 0;
        if (ntemp > 2)
            flag = 1;
        
        int id;
        if (flag == 0)
            id = temp[0];
        else
            id = 0;
        
        // id is used for checking that executable file of player run correctly.
        if (id == this->iTurn && temp[1] > 0)
            this->MoneyForTankOfPlayer[iPlayer].tArray[this->iTurn] = temp[1];

        int MoneyOfPlayer = this->MoneyOfPlayer[iPlayer].tArray[this->iTurn -1];

        if (this->MoneyForTankOfPlayer[iPlayer].tArray[this->iTurn] <= MoneyOfPlayer)
            if (this->MoneyForTankOfPlayer[iPlayer].tArray[this->iTurn] == MaxMoneyForTank)
                PlayerTakeTank = 0;
            else if (MaxMoneyForTank < this->MoneyForTankOfPlayer[iPlayer].tArray[this->iTurn])
            {
                MaxMoneyForTank = this->MoneyForTankOfPlayer[iPlayer].tArray[this->iTurn];
                PlayerTakeTank = iPlayer + 1;
            }
    }
    fclose(f);
}

void Auction::play(void * p){
    PRES_AUCTION pres = (PRES_AUCTION)p;
   
    // Check turn
    if (this->iTurn > this->AuctioningTank.n){
        if (pres != nullptr){
            pres->exitCode = __RES_AUCTION::CLOSED;
        }
        return;
    }

    // Creating 2 variables to handle Process of 2 executable files
    HANDLE hEx[2];

    // Launching two executable files of 2 players
    // it is player1/market.exe and player2/market.exe
    // time out is 1 second
    
    hEx[0] = CreateMarker(this->player[0], "market.exe", 1);
    hEx[1] = CreateMarker(this->player[1], "market.exe", 1);

    WaitForSingleObject(hEx[0], 1000);
    WaitForSingleObject(hEx[1], 1000);

    CloseHandle(hEx[0]);
    CloseHandle(hEx[1]);

    // Reading infomation from playerX/IO/market.out
    // Identifying which player will take the tank

    int MaxMoneyForTank = 0;
    int PlayerTakeTank = 0;
    
    // Reading from player/IO/market.out
    this->ReadOutputFromPlayer(0, MaxMoneyForTank, PlayerTakeTank);
    this->ReadOutputFromPlayer(1, MaxMoneyForTank, PlayerTakeTank);

    // Deciding which player take that tank
    if (MaxMoneyForTank < this->AuctioningTank.tArray[this->iTurn].price){
        // if the money that player pays for tank is less than price of tank
        // there is not any player taking this tank. 
        PlayerTakeTank = 0;
    }

    // Calculating infomation for next auction
    // include:
    // TankOfPlayer
    // and
    // MoneyOfPlayer

    this->MoneyOfPlayer[0].tArray[this->iTurn] = this->MoneyOfPlayer[0].tArray[this->iTurn - 1];
    this->MoneyOfPlayer[1].tArray[this->iTurn] = this->MoneyOfPlayer[1].tArray[this->iTurn - 1]; 

    if (PlayerTakeTank == 1){
        // If Player 1 take that tank
        // Sending tank into TankOfPlayer1
        this->TankOfPlayer[0].tArray[this->TankOfPlayer[0].n] 
            = this->AuctioningTank.tArray[this->iTurn];
        this->TankOfPlayer[0].n++;
        
        // Remain money of player 1 for next turn is equal to
        // MoneyOfLastTurn - MoneyPayForTank
        this->MoneyOfPlayer[0].tArray[this->iTurn] 
            = this->MoneyOfPlayer[0].tArray[this->iTurn - 1] - this->MoneyForTankOfPlayer[0].tArray[this->iTurn]; 
    }
    else if (PlayerTakeTank == 2){
        // If Player 2 take that tank
        // Sending tank into TankOfPlayer1
        this->TankOfPlayer[1].tArray[this->TankOfPlayer[1].n] 
            = this->AuctioningTank.tArray[this->iTurn];
        this->TankOfPlayer[1].n++;

        // Remain Money of player 2 for next turn is equal to
        // MoneyOfLastTurn - MoneyPayForTank
        this->MoneyOfPlayer[1].tArray[this->iTurn] 
            = this->MoneyOfPlayer[1].tArray[this->iTurn - 1] - this->MoneyForTankOfPlayer[1].tArray[this->iTurn]; 
    }
       
    // Return data of this turn
    if (pres != nullptr){
        if (this->iTurn == this->AuctioningTank.n){
            pres->exitCode = __RES_AUCTION::OUT_OF_STOCK;
        }
        else{
            pres->exitCode = __RES_AUCTION::GO_NEXT_TANK;
        }

        pres->MoneyForTankOfPlayer1 = this->MoneyForTankOfPlayer[0].tArray[this->iTurn];
        pres->MoneyForTankOfPlayer2 = this->MoneyForTankOfPlayer[1].tArray[this->iTurn];
        pres->PlayerSuccess = PlayerTakeTank;
        pres->MoneyOfPlayer1 = this->MoneyOfPlayer[0].tArray[this->iTurn];
        pres->MoneyOfPlayer2 = this->MoneyOfPlayer[1].tArray[this->iTurn];
        pres->tank = this->AuctioningTank.tArray[this->iTurn];
        pres->iTurn = this->iTurn;
    }
   
    // Generating data of new market.inp for next turn
    
    // Create two file market.inp of each player, prepare for next auction
    // File market.inp has following contruction:
    //      first line is a integer n  : the number of tanks is auctioning
    //      second line is a integer m : the index of tanks is being auctioned.
    //      third line is a interget K : the remain money responding of each player
    //      next n lines, ith line has 4 integer pi ai ri si
    //      where pi : price of ith tank
    //            ai : armor of ith tank
    //            ri : range of ith tank
    //            si : strength of ith tank
  
    this->WriteInputForPlayer(0);
    this->WriteInputForPlayer(1);
    // Increasing turn
    this->iTurn++;
}

void Auction::WriteLog(){
// Logging data to log file
    // Neccessary data for storing includes:
    //      + the number of tanks
    //      + first money of player
    //      + attribute of tank and how much each player pay for it
    //      + remain money of each player
    //      + Each team own which tank

    FILE * f = fopen(this->log, "w");

    // Writing number of tanks and first money
    fprintf(f, "%d\n", this->AuctioningTank.n);             // the number of tanks
    fprintf(f, "%d\n", this->MoneyOfPlayer[0].tArray[0]);     // First money of each team
    

    // Writing attribute of tank and infomation of auction at all turn
    for (int i = 1; i<=this->AuctioningTank.n; i++){
        fprintf(f, "%d %d %d %d %d %d %d %d\n",
            this->AuctioningTank.tArray[i].price,           // least price of tank
            this->AuctioningTank.tArray[i].armor,           // armor of tank
            this->AuctioningTank.tArray[i].range,           // range of tank
            this->AuctioningTank.tArray[i].strength,        // strength of tank
            this->MoneyForTankOfPlayer[0].tArray[i],          // the money which player 1 pay for tank
            this->MoneyForTankOfPlayer[1].tArray[i],          // the money which player 2 pay for tank
            this->MoneyOfPlayer[0].tArray[i],                 // remain money of player 1 after this turn
            this->MoneyOfPlayer[1].tArray[i]);                // remain moeny of player 2 after this turn
    }


    // Writing tanks which each player owns
    fprintf(f, "%d %d\n", 
        this->TankOfPlayer[0].n,           // the number of tanks which player 1 owns
        this->TankOfPlayer[1].n);          // the number of tanks which player 2 owns

    for (int i = 0; i<this->TankOfPlayer[0].n; i++){
        fprintf(f, "%d %d %d\n",
            this->TankOfPlayer[0].tArray[i].armor,        // Armor of tank
            this->TankOfPlayer[0].tArray[i].range,        // Range of tank
            this->TankOfPlayer[0].tArray[i].strength);    // Strength of tank
    }
    for (int i = 0; i<this->TankOfPlayer[1].n; i++){
        fprintf(f, "%d %d %d\n",
            this->TankOfPlayer[1].tArray[i].armor,        // Armor of tank
            this->TankOfPlayer[1].tArray[i].range,        // Range of tank
            this->TankOfPlayer[1].tArray[i].strength);    // Strength of tank
    }

    fclose(f);
    // END of writing log
}

void Auction::WriteNextStageData(){
    // Writing data to embattle.data
    // File selection.data has following construction:
    //      + first line is n1 and n2 : describe numbers of tank of each player
    //      + Next n1 lines, each line has 3 values of player1's tank attribute, ai, ri and si
    //      + Next n2 lines, each line has 3 values of player2's tank attribute, ai, ri and si
    __FILE selectionFile = __strcat(this->game, "\\IO\\embattle.data");

    FILE * hFile = fopen(selectionFile, "w");
    
    fprintf(hFile, "%d %d\n", this->TankOfPlayer[0].n, this->TankOfPlayer[1].n);

    for (int i = 0; i< this->TankOfPlayer[0].n; i++){
        fprintf(hFile, "%d %d %d\n",
            this->TankOfPlayer[0].tArray[i].armor,
            this->TankOfPlayer[0].tArray[i].range,
            this->TankOfPlayer[0].tArray[i].strength);
    }
    
    for (int i = 0; i< this->TankOfPlayer[1].n; i++){
        fprintf(hFile, "%d %d %d\n",
            this->TankOfPlayer[1].tArray[i].armor,
            this->TankOfPlayer[1].tArray[i].range,
            this->TankOfPlayer[1].tArray[i].strength);
    }

    fclose(hFile);
}

void Auction::end(void * p){
    
    this->WriteLog();
    this->WriteNextStageData();
    this->isAuctionCompleted = true;
}