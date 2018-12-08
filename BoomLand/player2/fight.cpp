#include <stdio.h>
#include <random>
#include <time.h>
int main(){
    FILE * f = fopen("IO/fight.inp", "r");
    srand(time(NULL));
    srand(rand() + rand() % 2 + time(NULL));
 
    int iTurn;
    int myTeam;
    int nTank[2];

    int tank[2][10][3];
    
    int battle[9][9];

    fscanf(f, "%d", &iTurn);

    fscanf(f, "%d", &myTeam);
    fscanf(f, "%d %d", &nTank[0], &nTank[1]);
    for (int i = 1; i <= nTank[0]; i++)
        fscanf(f, "%d %d %d ", &tank[0][i][0], &tank[0][i][1], &tank[0][i][2]);
    for (int i = 1; i <= nTank[1]; i++)
        fscanf(f, "%d %d %d", &tank[1][i][0], &tank[1][i][1], &tank[1][i][2]);
    int signal = myTeam == 1 ? 1 : -1;
    int EnemyTeam = myTeam == 1 ? 1 : 0;
    myTeam -= 1;
    int MyPosTank[10][2], EnemyPosTank[10][2];
    for (int i = 0; i < 10; i++)
    {
        MyPosTank[i][0] = -1;
        MyPosTank[i][1] = -1;
        EnemyPosTank[i][0] = -1;
        EnemyPosTank[i][1] = -1;
    }
    for (int i = 1; i <= 8; i++)
        for (int j = 1; j <= 8; j++){
            fscanf(f, "%d", &battle[i][j]);
            if (signal * battle[i][j] > 0){
                MyPosTank[battle[i][j] * signal][0] = i;
                MyPosTank[battle[i][j] * signal][1] = j;
            }
            else if (signal * battle[i][j] < 0){
                EnemyPosTank[-battle[i][j] * signal][0] = i;
                EnemyPosTank[-battle[i][j] * signal][1] = j;
            }
        }
    fclose(f);
    f = fopen("IO/fight.out", "w");
    fprintf(f, "%d\n", iTurn);
    int r = rand();
    printf("%d", r);
    int typemove = (r * rand()) % 2+ 1;
    fprintf(f, "%d ", typemove);

    int moving[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    r = rand();
    if (typemove == 1){
        int pos = (rand() + (rand() % 2) * 13) % 4;
        int itank;
        do{
            itank = (rand() + (rand() % 2) * 13) % nTank[myTeam] + 1;
            if (MyPosTank[itank][0] != -1)
                break;
        }while(1);
        fprintf(f, "%d ", itank);

        fprintf(f, "%d %d", MyPosTank[itank][0] + moving[pos][0], MyPosTank[itank][1] + moving[pos][1]);
    }
    else{
        int itank;
        do{
            itank = rand() % nTank[myTeam] + 1;
            if (MyPosTank[itank][0] != -1)
                break;
        }while(1);

        int x = rand() % 8 + 1;
        int y = rand() % 8 + 1;

        fprintf(f, "%d ", itank);
        fprintf(f, "%d %d", x, y);
    }
    fclose(f);

    return 0;    
}