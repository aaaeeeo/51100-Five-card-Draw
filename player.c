//
// Created by Li Zuoming on 12/4/15.
//

#include <stdio.h>
#include "player.h"
#include "mc.h"

void players_init(Player* pys, int cash)
{
    int i;
    for(i=0; i<4; i++)
    {
        pys[i].amount=cash;
        pys[i].ante=0;
        pys[i].num=i+1;
        pys[i].isFold=0;
        pys[i].isAI=1;
        pys[i].isIn=1;
        pys[i].wins=0;
    }
    pys[3].isAI=0;
}

void players_deal(Player* pys, Deck* dk)
{
    int i,j;
    for(i=0;i<5;i++)
        for(j=0;j<4;j++)
        {
            pys[j].hand.card[i]=deck_deal(dk);
        }
    players_compute(pys);

}

void players_print(Player* pys)
{
    int i;
    for(i=0; i<4; i++)
    {
        if(pys[i].isAI==0)
            printf("\033[34mYOU: ");
        else
            printf("AI : ");

        if(pys[i].isIn)
            printf("player %d --- reamining money: %d wins: %d\n", pys[i].num,pys[i].amount,pys[i].wins);
        else
            printf("player %d --- \033[31mOUT\033[0m                 wins: %d\n", pys[i].num,pys[i].wins);
    }
    printf("\033[0m");
}

void players_printround(Player* pys,int show)
{
    int i;
    for(i=0; i<4; i++)
    {
        if(pys[i].isAI==0)
            printf("\033[34mYOU: ");
        else
            printf("AI : ");
        if(pys[i].isIn)
            printf("player %d --- ante: %d %s\n", pys[i].num, pys[i].ante, (pys[i].isFold?"\033[31mFOLDED\033[0m":"") );
        else
            printf("player %d --- \033[31mOUT\033[0m\n", pys[i].num );
        if((show==1 || i==3) && pys[i].isIn)
        {
            printf("hand:");
            hand_print(&pys[i].hand);
        }
        printf("\033[0m");

    }
}

void players_compute(Player* pys)
{
    int i;
    for(i=0; i<4; i++)
    {
        hand_compute(&pys[i].hand);
    }
}

int* player_exchange(Deck* dk,Player* py)
{
    int i;
    int* change = MC_expect(&py->hand);
    if(py->isAI)
    {
        for (i = 0; i < 5; i++)
        {
            if (change[i] == 1)
                py->hand.card[i] = deck_deal(dk);
        }
        hand_compute(&py->hand);
    }
    return change;
}

void player_exchange_input(Deck* dk,Player* py,int* in)
{
    int i;
    for (i = 0; i < 5; i++)
    {
        if (in[i] == 1)
            py->hand.card[i] = deck_deal(dk);
    }
    hand_compute(&py->hand);
}