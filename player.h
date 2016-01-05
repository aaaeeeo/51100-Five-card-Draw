//
// Created by Li Zuoming on 12/4/15.
//

#ifndef PLAYER_H
#define PLAYER_H

#include "hand.h"

typedef struct {
    Hand hand;
    int num;
    int isAI;
    int isIn;
    int isFold;
    int ante;
    int amount;
    int wins;
} Player;


void players_init(Player* pys, int cash);
void players_deal(Player* pys, Deck* dk);
void players_compute(Player* pys);
void players_print(Player* pys);
void players_printround(Player* pys,int show);

int* player_exchange(Deck* dk,Player* py);
void player_exchange_input(Deck* dk,Player* py,int* in);


#endif //PLAYER_H
