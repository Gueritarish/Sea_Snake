#include "snake_utils.h"

void snake_growth(Game_data* game)
{
    Snake_node* iter = game->snake->tail;
    iter->next = malloc(sizeof(Snake_node));
    iter->next->x = iter->x;
    iter->next->y = iter->y;
    iter->next->next = NULL;
    iter->next->prev = iter;
    game->snake->tail = iter->next;
    game->snake->size += 1;
}

int move_snake(Game_data* game)
{
    Snake* snake = game->snake;
    Snake_node* iter = snake->tail;
    game->map[iter->y][snake->tail->x] = '.';
    if (iter->prev != NULL && (iter->x == iter->prev->x && iter->y == iter->prev->y))
        iter = iter->prev; // Skip the last node if the snake just grew
    while(iter != snake->head)
    {
        iter->y = iter->prev->y;
        iter->x = iter->prev->x;
        iter = iter->prev;
    }
    switch (snake->direction)
    {
        case UP:
            if (snake->head->y == 0)
                return 0;
            iter->y -= 1;
            break;

        case DOWN:
            if (snake->head->y + 1 >= game->y)
                return 0;
            iter->y += 1;
            break;

        case LEFT:
            if (snake->head->x == 0)
                return 0;
            iter->x -= 1;
            break;
        
        case RIGHT:
            if (snake->head->x + 1 >= game->x)
                return 0;
            iter->x += 1;
            break;
        default:
            return 0;
    }
    return 1;
}