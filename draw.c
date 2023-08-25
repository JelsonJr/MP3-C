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

void draw_music_timer(int seconds, ALLEGRO_AUDIO_STREAM* audioStream, ALLEGRO_FONT* font, Position* pos) {
    double durationInSeconds = al_get_audio_stream_length_secs(audioStream) + 2; // procurar bug que prolonga o audio por 2s e consertar essa gambiarra aqui

    int minutes = seconds / 60;
    int remaining_seconds = seconds % 60;

    char current_time[6];
    char total_time[6];
    
    snprintf(current_time, sizeof(current_time), "%02d:%02d", minutes, remaining_seconds);
    snprintf(total_time, sizeof(total_time), "%02d:%02d", (int)durationInSeconds / 60, (int)durationInSeconds % 60);

    int rect_width = 44;
    int rect_height = 20;

    al_draw_filled_rectangle(pos->x - 140, pos->y, (pos->x - 140) + rect_width, pos->y + rect_height, al_map_rgb(255, 255, 255));
    al_draw_textf(font, al_map_rgb(0, 0, 0), pos->x - 140, pos->y, ALLEGRO_ALIGN_LEFT, "%s", current_time);

    al_draw_filled_rectangle(pos->x + 160, pos->y, (pos->x + 160) + rect_width, pos->y + rect_height, al_map_rgb(255, 255, 255));
    al_draw_textf(font, al_map_rgb(0, 0, 0), pos->x + 160, pos->y, ALLEGRO_ALIGN_LEFT, "%s", total_time);

    double progress = (240 / durationInSeconds) * seconds;

    al_draw_filled_rectangle(pos->x - 85, pos->y + 4, pos->x - 85 + progress, pos->y + 8, al_map_rgb(0, 0, 255));

    al_flip_display();
}

void draw_buttons() {
    ALLEGRO_BITMAP* nextReturnImage = al_load_bitmap("./static/return-next.png");
    ALLEGRO_BITMAP* startEndImage = al_load_bitmap("./static/start-end.png");

    if (nextReturnImage == NULL || startEndImage == NULL) {
        return;
    }

    int img_width = al_get_bitmap_width(nextReturnImage);
    int img_height = al_get_bitmap_height(nextReturnImage);

    int img_width_initEnd = al_get_bitmap_width(startEndImage);
    int img_height_initEnd = al_get_bitmap_height(startEndImage);

    float scale_factor = 0.25;

    int new_img_width = img_width * scale_factor;
    int new_img_height = img_height * scale_factor;

    int new_img_width_initEnd = img_width * scale_factor;
    int new_img_height_initEnd = img_height * scale_factor;

    int center_x = 300;
    int center_y = 420;
    
    al_draw_scaled_bitmap(nextReturnImage, 0, 0, img_width, img_height, (920 / 2) - center_x, center_y, new_img_width, new_img_height, 0);
    al_draw_scaled_bitmap(nextReturnImage, 0, 0, img_width, img_height, (920 / 2) + center_x, center_y, -new_img_width, new_img_height, 0);

    al_draw_scaled_bitmap(startEndImage, 0, 0, img_width_initEnd, img_height_initEnd, (920 / 2) - 230, center_y, new_img_width_initEnd, new_img_height_initEnd, 0);
    al_draw_scaled_bitmap(startEndImage, 0, 0, img_width_initEnd, img_height_initEnd, (920 / 2) + 230, center_y, -new_img_width_initEnd, new_img_height_initEnd, 0); // segundo botão 
   
    al_destroy_bitmap(startEndImage);
    al_destroy_bitmap(nextReturnImage);

    draw_play_pause_button(-1);
}

void draw_play_pause_button(int playOrPause) {
    ALLEGRO_BITMAP* playImage = al_load_bitmap("./static/play.png");
    ALLEGRO_BITMAP* pauseImage = al_load_bitmap("./static/pause.png");

    if (playImage == NULL || pauseImage == NULL) {
        return;
    }

    int img_width = al_get_bitmap_width(playImage);
    int img_height = al_get_bitmap_height(playImage);

    float scale_factor = 0.25;

    int new_img_width = img_width * scale_factor;
    int new_img_height = img_height * scale_factor;
   
    int center_x = 300;
    int center_y = 440;  

    al_draw_filled_rectangle((920 / 2) - new_img_width / 2, center_y, (920 / 2) + new_img_width / 2, center_y + new_img_height, al_map_rgb(255, 255, 255));

    if (playOrPause > 0) {
        al_draw_scaled_bitmap(pauseImage, 0, 0, img_width, img_height, (920 / 2) - new_img_width / 2, center_y, new_img_width, new_img_height, 0);
    } else {
        al_draw_scaled_bitmap(playImage, 0, 0, img_width, img_height, (920 / 2) - new_img_width / 2, center_y, new_img_width, new_img_height, 0);
    }

    al_destroy_bitmap(playImage);
    al_destroy_bitmap(pauseImage);

    al_flip_display();
}