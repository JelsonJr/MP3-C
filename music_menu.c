#include "music_menu.h"

int music_menu(Display* display, int num_musics, char** musics, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_TIMER* timer) {
	draw_gradient(display);
	al_draw_filled_rectangle(0, 400, 920, 480, al_map_rgb(255, 255, 255));

	ALLEGRO_BITMAP* nextReturnImage = al_load_bitmap("./static/return-next.png");
	ALLEGRO_BITMAP* startEndImage = al_load_bitmap("./static/start-end.png");
	if (nextReturnImage && startEndImage) {
		int img_width = al_get_bitmap_width(nextReturnImage);
		int img_height = al_get_bitmap_height(nextReturnImage);

		int img_width_initEnd = al_get_bitmap_width(startEndImage);
		int img_height_initEnd = al_get_bitmap_height(startEndImage);

		float scale_factor = 0.25;

		int new_img_width = img_width * scale_factor;
		int new_img_height = img_height * scale_factor;

		int new_img_width_initEnd = img_width * scale_factor;
		int new_img_height_initEnd = img_height * scale_factor;

		int center_x = 300;
		int center_y = 420;

		al_draw_scaled_bitmap(nextReturnImage, 0, 0, img_width, img_height, (920 / 2) - center_x, center_y, new_img_width, new_img_height, 0);
		al_draw_scaled_bitmap(nextReturnImage, 0, 0, img_width, img_height, (920 / 2) + center_x, center_y, -new_img_width, new_img_height, 0);

		al_draw_scaled_bitmap(startEndImage, 0, 0, img_width_initEnd, img_height_initEnd, (920 / 2) - 230, center_y, new_img_width_initEnd, new_img_height_initEnd, 0);
		al_draw_scaled_bitmap(startEndImage, 0, 0, img_width_initEnd, img_height_initEnd, (920 / 2) + 230, center_y, -new_img_width_initEnd, new_img_height_initEnd, 0);

		al_destroy_bitmap(startEndImage);
		al_destroy_bitmap(nextReturnImage);

	}
	
	draw_musics_list(num_musics, musics);

	int musicPlaying = 0;
	int threadPaused = 0;

	ThreadArguments* arguments = create_thread(musics[0]);
	Position* mousePosition = create_position(0, 0);
	ALLEGRO_THREAD* sound_thread = NULL;
	ALLEGRO_AUDIO_STREAM* audioStream = al_load_audio_stream(arguments->filepath, 4, 2048);
	ALLEGRO_FONT* font_timer = al_load_font(MONTSERRAT_BOLD, 14, 0);
	Position* pos_timer = create_position(find_screen_center(display, font_timer, "444:444"), 420);

	while (1) {
		if (musicPlaying) {
			draw_music_timer(arguments->seconds, audioStream, font_timer, pos_timer);
		}
		
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		mousePosition->x = ev.mouse.x;
		mousePosition->y = ev.mouse.y;
	
		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && !musicPlaying) {
			sound_thread = al_create_thread(play_sound, arguments);
			al_start_thread(sound_thread);
			
			printf("start\n");
			arguments->done = 0;
			musicPlaying = !musicPlaying;

			continue;
		}

		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && musicPlaying) {
			printf("stop \n");
			
			arguments->done = 1;
			musicPlaying = !musicPlaying;
			
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