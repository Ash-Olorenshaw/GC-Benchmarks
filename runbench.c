#if 0 
    gcc "$0" ./scripts/utils.c ./scripts/args.c \
		./scripts/benchmarks.c ./scripts/sort.c -o ./runbench \
		-Wall -Wextra -pedantic
	./runbench "$@"
    rm -f ./runbench
    exit
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "scripts/sort.h"
#include "scripts/benchmarks.h"
#include "scripts/args.h"
#include "scripts/utils.h"

int main(int argv, const char **argc) {
	const int benchtime_count = 6;
	benchmark final_benchtimes[6] = {0};
	final_benchtimes[0] = (benchmark){ .name = "C#", .time = 0 };
	final_benchtimes[1] = (benchmark){ .name = "F#", .time = 0 };
	final_benchtimes[2] = (benchmark){ .name = "D", .time = 0 };
	final_benchtimes[3] = (benchmark){ .name = "Go", .time = 0 };
	final_benchtimes[4] = (benchmark){ .name = "OCaml", .time = 0 };
	final_benchtimes[5] = (benchmark){ .name = "Java", .time = 0 };

	exists_dotnet = command_exists("dotnet");
	exists_dmd = command_exists("dmd");
	exists_go = command_exists("go");
	exists_opam = command_exists("opam");
	exists_java = command_exists("java") && command_exists("javac");
	
	args arguments = { .arg_count = argv, .args = argc };

	int iterations_pos = arg_pos("--iterations", arguments);
	if (iterations_pos == -1) {
		printf("Flag '--iterations' missing\n");
		exit(1);
	}
	else if (iterations_pos < argv)
		iterations = strdup(argc[iterations_pos + 1]);
	else {
		printf("Flag '--iterations' requires an integer value\n");
		exit(1);
	}

	int loop_pos = arg_pos("--loops", arguments);
	if (loop_pos == -1) {
		printf("Flag '--loops' missing\n");
		exit(1);
	}
	else if (loop_pos < argv) {
		loops = strtol(argc[loop_pos + 1], NULL, 10);
		if (loops < 1) {
			printf("Flag '--loops' requires a positive, non-zero, integer value\n");
			exit(1);
		}
	}
	else {
		printf("Flag '--loops' requires an integer value\n");
		exit(1);
	}

	int java_compiler_src_pos = arg_pos("--java-compiler", arguments);
	if (java_compiler_src_pos == -1)
		printf("Flag '--java-compiler' missing.\n"
			"Instead of using GraalVM to compile to native code, Java benchmark will\n"
			"instead compile to Java bytecode and execute using the JVM.\n\n");
	else if (java_compiler_src_pos < argv) {
		exists_java_compiler = strdup(argc[java_compiler_src_pos + 1]);
	}
	else {
		printf("Flag '--java-compiler' requires a string value\n");
		exit(1);
	}

	printf(">>> Running benchmark %d times, with each benchmark running %s iterations\n", loops, iterations);
	build_benchmarks();

	double benchtimes[6] = {0};
	for (int i = 0; i < loops; i++) {
		if (loops > 1)
			printf(">>> Running benchmark loop %d\n", i + 1);
		memset(benchtimes, 0, sizeof benchtimes);
		run_benchmarks(&benchtimes[0], &benchtimes[1], &benchtimes[2], &benchtimes[3], &benchtimes[4], &benchtimes[5]);
		for (int b = 0; b < benchtime_count; b++)
			final_benchtimes[b].time += benchtimes[b];
	}

	for (int b = 0; b < benchtime_count; b++)
		final_benchtimes[b].time /= loops;

	qsort(final_benchtimes, benchtime_count, sizeof(benchmark), compare_benchmarks);

	printf("\nAVERAGE TIMES (sorted fastest first):\n");
	for (int i = 0; i < benchtime_count; i++) {
		if (final_benchtimes[i].time == 0)
			printf("%d. %s (benchmark not run)\n", i + 1, final_benchtimes[i].name);
		else
			printf("%d. %s (time: %fs)\n", i + 1, final_benchtimes[i].name, final_benchtimes[i].time);
	}
	printf("Benchmarks finished with %d loops and with %s iterations each\n", loops, iterations);

	free(iterations);

	return 0;
}
