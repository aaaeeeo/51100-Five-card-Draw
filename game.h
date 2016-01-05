//
// Created by Li Zuoming on 12/4/15.
//

#ifndef GAME_H
#define GAME_H

#include "player.h"

typedef struct {
    int round;
    int playerNum;
    Deck deck;
    Player players[4];
    int pot;
    int ante;
    int isShow;
    char* msg;
} Game;

void game_play(int init_money, int show);
void game_round(Game * gm);
void game_init(Game * gm, int init_money, int show);
void game_bet(Game * gm ,int round);
void game_exchange(Game * gm );
void game_determine(Game * gm );
void game_print(Game* gm, int round, int show);
void game_reset(Game* gm);
int game_bet_helper(Game* gm, int i, int bet);
void game_msg(Game* gm, char* msg, int show);
void game_pause();
int game_check(Game* gm);

#endif //GAME_H
