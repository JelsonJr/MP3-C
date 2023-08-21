#pragma once

#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include "display.h"
#include "position.h"
#include "fonts.h"
#include "util.h"

void draw_gradient(Display* display);
void draw_text(ALLEGRO_FONT* font, Position* pos, ALLEGRO_COLOR color,
    char* text, char* format, int flags);
void draw_music_timer(int seconds, ALLEGRO_AUDIO_STREAM* audioStream, 
    ALLEGRO_FONT* font, Position* pos);
void draw_musics_list(int num_musics, char** musics);