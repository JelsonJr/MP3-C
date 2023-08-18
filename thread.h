#pragma once

#include "display.h"
#ifdef _WIN32
	#include <io.h>
	#include <Windows.h>
#else
	#include <unistd.h>
#endif

typedef struct {
	const char* filepath;
	int* done;
	Display* display;
} ThreadArguments;

ThreadArguments* create_thread(Display* display, const char* string);
