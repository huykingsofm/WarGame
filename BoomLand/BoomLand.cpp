#include "Auction.h"
#include "Embattle.h"
#include "Fight.h"
#include <stdio.h>



int main(int argc, char *argv[]){
    if (argc == 6){
        Auction auction;
        auction.setPlayerFolder(argv[1], argv[2]);
        auction.setGameFolder(argv[3]);
        auction.setLogFile(argv[4]);

        __FILE file = argv[5];
        
        // AUCTION STAGE
        // Wait for starting
        scanf("%c");
        RES_START_AUCTION res_start_auction;
        auction.start(&res_start_auction);
        if (!res_start_auction.bSuccess){
            // if failure in start stage, exit program
            printf("File not found!!");
            return 1;
        }
        else{
            FILE * f = fopen(file, "w");
            if (f == nullptr)
                return 0;
            fprintf(f, "%d\n", res_start_auction.bSuccess);
            fprintf(f, "%d\n", 0);
            fprintf(f, "%d %d %d %d\n", 0, 0, 0, 0);
            fprintf(f, "%d %d\n", 0, 0);
            fprintf(f, "%d %d\n", res_start_auction.MoneyOfPlayer1, res_start_auction.MoneyOfPlayer2);
            fprintf(f, "%d\n", 0);    
            fclose(f);   
            // END OF STARTING OF AUCTION STAGE

            RES_AUCTION res;
            res.exitCode = 0;
            char n;
            while (res.exitCode == RES_AUCTION::GO_NEXT_TANK){
                scanf("%c\n", &n);
                FILE * f = fopen(file, "w");
                if (f == nullptr)
                    return 0;

                auction.play(&res);
                fprintf(f, "%d\n", res.exitCode);
                fprintf(f, "%d\n", res.iTurn);
                fprintf(f, "%d %d %d %d\n", res.tank.price, res.tank.armor, res.tank.range, res.tank.strength);
                fprintf(f, "%d %d\n", res.MoneyForTankOfPlayer1, res.MoneyForTankOfPlayer2);
                fprintf(f, "%d %d\n", res.MoneyOfPlayer1, res.MoneyOfPlayer2);
                fprintf(f, "%d\n", res.PlayerSuccess);    
                fclose(f);   
            }
            auction.end(NULL);    
        }//--> END OF AUCTION STAGE

        // EMBATTLE STAGE
        int n;
        scanf("%c\n", &n);
        Embattle embattle(auction);

        // STARTING THIS STAGE
        RES_START_EMBATTLE res_start_embattle;
        embattle.start(&res_start_embattle);

        if (!res_start_embattle.bSuccess){
            printf("file not found!");
            return 1;
        }
        else{
            FILE * f = fopen(file, "w");
            if (f == nullptr)
                return 0;
            fprintf(f, "%d\n", res_start_embattle.bSuccess);
            fprintf(f, "%d %d\n", res_start_embattle.tank1.n, res_start_embattle.tank2.n);
            for (int i = 1; i <= res_start_embattle.tank1.n; i++){
                fprintf(f, "%d %d %d\n",
                    res_start_embattle.tank1.tArray[i].armor,
                    res_start_embattle.tank1.tArray[i].range,
                    res_start_embattle.tank1.tArray[i].strength);
            }
            for (int i = 1; i <= res_start_embattle.tank2.n; i++){
                fprintf(f, "%d %d %d\n",
                    res_start_embattle.tank2.tArray[i].armor,
                    res_start_embattle.tank2.tArray[i].range,
                    res_start_embattle.tank2.tArray[i].strength);
            }
            fclose(f);
            //--> END OF START EMBATTLE STAGE
            
            
            scanf("%c\n", &n);
            RES_EMBATTLE res;
            embattle.play(&res);

            f = fopen(file, "w");
            if (f == nullptr)
                return 0;

            for (int i = 1; i <= 8; i++){
                for (int j = 1; j <= 8; j++)
                    fprintf(f, "%d ", res.battle[i][j]);
                fprintf(f, "\n");
            }
            fclose(f);
            embattle.end(NULL);
        }//--> END OF EMBATTLE STAGE
      char x;
        // FIGHT STAGE
        Fight fight(embattle);
        printf("start:");
        scanf("%c\n", &x);
        fight.start(nullptr);
        //--> END OF FIGHT STAGE
        RES_FIGHT res;
        do{
            scanf("%c\n", &x);
            fight.play(&res);
            FILE * f = fopen(file, "w");

            fprintf(f, "%d\n", res.exitCode);
            fprintf(f, "%d\n", res.iTurn);
            fprintf(f, "%d %d\n", res.tank[0].n, res.tank[1].n);
            for (int i = 1; i <= res.tank[0].n; i++)
                fprintf(f, "%d %d %d\n", 
                    res.tank[0].tArray[i].armor,
                    res.tank[0].tArray[i].range,
                    res.tank[0].tArray[i].strength);
            
            for (int i = 1; i <= res.tank[1].n; i++)
                fprintf(f, "%d %d %d\n", 
                    res.tank[1].tArray[i].armor,
                    res.tank[1].tArray[i].range,
                    res.tank[1].tArray[i].strength);
            for (int i = 1; i <= 8; i++){
                for (int j = 1; j <= 8; j++)
                    fprintf(f, "%d ", res.battle[i][j]);
                fprintf(f, "\n");
            }

            fprintf(f, "%d\n", res.nevent);
            for (int i = 0; i < res.nevent; i++){
                fprintf(f, "%d %d %d %d %d %d\n",
                    res.event[i].type,
                    res.event[i].idTank,
                    res.event[i].pos1.x,
                    res.event[i].pos1.y,
                    res.event[i].pos2.x,
                    res.event[i].pos2.y);
            }

            fclose(f);
        }while(res.exitCode == RES_FIGHT::GO_NEXT_TURN);
        fight.end(nullptr);
    }
    else
        printf("Bad parameters...\n");
    return 0;
}