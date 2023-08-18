#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <dirent.h>
#include "display.h"
#include "position.h"
#include "draw.h"
#include "fonts.h"
#include "thread.h"


#ifdef _WIN32
#include <io.h>
#include <Windows.h>
#define getcwd _getcwd
#else
#include <unistd.h>
#endif

char** list_files_directory(int* num_files);
const char* get_path_user();
DWORD WINAPI play_sound(LPVOID arg);
