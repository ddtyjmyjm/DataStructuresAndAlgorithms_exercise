#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "textbuffer.h"

typedef struct textLine *Line;
typedef struct chairPairQueue *Queue;

struct textbuffer
{
    int len;
    int textSize; //string length + '\0'
    Line first;
    Line last;
} textbuffer;

struct textLine
{
    char *line;
    int lineSize; // not include '\n',include '\0'
    Line next;
    Line prev;
};

/**
 * helper struct to store char
 */
struct charQueue
{
    char *node;
    struct charQueue *next;
};

/**
 * helper struct to store pair of chars
 */
struct chairPairQueue
{
    char *node1;
    char *node2;
    int type; //  1==Italic, 2==Bold
    struct chairPairQueue *next;
};

//helper function
static Line newTextLine(char *text);

static void addLine(TB tb, Line line);

static char *pureStr(int len);

static bool editValid(TB tb, int from, int to);

static void combineValid(TB tb, int pos);

static void
addChairPairQueue(Queue *first, Queue *last, char *ptr1, char *ptr2, int type)
{
    Queue newQueue = malloc(sizeof *newQueue);
    newQueue->node1 = ptr1;
    newQueue->node2 = ptr2;
    newQueue->type = type;
    newQueue->next = NULL;
    if (*first == NULL)
    {
        *first = *last = newQueue;
    }
    else
    {
        (*last)->next = newQueue;
        *last = newQueue;
    }
}

static void releaseCharPairQueue(Queue *first, Queue *last)
{
    Queue temp;
    while (*first != NULL)
    {

        if (*first == *last)
        {
            free(*first);
            break;
        }

        temp = *first;
        *first = (*first)->next;
        free(temp);
    }
    *first = *last = NULL;
}
/*
static void catCharPairQueue(Queue *first1, Queue *last1, Queue *first2, Queue *last2)
{
    if (*first1 == NULL)
    {
        *first1 = *first2;
        *last1 = *last2;
    } else
    {
        (*last1)->next = *first2;
    }
}
*/
/**
 * Allocate a new textLine whose contents is initialised with the text
 * in the given string.
 */
static Line newTextLine(char *text)
{
    int size = strlen(text) + 1;

    struct textLine *newLine = malloc(sizeof *newLine);
    char *strLine = malloc(size);

    strcpy(strLine, text);
    newLine->line = strLine;
    newLine->lineSize = size;
    newLine->prev = newLine->next = NULL;
    return newLine;
}

/**
 * add new Line to TB
 */
static void addLine(TB tb, Line line)
{
    if (tb == NULL)
        abort();
    if (tb->len == 0)
    {
        tb->first = tb->last = line;
    }
    else
    {
        tb->last->next = line;
        line->prev = tb->last;
        tb->last = line;
    }
    tb->len++;
}

/*
 *  Allocate a string and make all of it having '\000'
 */
static char *pureStr(int len)
{
    char *newStr = malloc(len);
    for (int i = 0; i < len; i++)
    {
        newStr[i] = '\000';
    }
    return newStr;
}

/**
 * Allocate a new textbuffer whose contents is initialised with the text
 * in the given string.
 */
TB newTB(char *text)
{
    struct textbuffer *tb;

    // NULL string
    if (text == NULL)
        abort();

    // Initialize tb
    tb = malloc(sizeof *tb);
    tb->len = 0;
    tb->textSize = strlen(text) + 1;
    tb->first = tb->last = NULL;

    // empty text will return empty textbuffer
    if (tb->textSize != 0)
    {
        // save a copy of text in stack which can be modified
        char tempText[tb->textSize];
        strcpy(tempText, text);


        char *ptr = tempText;
        char *ptrPre = tempText;
        while (1)
        {
            ptr = strchr(ptr, '\n');
            if (ptr == NULL)
                break;
            *ptr = '\0';

            // add new line to tb
            char strLine[ptr - ptrPre + 1];
            strcpy(strLine, ptrPre);
            Line line = newTextLine(strLine);
            addLine(tb, line);

            // update temporary pointer
            ptr++;
            ptrPre = ptr;
        }
    }

    return tb;
}

/**
 * Free  the  memory occupied by the given textbuffer. It is an error to
 * access the buffer afterwards.
 */
void releaseTB(TB tb)
{
    if (tb == NULL)
        ;
    else
    {
        Line ptr = tb->first;
        while (ptr != NULL)
        {
            Line temp = ptr;
            ptr = ptr->next;

            free(temp->line);
            free(temp);
        }
        free(tb);
    }
}

