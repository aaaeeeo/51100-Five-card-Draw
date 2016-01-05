//
// Created by Li Zuoming on 12/6/15.
//

#include <stdlib.h>
#include <string.h>
#include "mc.h"

int* MC_expect(Hand* hd)
{
    int i,j,k,l;
    int change[32][5] = {{0,0,0,0,0},
                         {1,0,0,0,0}, {0,1,0,0,0}, {0,0,1,0,0}, {0,0,0,1,0}, {0,0,0,0,1},
                         {1,1,0,0,0}, {1,0,1,0,0}, {1,0,0,1,0}, {1,0,0,0,1}, {0,1,1,0,0}, {0,1,0,1,0}, {0,1,0,0,1}, {0,0,1,1,0}, {0,0,1,0,1}, {0,0,0,1,1},
                         {1,1,1,0,0}, {1,1,0,1,0}, {1,1,0,0,1}, {1,0,0,1,1}, {1,0,1,0,1}, {1,0,1,1,0}, {0,1,1,1,0}, {0,1,0,1,1}, {0,1,1,0,1}, {0,0,1,1,1},
                         {1,1,1,1,0}, {1,1,1,0,1}, {1,1,0,1,1}, {1,0,1,1,1}, {0,1,1,1,1},
                         {1,1,1,1,1} };
    int score[32];
    //Deck dk;
    Hand nhd;
    Card cd;
    int isV=0;
    int max=-1;
    int maxi=0;
    int* re;


    for(i=0;i<32;i++)
    {
        score[i]=0;
    }

    for(i=1;i<32;i++)
    {
        for(j=0;j<SAMPLES;j++)
        {
            for (l = 0; l < 5; l++)
            {
                nhd.card[l].value=-1;
                nhd.card[l].suit=-1;
            }
            //deck_init(&dk);
            for(k=0;k<5;k++)
            {
                if(change[i][k]==0)
                {
                    nhd.card[k]=hd->card[k];
                }
                else
                {
                    while(isV==0)
                    {
                        cd.suit=rand()%4;
                        cd.value=rand()%13;
                        for (l = 0; l < 5; l++)
                        {
                            if ((cd.value == hd->card[l].value && cd.suit == hd->card[l].suit) ||
                                (cd.value == nhd.card[l].value && cd.suit == nhd.card[l].suit)  )
                            {
                                isV = 0;
                                break;
                            }
                        }
                        isV = 1;
                    }
                    nhd.card[k]=cd;
                    isV=0;
                }
            }
            hand_compute(&nhd);
            score[i]+=nhd.value;
        }
    }

    for(i=0;i<32;i++)
    {
        if(score[i]>max)
        {
            max=score[i];
            maxi=i;
        }
    }

    re=malloc(sizeof(int)*5);
    memcpy(re, &change[maxi], sizeof(int) * 5);
    return re;
}
