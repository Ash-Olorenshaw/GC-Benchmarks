#if 0 
    gcc "$0" ./scripts/utils.c -o ./runbench
	./runbench
    rm -f ./runbench
    exit
#endif

#include "scripts/utils.h"
#include <stdio.h>
#include <unistd.h>

#define DOTNET_ARGS "-c", "Release", "-r", "linux-x64",\
	"--self-contained", "-p:PublishAot=true", "-p:StackTraceSupport=false",\
	"-p:TrimMode=full", "-p:OptimizationPreference=Speed"

#define DMD_ARGS "-O", \
	"-release", "-inline", "-boundscheck=off", "-mcpu=native"

#define GO_ARGS "-ldflags=-s -w", "-gcflags=-l=4 -B -C"
#define OCAML_ARGS "--profile", "release"

#define ITERATIONS "1000"

bool exists_dotnet = false,
	 exists_dmd = false,
	 exists_go = false,
	 exists_opam = false;

void build_benchmark(char *args[], const char *path, const char *name) {
	printf("\nBuilding %s benchmark\n", name);
	run_command(args, path);
	printf("\tDone.\n");
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

void run_benchmark(char *bin_name, const char *dir, const char *name, double *benchtime) {
	char *run_args[] = { bin_name, ITERATIONS, NULL };
	printf("\nRunning %s benchmark\n", name);
	*benchtime = run_command(run_args, dir);
	printf("\tDone.\n");
	chdir("..");
}

void run_benchmarks() {
	double cs_benchtime, fs_benchtime, d_benchtime, go_benchtime, ocaml_benchtime;
	if (exists_dmd) {
		run_benchmark("./bench", "./D", "D", &d_benchtime);
	}

	if (exists_go) {
		run_benchmark("./bench", "./Go", "Go", &go_benchtime);
	}

	if (exists_dotnet) {
		run_benchmark("./CSharp", "./CSharp/bin/Release/net10.0/linux-x64", "C#", &cs_benchtime);
		chdir("../../../..");
		run_benchmark("./FSharp", "./FSharp/bin/Release/net10.0/linux-x64", "F#", &fs_benchtime);
		chdir("../../../..");
	}

	if (exists_opam) {
		run_benchmark("./_build/default/bin/main.exe", "./OCaml", "OCaml", &ocaml_benchtime);
	}

	printf("\n");
	printf("D bench time: %fs\n", d_benchtime);
	printf("Go bench time: %fs\n", go_benchtime);
	printf("C# bench time: %fs\n", cs_benchtime);
	printf("F# bench time: %fs\n", fs_benchtime);
	printf("OCaml bench time: %fs\n", ocaml_benchtime);
	printf("Benchmarks finished with '%s' iterations each", ITERATIONS);
}

int main(int argv, const char **argc) {
	exists_dotnet = command_exists("dotnet");
	exists_dmd = command_exists("dmd");
	exists_go = command_exists("go");
	exists_opam = command_exists("opam");
	
	build_benchmarks();
	run_benchmarks();
}