/**
 * Allocate  and return a string containing all of the text in the given
 * textbuffer. If showLineNumbers is true, add a prefix corresponding to
 * the line number.
 */
char *dumpTB(TB tb, bool showLineNumbers)
{
    // empty text buffer
    if (tb->len == 0)
        return "";

    // dumpText:     return text buffer
    // dumpTextSize: length of return text buffer
    // maxNumStrLen: the max length of one line number except '.' and ' '
    char *dumpText;
    int dumpTextSize;
    int maxNumStrLen = (int)log10(tb->len) + 1;

    // calculate the length of return text buffer
    if (!showLineNumbers)
    {
        dumpTextSize = tb->textSize;
    }
    else
    {
        // calculate the length of text with number order
        //
        // e.g  if TB has 9876 lines , it has (9876-999) of numbers show 4 words in length,
        // (999-99) of numbers show 3 words, (99-9) of numbers show 2 words, (9) of numbers show 1 words
        // In this case, 'maxNumStrLen' = 4, 'placeBase' = 1000
        int placeBase = pow(10, maxNumStrLen - 1);
        int numStrLen = maxNumStrLen;
        int addLength = (tb->len - (placeBase - 1)) * (numStrLen + 2); // addLength adds numbers*(length+2)
        while (placeBase != 0)
        {
            placeBase /= 10;
            numStrLen--;
            addLength += placeBase * 9 * (numStrLen + 2); // addLength adds numbers*(length+2)
        }
        dumpTextSize = tb->textSize + addLength;
    }

    // string catenate
    int number = 1;
    char numberStr[maxNumStrLen + 2];
    Line currLine = tb->first;
    dumpText = pureStr(dumpTextSize);
    while (currLine != NULL)
    {
        if (showLineNumbers)
        {
            sprintf(numberStr, "%d. ", number);
            strcat(dumpText, numberStr);
            number++;
        }

        strcat(dumpText, currLine->line);
        strcat(dumpText, "\n");
        currLine = currLine->next;
    }

    return dumpText;
}

/**
 * Return the number of lines of the given textbuffer.
 */
int linesTB(TB tb)
{
    return tb->len;
}

/*
 *  Processing the validation of editing
 *  -If 'from' or 'to' is out of range ,it wil show the message and absorb.
 */
static bool editValid(TB tb, int from, int to)
{
    if (from < 1 || to > tb->len)
    {
        printf("error: out of range!\n");
        abort();
    }
    if (from > to)
    {
        return false;
    }
    return true;
}

/**
 * Add a given prefix to all lines between 'from' and 'to', inclusive.
 * - The  program  should abort() wih an error message if 'from' or 'to'
 *   is out of range. The first line of a textbuffer is at position 1.
 */
void addPrefixTB(TB tb, int from, int to, char *prefix)
{
    // test validation
    editValid(tb, from, to);
    // null prefix
    if (prefix == NULL)
    {
        printf("null prefix");
        abort();
    }

    int prefixLen = strlen(prefix);
    Line currLine = tb->first;
    for (int num = 1; num <= to; num++)
    {
        if (num >= from && num <= to)
        {
            char *newStr = pureStr(prefixLen + currLine->lineSize);
            strcat(newStr, prefix);
            strcat(newStr, currLine->line);

            currLine->lineSize += prefixLen;
            char *freeStr = currLine->line;
            currLine->line = newStr;
            free(freeStr);
        }
        currLine = currLine->next;
    }
    tb->textSize += prefixLen * (to - from + 1);
}

/**
 * Remove  the  lines between 'from' and 'to' (inclusive) from the given
 * textbuffer 'tb'.
 * - The  program should abort() with an error message if 'from' or 'to'
 *   is out of range.
 */
void deleteTB(TB tb, int from, int to)
{
    // test validation
    editValid(tb, from, to);

    Line currLine = tb->first;
    for (int num = 1; num <= to; num++)
    {
        if (num >= from && num <= to)
        {
            //  condition of delete
            Line freeLine = currLine;

            if (tb->len == 1)
            { // one line
                tb->first = tb->last = NULL;
            }
            else if (currLine == tb->first)
            { // first line
                currLine = freeLine->next;
                freeLine->next->prev = NULL;
                tb->first = freeLine->next;
            }
            else if (currLine == tb->last)
            { //last line
                freeLine->prev->next = NULL;
                tb->last = freeLine->prev;
            }
            else
            {
                currLine = freeLine->next;
                freeLine->prev->next = freeLine->next;
                freeLine->next->prev = freeLine->prev;
            }

            tb->textSize -= freeLine->lineSize;
            tb->len--;

            free(freeLine->line);
            free(freeLine);
        }
        else
        {
            currLine = currLine->next;
        }
    }
}

