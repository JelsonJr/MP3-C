#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include "display.h"

int init_allegro() {
    if (!al_init()) {
        al_show_native_message_box(NULL, "Erro", "Inicializa\xc3\xa7\xc3\xa3o do Allegro", "Falha ao inicializar Allegro.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }

    if (!al_install_keyboard()) {
        al_show_native_message_box(NULL, "Erro", "Inicializa\xc3\xa7\xc3\xa3o do Teclado", "Falha ao inicializar teclado.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }

    if (!al_install_mouse()) {
        al_show_native_message_box(NULL, "Erro", "Inicializa\xc3\xa7\xc3\xa3o do Mouse", "Falha ao inicializar mouse.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }

    if (!al_init_primitives_addon()) {
        al_show_native_message_box(NULL, "Erro", "Inicializa\xc3\xa7\xc3\xa3o de Primitivas", "Falha ao inicializar add-on de primitivas.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }

    if (!al_init_font_addon()) {
        al_show_native_message_box(NULL, "Erro", "Inicializa\xc3\xa7\xc3\xa3o de Fontes", "Falha ao inicializar add-on de fontes.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }

    if (!al_init_ttf_addon()) {
        al_show_native_message_box(NULL, "Erro", "Inicializa\xc3\xa7\xc3\xa3o de Fontes", "Falha ao inicializar ttf para importa\xc3\xa7\xc3\xa3o de fontes.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }
    
    return 1;
}

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


int main() {
    if (!init_allegro()) {
        return -1;
    }

    Display* display = create_display(720, 480);
    if (!display) {
        return -1;
    }

    al_set_window_title(display->screen, "MPC");

    ALLEGRO_MONITOR_INFO monitorInfo;
    al_get_monitor_info(0, &monitorInfo);

    int windowPosX = monitorInfo.x1 + (monitorInfo.x2 - monitorInfo.x1 - display->SCREEN_WIDTH) / 2;
    int windowPosY = monitorInfo.y1 + (monitorInfo.y2 - monitorInfo.y1 - display->SCREEN_HEIGHT) / 2;
    
    al_set_window_position(display->screen, windowPosX, windowPosY);
    draw_gradient(display);

    ALLEGRO_FONT* font = al_load_font("montserrat-bold.ttf", 24, 0);
    if (!font) {
        al_show_native_message_box(NULL, "Erro", "Inicializa\xc3\xa7\xc3\xa3o de Fontes", "Falha ao criar/inicializar a fonte.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        destroy_display(display);
        return -1;
    }

    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    const char* text = "Selecione a pr\xc3\xb3xima m\xc3\xbasica!";
    int textPosX = display->SCREEN_WIDTH / 2 - al_get_text_width(font, text) / 2;
    int textPosY = 0;
    
    al_draw_textf(font, al_map_rgb(250, 250, 250), textPosX, textPosY, ALLEGRO_ALIGN_LEFT, "%s", text);
    al_destroy_font(font);
    
    ALLEGRO_COLOR buttonColor = al_map_rgb(255, 255, 255);

    int buttonRadius = 50;
    int buttonX = display->SCREEN_WIDTH / 2;
    int buttonY = display->SCREEN_HEIGHT / 2;

    ALLEGRO_COLOR symbolColor = al_map_rgb(138, 43, 226);
    int symbolSize = 30;

    al_draw_filled_circle(buttonX, buttonY, buttonRadius, buttonColor);
    al_draw_filled_triangle(buttonX - symbolSize + 8, buttonY - symbolSize, buttonX + symbolSize + 8, buttonY, buttonX - symbolSize + 8, buttonY + symbolSize, symbolColor);

    al_flip_display();  
    al_rest(15.0);

    destroy_display(display);

    return 0;
}
