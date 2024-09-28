#include "game.h"
#include "snake_utils.h"
#include "input.h"

#include <stdio.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define _XOPEN_SOURCE 500

void init_snake(Game_data* game)
{
    size_t snake_x = game->x / 4 - 1;
    size_t half_y = game->y / 2;
    size_t apple_x = 3 * snake_x + 1;
    game->app_x = apple_x;
    game->app_y = half_y;

    game->snake = malloc(sizeof(Snake));
    game->snake->size = 1;
    game->snake->direction = RIGHT;
    
    Snake_node* head = malloc(sizeof(Snake_node));
    head->next = NULL;
    head->prev = NULL;
    head->x = snake_x;
    head->y = half_y;

    game->snake->head = head;
    game->snake->tail = head;
}

void build_area(Game_data* game, size_t height, size_t width)
{
    // Want a map of a size minimal ( to be at least playble )
    if (height < 9)
        height = 9;

    if (width < 9)
        width = 9;
    game->finished = 0;
    game->x = width;
    game->y = height;
    game->map = calloc(sizeof(char *),game->y);
    init_snake(game);

    for (size_t i = 0; i < game->y; i++)
    {
        game->map[i] = calloc(sizeof(char),game->x);
        for (size_t j = 0; j < game->x; j++)
        {
            game->map[i][j] = '.';
        }
    }
}

void display_frame(Game_data* game)
{
    Snake_node* iter = game->snake->tail;
    while(iter != game->snake->head)
    {
        game->map[iter->y][iter->x] = '#';
        iter = iter->prev;
    }
    game->map[iter->y][iter->x] = '@';
    game->map[game->app_y][game->app_x] = 'o';
    for (size_t i = 0; i < game->y + 2;i++)
    {
        for (size_t j = 0; j < 2 * game->x + 2; j++)
        {
            if (i == 0 || i == game->y + 1)
            {
                printf("_");
            }
            else if (j == 0 || j == 2 * game->x + 1)
            {
                printf("|");
            }
            else if (((j + 1) % 2 == 0) || (i == game->y + 1 && (j == 0 || j == 2 * game->x + 1)))
            {
                printf(" ");
            }
            else
            {
                if (j == 1)
                    printf("%c",game->map[i][0]);
                else
                    printf("%c",game->map[i - 1][(j - 2) / 2]);
            }
        }
        printf("\n");    
    }
}

void free_game(Game_data* game)
{
    for (size_t i = 0; i < game->y; i++)
    {
        free(game->map[i]);
    }
    free(game->map);

    Snake_node* head = game->snake->head;
    Snake_node* temp = head->next;
    while (temp != NULL)
    {
        free(head);
        head = temp;
        temp = head->next;
    }
    free(head);
    free(game->snake);
}


int game_start(size_t heigth, size_t width)
{
    srand(time(NULL));
    Game_data game;

    build_area(&game,heigth,width);
    snake_growth(&game);
    move_snake(&game);
    display_frame(&game);
    printf("\n\n");

    sleep(4); 
    int move_status;
    while (!game.finished)
    {

        usleep(350000);
        gather_input(&game);
        move_status = move_snake(&game);
        if (move_status == 0)
        {
            game.finished = 1;
            printf("You lost. Score : %ld.\n",game.snake->size - 2);
        }
        else if (move_status == 2)
        {
            game.finished = 1;
            printf("You win, congratulations.\n");
        }
        else
        {
            display_frame(&game);
        }
    }
    free_game(&game);
    // Restore the terminal settings
    disable_raw_mode();

    return 0;
}