/*
 *  Processing the validation of combining
 *  -If 'from' or 'to' is out of range ,it wil show the message and absorb.
 */
static void combineValid(TB tb, int pos)
{
    if (pos > tb->len + 1 || pos < 1)
    {
        printf("error: out of range!\n");
        abort();
    }
}

/**
 * Merge 'tb2' into 'tb1' at line 'pos'.
 * - After this operation:
 *   - What was at line 1 of 'tb2' will now be at line 'pos' of 'tb1'.
 *   - Line  'pos' of 'tb1' will moved to line ('pos' + linesTB('tb2')),
 *     after the merged-in lines from 'tb2'.
 *   - 'tb2' can't be used anymore (as if we had used releaseTB() on it)
 * - The program should abort() with an error message if 'pos' is out of
 *   range.
 */
void mergeTB(TB tb1, int pos, TB tb2)
{
    // test validation of 'pos'
    combineValid(tb1, pos);

    // ignore 'tb2' is 'tb1' itself
    if (tb1 != tb2)
    {
        if (tb2->len == 0) //tb2 is empty
        {
            ;
        }
        else if (tb1->len == 0) //tb1 is empty
        {
            tb1->first = tb2->first;
            tb1->last = tb2->last;
        }
        else if (pos == 1) // pos is at first line
        {
            tb2->last->next = tb1->first;
            tb1->first->prev = tb2->last->next;
            tb1->first = tb2->first;
        }
        else if (pos == tb1->len + 1) //pos is after last line
        {
            tb1->last->next = tb2->first;
            tb2->first->prev = tb1->last;
            tb1->last = tb2->last;
        }
        else //else condition
        {
            Line posLine = tb1->first;
            for (int num = 1; num < pos; num++)
            {
                posLine = posLine->next;
            }
            posLine->prev->next = tb2->first;
            tb2->first->prev = posLine->prev;
            tb2->last->next = posLine;
            posLine->prev = tb2->last;
        }

        tb1->len += tb2->len;
        tb1->textSize += tb2->textSize - 1;

        free(tb2);
    }
}

/**
 * Copy 'tb2' into 'tb1' at line 'pos'.
 * - After this operation:
 *   - What was at line 1 of 'tb2' will now be at line 'pos' of 'tb1'.
 *   - Line  'pos' of 'tb1' will moved to line ('pos' + linesTB('tb2')),
 *     after the pasted-in lines from 'tb2'.
 *   - 'tb2' is unmodified and remains usable independent of tb1.
 * - The program should abort() with an error message if 'pos' is out of
 *   range.
 */
void pasteTB(TB tb1, int pos, TB tb2)
{
    // test validation of 'pos'
    combineValid(tb1, pos);

    TB tb2Copy;

    // tb2 is NULL
    if (tb2 == NULL)
        abort();
    else if (tb2->len == 0) // tb2 is empty
    {
        tb2Copy = newTB("");
    }
    else // generate a copy of tb2
    {
        tb2Copy = cutTB(tb2, 1, tb2->len);
    }
    // merge the copy into tb1
    mergeTB(tb1, pos, tb2Copy);
}

/**
 * Cut  the lines between and including 'from' and 'to' out of the given
 * textbuffer 'tb' into a new textbuffer.
 * - The result is a new textbuffer (much as one created with newTB()).
 * - The cut lines will be deleted from 'tb'.
 * - The  program should abort() with an error message if 'from' or 'to'
 *   is out of range.
 */
TB cutTB(TB tb, int from, int to)
{
    // Testing valid range of 'from' to 'to'.
    // If 'from' is less than 'to' , return NULL.
    // If they are out of range, call aborb().
    if (!editValid(tb, from, to))
        return NULL;

    TB returnTb = newTB("");
    Line currLine = tb->first;
    for (int i = 1; i <= to; i++)
    {
        if (i >= from)
        {
            Line newLine = newTextLine(currLine->line);
            addLine(returnTb, newLine);
        }
        currLine = currLine->next;
    }
    return returnTb;
}

/**
 * Return  a  linked list of match nodes containing the positions of all
 * of the matches of string 'search' in 'tb'.
 * - The textbuffer 'tb' should remain unmodified.
 * - The user is responsible for freeing the returned list.
 */
