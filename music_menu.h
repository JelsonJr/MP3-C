#pragma once

#include "display.h"
#include "position.h"
#include "draw.h"
#include "fonts.h"
#include "thread.h"
#include "music.h"
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>

int music_menu(Display* display, int num_musics, char** musics, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_TIMER* timer);