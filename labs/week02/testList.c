// testList.c - testing DLList data type
// Written by John Shepherd, March 2013

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "DLList.h"

void test(DLList myList, int chooseFunc);
void genList(DLList myList, int chooseStates);
void displayDLLiState(DLList myList);

int main(void) {
    DLList myList = getDLList(stdin);
    putDLList(stdout, myList);
    assert(validDLList(myList));

    // TODO: more tests needed here
    for (int chooseFunc; chooseFunc < 3; chooseFunc++) {
        for (int chooseStates = 0; chooseStates < 7; chooseStates++) {
            genList(myList, chooseStates);
            test(myList,chooseFunc);
            freeDLList(myList);
        }
    }


    freeDLList(myList);
    return EXIT_SUCCESS;
}

void test(DLList myList, int chooseFunc) {
    char **funcName =["DLListBefore", "DLListAfter", "DLListDelete"];
    printf("----test %pF function----\n", *funcName[chooseFunc]);
    if (chooseFunc == 0) {
        DLListBefore(myList, "insert node\n");
    } else if (chooseFunc == 1) {
        DLListAfter(myList, "insert node\n");
    } else if (chooseFunc == 2) {
        DLListDelete(myList);
    }
    printf("state after:\n");
    displayDLLiState(myList);
    printf(validDLList(myList) ? "Valid\n" : "Disvalid\n")
}

void genList(DLList myList, int chooseStates) {
    switch (chooseStates) {
        case 0: {
            //curr points to null, nitems = 0
            myList = newDLList();
            break;
        }
        case 1: {
            //curr points to first, nitems = 1
            myList = newDLList();
            getDLList("one line\n");
            break;
        }
    }
}



void displayDLLiState(DLList myList) {
    printf("cuur: %s\n", DLListCurrent(myList));
    printf("nitems: %s\n ", DLListLength(myList));
    printf("show all line:\n");
    putDLList(stdout, myList);
    printf("----");
}