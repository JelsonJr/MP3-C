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
	int seconds;
} ThreadArguments;

ThreadArguments* create_thread(const char* string);
