#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Point {
    int x, y, z;
};
typedef struct Point P;

struct Rect {
    P left_top, right_bottom;
};

typedef struct Rect R;

void setRandPoint(P* p) {
    p->x = rand() % 10;
    p->y = rand() % 10;
    p->z = rand() % 10;
}

void setRandRect(R* r) {
    setRandPoint(&r->left_top);
    setRandPoint(&r->right_bottom);
}

void printPoint(P* p) {
    printf("(%d, %d, %d) ", p->x, p->y, p->z);
}

void printRect(R* r) {
    printPoint(&r->left_top);
    printf("-> ");
    printPoint(&r->right_bottom);
    printf("\n");
}

typedef void(*Function)(R*);
void map(Function f, R rs[], int n) {
    for(int i = 0; i != n; ++i) {
        f(rs + i);
    }
}

void mapSeq(Function fs[], int nf, R* rs, int n) {
    for(int i = 0; i != nf; ++i) {
        map(fs[i], rs, n);
    }
}

int LX(const R* a, const R* b) {
    int ax = a->left_top.x, bx = b->left_top.x;
    return ax < bx ? -1 : ax > bx ? 1 : 0;
}
int LY(const R* a, const R* b) {
    int ay = a->left_top.y, by = b->left_top.y;
    return ay < by ? -1 : ay > by ? 1 : 0;
}
int LZ(const R* a, const R* b) {
    int az = a->left_top.z, bz = b->left_top.z;
    return az < bz ? -1 : az > bz ? 1 : 0;
}

int RX(const R* a, const R* b) {
    int ax = a->right_bottom.x, bx = b->right_bottom.x;
    return ax < bx ? -1 : ax > bx ? 1 : 0;
}
int RY(const R* a, const R* b) {
    int ay = a->right_bottom.y, by = b->right_bottom.y;
    return ay < by ? -1 : ay > by ? 1 : 0;
}
int RZ(const R* a, const R* b) {
    int az = a->right_bottom.z, bz = b->right_bottom.z;
    return az < bz ? -1 : az > bz ? 1 : 0;
}

typedef int(*Comparator)(const R*, const R*);
struct SortSeq_impl {
    int n;
    Comparator* sorts;
};
typedef struct SortSeq_impl SortSeq;



int getOffset(char c) {
    return c == 'L' ? 0 : c == 'R' ? 3 : c - 'X';
}

SortSeq parseSorts(char sort_choise[], int len) {
    SortSeq sort_seq;

    sort_seq.sorts = (Comparator*) malloc((sort_seq.n = len/2) * sizeof(Function*));
    Comparator sorts_variant[] = { LX, LY, LZ, RX, RY, RZ };

    //  012345
    // "LXLYLZ"
    for(int i = 0; i != sort_seq.n; ++i) {
        int f_idx = getOffset(sort_choise[i*2]) + getOffset(sort_choise[i*2 + 1]);
        sort_seq.sorts[i] = sorts_variant[f_idx];
    }

    return sort_seq;
}

SortSeq CCS_sort_seq;
int compCompositeSort(const void* av, const void* bv) {
    const R *a = av, *b = bv;
    int res = 0;
    for(int i = 0; i != CCS_sort_seq.n && res == 0; ++i) {
            res = CCS_sort_seq.sorts[i](a, b);
    }
    return res;
}


int main() {
    int n = 50;
    R* rs = (R*) malloc(sizeof(R)*n);
    Function permuts[] = { setRandRect, printRect };
    mapSeq(permuts, sizeof(permuts)/sizeof(permuts[0]), rs, n);

    printf("По чём сортировать будем, браток? ([L|R][X|Y|Z]): ");
    char sort_choise[512] = { '\0' };
    scanf("%s", sort_choise);
    CCS_sort_seq = parseSorts(sort_choise, strlen(sort_choise));

    qsort(rs, n, sizeof(rs[0]), compCompositeSort);

    printf("\n");
    map(printRect, rs, n);
}


