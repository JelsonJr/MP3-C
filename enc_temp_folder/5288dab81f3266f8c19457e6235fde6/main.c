#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <dirent.h>
#include "display.h"
#include "position.h"
#include "draw.h"
#include "fonts.h"
#include "initial_screen.h"

#ifdef _WIN32
    #include <io.h>
    #define getcwd _getcwd
#else
    #include <unistd.h>
#endif

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

    if (!al_install_audio()) {
        al_show_native_message_box(NULL, "Erro", "Inicializa\xc3\xa7\xc3\xa3o de \u00E1udio", "Falha ao inicializar add-on de \u00E1udio.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }

    if (!al_init_acodec_addon()) {
        al_show_native_message_box(NULL, "Erro", "Inicializa\xc3\xa7\xc3\xa3o de \u00E1udio", "Falha ao inicializar add-on de \u00E1udio.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_uninstall_audio();
        return 0;
    }

    return 1;
}

int main() {
    if (!init_allegro()) {
        return -1;
    }

    Display* displayInicial = create_display(720, 480);
    if (!displayInicial) {
        return -1;
    }

    ALLEGRO_EVENT_QUEUE* event_queue = NULL;
    ALLEGRO_TIMER* timer = NULL;

    init_events(displayInicial, &event_queue, &timer);
    draw_initial_screen(displayInicial, event_queue, timer);

    al_uninstall_audio();
    destroy_display(displayInicial);

    return 0;
}
