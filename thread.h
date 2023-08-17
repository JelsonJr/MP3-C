#pragma once

#include "display.h"

typedef struct {
	const char* filepath;
	Display* display;
} ThreadArguments;

