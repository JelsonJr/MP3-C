#include "thread.h"

ThreadArguments* create_thread(Display* display, const char* string) {
	ThreadArguments* thread = malloc((sizeof(ThreadArguments*)));

	if (!thread) {
		free(thread);
		return NULL;
	}

	thread->display = display;
	thread->filepath = string;
	thread->done = 0;

	return thread;
}
