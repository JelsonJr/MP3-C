#include "music_menu.h"

int music_menu(Display* display, int num_musics, char** musics) {
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

		int text_height = 14;
		int space_between_music = 10;
		ALLEGRO_FONT* font = al_load_font(MONTSERRAT_BOLD, text_height, 0);

		for (int i = 0; i < num_musics; i++) {
			const char* music = musics[i];
			const char* music_name = strrchr(music, '\\') + 1;

			Position* pos = create_position(320, i * (text_height + space_between_music));
			al_draw_textf(font, al_map_rgb(250, 250, 250), pos->x, pos->y, ALLEGRO_ALIGN_LEFT, "%s", music_name);

			destroy_position(pos);
		}

		al_flip_display();
	}

	return 1;
}