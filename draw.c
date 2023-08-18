#include "draw.h"

void draw_gradient(Display* display) {
    int startRed = 138;
    int startGreen = 43;
    int startBlue = 226;

    int numIterations = display->SCREEN_HEIGHT;

    for (int y = 0; y < numIterations; y++) {
        int red = startRed - (y / 6);
        int green = startGreen;
        int blue = startBlue + (y / 54);

        ALLEGRO_COLOR color = al_map_rgb(red, green, blue);

        al_draw_filled_rectangle(0, y, display->SCREEN_WIDTH, y + 1, color);
    }

    al_flip_display();
}

void draw_text(ALLEGRO_FONT* font, Position* pos, ALLEGRO_COLOR color,
    char* text, char* format, int flags) {
    al_draw_textf(font, color, pos->x, pos->y, flags, format, text);

    al_destroy_font(font);
    destroy_position(pos);
}

void draw_musics_list(int num_musics, char** musics) {
    int text_height = 14;
    int space_between_music = 10;
    ALLEGRO_FONT* font = al_load_font(MONTSERRAT_BOLD, text_height, 0);
 
    for (int i = 0; i < num_musics; i++) {
        const char* music = musics[i];
        const char* music_name = strrchr(music, '\\') + 1;

        Position* pos = create_position(320, i * (text_height + space_between_music));
        al_draw_textf(font, al_map_rgb(250, 250, 250), pos->x, pos->y, ALLEGRO_ALIGN_LEFT, "%s", music_name);

        destroy_position(pos);
    }

    al_flip_display();
}

void draw_music_timer(int seconds, ALLEGRO_AUDIO_STREAM* audioStream, Display* display, ALLEGRO_FONT* font, Position* pos) {
    double durationInSeconds = al_get_audio_stream_length_secs(audioStream);
    double totalSeconds = durationInSeconds;

    int minutes = seconds / 60;
    int remaining_seconds = seconds % 60;

    char time_str[20];
    snprintf(time_str, sizeof(time_str), "%02d:%02d | %02d:%02d", minutes, remaining_seconds, (int)durationInSeconds / 60, (int)durationInSeconds % 60);

    int rect_width = 120;
    int rect_height = 25;

    al_draw_filled_rectangle(pos->x - 10, pos->y, pos->x + rect_width, pos->y + rect_height, al_map_rgb(255, 255, 255));
    al_draw_textf(font, al_map_rgb(0, 0, 0), pos->x, pos->y, ALLEGRO_ALIGN_LEFT, "%s", time_str);

    double progress = (seconds / totalSeconds) * rect_width;
    al_draw_filled_rectangle(pos->x - 10, pos->y + rect_height + 5, pos->x - 10 + progress, pos->y + rect_height + 10, al_map_rgb(0, 0, 255));

    al_flip_display();
}
