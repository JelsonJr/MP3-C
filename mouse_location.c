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

int is_over_return_button(int mouseX, int mouseY, int buttonCenterX, int buttonCenterY, int new_img_width, int new_img_height) {
    int buttonWidth = new_img_width;
    int buttonHeight = new_img_height;
    int buttonTop = buttonCenterY - buttonHeight / 2;
    int buttonBottom = buttonCenterY + buttonHeight / 2;
    int buttonLeft = buttonCenterX - buttonWidth / 2;
    int buttonRight = buttonCenterX + buttonWidth / 2;

    return (mouseX >= buttonLeft && mouseX <= buttonRight && mouseY >= buttonTop && mouseY <= buttonBottom);
}

int is_over_next_button(int mouseX, int mouseY, int buttonCenterX, int buttonCenterY, int new_img_width, int new_img_height) {
    int buttonWidth = new_img_width;
    int buttonHeight = new_img_height;
    int buttonTop = buttonCenterY - buttonHeight / 2;
    int buttonBottom = buttonCenterY + buttonHeight / 2;
    int buttonLeft = buttonCenterX - buttonWidth / 2;
    int buttonRight = buttonCenterX + buttonWidth / 2;

    return (mouseX >= buttonLeft && mouseX <= buttonRight && mouseY >= buttonTop && mouseY <= buttonBottom);
}

int is_over_init_button(int mouseX, int mouseY) {
    int buttonWidth = 207; 
    int buttonHeight = 138;

    int buttonCenterX = (920 / 2) - 230;
    int buttonCenterY = 420;

    int buttonTop = buttonCenterY - buttonHeight / 2;
    int buttonBottom = buttonCenterY + buttonHeight / 2;
    int buttonLeft = buttonCenterX - buttonWidth / 2;
    int buttonRight = buttonCenterX + buttonWidth / 2;

    return (mouseX >= buttonLeft && mouseX <= buttonRight && mouseY >= buttonTop && mouseY <= buttonBottom);
}

int is_over_end_button(int mouseX, int mouseY) {
    int buttonWidth = 207;
    int buttonHeight = 138;

    int buttonCenterX = (920 / 2) + 230;
    int buttonCenterY = 420;

    int buttonTop = buttonCenterY - buttonHeight / 2;
    int buttonBottom = buttonCenterY + buttonHeight / 2;
    int buttonLeft = buttonCenterX - buttonWidth / 2;
    int buttonRight = buttonCenterX + buttonWidth / 2;

    return (mouseX >= buttonLeft && mouseX <= buttonRight && mouseY >= buttonTop && mouseY <= buttonBottom);
}

int is_over_play_pause_button(int mouseX, int mouseY) {
    int buttonWidth = 31;
    int buttonHeight = 37;

    int buttonCenterX = 300;
    int buttonCenterY = 440;

    int buttonTop = buttonCenterY;
    int buttonBottom = buttonCenterY + buttonHeight;
    int buttonLeft = (920 / 2) - buttonWidth / 2;
    int buttonRight = (920 / 2) + buttonWidth / 2;

    return (mouseX >= buttonLeft && mouseX <= buttonRight && mouseY >= buttonTop && mouseY <= buttonBottom);
}

int is_mouse_over_music(int mouseX, int mouseY, Position* musicPosition) {
    int text_width = 250; // Altere para um nome de música típico
    int text_height = 14;

    int musicTop = musicPosition->y;
    int musicBottom = musicPosition->y + text_height;
    int musicLeft = musicPosition->x;
    int musicRight = musicPosition->x + text_width;

    return (mouseX >= musicLeft && mouseX <= musicRight && mouseY >= musicTop && mouseY <= musicBottom);
}