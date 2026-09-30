#ifndef SORT_H
#define SORT_H
#include <stddef.h>

/* key로 정렬하고, tag에는 입력 순서를 새겨 안정성을 잰다 */
typedef struct { int key; int tag; } Rec;

typedef struct {
    unsigned long long cmp;   /* 비교 횟수 */
    unsigned long long mov;   /* 이동(대입) 횟수. 교환 1회 = 3 */
    int depth, maxDepth;      /* 재귀 깊이 */
    size_t extra;             /* 입력 배열 밖에 잡은 메모리(바이트) */
} Stats;

extern Stats g;
#define LT(x, y) (g.cmp++, (x).key < (y).key)
#define SWAP(x, y) do { Rec _t = (x); (x) = (y); (y) = _t; g.mov += 3; } while (0)

void quickSort(Rec *a, int n);
void mergeSort(Rec *a, int n);
void combSort(Rec *a, int n);

#endif
