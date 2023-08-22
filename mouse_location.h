#pragma once

#include "position.h"
#include "display.h"
#include <allegro5/allegro_font.h>

int is_mouse_over_credits(int mouseX, int mouseY, Position* posCredits, ALLEGRO_FONT* fontCredits);
int is_mouse_over_button(Display* display, int mouseX, int mouseY, const int buttonRadius);
int is_over_return_button(int mouseX, int mouseY, int buttonCenterX, int buttonCenterY, int new_img_width, int new_img_height);
int is_over_next_button(int mouseX, int mouseY, int buttonCenterX, int buttonCenterY, int new_img_width, int new_img_height);
int is_over_init_button(int mouseX, int mouseY);
int is_over_end_button(int mouseX, int mouseY);