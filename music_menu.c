#include "music_menu.h"

int music_menu(Display* display, int num_musics, char** musics, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_TIMER* timer) {
	draw_gradient(display);
	al_draw_filled_rectangle(0, 400, 920, 480, al_map_rgb(255, 255, 255));
	
	draw_buttons();
	draw_musics_list(num_musics, musics);
	
	int musicPlaying = 0;
	int threadPaused = 0;

	ThreadArguments* arguments = create_thread(musics[0]);
	Position* mousePosition = create_position(0, 0);
	ALLEGRO_THREAD* sound_thread = NULL;
	ALLEGRO_AUDIO_STREAM* audioStream = al_load_audio_stream(arguments->filepath, 4, 2048);
	ALLEGRO_FONT* font_timer = al_load_font(MONTSERRAT_BOLD, 14, 0);
	Position* pos_timer = create_position(find_screen_center(display, font_timer, "444:444"), 430);
	
	al_start_timer(timer);

	while (1) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		mousePosition->x = ev.mouse.x;
		mousePosition->y = ev.mouse.y;
		
		if (ev.type == ALLEGRO_EVENT_TIMER && musicPlaying) {
			draw_music_timer(arguments->seconds, audioStream, font_timer, pos_timer);
		}

		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && !musicPlaying) {
			arguments->done = 0;
			musicPlaying = !musicPlaying;

			draw_play_pause_button(1);
			
			sound_thread = al_create_thread(play_sound, arguments);
			al_start_thread(sound_thread);
			
			continue;
		}

		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && musicPlaying) {
			arguments->done = 1;
			musicPlaying = !musicPlaying;

			draw_play_pause_button(-1);

			if (sound_thread) {
				al_destroy_thread(sound_thread);
			}

			continue;
		}
	}

	al_drain_audio_stream(audioStream);
	al_destroy_audio_stream(audioStream);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	destroy_display(display);

	return 1;
}