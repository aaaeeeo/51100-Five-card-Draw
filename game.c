//
// Created by Li Zuoming on 12/4/15.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "game.h"
#include "player.h"
#include "hand.h"


void game_init(Game * gm, int init_money, int show)
{
    gm->isShow=show;
    gm->playerNum =4;
    gm->round=0;
    gm->ante=0;
    gm->pot=0;
    deck_init(&gm->deck);
    players_init(gm->players,init_money);
    gm->msg=malloc(10000);
    strcpy(gm->msg, "");
}
void game_play( int init_money, int show)
{
    int i;
    srand((unsigned int) time(0));
    Game* gm = malloc(sizeof(Game));
    game_init(gm,init_money,show);
    while(gm->playerNum>1)
    {
        game_print(gm,0,gm->isShow);
        game_round(gm);

        for(i=0;i<4;i++)
        {
            Player *ply = &gm->players[i];
            if(ply->isIn==1 && ply->amount<=0)
            {
                ply->isIn=0;
                gm->playerNum--;
            }
        }
    }

    game_print(gm,0,0);
    for(i=0;i<4;i++)
    {
        Player *ply = &gm->players[i];
        if(ply->isIn)
            printf("\033[32mplayer %d WINS finaly!\033[0m\n", ply->num);
    }
    game_pause();
}
void game_round(Game * gm)
{
    int i;
    char* t;
    gm->round++;
    printf("ENTER to start round %d: ", gm->round);
    getchar();
    game_reset(gm);

    gm->pot=0;
    for(i=0;i<4;i++)
    {
        Player* ply= &gm->players[i];
        if(ply->isIn)
        {
            ply->ante=1;
            ply->amount--;
            gm->pot++;
        }
    }
    //gm->pot=4;
    gm->ante=1;
    players_deal(gm->players,&gm->deck);
    game_print(gm,1,gm->isShow);

    strcpy(gm->msg, "");
    game_pause();
    game_msg(gm, "\033[32mBETTING 1\033[0m\n",gm->isShow);
    game_bet(gm,1);

    if(game_check(gm)>1)
    {
        strcpy(gm->msg, "");
        game_pause();
        game_msg(gm, "\033[32mEXCHANGE\033[0m\n", gm->isShow);
        game_exchange(gm);
    }

    if(game_check(gm)>1)
    {
        strcpy(gm->msg, "");
        game_pause();
        game_msg(gm, "\033[32mBETTING 2\033[0m\n", gm->isShow);
        game_bet(gm,2);
    }

    strcpy(gm->msg, "");
    game_pause();
    game_msg(gm, "\033[32mRESULT\033[0m\n",gm->isShow);
    game_determine(gm);

    strcpy(gm->msg, "");
    game_pause();

}
void game_bet(Game * gm ,int round)
{
    int i=0,last=-1,bet;
    char in_line[20];
    char input[10];
    int in_bet,i_vali;
    char ts[100];
    int h_de, l_de, avl;
    int raise=0;

    while(1)
    {
        if(game_check(gm)<=1)
            return;

        Player *ply = &gm->players[i];
        if (!ply->isFold)
        {
            int ante = gm->ante;
            double val = ply->hand.value;

            if (ply->isAI == 0)
            {
                i_vali=0;
                while(i_vali==0)
                {
                    printf("\033[32mYour turn to BET, type call, raise num, or fold:\033[0m\n");
                    fgets(in_line,20,stdin);
                    sscanf(in_line,"%s %d", input,&in_bet);

                    if (strcmp(input,"call")==0)
                    {
                        bet = ante;
                        sprintf(ts,"player %d: call\n", ply->num);
                    }
                    else if (strcmp(input,"raise")==0)
                    {
                        //scanf("%d", &in_bet);
                        bet = in_bet;
                        sprintf(ts,"player %d: raise to %d\n", ply->num, bet);
                    }
                    else if (strcmp(input,"fold")==0)
                    {
                        i_vali=1;
                        ply->isFold = 1;
                        sprintf(ts,"player %d: fold\n", ply->num);
                    }
                    else
                        bet=0;

                    if(bet>=ante)
                        i_vali=1;
                    else
                    {
                        game_print(gm,1,gm->isShow);
                        printf("\033[32mINVAID INPUT!\033[0m\n");
                    }

                    if(ply->amount <= (bet-ply->ante))
                    {
                        game_bet_helper(gm, i, bet);
                        sprintf(ts,"player %d: all-in\n", ply->num);
                    }

                }
                if(strcmp(input,"call")==0)
                    game_bet_helper(gm, i, bet);
                else if(strcmp(input,"raise")==0)
                {
                    last=i;
                    game_bet_helper(gm, i, bet);
                }
                game_msg(gm,ts,gm->isShow);
                if(game_check(gm)<=1)
                    return;

            }
            else
            {
                //val -= 10;
                val=val/80.0 * ply->amount;
                if(ante>ply->amount)
                    avl=ply->amount;
                else
                    avl=ante;
                if(round==1)
                {
                    h_de=10;
                    l_de=20;
                }
                else if(round==2)
                {
                    h_de=7;
                    if(raise)
                        l_de=15;
                    else
                        l_de=INT32_MAX;
                }
                if (val > (avl + h_de)) {
                    //raise
                    bet=game_bet_helper(gm, i, val*0.8);
                    if(bet>ante)
                    {
                        last=i;
                        raise=1;
                        sprintf(ts,"player %d: raise to %d\n", ply->num, bet);
                    }
                    else if(bet==ante)
                        sprintf(ts,"player %d: call\n", ply->num);
                    else
                    {
                        //ply->isFold = 1;
                        sprintf(ts,"player %d: all-in\n", ply->num);
                    }
                    game_msg(gm,ts,gm->isShow);
                }
                else if (val < (avl - l_de)) {
                    //fold
                    ply->isFold = 1;
                    sprintf(ts,"player %d: fold\n", ply->num);
                    game_msg(gm,ts,gm->isShow);
                    if(game_check(gm)<=1)
                        return;
                }
                else {
                    //call
                    bet=game_bet_helper(gm, i, ante);
                    if(bet==ante)
                        sprintf(ts,"player %d: call\n", ply->num);
                    else
                    {
                        //ply->isFold = 1;
                        sprintf(ts,"player %d: all-in\n", ply->num);
                    }
                    game_msg(gm,ts,gm->isShow);
                }
            }
        }
        i=(i+1)%4;
        if(i==last)
            break;
        if(last==-1&&i==0)
            break;
    }
}

