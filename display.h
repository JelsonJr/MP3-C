#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <stdio.h>

typedef struct {
    ALLEGRO_DISPLAY* screen;
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
} Display;

Display* create_display(int width, int height);
void destroy_display(Display* display);