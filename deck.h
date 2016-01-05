//
// Created by Li Zuoming on 12/4/15.
//

#ifndef DECK_H
#define DECK_H


typedef struct {
    int value;
    int suit;
} Card;

typedef struct {
    Card cards[52];
    int num;
    int head;
    int tail;
} Deck;

void card_print(Card* cd);
void card_swap(Card *cd1, Card *cd2);

void deck_init(Deck* dk);
void deck_shuffle(Deck* dk);
Card deck_deal(Deck* dk);
void deck_back(Deck* dk, Card cd);
void deck_print(Deck* dk);

#endif //DECK_H