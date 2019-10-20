// testList.c - testing DLList data type
// Written by John Shepherd, March 2013
// Last modified by Jingming Yang , Sep 2019
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "DLList.h"

void test(DLList myList, int chooseFunc);
DLList genList(int chooseStates);
void displayDLListState(DLList myList);

int main(void)
{
    // functions each tests 7 different lists(empty, one node, two nodes and curr point to the first...)
    char *funcName[] = {"DLListBefore", "DLListAfter", "DLListDelete"};
    for (int chooseFunc = 0; chooseFunc < 3; chooseFunc++)
    {
        printf("\n----test %s function----\n", funcName[chooseFunc]);
        for (int chooseStates = 0; chooseStates < 7; chooseStates++)
        {
            DLList myList = genList(chooseStates);
            printf("\n----State %d----\n", chooseStates);
            test(myList, chooseFunc);
            freeDLList(myList);
        }
    }

    return EXIT_SUCCESS;
}

/**choose test fuction**/
void test(DLList myList, int chooseFunc)
{

    printf("state before:\n");
    displayDLListState(myList);

    if (chooseFunc == 0)
    {
        DLListBefore(myList, "insert node");
    }
    else if (chooseFunc == 1)
    {
        DLListAfter(myList, "insert node");
    }
    else if (chooseFunc == 2)
    {
        DLListDelete(myList);
    }
    printf("state after:\n");
    displayDLListState(myList);
    printf(validDLList(myList) ? "Valid\n" : "Disvalid\n");
}

/**generate different test list**/
DLList genList(int chooseStates)
{
    switch (chooseStates)
    {
    case 0:
    {
        //curr points to null, nitems = 0
        return newDLList();
    }
    case 1:
    {
        //curr points to first, nitems = 1
        DLList myList = newDLList();
        DLListAfter(myList, "first line");
        return myList;
    }
    case 2:
    {
        //curr points to first, nitems = 2
        DLList myList = genList(1);
        DLListAfter(myList, "second line");
        DLListMoveTo(myList, 1);
        return myList;
    }
    case 3:
    {
        //curr points to last, nitems = 2
        DLList myList = genList(2);
        DLListMove(myList, 1);
        return myList;
    }
    case 4:
    {
        //curr points to last, nitems = 3
        DLList myList = genList(1);
        DLListAfter(myList, "second line");
        DLListAfter(myList, "third line");
        DLListMoveTo(myList, 1);
        return myList;
    }
    case 5:
    {
        //curr points to middle, nitems = 3
        DLList myList = genList(4);
        DLListMove(myList, 1);
        return myList;
    }
    case 6:
    {
        //curr points to last, nitems = 3
        DLList myList = genList(5);
        DLListMove(myList, 1);
        return myList;
    }
    }
    return NULL;
}

/**display DLLiSt state**/
void displayDLListState(DLList myList)
{
    printf("cuur: \"%s\"\n", DLListCurrent(myList));
    printf("nitems: %zu\n", DLListLength(myList));
    printf("show all line:\n");
    putDLList(stdout, myList);
    printf("--EOF--\n");
}
