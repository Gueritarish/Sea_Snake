#pragma once

#include <stdlib.h>

enum Direction{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

typedef struct Snake_node {
    size_t x,y;
    struct Snake_node* next;
    struct Snake_node* prev;
}  Snake_node;

typedef struct{
    Snake_node* head;
    Snake_node* tail;
    size_t size;
    enum Direction direction;
} Snake;

typedef struct Game_data{
    char** map;
    Snake* snake;
    size_t x,y;
    size_t app_x,app_y;
    int finished;
} Game_data;

void init_snake(Game_data* game);
void build_area(Game_data* game, size_t height, size_t width);
void display_frame(Game_data* game);
int game_start(size_t heigth, size_t width);