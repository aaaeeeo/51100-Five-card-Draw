//
// Created by Li Zuoming on 12/4/15.
//

#ifndef HAND_H
#define HAND_H


#include "deck.h"

typedef struct {
    Card card[5];
    int value;
    int class;
    int rank;
} Hand;

void hand_compute(Hand *hd);
void hand_class(Hand* hd);
void hand_value(Hand* hd);
void hand_print(Hand* hd);
int hand_compare(const void * a, const void * b);

#endif //HAND_H
