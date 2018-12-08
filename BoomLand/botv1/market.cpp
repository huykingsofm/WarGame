#include <stdio.h>
#include <random>
#include <time.h>
#include <windows.h>

struct TANK{
    int p, a, r, s;
};

int main(int argc, char const *argv[]){
    srand(time(NULL) * 13);
    FILE *fin = fopen("IO/market.inp", "r");

    if (fin == nullptr){
        return 0;
    }
    int n, m, K;
    fscanf(fin, "%d", &n);
    fscanf(fin, "%d", &K);
    fscanf(fin, "%d", &m);
    
    TANK tank[100];

    for (int i = 1; i<=n; i++){
        fscanf(fin, "%d %d %d %d", &tank[i].p, &tank[i].a, &tank[i].r, &tank[i].s);
    }
    fclose(fin);

    int money = 0;
    int x = n - m + 1 > 4 ? 4 : n - m + 1;
    money = tank[m].p + rand() % (K / x);

    FILE * fout = fopen("IO/market.out", "w");
    fprintf(fout, "%d\n%d", m, money);
    fclose(fout);


    return 0;
}
