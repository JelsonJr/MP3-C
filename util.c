#include "util.h"

int find_screen_center(Display* display, ALLEGRO_FONT* font, const char* text) {
    int center = display->SCREEN_WIDTH / 2 - al_get_text_width(font, text) / 2;

    return center;
}

void init_monitor(Display* display) {
    ALLEGRO_MONITOR_INFO monitorInfo;
    al_get_monitor_info(0, &monitorInfo);

    const int windowPosX = monitorInfo.x1 + (monitorInfo.x2 - monitorInfo.x1 - display->SCREEN_WIDTH) / 2;
    const int windowPosY = monitorInfo.y1 + (monitorInfo.y2 - monitorInfo.y1 - display->SCREEN_HEIGHT) / 2;

    al_set_window_position(display->screen, windowPosX, windowPosY);
}