#include <stdio.h>
#include <random>
#include <time.h>

int distance(int x, int y, int nx, int ny){
    return (x - nx) * (x - nx) + (y - ny) * (y - ny);
}

int main(){
    FILE * f = fopen("IO/fight.inp", "r");
    srand(time(NULL));
 
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
    int typemove = r % 2 + 1;
    
    int moving[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    r = rand();
    int itank;
    do{
        itank = rand() % nTank[myTeam] + 1;
        if (MyPosTank[itank][0] != -1)
            break;
    }while(1);

    int flag = 0;
    for (int i = 1; i <= nTank[EnemyTeam]; i++){
        if (EnemyPosTank[i][0] != -1){
            int d = distance(MyPosTank[itank][0], MyPosTank[itank][1], 
                            EnemyPosTank[i][0], EnemyPosTank[i][1]);

            if (d < tank[myTeam][itank][1] * tank[myTeam][itank][1]){
                flag = 1;
                fprintf(f, "%d %d %d %d", 2, itank, EnemyPosTank[i][0], EnemyPosTank[i][1]);
                break;
            }
        }
    }

    if (flag == 0){
        int direct;
        
        while(1){
            direct = (rand() + (rand() %2 ) * 4) % 4;
            int newx = MyPosTank[itank][0] + moving[direct][0];
            int newy = MyPosTank[itank][1] + moving[direct][1]; 
            if (newx > 0 && newx < 9 && newy > 0 && newy < 9 &&
                battle[newx][newy] * signal <= 0)
                break; 
        }
        fprintf(f, "%d %d %d %d", 1, itank, MyPosTank[itank][0] + moving[direct][0],
            MyPosTank[itank][1] + moving[direct][1]);
    }

    fclose(f);

    return 0;    
}