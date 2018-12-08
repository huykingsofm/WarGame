#include <stdio.h>
#include <random>
#include <time.h>
int main(){
    FILE * f = fopen("\\IO\\fight.inp", "r");
    srand(clock());
    if (f == nullptr)
        printf("huy");
    int iTurn;
    int myTeam;
    int nTank[2];

    int tank[2][10][3];
    
    int battle[9][9];
    printf("5");

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
    int typemove = r % 2 + 1;
    fprintf(f, "%d ", typemove);

    int moving[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    if (typemove == 1){
        int pos = r % 4;
        int itank = r % nTank[myTeam] + 1;
        fprintf(f, "%d ", itank);
        fprintf(f, "%d %d", MyPosTank[itank][0] + moving[pos][0], MyPosTank[itank][1] + moving[pos][1]);
    }
    else{
        int itank = r % nTank[myTeam] + 1;
        int destank = r % nTank[EnemyTeam] + 1;
        fprintf(f, "%d ", itank);
        fprintf(f, "%d %d", EnemyPosTank[destank][0], EnemyPosTank[destank][1]);
    }
    fclose(f);

    return 0;    
}