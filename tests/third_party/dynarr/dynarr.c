#define DYNARR_STATIC //static version
#include "dynarr.h" //include dynarr
#include <stdio.h> //printf

static int sortInt(const int*, const int*);

int main () {
    //declare and initialize dynarr
    int* darr = DYNARR_NEW(int);
    //push a couple of elements
    DYNARR_PUSH(darr, 4);
    DYNARR_PUSH(darr, 1);
    DYNARR_PUSH(darr, 3);
    DYNARR_PUSH(darr, 2);
    //sort dynarr in ascending order
    DYNARR_SORT_STD(darr, sortInt);
    //iterate through the dynarr and print its elements
    for (int i = 0; i < DYNARR_SIZE(darr); i++)
        printf("darr[%d] = %d\n", i, DYNARR_AT(darr, i));
    //free the dynarr
    DYNARR_FREE(darr);
    //return
    return 0;
}

static int sortInt (const int* a, const int* b) {
    return *a-*b; //ascending order
}
