#include <stdio.h>

struct TANK{
    int p, a, r, s;
};

int main(){
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
    float value[100];
    int v = 0;
    int d = 0;
    float all = 0;
    for (int i = 1; i<=n; i++){
        value[i] = float(tank[i].a + tank[i].r + tank[i].s)/ tank[i].p;
        v += tank[i].a + tank[i].r + tank[i].s;
        d += tank[i].p;
        if (i >= m)
        all += value[i];
    }
    float mean = float(v)/d;

    int minTank = 5 < n - m + 1 ? 5 : n - m + 1;

    int money = 0;
    if (value[m] > mean / 3){
        money = value[m] / all * (n - m + 1) * (K / minTank);
        money = money <= K ? money : K; 
    }

    FILE * fout = fopen("IO/market.out", "w");
    fprintf(fout, "%d\n%d", m, money);
    fclose(fout);
}