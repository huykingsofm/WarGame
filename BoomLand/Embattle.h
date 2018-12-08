#include "Game.h"

typedef struct __RES_START_EMBATTLE{
    bool bSuccess;
    ARRAY<TANK> tank1;
    ARRAY<TANK> tank2;
} RES_START_EMBATTLE, *PRES_START_EMBATTLE;

typedef struct __RES_EMBATTLE{
    int exitCode;

    int battle[9][9];
} RES_EMBATTLE, *PRES_EMBATTLE;


class Embattle : public Game{
public:
    Embattle();
    Embattle(const Game&);

    ~Embattle();

public:
    void start(void *) override;
    void play(void *)  override;
    void end(void *)   override;

private:
    bool ReadData(PRES_START_EMBATTLE);
    void WriteInputForPlayer(int);
    void ReadOutputFromPlayer(int);

private:
    ARRAY<TANK> TankOfPlayer[2];  // Tanks which player buy successfully
    
    int battle[9][9];          // battle of game
};