#include "display.h"

Display* create_display(int width, int height) {
    Display* display = malloc(sizeof(Display));

    if (display == NULL) {
        al_show_native_message_box(NULL, "Erro", "Aloca\xc3\xa7\xc3\xa3o de mem\xc3\xb3ria", "Falha ao alocar mem\xc3\xb3ria para o display.\n", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return NULL;
    }

    display->SCREEN_WIDTH = width;
    display->SCREEN_HEIGHT = height;
    display->screen = al_create_display(display->SCREEN_WIDTH, display->SCREEN_HEIGHT);

    if (!display->screen) {
        al_show_native_message_box(NULL, "Erro", "Instanciar display", "Falha ao criar janela.\n", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        free(display);

        return NULL;
    }

    return display;
}

void destroy_display(Display* display) {
    if (display) {
        if (display->screen) {
            al_destroy_display(display->screen);
        }

        free(display);
    }
}