//
// Created by Li Zuoming on 12/4/15.
//

#include <stdio.h>
#include <stdlib.h>
#include "hand.h"

void hand_compute(Hand *hd)
{
    hand_class(hd);
    hand_value(hd);
}

int hand_compare(const void * a, const void * b)
{
    const Card * cd1 = (const Card *)a;
    const Card * cd2 = (const Card *)b;
    return (cd1->value - cd2->value);
}

void hand_class(Hand* hd)
{
    int i, s1, s2, s3, s4, s5, v1, v2, v3, v4, v5;

    qsort(hd->card, 5, sizeof(*(hd->card)), hand_compare);

    s1 = hd->card[0].suit; v1 = hd->card[0].value;
    s2 = hd->card[1].suit; v2 = hd->card[1].value;
    s3 = hd->card[2].suit; v3 = hd->card[2].value;
    s4 = hd->card[3].suit; v4 = hd->card[3].value;
    s5 = hd->card[4].suit; v5 = hd->card[4].value;


    if (s1 == s2 && s2 == s3 && s3 == s4 && s4 == s5 &&
        v5 == v4 + 1 && v4 == v3 + 1 && v3 == v2 + 1 && v2 == v1 + 1){

        hd->class = 8;
        hd->rank = v5;
        return;
    }

    else if ((v1 == v2 && v2 == v3 && v3 == v4) ||
        (v2 == v3 && v3 == v4 && v4 == v5)){

        hd->class = 7;
        hd->rank = v3;
        return;
    }

    else if ((v1 == v2 && v2 == v3 && v4 == v5) ||
        (v1 == v2 && v3 == v4 && v4 == v5)) {

        hd->class = 6;
        hd->rank = v5;
        return;
    }

    else if (s1 == s2 && s2 == s3 && s3 == s4 && s4 == s5){

        hd->class = 5;
        hd->rank = v5;
        return;
    }

    else if (v5 == v4 + 1 && v4 + 1 == v3 + 2 && v3 + 2 == v2 + 3 && v2 + 3 == v1 + 4){

        hd->class = 4;
        hd->rank = v5;
        return;
    }

    for (i = 0; i < 3; ++i){
        if ((hd->card[i].value == hd->card[i+1].value) && (hd->card[i+1].value == hd->card[i+2].value)){
            hd->class = 3;
            hd->rank = v3;
            return;
        }
    }

    if ((v1 == v2 && v3 == v4) ||
        (v2 == v3 && v4 == v5) ||
        (v1 == v2 && v4 == v5)) {

        hd->class = 2;
        hd->rank = v4;
        return;
    }

    for (i = 0; i < 4; i++) {
        int j, value;
        if (hd->card[i].value == hd->card[i+1].value){
            value = hd->card[i].value;

            hd->class = 1;
            hd->rank = value;
            return;
        }
    }

    hd->class = 0;
    hd->rank = v5;
    return;
}
void hand_value(Hand* hd)
{
    hd->value = 13*hd->class+hd->rank;
}

void hand_print(Hand* hd)
{
    int i;
    printf(" { ");
    for(i=0; i<5; i++)
    {
        card_print(&hd->card[i]);
        if(i!=4)
            printf(", ");
    }
    printf(" } ");
    char* class_name[9]={"High Card","One Pair","Two Pair","Three of a Kind","Straight","Flush","Full House","Four of a Kind","Straight Flush"};
    printf("%s %d\n", class_name[hd->class], hd->value);
}