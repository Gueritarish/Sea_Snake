#include "snake_utils.h"

#include <stdio.h>

void prepend(size_t x, size_t y, LinkedList* list)
{
    Node* new_node = malloc(sizeof(Node));
    new_node->x = x;
    new_node->y = y;
    new_node->next = list->head;
    list->head = new_node;
    list->size++;
}

void free_list(LinkedList* list)
{
    Node* temp = list->head;
    if (!temp)
        return;
    while (list->size != 0)
    {
        temp = list->head->next;
        free(list->head);
        list->head = temp;
        list->size--;
    }
}

void apple_eaten(Game_data* game)
{
    snake_growth(game);
    game->app_x = game->x;
    game->app_y = game->y;
    LinkedList list;
    list.head->next = NULL;
    list.size = 0;
    for (size_t i = 0; i < game->y; i++)
    {
        for (size_t j = 0; j < game->x; j++)
        {
            if (game->map[i][j] == '.')
                prepend(j,i,&list);
        }
    }
    if (list.size != 0)
    {
        size_t chosen = rand() % list.size;
        Node* temp = list.head;
        for (size_t i = 0; i < chosen; i++)
            temp = temp->next;
        game->app_x = temp->x;
        game->app_y = temp->y;
    }
    free_list(&list);
}


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
    game->map[snake->head->y][snake->head->x] = '.';

    Snake_node* iter = snake->tail;
    if (iter->prev != NULL && (iter->x == iter->prev->x && iter->y == iter->prev->y))
        iter = iter->prev; // Skip the last node if the snake just grew
    else
        game->map[iter->y][iter->x] = '.';
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
            snake->head->y -= 1;
            break;

        case DOWN:
            if (snake->head->y + 1 >= game->y)
                return 0;
            snake->head->y += 1;
            break;

        case LEFT:
            if (snake->head->x == 0)
                return 0;
            snake->head->x -= 1;
            break;
        
        case RIGHT:
            if (snake->head->x + 1 >= game->x)
                return 0;
            snake->head->x += 1;
            break;
        default:
            return 0;
    }
    if (snake->head->y == game->app_y && snake->head->x == game->app_x)
        apple_eaten(game);
    Snake_node* temp = snake->head->next;
    while (temp != NULL)
    {
        if (temp->x == snake->head->x && temp->y == snake->head->y)
            return 1;
        temp = temp->next;
    }
    game->map[snake->head->y][snake->head->x] = '@';
    if (snake->size > 1)
    {
        Snake_node* second = snake->head->next;
        game->map[second->y][second->x] = '#';
    }
    if (game->app_x == game->x && game->app_y == game->y)
        return 2; // 2 Mean Victory
    return 1;
}