#include "mouse_location.h"

int is_mouse_over_credits(int mouseX, int mouseY, Position* posCredits, ALLEGRO_FONT* fontCredits) {
    int textWidth = al_get_text_width(fontCredits, "Criado por: Jelson Rodrigues Junior");
    int textHeight = al_get_font_line_height(fontCredits);

    return (mouseX >= posCredits->x && mouseX <= posCredits->x + textWidth &&
        mouseY >= posCredits->y && mouseY <= posCredits->y + textHeight);
}

int is_mouse_over_button(Display* display, int mouseX, int mouseY, const int buttonRadius) {
    return (mouseX >= display->SCREEN_WIDTH / 2 - buttonRadius && mouseX <= display->SCREEN_WIDTH / 2 + buttonRadius &&
        mouseY >= display->SCREEN_HEIGHT / 2 - buttonRadius && mouseY <= display->SCREEN_HEIGHT / 2 + buttonRadius);
}