Match searchTB(TB tb, char *search)
{
    if (tb->len == 0)
        return NULL;

    Match matchFirst = NULL;
    Match matchLast = matchFirst;
    int searchLen = strlen(search);
    Line currLine = tb->first;
    // test each line
    for (int lineNum = 1; lineNum <= tb->len; lineNum++)
    {
        char *strPtr = currLine->line;
        while (1)
        {
            strPtr = strstr(strPtr, search);

            if (strPtr == NULL)
                break; // if is NULL, go to next line
            else
            {
                // allocate a new Match
                matchNode *newMatch = malloc(sizeof *newMatch);
                newMatch->lineNumber = lineNum;
                newMatch->columnNumber = strPtr - currLine->line + 1;
                newMatch->next = NULL;

                // add new Match to list
                if (matchFirst == NULL) // if list is empty
                {
                    matchFirst = matchLast = newMatch;
                }
                else
                {
                    matchLast->next = newMatch;
                    matchLast = newMatch;
                }

                // set strPtr to the rest of the line.
                strPtr += searchLen;
            }
        }
        currLine = currLine->next;
    }

    return matchFirst;
}

///////////
/**
 * Search  every  line of the given textbuffer for every occurrence of a
 * set of specified substitutions and alter them accordingly.
 * - Refer to the spec for details.
 */
void formRichText(TB tb)
{
    const int addLen[3] = {9, 7, 7};
    const char *addBefore[] = {"<h1>", "<i>", "<B>"};
    const char *addAfter[] = {"</h1>", "</i>", "</B>"};
    Line currLine = tb->first;
    // test each line
    for (int lineNum = 1; lineNum <= tb->len; lineNum++)
    {
        char *strPtr = currLine->line;
        if (*strPtr == '#') // first char is '#'
        {
            char *newStr = pureStr(currLine->lineSize + addLen[0]);
            strcat(newStr, "<h1>");
            strcat(newStr, currLine->line + 1);
            strcat(newStr, "</h1>");

            currLine->lineSize += addLen[0];
            tb->textSize += addLen[0];
            char *freeStr = currLine->line;
            free(freeStr);
            currLine->line = newStr;
        }
        else
        {
            // scan the line , add address of '*' or '_' appeared to queues of pairs
            char *italicWatch = NULL;
            char *boldWatch = NULL;
            Queue queueFirst = NULL;
            Queue queueLast = NULL;

            for (int i = 0; i < currLine->lineSize; i++)
            {
                if (*strPtr == '_')
                {
                    // 3 different condition
                    if (italicWatch == NULL) // #1
                        italicWatch = strPtr;

                    else if (strPtr - italicWatch == 1) // #2 their addresses are near
                        italicWatch = strPtr;
                    else // #3
                    {
                        if (boldWatch != NULL) //  '_' is at left of the '*'
                            if (boldWatch - italicWatch > 0)
                                boldWatch = NULL;

                        addChairPairQueue(&queueFirst, &queueLast, italicWatch, strPtr, 1);
                        italicWatch = NULL;
                    }
                }
                else if (*strPtr == '*')
                {
                    // 3 different condition
                    if (boldWatch == NULL)
                        boldWatch = strPtr;
                    else if (strPtr - boldWatch == 1) // their addresses are near
                        boldWatch = strPtr;
                    else
                    {
                        if (italicWatch != NULL)
                            if (italicWatch - boldWatch > 0) // '*' is at left of the '_'
                                italicWatch = NULL;

                        addChairPairQueue(&queueFirst, &queueLast, boldWatch, strPtr, 2);
                        boldWatch = NULL;
                    }
                }
                strPtr++;
            } //end scan

            // edit string
            if (queueFirst != NULL)
            {

                Queue currQueue = queueFirst;
                while (1)
                {
                    char *strMid = currQueue->node1 + 1;
                    char *strAfter = currQueue->node2 + 1;
                    *currQueue->node1 = *currQueue->node2 = '\0';

                    char *newStr = pureStr(currLine->lineSize + addLen[currQueue->type]);
                    strcat(newStr, currLine->line);
                    strcat(newStr, addBefore[currQueue->type]);
                    strcat(newStr, strMid);
                    strcat(newStr, addAfter[currQueue->type]);
                    strcat(newStr, strAfter);

                    char *freeStr = currLine->line;
                    currLine->line = newStr;
                    free(freeStr);

                    currLine->lineSize +=  addLen[currQueue->type];
                    tb->textSize += addLen[currQueue->type];

                    currQueue = currQueue->next;
                    if (currQueue == NULL)
                        break;
                }
            }
            releaseCharPairQueue(&queueFirst,&queueLast);
        }
    }
}
////////////////////////////////////////////////////////////////////////
// Bonus challenges

char *diffTB(TB tb1, TB tb2)
{
    return NULL;
}

void undoTB(TB tb)
{
}

void redoTB(TB tb)
{
}
