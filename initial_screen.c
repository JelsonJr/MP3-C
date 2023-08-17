#include "initial_screen.h"
#include "mouse_location.h"

int initial_screen(Display* display, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_TIMER* timer) {
    al_set_window_title(display->screen, "MPC");
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    init_monitor(display);
    draw_gradient(display);

    const char* textTitle = "Selecione a pr\xc3\xb3xima m\xc3\xbasica e curta o som!";
    const char* textApp = "MPC";

    ALLEGRO_FONT* fontTitle = al_load_font(MONTSERRAT_BOLD, 28, 0);
    Position* posTitle = create_position(find_screen_center(display, fontTitle, textTitle), 20);

    ALLEGRO_FONT* fontApp = al_load_font(MONTSERRAT_BOLD, 36, 0);
    Position* posApp = create_position(find_screen_center(display, fontApp, textApp), 350);

    ALLEGRO_FONT* fontCredits = al_load_font(MONTSERRAT_SEMIBOLD, 12, 0);
    Position* posCredits = create_position(10, 460);

    draw_text(fontTitle, posTitle, al_map_rgb(250, 250, 250), textTitle, "%s", ALLEGRO_ALIGN_LEFT);
    draw_text(fontApp, posApp, al_map_rgb(250, 250, 250), textApp, "%s", ALLEGRO_ALIGN_LEFT);
    draw_text(fontCredits, posCredits, al_map_rgb(250, 250, 250), "Saiba mais sobre o projeto", "%s", ALLEGRO_ALIGN_LEFT);

    fontCredits = al_load_font(MONTSERRAT_SEMIBOLD, 12, 0);
    posCredits = create_position(10, 460);

    ALLEGRO_COLOR buttonColor = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR symbolColor = al_map_rgb(138, 43, 226);

    float symbolSizeScale = 1.0;
    float targetScale = 1.0;
    float scaleStep = 0.0;

    al_draw_filled_circle((float)display->SCREEN_WIDTH / 2, (float)display->SCREEN_HEIGHT / 2, BUTTON_RADIUS, buttonColor);
    al_draw_filled_triangle((float)display->SCREEN_WIDTH / 2 - SYMBOL_SIZE + 8, (float)display->SCREEN_HEIGHT / 2 - SYMBOL_SIZE,
        (float)display->SCREEN_WIDTH / 2 + SYMBOL_SIZE + 8, (float)display->SCREEN_HEIGHT / 2,
        (float)display->SCREEN_WIDTH / 2 - SYMBOL_SIZE + 8, (float)display->SCREEN_HEIGHT / 2 + SYMBOL_SIZE, symbolColor);

    al_flip_display();
    al_start_timer(timer);

    int mouseX = 0;
    int mouseY = 0;
    int option = 0;
    int mouseCursorDefault = 1;

    while (1) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        mouseX = ev.mouse.x;
        mouseY = ev.mouse.y;

        if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
            if (!al_is_event_queue_empty(event_queue)) {
                continue;
            }

            if (is_mouse_over_credits(mouseX, mouseY, posCredits, fontCredits)) {
                const char* command = "start https://github.com/JelsonJr/MP3-C";

                system(command);
                continue;
            }

            if (is_mouse_over_button(display, mouseX, mouseY, BUTTON_RADIUS)) {
                option = 1;
                break;
            }
        }

        if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
            if (is_mouse_over_credits(mouseX, mouseY, posCredits, fontCredits)) {
                al_set_system_mouse_cursor(display->screen, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
                mouseCursorDefault = 0;

                continue;
            }

            if (is_mouse_over_button(display, mouseX, mouseY, BUTTON_RADIUS)) {
                if (!al_is_event_queue_empty(event_queue)) {
                    continue;
                }

                al_set_system_mouse_cursor(display->screen, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
                mouseCursorDefault = 0;

                targetScale = 1.15;
                scaleStep = (targetScale - symbolSizeScale) / (ANIMATION_DURATION * 60.0);

                while (symbolSizeScale < targetScale) {
                    animateButton(display, symbolColor, buttonColor, symbolSizeScale);
                    symbolSizeScale += scaleStep;
                    al_rest(1.0 / 120.0);
                }

                animateButton(display, symbolColor, buttonColor, targetScale);

                continue;
            }

            if (!al_is_event_queue_empty(event_queue)) {
                continue;
            }

            if (!mouseCursorDefault) {
                al_set_system_mouse_cursor(display->screen, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
                mouseCursorDefault = 1;
            }

            targetScale = 1.0;
            scaleStep = (targetScale - symbolSizeScale) / (ANIMATION_DURATION * 60.0);

            while (symbolSizeScale > targetScale) {
                animateButton(display, buttonColor, symbolColor, symbolSizeScale);
                symbolSizeScale += scaleStep;
                al_rest(1.0 / 120.0);
            }

            animateButton(display, buttonColor, symbolColor, targetScale);
        }

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }
    }

    destroy_position(posCredits);
    al_destroy_font(fontCredits);
    destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);

    return option;
}

void animateButton(Display* display, ALLEGRO_COLOR buttonColor, ALLEGRO_COLOR symbolColor, float symbolSizeScale) {
    int buttonX = display->SCREEN_WIDTH / 2;
    int buttonY = display->SCREEN_HEIGHT / 2;
    int buttonRadius = BUTTON_RADIUS;
    int symbolSize = SYMBOL_SIZE * symbolSizeScale;

    al_draw_filled_circle(buttonX, buttonY, buttonRadius, buttonColor);
    al_draw_filled_triangle(buttonX - symbolSize + 8, buttonY - symbolSize, buttonX + symbolSize + 8, buttonY, buttonX - symbolSize + 8, buttonY + symbolSize, symbolColor);

    al_flip_display();
}
