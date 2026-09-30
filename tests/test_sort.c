#include <stdio.h>
#include <stdlib.h>
#include "../src/sort.h"

Stats g;
static int fails = 0, checks = 0;
static int cmpKey(const void *x, const void *y) {
    int a = ((const Rec *)x)->key, b = ((const Rec *)y)->key;
    return (a > b) - (a < b);
}
static void expect(int ok, const char *what, int n) {
    checks++;
    if (!ok) { fails++; printf("FAIL %s n=%d\n", what, n); }
}

int main(void) {
    void (*fn[])(Rec *, int) = {quickSort, mergeSort, combSort};
    const char *name[] = {"quickSort", "mergeSort", "combSort"};
    for (int f = 0; f < 3; f++)
        for (int n = 0; n <= 300; n++)
            for (int mode = 0; mode < 4; mode++) {            /* 무작위 · 정렬됨 · 역순 · 모두 같은 값 */
                Rec *a = malloc((n + 1) * sizeof *a), *b = malloc((n + 1) * sizeof *b);
                for (int i = 0; i < n; i++) {
                    a[i].key = mode == 0 ? rand() % 50 : mode == 1 ? i : mode == 2 ? n - i : 7;
                    a[i].tag = i;
                    b[i] = a[i];
                }
                fn[f](a, n);
                qsort(b, n, sizeof *b, cmpKey);
                int ok = 1;
                for (int i = 0; i < n; i++) if (a[i].key != b[i].key) ok = 0;
                expect(ok, name[f], n);
                if (f == 1) {                                   /* mergeSort는 안정이어야 한다 */
                    int st = 1;
                    for (int i = 0; i + 1 < n; i++)
                        if (a[i].key == a[i + 1].key && a[i].tag > a[i + 1].tag) st = 0;
                    expect(st, "mergeSort stable", n);
                }
                free(a); free(b);
            }
    printf("%d checks, %d failures\n", checks, fails);
    return fails != 0;
}