int game_check(Game* gm)
{
    int i,count=0;
    for(i=0;i<4;i++)
    {
        Player *ply = &gm->players[i];
        if(ply->isIn==1 && ply->isFold==0)
            count ++;
    }
    return count;
}

int game_bet_helper(Game* gm, int i, int bet)
{
    Player* ply= &gm->players[i];
    int diff=bet-ply->ante;
    if(diff>ply->amount)
    {
        diff=ply->amount;
        bet=ply->ante+diff;
    }
    /*
    if(bet<gm->ante)
    {
        diff=0;
        bet=ply->ante+diff;
    }
     */
    //else
    if(bet>gm->ante)
    {
        gm->ante = bet;
    }
    {
        gm->pot += diff;
        ply->ante = bet;
        ply->amount -= diff;
    }

    return bet;
}
void game_exchange(Game * gm )
{
    int i,j,count=0;
    char c[2];
    int* change;
    char str_change[11];
    char ts[100];
    c[1]='\0';
    char input[10];
    char in_line[20];
    int in_change[5];

    for(i=0;i<4;i++) {
        count = 0;
        Player *ply = &gm->players[i];
        if (ply->isFold == 0) {
            strcpy(str_change, "");
            change = player_exchange(&gm->deck, ply);

            for (j = 0; j < 5; j++) {
                if (change[j] == 1) {
                    count++;
                    c[0] = (char) (j + 49);
                    strcat(str_change, c);
                    strcat(str_change, " ");
                }
            }

            if (ply->isAI == 0) {
                printf("\033[32mYour turn to EXCHANGE:\nRecommand to exchange card { %s}\ntype YES to accept, NO to decline:\033[0m\n",
                       str_change);
                scanf("%s", input);
                getchar();
                if (strcmp(input, "YES") == 0) {
                    player_exchange_input(&gm->deck, ply, change);
                }
                else if (strcmp(input, "NO") == 0) {
                    printf("\033[32mtype card to exchange, ig. 1,2,3 :\033[0m\n");
                    fgets(in_line, 20, stdin);
                    count = sscanf(in_line, "%d,%d,%d,%d,%d", &in_change[0], &in_change[1], &in_change[2],
                                   &in_change[3], &in_change[4]);
                    for (j = 0; j < 5; j++) {
                        change[j] = 0;
                    }
                    for (j = 0; j < count; j++) {
                        change[in_change[j] - 1] = 1;
                    }
                    player_exchange_input(&gm->deck, ply, change);

                    strcpy(str_change, "");
                    for (j = 0; j < 5; j++) {
                        if (change[j] == 1) {
                            count++;
                            c[0] = (char) (j + 49);
                            strcat(str_change, c);
                            strcat(str_change, " ");
                        }
                    }
                }

            }


            if (count <= 0)
                sprintf(ts, "player %d: remain unchange\n", ply->num);
            else {
                sprintf(ts, "player %d: change card { %s}\n", ply->num, str_change);
            }
            game_msg(gm, ts, gm->isShow);

        }
    }
}

void game_determine(Game * gm )
{
    char ts[100];
    int i,max,w_num=0,award;
    for(i=0;i<4;i++)
    {
        Player *ply = &gm->players[i];
        if(ply->isFold==0 && ply->hand.value > max)
            max=ply->hand.value;
    }
    for(i=0;i<4;i++)
    {
        Player *ply = &gm->players[i];
        if(ply->isFold==0 && ply->hand.value == max)
            w_num++;
    }

    award=gm->pot/w_num;
    gm->pot-=award*w_num;

    for(i=0;i<4;i++)
    {
        Player *ply = &gm->players[i];
        if(ply->isFold==0 && ply->hand.value == max)
        {
            ply->wins++;
            ply->amount+=award;
            sprintf(ts,"\033[32mplayer %d WINS, %d awarded\033[0m\n",ply->num,award);
            game_msg(gm,ts,1);
        }
    }
}

void game_pause()
{
    printf("ANY KEY to continue:");
    getchar();
}

void game_msg(Game* gm, char* msg, int show)
{
    strcat(gm->msg,msg);
    game_print(gm,1,show);
}

void game_print(Game* gm, int round,int show)
{
    printf("\e[1;1H\e[2J");
    printf("------------------------------------------------------------\n");
    players_print(gm->players);
    printf("------------------------------------------------------------\n");
    if(round)
    {
        printf("\033[35mROUND %d: ante: %d pot: %d\033[0m\n", gm->round, gm->ante, gm->pot);
        printf("------------------------------------------------------------\n");
        players_printround(gm->players,show);
        printf("------------------------------------------------------------\n");
        printf("%s",gm->msg);
    }
    //sleep(1);
}

void game_reset(Game* gm)
{
    int i;
    deck_init(&gm->deck);
    for(i=0;i<4;i++)
    {
        Player *ply = &gm->players[i];
        if(ply->isIn==1)
            ply->isFold = 0;
        else
            ply->isFold = 1;
        ply->ante=0;
    }

}