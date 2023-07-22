#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include "display.h"

int find_screen_center(Display* display, ALLEGRO_FONT* font, const char* text);
void init_monitor(Display* display);