#pragma once

#include <allegro5/allegro_native_dialog.h>

typedef struct {
	int x;
	int y;
} Position;

Position* create_position(int x, int y);
void destroy_position(Position* position);