#include "sort.h"

/* 버블 정렬은 이웃(간격 1)만 비교한다. 콤 정렬은 큰 간격에서 시작해
   간격을 1.3배씩 줄이며 같은 일을 한다. 끝에 간격 1이 되면 버블 정렬이다. */
void combSort(Rec *a, int n) {
    int gap = n, swapped = 1;
    while (gap > 1 || swapped) {
        gap = gap * 10 / 13;                       /* 줄임 비율 1.3 */
        if (gap == 9 || gap == 10) gap = 11;       /* rule of 11 */
        if (gap < 1) gap = 1;
        swapped = 0;
        for (int i = 0; i + gap < n; i++)
            if (LT(a[i + gap], a[i])) { SWAP(a[i], a[i + gap]); swapped = 1; }
    }
}
