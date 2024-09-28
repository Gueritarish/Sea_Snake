#pragma once

#include "game.h"

typedef struct Node {
    size_t x,y;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    size_t size;
} LinkedList;

void prepend(size_t x, size_t y, LinkedList* list);
void free_list(LinkedList* list);

void snake_growth(Game_data* game);
int move_snake(Game_data* game);