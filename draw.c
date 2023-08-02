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
