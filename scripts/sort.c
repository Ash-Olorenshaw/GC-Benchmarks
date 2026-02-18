#include "./benchmarks.h"

int compare_doubles(const void * a, const void * b) {
    if (*(double*)a > *(double*)b) return 1;
    else if (*(double*)a < *(double*)b) return -1;
    else return 0;
}

int compare_benchmarks(const void * a, const void * b) {
    if ((*(benchmark*)a).time > (*(benchmark*)b).time) return 1;
    else if ((*(benchmark*)a).time < (*(benchmark*)b).time) return -1;
    else return 0;
}

