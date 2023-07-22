#include "initial_screen.h"

int init_events(Display* display, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_TIMER* timer) {
    event_queue = al_create_event_queue();
    timer = al_create_timer(1.0 / 60);

    al_register_event_source(event_queue, al_get_display_event_source(display->screen));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    return (event_queue != NULL && timer != NULL);
}

void draw_initial_screen(Display* display) {
    ALLEGRO_EVENT_QUEUE* event_queue = NULL;
    ALLEGRO_TIMER* timer = NULL;

    al_set_window_title(display->screen, "MPC");
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    init_monitor(display);
    draw_gradient(display);

    int posX;
    const char* textTitle = "Selecione a pr\xc3\xb3xima m\xc3\xbasica e curta o som!";
    const char* textApp = "MPC";

    ALLEGRO_FONT* fontTitle = al_load_font(MONTSERRAT_BOLD, 28, 0);
    posX = find_screen_center(display, fontTitle, textTitle);
    Position* posTitle = create_position(posX, 20);

    ALLEGRO_FONT* fontApp = al_load_font(MONTSERRAT_BOLD, 36, 0);
    posX = find_screen_center(display, fontApp, textApp);
    Position* posApp = create_position(posX, 350);

    ALLEGRO_FONT* fontCredits = al_load_font(MONTSERRAT_SEMIBOLD, 12, 0);
    Position* posCredits = create_position(10, 460);

    draw_text(fontTitle, posTitle, al_map_rgb(250, 250, 250), textTitle, "%s", ALLEGRO_ALIGN_LEFT);
    draw_text(fontApp, posApp, al_map_rgb(250, 250, 250), textApp, "%s", ALLEGRO_ALIGN_LEFT);
    draw_text(fontCredits, posCredits, al_map_rgb(250, 250, 250), "Criado por: Jelson Rodrigues Junior", "%s", ALLEGRO_ALIGN_LEFT);

    if (init_events(display, event_queue, timer)) {
        destroy_display(display);
        destroy_position(posCredits);
        al_destroy_font(fontCredits);
        al_destroy_event_queue(event_queue);

        return;
    }

    ALLEGRO_COLOR buttonColor = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR symbolColor = al_map_rgb(138, 43, 226);

    float symbolSizeScale = 1.0;
    float targetScale = 1.0;
    float scaleStep = 0.0;

    al_draw_filled_circle(display->SCREEN_WIDTH / 2, display->SCREEN_HEIGHT / 2, BUTTON_RADIUS, buttonColor);
    al_draw_filled_triangle(display->SCREEN_WIDTH / 2 - SYMBOL_SIZE + 8, display->SCREEN_HEIGHT / 2 - SYMBOL_SIZE,
        display->SCREEN_WIDTH / 2 + SYMBOL_SIZE + 8, display->SCREEN_HEIGHT / 2,
        display->SCREEN_WIDTH / 2 - SYMBOL_SIZE + 8, display->SCREEN_HEIGHT / 2 + SYMBOL_SIZE, symbolColor);

    al_flip_display();
    al_start_timer(timer);

    int mouseX = 0;
    int mouseY = 0;
    int mouseCursorDefault = 1;
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
