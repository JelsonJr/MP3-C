#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include "display.h"
#include "position.h"
#include "draw.h"
#include "fonts.h"

#define BUTTON_RADIUS 70
#define SYMBOL_SIZE 30
#define ANIMATION_DURATION 0.6

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

void init_monitor(Display* display) {
    ALLEGRO_MONITOR_INFO monitorInfo;
    al_get_monitor_info(0, &monitorInfo);

    const int windowPosX = monitorInfo.x1 + (monitorInfo.x2 - monitorInfo.x1 - display->SCREEN_WIDTH) / 2;
    const int windowPosY = monitorInfo.y1 + (monitorInfo.y2 - monitorInfo.y1 - display->SCREEN_HEIGHT) / 2;

    al_set_window_position(display->screen, windowPosX, windowPosY);
}

void animateButton(Display* display, ALLEGRO_COLOR buttonColor, ALLEGRO_COLOR symbolColor, float buttonScale) {
    int buttonRadius = BUTTON_RADIUS * buttonScale;
    int symbolSize = SYMBOL_SIZE * buttonScale;
    int buttonX = display->SCREEN_WIDTH / 2;
    int buttonY = display->SCREEN_HEIGHT / 2;

    // Desenhar o botão (círculo e triângulo)
    al_draw_filled_circle(buttonX, buttonY, buttonRadius, buttonColor);
    al_draw_filled_triangle(buttonX - symbolSize + 8, buttonY - symbolSize, buttonX + symbolSize + 8, buttonY, buttonX - symbolSize + 8, buttonY + symbolSize, symbolColor);

    // Atualizar a tela
    al_flip_display();
}

int main() {
    if (!init_allegro()) {
        return -1;
    }

    Display* displayInicial = create_display(720, 480);
    if (!displayInicial) {
        return -1;
    }

    al_set_window_title(displayInicial->screen, "MPC");
    init_monitor(displayInicial);

    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    draw_gradient(displayInicial);

    ALLEGRO_FONT* font = al_load_font(MONTSERRAT_BOLD, 28, 0);;
    const char* text = "Selecione a pr\xc3\xb3xima m\xc3\xbasica e curta o som!";
    const int posX = displayInicial->SCREEN_WIDTH / 2 - al_get_text_width(font, text) / 2;
    Position* pos = create_position(posX, 20);

    draw_text(font, pos, al_map_rgb(250, 250, 250), text, "%s", ALLEGRO_ALIGN_LEFT);

    font = al_load_font(MONTSERRAT_SEMIBOLD, 12, 0);
    pos = create_position(10, 460);

    draw_text(font, pos, al_map_rgb(250, 250, 250), "Criado por: Jelson Rodrigues Junior", "%s", ALLEGRO_ALIGN_LEFT);

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60);

    al_register_event_source(event_queue, al_get_display_event_source(displayInicial->screen));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    ALLEGRO_COLOR buttonColor = al_map_rgb(255, 255, 255);

    int buttonRadius = 70;
    int buttonX = displayInicial->SCREEN_WIDTH / 2;
    int buttonY = displayInicial->SCREEN_HEIGHT / 2;

    ALLEGRO_COLOR symbolColor = al_map_rgb(138, 43, 226);
    int symbolSize = 30;
 
    int mouseOverButton = 0;

    al_draw_filled_circle(buttonX, buttonY, buttonRadius, buttonColor);
    al_draw_filled_triangle(buttonX - symbolSize + 8, buttonY - symbolSize, buttonX + symbolSize + 8, buttonY, buttonX - symbolSize + 8, buttonY + symbolSize, symbolColor);

    al_flip_display();
    float buttonScale = 1.0;
    float targetScale = 1.0;
    float scaleStep = 0.0;
    float animationTimer = 0.0;

    al_start_timer(timer);

    while (1) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
            int mouseX = ev.mouse.x;
            int mouseY = ev.mouse.y;

            if (mouseX >= displayInicial->SCREEN_WIDTH / 2 - BUTTON_RADIUS && mouseX <= displayInicial->SCREEN_WIDTH / 2 + BUTTON_RADIUS &&
                mouseY >= displayInicial->SCREEN_HEIGHT / 2 - BUTTON_RADIUS && mouseY <= displayInicial->SCREEN_HEIGHT / 2 + BUTTON_RADIUS) {
                if (!al_is_event_queue_empty(event_queue)) {
                    continue;
                }

                targetScale = 1.15;
                scaleStep = (targetScale - buttonScale) / (ANIMATION_DURATION * 60.0);
                animationTimer = 0.0;

                while (buttonScale < targetScale) {
                    animateButton(displayInicial, al_map_rgb(255, 255, 255), al_map_rgb(138, 43, 226), buttonScale);
                    buttonScale += scaleStep;
                    al_rest(1.0 / 120.0);  // Acelerar a animação
                }

                animateButton(displayInicial, al_map_rgb(255, 255, 255), al_map_rgb(138, 43, 226), targetScale);
            }
            else {
                if (!al_is_event_queue_empty(event_queue)) {
                    continue;
                }

                targetScale = 1.0;
                scaleStep = (targetScale - buttonScale) / (ANIMATION_DURATION * 60.0);
                animationTimer = 0.0;

                while (buttonScale > targetScale) {
                    animateButton(displayInicial, al_map_rgb(255, 255, 255), al_map_rgb(138, 43, 226), buttonScale);
                    buttonScale += scaleStep;
                    al_rest(1.0 / 120.0);  // Acelerar a animação
                }

                animateButton(displayInicial, al_map_rgb(255, 255, 255), al_map_rgb(138, 43, 226), targetScale);
            }
        }
        else if (ev.type == ALLEGRO_EVENT_TIMER) {
            // Timer event
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }
    }

    destroy_display(displayInicial);
    al_destroy_event_queue(event_queue);

    return 0;
}