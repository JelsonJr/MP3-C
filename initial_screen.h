#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include "draw.h"
#include "fonts.h"
#include "position.h"
#include "display.h"
#include "util.h"

#define BUTTON_RADIUS 70
#define SYMBOL_SIZE 30
#define ANIMATION_DURATION 0.8

void draw_initial_screen(Display* display);
void animateButton(Display* display, ALLEGRO_COLOR buttonColor, ALLEGRO_COLOR symbolColor, float symbolSizeScale);