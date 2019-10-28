// testTextbuffer.c
// A stub file for you to test your textbuffer implementation.
// Note that you will NOT be submitting this - this is simply for you to
// test  your  functions as you go. We will have our own testTexbuffer.c
// for testing your submission.

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "textbuffer.h"



void testFormRichText(void);
// TODO: Add more function prototypes


int main(void)
{


    testFormRichText();
    printf("All tests passed! You are awesome!\n");
}





void testFormRichText(void)
{
    printf("-----------------------------------------\n"
           "               formRichText tests               \n"
           "-----------------------------------------\n");
    TB tb1 = newTB("*_ll_*o there,\nhow\nare\nthings\na\nb\nc\nare are are\ne\nf\n");
    formRichText(tb1);
    printf("%s", dumpTB(tb1,false));
}
