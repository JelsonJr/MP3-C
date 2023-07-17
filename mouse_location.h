#pragma once

#include "position.h"
#include "display.h"
#include <allegro5/allegro_font.h>

int is_mouse_over_credits(int mouseX, int mouseY, Position* posCredits, ALLEGRO_FONT* fontCredits);
int is_mouse_over_button(Display* display, int mouseX, int mouseY, const int buttonRadius);