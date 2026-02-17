#ifndef BENCHMARKS_H
#define BENCHMARKS_H

#include <stdbool.h>

extern char *iterations;
extern int loops;
extern bool exists_dotnet, exists_dmd, exists_go, exists_opam, exists_java;
extern char *exists_java_compiler;

void build_benchmark(char *args[], const char *path, const char *name);
void run_benchmark(char *bin_name, const char *dir, const char *name, double *benchtime);

void build_benchmarks();
void run_benchmarks(
	double *cs_benchtime, 
	double *fs_benchtime, 
	double *d_benchtime, 
	double *go_benchtime, 
	double *ocaml_benchtime, 
	double *java_benchtime
);

#endif
