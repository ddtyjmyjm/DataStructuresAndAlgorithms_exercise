// testList.c - testing DLList data type
// Written by John Shepherd, March 2013

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "DLList.h"

bool test(DLList myList);

void displayDLLiState(DLList myList);

int main(void) {
    DLList myList = getDLList(stdin);
    putDLList(stdout, myList);
    assert(validDLList(myList));

    // TODO: more tests needed here

    freeDLList(myList);
    return EXIT_SUCCESS;
}

bool test(DLList myList) {
    printf("----test DLListBefore function----\n");
    printf("state before:\n");
    displayDLLiState(myList);
    DLListMoveTo(myList, 1);
    DLListBefore(myList, "first node\n");
    DLListMove(myList, 1);
    DLListBefore(myList, "middle node\n");
    DLListMoveTo(myList, DLListLength(myList));
    DLListBefore(myList, "last node\n");
    printf("state after:\n");
    displayDLLiState(myList);
    printf(validDLList(myList) ? "Valid\n" : "Disvalid\n")

}

void displayDLLiState(DLList myList) {
    printf("cuur: %s\n", DLListCurrent(myList));
    printf("nitems: %s\n ", DLListLength(myList));
    printf("show all line:\n");
    putDLList(stdout, myList);
    printf("----");
}