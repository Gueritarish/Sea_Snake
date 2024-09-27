#include "game.h"

int main(int argc, char* argv[])
{
    if (argc < 3)
        game_start(0,0);
    else
        game_start(atoi(argv[1]),atoi(argv[2]));
    return 0;
}