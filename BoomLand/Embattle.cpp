#include "Embattle.h"
#include "utilities.h"
#include <stdio.h>

Embattle::Embattle() : Game(){
    for (int i = 1; i <= 8; i++)
        for (int j = 1; j <= 8; j++)
            this->battle[i][j] = 0;
}

Embattle::Embattle(const Game &game) : Game(game){
     for (int i = 1; i <= 8; i++)
        for (int j = 1; j <= 8; j++)
            this->battle[i][j] = 0;
}

Embattle::~Embattle(){
}

bool Embattle::ReadData(PRES_START_EMBATTLE pres){
    __FILE embattlePath = __strcat(this->game, "\\IO\\embattle.data");
    FILE * f = fopen(embattlePath, "r");
    delete[] embattlePath;
   
    if (f == nullptr){
        if (pres != nullptr)
            pres->bSuccess = false;
        return false;
    }
    
    // File embattle.data has following contruction:
    //  + first line is n1, n2: describes number of tanks of player1 and player2
    //  + n1 next lines, each line has 3 attributes is armor, range and strength of tanks of player1
    //  + n2 next lines, each line has 3 attributes is armor, range and strength of tanks of player2
    
    // Read number of tanks of each player
    fscanf(f, "%d %d", &this->TankOfPlayer[0].n, &this->TankOfPlayer[1].n);
    
    // Allocating memory for tankofplayer array
    this->TankOfPlayer[0].create(this->TankOfPlayer[0].n + 1);
    this->TankOfPlayer[1].create(this->TankOfPlayer[1].n + 1);


    // Reading attribute of tanks
    for (int i = 1; i <= this->TankOfPlayer[0].n; i++){
        fscanf(f, "%d %d %d",
            &this->TankOfPlayer[0].tArray[i].armor,
            &this->TankOfPlayer[0].tArray[i].range,
            &this->TankOfPlayer[0].tArray[i].strength);
    }
    
    for (int i = 1; i <= this->TankOfPlayer[1].n; i++){
        fscanf(f, "%d %d %d",
            &this->TankOfPlayer[1].tArray[i].armor,
            &this->TankOfPlayer[1].tArray[i].range,
            &this->TankOfPlayer[1].tArray[i].strength);
    }

    fclose(f);
    //--> END READING
    return true;

}

void Embattle::WriteInputForPlayer(int iPlayer){
    __FILE embattlePath = __strcat(this->player[iPlayer], "\\IO\\embattle.inp");
    FILE * f = fopen(embattlePath, "w");
    delete[] embattlePath;

    fprintf(f, "%d\n", iPlayer + 1);
    fprintf(f, "%d %d\n", this->TankOfPlayer[0].n, this->TankOfPlayer[1].n);
    
    for (int i = 1; i<=this->TankOfPlayer[0].n; i++){
        fprintf(f, "%d %d %d\n",
            this->TankOfPlayer[0].tArray[i].armor,
            this->TankOfPlayer[0].tArray[i].range,
            this->TankOfPlayer[0].tArray[i].strength);
    }

    for (int i = 1; i<=this->TankOfPlayer[1].n; i++){
        fprintf(f, "%d %d %d\n",
            this->TankOfPlayer[1].tArray[i].armor,
            this->TankOfPlayer[1].tArray[i].range,
            this->TankOfPlayer[1].tArray[i].strength);
    }
    fclose(f);
    // END WRITING FILE PLAYER
}

void Embattle::start(void *res){
    PRES_START_EMBATTLE pres = (PRES_START_EMBATTLE) res;

    if (!this->ReadData(pres))
        return;

    
    // Create 2 files player1/embattle.inp and player2/embattle.inp have following contruction:
    //      + first line is id - id of player (player1's id is 1 , player2's id is 2)
    //      + second line is n1 n2: describes number of tanks of player1 and player2
    //      + n1 next lines, each line has 3 attributes is armor, range and strength of tanks of player1
    //      + n2 next lines, each line has 3 attributes is armor, range and strength of tanks of player2
     
    this->WriteInputForPlayer(0);
    this->WriteInputForPlayer(1);

    if (pres != nullptr){
        pres->bSuccess = true;
        
        pres->tank1.n = this->TankOfPlayer[0].n;
        pres->tank1.create(pres->tank1.n + 1);
        for (int i = 1; i<=pres->tank1.n; i++){
            pres->tank1.tArray[i] = this->TankOfPlayer[0].tArray[i];
        }

        pres->tank2.n = this->TankOfPlayer[1].n;
        pres->tank2.create(pres->tank2.n + 1);
        for (int i = 1; i<=pres->tank2.n; i++){
            pres->tank2.tArray[i] = this->TankOfPlayer[1].tArray[i];
        }
    }
}

