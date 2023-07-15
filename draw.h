#pragma once

#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include "display.h"
#include "position.h"

void draw_gradient(Display* display);
void draw_text(ALLEGRO_FONT* font, Position* pos, ALLEGRO_COLOR color,
    char* text, char* format, int flags);