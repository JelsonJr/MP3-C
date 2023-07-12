#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

int main() {
    ALLEGRO_DISPLAY* display = NULL;
    const int screenWidth = 800;
    const int screenHeight = 600;

    if (!al_init()) {
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
        return -1;
    }

    display = al_create_display(screenWidth, screenHeight);
    if (!display) {
        fprintf(stderr, "Falha ao criar janela.\n");
        return -1;
    }

    al_set_window_title(display, "Hello World");

    ALLEGRO_MONITOR_INFO monitorInfo;
    al_get_monitor_info(0, &monitorInfo);
    int windowPosX = monitorInfo.x1 + (monitorInfo.x2 - monitorInfo.x1 - screenWidth) / 2;
    int windowPosY = monitorInfo.y1 + (monitorInfo.y2 - monitorInfo.y1 - screenHeight) / 2;
    al_set_window_position(display, windowPosX, windowPosY);

    ALLEGRO_COLOR backgroundColor = al_map_rgb(200, 200, 200);  // Cor cinza claro (RGB: 200, 200, 200)
    al_clear_to_color(backgroundColor);

    ALLEGRO_FONT* font = al_create_builtin_font();
    if (!font) {
        fprintf(stderr, "Falha ao criar fonte.\n");
        al_destroy_display(display);
        return -1;
    }
   
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA); // Configura o modo de mesclagem para desenhar o texto com transparência
   
    const char* text = "Hello World";
    int fontSize = 36;  // Tamanho da fonte (36 pixels)
    int textPosX = screenWidth / 2 - al_get_text_width(font, text) / 2;
    int textPosY = screenHeight / 2 - al_get_font_ascent(font) / 2;
    al_draw_textf(font, al_map_rgb(0, 0, 100), textPosX, textPosY, ALLEGRO_ALIGN_LEFT, "%s", text);

    al_destroy_font(font);

    al_flip_display();  // Atualiza o display

    al_rest(5.0);  // Aguarda 5 segundos

    al_destroy_display(display);

    return 0;
}