void Embattle::ReadOutputFromPlayer(int iPlayer){
    __FILE embattlePath = __strcat(this->player[iPlayer], "\\IO\\embattle.out");

    FILE * f = fopen(embattlePath, "r");
    delete[] embattlePath;

    
    int InputBattle[9][9];
    for (int i = 1; i <= 8; i++)
        for (int j = 1; j <= 8; j++)
            InputBattle[i][j] = 0;

    bool IsCorrect = false;
    if (f != nullptr){
        IsCorrect = true;
        int nCell = 0;
        while (!feof(f)){
            int temp;
            fscanf(f, "%d ", &temp);
            if (nCell <= 63){
                InputBattle[nCell / 8 + 1][nCell % 8 + 1] = temp;
            }
            nCell++;
        }

        fclose(f);
        if (nCell != 64){
            IsCorrect = false;
        }
        else{
            bool *isOnBattle = new bool[this->TankOfPlayer[iPlayer].n + 1];
            for (int i = 1; i<=this->TankOfPlayer[iPlayer].n; i++)
                isOnBattle[i] = false;

            for (int i = 1; i <= 8; i++)
                for (int j = 1; j <= 8; j++){
                    if ((iPlayer == 0 && i < 6 && InputBattle[i][j] != 0)||
                        (iPlayer == 1 && i > 3 && InputBattle[i][j] != 0))
                    {
                        IsCorrect = false;
                    }

                    if (InputBattle[i][j] > this->TankOfPlayer[iPlayer].n){
                        IsCorrect = false;
                    }

                    if (InputBattle[i][j] != 0 && isOnBattle[InputBattle[i][j]]){
                        IsCorrect = false;
                    }

                    isOnBattle[InputBattle[i][j]] = true;
                }

            delete[] isOnBattle;
        }
    }
    
    if (IsCorrect){
        for (int i = 1; i <= 8; i++)
            for (int j = 1; j <= 8; j++)
                if (InputBattle[i][j] != 0){
                    int Signal = iPlayer == 0? 1 : -1;
                    this->battle[i][j] = Signal * InputBattle[i][j];
                }
    }
    else{
        for (int i = 1; i <= this->TankOfPlayer[iPlayer].n; i++){
            if (iPlayer == 0)
                this->battle[8 - (i-1)/8][i] = i;
            else 
                this->battle[1 + (i-1)/8][i] = -i;
        }
    }
}

void Embattle::play(void * res){
    // Run 2 files player1/embattle.exe and player2/embattle.exe
    PRES_EMBATTLE pres = (PRES_EMBATTLE) res;

    HANDLE hEx[2];
    hEx[0] = CreateMarker(this->player[0], "embattle.exe", 1);
    hEx[1] = CreateMarker(this->player[1], "embattle.exe", 1);

    WaitForSingleObject(hEx[0], 1000);
    WaitForSingleObject(hEx[1], 1000);

    CloseHandle(hEx[0]);
    CloseHandle(hEx[1]);

    // Reading file player/IO/embattle.out
    this->ReadOutputFromPlayer(0);
    this->ReadOutputFromPlayer(1);
    

    if (pres != nullptr){
        pres->exitCode = 0;
        for (int i = 1; i <= 8; i++)
            for (int j = 1; j <= 8; j++)
                pres->battle[i][j] = this->battle[i][j];
    }
    return;
}

void Embattle::end(void *){
    // Writing to fight.data the infomation of this stage, include
    //      list of tanks of each player
    //      position of tans on battle
    // fight.data has following contruction:
    //      + first line is n1, n2 : describes numbers of tanks of both player 
    //      + n1 next lines, each line is 3 elements, include armor, range and strength of player1's tanks
    //      + n2 next lines, each line is 3 elements, include armor, range and strength of player2's tanks
    //      + 8x8 matrix represents for battle, positive elements is player1's tanks, negative elements is
    //         ..player2's tank
    __FILE battleFile = __strcat(this->game, "\\IO\\fight.data");
    FILE * f = fopen(battleFile, "w");
    delete[] battleFile;

    if (f != nullptr){
        fprintf(f, "%d %d\n", this->TankOfPlayer[0].n, this->TankOfPlayer[1].n);
        for (int i = 1; i<=this->TankOfPlayer[0].n; i++){
            fprintf(f, "%d %d %d\n",
                this->TankOfPlayer[0].tArray[i].armor,
                this->TankOfPlayer[0].tArray[i].range,
                this->TankOfPlayer[0].tArray[i].strength);
        }
        for (int i = 1; i<=this->TankOfPlayer[1].n; i++){
            fprintf(f, "%d %d %d\n",
                this->TankOfPlayer[1].tArray[i].armor,
                this->TankOfPlayer[1].tArray[i].range,
                this->TankOfPlayer[1].tArray[i].strength);
        }
        for (int i = 1; i <= 8; i++){
            for (int j = 1; j <= 8; j++)
                fprintf(f, "%d ", this->battle[i][j]);
            fprintf(f, "\n");
        }
        fclose(f);
    }

    // Appending to log file
    // log file includes only battle matrix
    f = fopen(this->log, "a");
    if (f != nullptr){
        for (int i = 1; i <= 8; i++){
            for (int j = 1; j <= 8; j++)
                fprintf(f, "%d ", this->battle[i][j]);
            fprintf(f, "\n");
        }
        fclose(f);
    }
}