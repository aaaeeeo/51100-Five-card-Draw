//
// Created by Li Zuoming on 12/4/15.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deck.h"

#define SHUFFLE_TIME 100

void card_print(Card* cd) {
    char * suits[4] = { "♡", "◆", "♤", "♧" };
    char * values[13] = { "2", "3", "4", "5", "6", "7", "8", "9",
                          "10", "J", "Q", "K", "A" };
    printf("%s %s",suits[cd->suit], values[cd->value]);
}

void card_swap(Card *cd1, Card *cd2)
{
    Card * temp = malloc(sizeof(Card));
    memcpy(temp, cd1, sizeof(Card));
    memcpy(cd1, cd2, sizeof(Card));
    memcpy(cd2, temp, sizeof(Card));
    free(temp);
}

void deck_init(Deck* dk)
{
    int i, j, n;
    n = 0;
    for (i=0; i<4; i++){
        for (j=0; j<13; j++) {
            dk->cards[n].suit = i;
            dk->cards[n].value = j;
            n++;
        }
    }
    dk->num=52;
    dk->head=0;
    dk->tail=51;
    deck_shuffle(dk);
}
void deck_shuffle(Deck* dk)
{
    int i;
    for(i=0; i<SHUFFLE_TIME; i++)
    {
        card_swap(&dk->cards[rand()%51], &dk->cards[rand()%51]);
    }
}

Card deck_deal(Deck* dk)
{
    Card dl;
    if(dk->num==0)
    {
        dl.value=-1;
        dl.suit=-1;
        return dl;
    }
    dl=dk->cards[dk->head];
    dk->num--;
    dk->head++;
    if(dk->head==52)
        dk->head=0;
    return dl;
}

void deck_back(Deck* dk, Card cd)
{
    if(dk->num==52)
        return;
    dk->num++;
    dk->tail++;
    if(dk->tail==52)
        dk->tail=0;
    dk->cards[dk->tail] = cd;
}

void deck_print(Deck* dk)
{
    int i, index;
    if(dk->num==0)
        return;
    for(i=0; i<dk->num; i++)
    {
        index=(dk->head+i)%52;
        card_print(&dk->cards[index]);
    }
}
