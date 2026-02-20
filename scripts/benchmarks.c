#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "./utils.h"

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
	 exists_opam = false,
	 exists_java = false;
char *exists_java_compiler = NULL;

void build_benchmark(char *args[], const char *path, const char *name) {
	printf(">>> Building %s benchmark.\n", name);
	run_command(args, path, true);
	chdir("..");
}

void run_benchmark(char *bin_name, const char *dir, const char *name, double *benchtime) {
	// if (loops < 3)
	printf(" : Running %s benchmark.\n", name);
	char *run_args[] = { bin_name, iterations, NULL };
	*benchtime = run_command(run_args, dir, false);
	printf(" : \n");
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

	if (exists_java) {
		char *build_args[] = { "javac", "Benchmark.java", NULL };
		build_benchmark(build_args, "./Java", "Java");
		if (exists_java_compiler != NULL) {
			char *native_compiler = realpath(exists_java_compiler, NULL);
			char *compiler_args_3[] = { native_compiler, "Benchmark", NULL };
			run_command(compiler_args_3, "./Java", true);
			chdir("..");
			free(native_compiler);
		}
	}
	else 
		printf("\n\nCommand 'javac' or 'java' does not exist.\nSkipping Java benchmark.\n\n");

}

void run_benchmarks(double *cs_benchtime, double *fs_benchtime, double *d_benchtime, double *go_benchtime, double *ocaml_benchtime, double *java_benchtime) {
	if (exists_dmd)
		run_benchmark("./bench", "./D", "D", d_benchtime);

	if (exists_go)
		run_benchmark("./bench", "./Go", "Go", go_benchtime);

	if (exists_dotnet) {
		run_benchmark("./CSharp", "./CSharp/bin/Release/net10.0/linux-x64", "C#", cs_benchtime);
		chdir("../../../..");
		run_benchmark("./FSharp", "./FSharp/bin/Release/net10.0/linux-x64", "F#", fs_benchtime);
		chdir("../../../..");
	}

	if (exists_opam)
		run_benchmark("./_build/default/bin/main.exe", "./OCaml", "OCaml", ocaml_benchtime);

	if (exists_java) {
		if (exists_java_compiler == NULL) {
			char *build_args[] = { "java", "Benchmark", iterations, NULL };
			// if (loops < 3)
			printf(" : Running Java benchmark.\n");
			*java_benchtime = run_command(build_args, "./Java", false);
			printf(" : \n");
			chdir("..");
		}
		else
			run_benchmark("./benchmark", "./Java", "Java", java_benchtime);
	}
}
