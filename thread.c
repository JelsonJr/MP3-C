#include "thread.h"

ThreadArguments* create_thread(const char* string) {
	ThreadArguments* thread = malloc((sizeof(ThreadArguments*)));

	if (!thread) {
		free(thread);
		return NULL;
	}

	thread->filepath = string;
	thread->done = 0;
	thread->seconds = 0;

	return thread;
}

void destroy_threadArguments(ThreadArguments* arguments) {
	free(arguments);
}