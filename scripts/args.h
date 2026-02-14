#ifndef ARGS_H
#define ARGS_H

typedef struct {
	int arg_count;
	const char **args;
} args;

int arg_pos(const char *arg, args arguments);

#endif
