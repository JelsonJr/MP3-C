#include "music.h"

void* play_sound(ALLEGRO_THREAD* thread, void* arg) {
	ThreadArguments* arguments = (ThreadArguments*)arg;

	if (!al_reserve_samples(1)) {
		return;
	}

	ALLEGRO_SAMPLE* audioSample = al_load_sample(arguments->filepath);
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

	while (al_get_sample_instance_playing(instance) && !arguments->done) {
		al_rest(1);
		arguments->seconds++;
	}
	
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
