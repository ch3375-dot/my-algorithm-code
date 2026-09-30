#include <stdlib.h>
#include "sort.h"

/* 하향식 병합. 같으면 왼쪽을 먼저 꺼내므로 안정 정렬이다 */
static void ms(Rec *a, Rec *t, int lo, int hi, int d) {   /* [lo, hi) */
    if (hi - lo < 2) return;
    if (d > g.maxDepth) g.maxDepth = d;
    int mid = lo + (hi - lo) / 2;
    ms(a, t, lo, mid, d + 1);
    ms(a, t, mid, hi, d + 1);
    int i = lo, j = mid, k = lo;
    while (i < mid && j < hi) {
        if (LT(a[j], a[i])) t[k++] = a[j++];   /* '<' 이므로 같으면 왼쪽 */
        else                t[k++] = a[i++];
        g.mov++;
    }
    while (i < mid) { t[k++] = a[i++]; g.mov++; }
    while (j < hi)  { t[k++] = a[j++]; g.mov++; }
    for (k = lo; k < hi; k++) { a[k] = t[k]; g.mov++; }
}

void mergeSort(Rec *a, int n) {
    if (n < 2) return;
    Rec *t = malloc((size_t)n * sizeof *t);
    g.extra = (size_t)n * sizeof *t;
    ms(a, t, 0, n, 1);
    free(t);
}
