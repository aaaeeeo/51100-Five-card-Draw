
#include <stdlib.h>
#include "game.h"


int main(int argc,char* argv[])
{
    if(argc==2 && atoi(argv[1])==1)
    {
        game_play(100,1);
    }
    game_play(100,0);
}