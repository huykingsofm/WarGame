#include "Game.h"

const int TIMEOUT_FOR_AUCTION = 1000; // milisecond


typedef struct __RES_START_AUCTION{
    bool bSuccess;
    int MoneyOfPlayer1;
    int MoneyOfPlayer2;
} RES_START_AUCTION, *PRES_START_AUCTION;

typedef struct __RES_AUCTION{
    const static int GO_NEXT_TANK = 0;
    const static int OUT_OF_STOCK = 1;
    const static int CLOSED = 2;

    int exitCode;
    
    TANK tank;

    int MoneyOfPlayer1;
    int MoneyOfPlayer2;

    int MoneyForTankOfPlayer1;
    int MoneyForTankOfPlayer2;

    int PlayerSuccess;
    int iTurn;

} RES_AUCTION, *PRES_AUCTION;


class Auction : public Game{
public:
    Auction();
    Auction(const Game&);

    ~Auction();

public:
    void start(void *) override;
    void play(void *)  override;
    void end(void *)   override;

private:
    bool ReadData(PRES_START_AUCTION);

    void WriteInputForPlayer(int);
    void ReadOutputFromPlayer(int, int&, int&);

    void WriteLog();
    void WriteNextStageData();
private:
    ARRAY<TANK> AuctioningTank;  //All of tank which is auctioning
                                 // Id of tank is indexing at 1.

    ARRAY<TANK> TankOfPlayer[2];   // Tanks which player buy successfully
    ARRAY<int> MoneyOfPlayer[2];   // Money of player at each turn

    //MoneyOfPlayerX[0] = Money of playerX is received at first
    //MoneyOfPlayerX[i] = Money of PlayerX after ith auction turn

    ARRAY<int> MoneyForTankOfPlayer[2];   // Money for a tank at each turn of player

    int iTurn;                    // current turn, turn is indexing at 1
};