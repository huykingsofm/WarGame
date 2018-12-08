#include "Game.h"

const int MAX_TURN = 70;

struct __POS{
    int x, y;
};

struct __EVENT_ON_CHESSBOARD{
    const static int EXPLOISIVE = 0;
    const static int DISAPPEAR = 1;
    const static int MOVING = 2;
    const static int SHOOTING = 3;

    int type;
    int idTank;
    __POS pos1, pos2;    
};

struct __MOVE{
    int type;
    int id;
    __POS pos;
};

typedef struct __RES_START_FIGHT{
    bool bSuccess;
    ARRAY<TANK> tank1;
    ARRAY<TANK> tank2;
    int battle[8][8];
} RES_START_FIGHT, *PRES_START_FIGHT;

typedef struct __RES_FIGHT{
    const static int GO_NEXT_TURN = 0;
    const static int LAST_TURN = 1;
    const static int CLOSED = 2; 
    const static int PLAYER1_WIN = 3;
    const static int PLAYER2_WIN = 4;

    int exitCode;
    int iTurn;
    ARRAY<TANK> tank[2];
   
    int battle[9][9];

    int nevent;
    __EVENT_ON_CHESSBOARD event[20];
} RES_FIGHT, *PRES_FIGHT;


class Fight : public Game{
public:
    Fight();
    Fight(const Game&);

    ~Fight();

public:
    void start(void *) override;
    void play(void *)  override;
    void end(void *)   override;

private:
    bool ReadData(PRES_START_FIGHT);

    void InitializationForNewTurn();

    const static int READ_VALID = 1;
    const static int READ_OUT_OF_RANGE = 2;
    const static int READ_INVALID = 0;

    int ReadOutputFromPlayer(int, int[]);
    void WriteInputForPlayer(int);

    void ConfigureMove(int iPlayer);

    void ContructChessBoard();

    void SolvingEventOnChessBoard(__EVENT_ON_CHESSBOARD *, int &);
    bool TankCrossOverEvent(__EVENT_ON_CHESSBOARD *, int&);
    void TankMovingEvent(int, __EVENT_ON_CHESSBOARD *, int&);
    void TankShootingEvent(int, __EVENT_ON_CHESSBOARD *, int&);

    void WriteResult(PRES_FIGHT, __EVENT_ON_CHESSBOARD[], int);

    void RemoveTank(int, int);

private:
    ARRAY<TANK> TankOfPlayer[2][MAX_TURN + 1];  // Tanks of player 1 in each turn
    int nRealTank[2][MAX_TURN + 1];
 
    ARRAY<__POS> pos[2][MAX_TURN + 1];
 
    __MOVE move[2][MAX_TURN + 1];          // move of each player in each turn
 
    int battle[MAX_TURN + 1][9][9];          // battle of game in each turn

    int iTurn;
};

// Utilities function
bool __OutOfChessBoard(__POS);
bool __OutOfChessBoard(int, int);

__EVENT_ON_CHESSBOARD __CreateEventMoving(int, int, __POS, __POS);
__EVENT_ON_CHESSBOARD __CreateEventShooting(int, int, __POS, __POS);
__EVENT_ON_CHESSBOARD __CreateEventExploisive(__POS);
__EVENT_ON_CHESSBOARD __CreateEventDisappear(int, int, __POS);