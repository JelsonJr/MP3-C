#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <dirent.h>
#include "display.h"
#include "position.h"
#include "draw.h"
#include "fonts.h"
#include "initial_screen.h"
#include "thread.h"
#include "music_menu.h"
#include "music.h"

#ifdef _WIN32
	#include <io.h>
	#include <Windows.h>
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

	if (!al_init_image_addon()) {
		al_show_native_message_box(NULL, "Erro", "Inicializa\xc3\xa7\xc3\xa3o de Imagem", "Falha ao inicializar add-on de imagens.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}

	if (!al_init_acodec_addon()) {
		al_show_native_message_box(NULL, "Erro", "Inicializa\xc3\xa7\xc3\xa3o de \u00E1udio", "Falha ao inicializar add-on de \u00E1udio.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		al_uninstall_audio();
		return 0;
	}

	return 1;
}

int init_events(Display* display, ALLEGRO_EVENT_QUEUE** event_queue, ALLEGRO_TIMER** timer) {
	*event_queue = al_create_event_queue();
	*timer = al_create_timer(1.0 / 60);

	al_register_event_source(*event_queue, al_get_display_event_source(display->screen));
	al_register_event_source(*event_queue, al_get_mouse_event_source());
	al_register_event_source(*event_queue, al_get_timer_event_source(*timer));

	return (*event_queue != NULL && *timer != NULL);
}

int main() {
	if (!init_allegro()) {
		return -1;
	}

	int num_musics = 0;
	char** musics = list_files_directory(&num_musics);

	Display* mainDisplay;
	Display* initialDisplay = create_display(720, 480);
	
	if (!initialDisplay) {
		return -1;
	}

	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	ALLEGRO_TIMER* timer = NULL;
	init_events(initialDisplay, &event_queue, &timer);

	int option = initial_screen(initialDisplay, event_queue, timer);
	
	switch (option) {
	case 1:
		mainDisplay = create_display(920, 480);
		if (!mainDisplay) {
			return -1;
		}

		init_events(mainDisplay, &event_queue, &timer);
		music_menu(mainDisplay, num_musics, musics, event_queue, timer);

		break;
	default:
		break;
	}

	printf("fim\n");
	al_uninstall_audio();

	return 0;
}
