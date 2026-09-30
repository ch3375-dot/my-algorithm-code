#include "sort.h"

/* Hoare 분할 + 가운데 pivot. 작은 쪽만 재귀해서 깊이를 O(log n)으로 묶는다 */
static void qs(Rec *a, int lo, int hi, int d) {
    while (lo < hi) {
        if (d > g.maxDepth) g.maxDepth = d;
        Rec p = a[lo + (hi - lo) / 2];
        int i = lo, j = hi;
        while (i <= j) {
            while (LT(a[i], p)) i++;
            while (LT(p, a[j])) j--;
            if (i <= j) { SWAP(a[i], a[j]); i++; j--; }
        }
        if (j - lo < hi - i) { qs(a, lo, j, d + 1); lo = i; }
        else                 { qs(a, i, hi, d + 1); hi = j; }
    }
}

void quickSort(Rec *a, int n) { qs(a, 0, n - 1, 1); }
