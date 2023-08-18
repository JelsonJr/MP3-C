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

DWORD WINAPI play_sound(LPVOID arg) {
	ThreadArguments* arguments = (ThreadArguments*)arg;

	Display* display = arguments->display;
	const char* filepath = arguments->filepath;

	if (!al_reserve_samples(1)) {
		return;
	}

	ALLEGRO_SAMPLE* audioSample = al_load_sample(filepath);
	if (!audioSample) {
		return;
	}

	ALLEGRO_SAMPLE_INSTANCE* instance = al_create_sample_instance(audioSample);
	if (!instance) {
		al_destroy_sample(audioSample);
		return;
	}

	if (!al_attach_sample_instance_to_mixer(instance, al_get_default_mixer())) {
		al_destroy_sample_instance(instance);
		al_destroy_sample(audioSample);
		return;
	}

	al_play_sample_instance(instance);

	while (al_get_sample_instance_playing(instance)) {}

	al_destroy_sample_instance(instance);
	al_destroy_sample(audioSample);
	arguments->done = 1;
}

const char* get_path_user() {
	char* path = (char*)malloc(MAX_PATH);
	if (!path) {
		return NULL;
	}

	const char* downloads = "\\Downloads\\";

	if (!GetEnvironmentVariableA("USERPROFILE", path, MAX_PATH)) {
		return NULL;
	}

	strcat_s(path, MAX_PATH, downloads);

	return path;
}

char** list_files_directory(int* num_files) {
	const char* directory = get_path_user();
	if (!directory) {
		free(directory);

		return NULL;
	}

	DIR* dir;
	struct dirent* entry;

	dir = opendir(directory);

	if (dir == NULL) {
		free(directory);
		*num_files = 0;

		return NULL;
	}

	char** paths_list = NULL;
	int num_paths = 0;

	while ((entry = readdir(dir)) != NULL) {
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			continue;

		char* extension = strrchr(entry->d_name, '.');
		if ((extension != NULL && strcmp(extension, ".wav") == 0)) {
			char absolute_path[512];
			snprintf(absolute_path, sizeof(absolute_path), "%s\%s", directory, entry->d_name);

			char** new_paths_list = (char**)realloc(paths_list, (num_paths + 1) * sizeof(char*));
			if (new_paths_list == NULL) {
				closedir(dir);

				free(directory);
				*num_files = num_paths;

				return paths_list;
			}

			paths_list = new_paths_list;
			paths_list[num_paths] = (char*)malloc(strlen(absolute_path) + 1);

			if (paths_list[num_paths] != NULL) {
				strcpy_s(paths_list[num_paths], strlen(absolute_path) + 1, absolute_path);
				num_paths++;

				continue;
			}

			closedir(dir);
			free(directory);
			*num_files = num_paths;

			return paths_list;
		}
	}

	closedir(dir);
	free(directory);
	*num_files = num_paths;

	return paths_list;
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
		music_menu(mainDisplay, num_musics, musics);

		ThreadArguments* arguments = create_thread(mainDisplay, musics[0]);

		HANDLE sound_thread = CreateThread(NULL, 0, play_sound, arguments, 0, NULL);
		if (sound_thread == NULL) {
			return -1;
		}

		ALLEGRO_AUDIO_STREAM* audioStream = al_load_audio_stream(arguments->filepath, 4, 2048);
		if (!audioStream) {
			return -1;
		}

		ALLEGRO_FONT* font_timer = al_load_font(MONTSERRAT_BOLD, 14, 0);
		Position* pos_timer = create_position(find_screen_center(arguments->display, font_timer, "444:444"), 420);

		unsigned int seconds = 0;

		while (!arguments->done) {
			al_rest(1);
			draw_music_timer(seconds, audioStream, arguments->display, font_timer, pos_timer);
			seconds++;
		}

		al_drain_audio_stream(audioStream);
		al_destroy_audio_stream(audioStream);
		al_destroy_event_queue(event_queue);
		al_destroy_timer(timer);
		destroy_display(mainDisplay);

		break;
	default:
		break;
	}

	al_uninstall_audio();

	return 0;
}
