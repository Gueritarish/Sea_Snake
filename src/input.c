#include "input.h"

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

void enable_raw_mode()
{
    struct termios tty;

    // Get the terminal attributes
    tcgetattr(STDIN_FILENO, &tty);

    // Disable canonical mode and echo
    tty.c_lflag &= ~(ICANON | ECHO);

    // Set the minimum number of characters for non-canonical read
    tty.c_cc[VMIN] = 1;

    // Set the timeout for non-canonical read
    tty.c_cc[VTIME] = 0;

    // Apply the modified attributes
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);

    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
}

void disable_raw_mode()
{
    struct termios tty;

    // Get the terminal attributes
    tcgetattr(STDIN_FILENO, &tty);

    // Restore canonical mode and echo
    tty.c_lflag |= (ICANON | ECHO);

    // Apply the modified attributes
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

void gather_input(Game_data* game)
{
    char c = EOF;
    char save;
    // Enable raw mode
    enable_raw_mode();
    // Read characters one by one without waiting for newline
    save = getchar();
    while(save != EOF)
    {
        c = save;
        save = getchar();
    }
    if (c == EOF)
        return;
    if (game->snake->direction == LEFT || game->snake->direction == RIGHT)
    {
        if (c == '8' || c == 'w')
        {
            game->snake->direction = UP;
        }
        else if (c == '5' || c == 's')
            game->snake->direction = DOWN;
    }
    else if (game->snake->direction == UP || game->snake->direction == DOWN)
    {
        if (c == '4' || c == 'a')
            game->snake->direction = LEFT;
        else if (c == '6' || c == 'd')
            game->snake->direction = RIGHT;
    }
}
