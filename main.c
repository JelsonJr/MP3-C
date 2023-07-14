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

    ALLEGRO_COLOR backgroundColor = al_map_rgb(200, 200, 200);
    al_clear_to_color(backgroundColor);

    ALLEGRO_FONT* font = al_load_font("montserrat-black.ttf", 24, 0);
    if (!font) {
        al_show_native_message_box(NULL, "Erro", "Inicializa\xc3\xa7\xc3\xa3o de Fontes", "Falha ao criar/inicializar a fonte.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        destroy_display(display);
        return -1;
    }

    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    const char* text = "Selecione a pr\xc3\xb3xima m\xc3\xbasica!";
    int textPosX = display->SCREEN_WIDTH / 2 - al_get_text_width(font, text) / 2;
    int textPosY = 0;
    
    al_draw_textf(font, al_map_rgb(75, 0, 130), textPosX, textPosY, ALLEGRO_ALIGN_LEFT, "%s", text);
    al_destroy_font(font);
    
    al_flip_display();  
    al_rest(15.0);

    destroy_display(display);

    return 0;
}
