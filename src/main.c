#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sort.h"

Stats g;

typedef struct { const char *name; void (*fn)(Rec *, int); } Algo;
static const Algo ALGOS[] = {
    {"quickSort", quickSort}, {"mergeSort", mergeSort}, {"combSort", combSort},
};
#define NALGO (int)(sizeof ALGOS / sizeof ALGOS[0])

enum { RANDOM, SORTED, REVERSED, DUPS, NSHAPE };
static const char *SHAPE[] = {"random", "sorted", "reversed", "dups"};

static unsigned long long rs = 88172645463325252ULL;   /* 고정 씨앗 xorshift */
static unsigned rnd(void) { rs ^= rs << 13; rs ^= rs >> 7; rs ^= rs << 17; return (unsigned)(rs >> 11); }

static void makeInput(Rec *a, int n, int shape) {
    rs = 88172645463325252ULL;
    for (int i = 0; i < n; i++) {
        int k;
        switch (shape) {
        case SORTED:   k = i; break;
        case REVERSED: k = n - i; break;
        case DUPS:     k = (int)(rnd() % 16); break;
        default:       k = (int)(rnd() % (unsigned)(n * 10)); break;
        }
        a[i].key = k; a[i].tag = i;
    }
}

static double nowMs(void) {
    struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1e3 + t.tv_nsec / 1e6;
}

/* 정렬 여부와 안정성(같은 key끼리 tag 오름차순인지)을 검사 */
static void check(const Rec *a, int n, int *sorted, int *stable) {
    *sorted = 1; *stable = 1;
    for (int i = 0; i + 1 < n; i++) {
        if (a[i].key > a[i + 1].key) *sorted = 0;
        if (a[i].key == a[i + 1].key && a[i].tag > a[i + 1].tag) *stable = 0;
    }
}

typedef struct { double ms; Stats s; int sorted, stable; } Result;

static Result run(const Algo *al, const Rec *in, int n, int reps) {
    Rec *w = malloc((size_t)n * sizeof *w);
    Result r = {0};
    for (int k = 0; k < reps; k++) {
        memcpy(w, in, (size_t)n * sizeof *w);
        memset(&g, 0, sizeof g);
        double t0 = nowMs();
        al->fn(w, n);
        r.ms += nowMs() - t0;
        r.s = g;
    }
    r.ms /= reps;
    check(w, n, &r.sorted, &r.stable);
    free(w);
    return r;
}

static void row(int csv, const char *shape, int n, const Algo *al, Result r) {
    if (!r.sorted) { fprintf(stderr, "FAIL: %s not sorted\n", al->name); exit(1); }
    if (csv)
        printf("%s,%d,%s,%.4f,%llu,%llu,%d,%zu,%d\n", shape, n, al->name, r.ms,
               r.s.cmp, r.s.mov, r.s.maxDepth, r.s.extra, r.stable);
    else
        printf("%-9s %7d %-10s %9.3f %12llu %12llu %5d %9zu %s\n", shape, n, al->name,
               r.ms, r.s.cmp, r.s.mov, r.s.maxDepth, r.s.extra, r.stable ? "stable" : "unstable");
}

int main(int argc, char **argv) {
    int csv = argc > 1 && !strcmp(argv[1], "--csv");
    if (csv) puts("shape,n,algo,ms,compares,moves,depth,extra_bytes,stable");
    else printf("%-9s %7s %-10s %9s %12s %12s %5s %9s %s\n", "input", "n", "algo",
                "ms", "compares", "moves", "depth", "extra(B)", "stability");

    int N = 20000, reps = 5;
    Rec *in = malloc((size_t)N * sizeof *in);
    for (int s = 0; s < NSHAPE; s++) {                    /* 입력 모양별 */
        makeInput(in, N, s);
        for (int a = 0; a < NALGO; a++) row(csv, SHAPE[s], N, &ALGOS[a], run(&ALGOS[a], in, N, reps));
    }
    int sizes[] = {1000, 2000, 4000, 8000, 16000, 32000, 64000};
    for (int i = 0; i < 7; i++) {                         /* n을 키우며 (random) */
        int n = sizes[i];
        Rec *b = malloc((size_t)n * sizeof *b);
        makeInput(b, n, RANDOM);
        for (int a = 0; a < NALGO; a++) row(csv, "grow", n, &ALGOS[a], run(&ALGOS[a], b, n, reps));
        free(b);
    }
    free(in);
    return 0;
}
