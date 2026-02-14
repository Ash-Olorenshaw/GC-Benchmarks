#if 0 
    gcc "$0" ./scripts/utils.c ./scripts/args.c -o ./runbench
	./runbench "$@"
    rm -f ./runbench
    exit
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "scripts/utils.h"
#include "scripts/args.h"

#define DOTNET_ARGS "-c", "Release", "-r", "linux-x64",\
	"--self-contained", "-p:PublishAot=true", "-p:StackTraceSupport=false",\
	"-p:TrimMode=full", "-p:OptimizationPreference=Speed"

#define DMD_ARGS "-O", \
	"-release", "-inline", "-boundscheck=off", "-mcpu=native"

#define GO_ARGS "-ldflags=-s -w", "-gcflags=-l=4 -B -C"
#define OCAML_ARGS "--profile", "release"

char *iterations;
int loops;


bool exists_dotnet = false,
	 exists_dmd = false,
	 exists_go = false,
	 exists_opam = false;

void build_benchmark(char *args[], const char *path, const char *name) {
	printf(">>> Building %s benchmark.\n", name);
	run_command(args, path);
	chdir("..");
}

void run_benchmark(char *bin_name, const char *dir, const char *name, double *benchtime) {
	char *run_args[] = { bin_name, iterations, NULL };
	*benchtime = run_command(run_args, dir);
	chdir("..");
}

void build_benchmarks() {
	if (exists_dmd) {
		char *build_args[] = { "dmd", "main.d", "-of=bench", DMD_ARGS, NULL };
		build_benchmark(build_args, "./D", "D");
	}
	else
		printf("\n\nCommand 'dmd' does not exist.\nSkipping D benchmark.\n\n");

	if (exists_go) {
		char *build_args[] = { "go", "build", "-o", "bench", GO_ARGS, NULL };
		build_benchmark(build_args, "./Go", "Go");
	}
	else
		printf("\n\nCommand 'go' does not exist.\nSkipping Go benchmark.\n\n");

	if (exists_dotnet) {
		char *build_args[] = { "dotnet", "build", DOTNET_ARGS, NULL };
		build_benchmark(build_args, "./CSharp", "C#");
		build_benchmark(build_args, "./FSharp", "F#");
	}
	else
		printf("\n\nCommand 'dotnet' does not exist.\nSkipping C# and F# benchmarks.\n\n");

	if (exists_opam) {
		char *build_args[] = { "opam", "exec", "--", "dune", "build", OCAML_ARGS, NULL };
		build_benchmark(build_args, "./OCaml", "OCaml");
	}
	else
		printf("\n\nCommand 'opam' does not exist.\nSkipping OCaml benchmark.\n\n");

}

void run_benchmarks(double *cs_benchtime, double *fs_benchtime, double *d_benchtime, double *go_benchtime, double *ocaml_benchtime) {
	if (exists_dmd) {
		run_benchmark("./bench", "./D", "D", d_benchtime);
	}

	if (exists_go) {
		run_benchmark("./bench", "./Go", "Go", go_benchtime);
	}

	if (exists_dotnet) {
		run_benchmark("./CSharp", "./CSharp/bin/Release/net10.0/linux-x64", "C#", cs_benchtime);
		chdir("../../../..");
		run_benchmark("./FSharp", "./FSharp/bin/Release/net10.0/linux-x64", "F#", fs_benchtime);
		chdir("../../../..");
	}

	if (exists_opam) {
		run_benchmark("./_build/default/bin/main.exe", "./OCaml", "OCaml", ocaml_benchtime);
	}

}

int compare_doubles(const void * a, const void * b) {
    if (*(double*)a > *(double*)b) return 1;
    else if (*(double*)a < *(double*)b) return -1;
    else return 0;
}

int main(int argv, const char **argc) {
	const int benchtime_count = 5;
	double final_benchtimes[5] = {0};
	const char *final_benchtime_names[5] = { "C#", "F#", "D", "Go", "OCaml" };

	exists_dotnet = command_exists("dotnet");
	exists_dmd = command_exists("dmd");
	exists_go = command_exists("go");
	exists_opam = command_exists("opam");
	
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

	printf(">>> Running benchmark %d times, with each benchmark running %s iterations\n", loops, iterations);
	build_benchmarks();

	double benchtimes[5] = {0};
	for (int i = 0; i < loops; i++) {
		printf(">>> Running benchmark loop %d\n", i + 1);
		memset(benchtimes, 0, sizeof benchtimes);
		run_benchmarks(&benchtimes[0], &benchtimes[1], &benchtimes[2], &benchtimes[3], &benchtimes[4]);
		for (int b = 0; b < benchtime_count; b++)
			final_benchtimes[b] += benchtimes[b];
	}

	for (int b = 0; b < benchtime_count; b++)
		final_benchtimes[b] /= loops;

	qsort(final_benchtimes, benchtime_count, sizeof(double), compare_doubles);

	printf("\nAVERAGE TIMES (sorted fastest first):\n");
	for (int i = 0; i < benchtime_count; i++) {
		printf("%d. %s (time: %fs)\n", i, final_benchtime_names[i], final_benchtimes[i]);
	}
	printf("Benchmarks finished with %d loops and with %s iterations each\n", loops, iterations);

	free(iterations);

	return 0;
}
