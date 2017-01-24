//  sfrobu.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

const long INITIAL_ARR_SIZE = 1000;
const int INITIAL_WORD_SIZE = 20;
int cmpNum = 0;

int frocmp(const void * pa, const void * pb)
{
    cmpNum++;
    const char *a = *(const char**) pa;
    const char *b = *(const char**) pb;
    
    int pos = 0;
    
    for (; a[pos] != ' ' && b[pos] != ' '; pos++)
    {
        char A = a[pos];
        char B = b[pos];
        if ((A^42) < (B^42))
            return -1;
        else if ((A^42) > (B^42))
            return 1;
    }
    
    if (a[pos] == ' ' && b[pos] != ' ')
        return -1;
    else if (a[pos] != ' ' && b[pos] == ' ')
        return 1;
    else
        return 0;
}

int main()
{
    //  initializing
    long arrSize = INITIAL_ARR_SIZE;
    int wordSize = INITIAL_WORD_SIZE;
    int isEmpty = 1;
    
    
    struct stat fileStat;
    if ( fstat(0, &fileStat) != 0)
    {
        fprintf(stderr, "Fail to check file status");
        exit(1);
    }
    
    if ( S_ISREG(fileStat.st_mode) && fileStat.st_size )
        arrSize = fileStat.st_size + 1;
    //  Initial allocation
    char** wordArr = (char**)malloc(sizeof(char*)*arrSize);
    
    
    //  memory allocation failures
    if (wordArr == NULL)
    {
        fprintf(stderr, "Allocation failure.");
        exit(1);
    }
    
    char currChar;
    
    int arrPos = 0;
    int wordPos = 0;
    
    wordArr[arrPos] = (char*)malloc(sizeof(char)*wordSize);
    if (wordArr[arrPos] == NULL)
    {
        fprintf(stderr, "Allocation failure.");
        exit(1);
    }
    
    int readStatus = (int)read(0, &currChar, 1);
//    while (read(0, &currChar, 1) > 0)
    while (readStatus > 0 || readStatus == 0 )
    {
        //  Check if we need to reallocate the array.
        //  If necessary, reallocate the array using realloc.
        if (arrPos >= arrSize)
        {
            arrSize = arrSize*2;
            wordArr = realloc(wordArr, sizeof(char*)*arrSize);
            //  memory allocation failures
            if (wordArr == NULL)
            {
                fprintf(stderr, "Allocation failure.");
                exit(1);
            }
        }
        if (readStatus == 0)
        {
            if (wordArr[arrPos][wordPos] != ' ' && wordPos != 0)   //   change
            {
                if (wordPos >= wordSize)
                {
                    wordSize = wordSize + 1;
                    wordArr[arrPos] = realloc(wordArr[arrPos], sizeof(char)*wordSize);
                    //  memory allocation failures
                    if (wordArr[arrPos] == NULL)
                    {
                        fprintf(stderr, "Allocation failure.");
                        exit(1);
                    }
                }
                wordArr[arrPos][wordPos] = ' ';
            }
            arrPos++;
            break;
        }
        if (currChar != ' ')
        {
            if (wordPos >= wordSize)
            {
                wordSize = wordSize*2;
                wordArr[arrPos] = realloc(wordArr[arrPos], sizeof(char)*wordSize);
                //  memory allocation failures
                if (wordArr[arrPos] == NULL)
                {
                    fprintf(stderr, "Allocation failure.");
                    exit(1);
                }
            }
            //  implement
            wordArr[arrPos][wordPos] = currChar;
            isEmpty = 0;    //  check
            wordPos++;  //  Haven't finished this word yet
        }
        if (currChar == ' ')
        {
            //  If current char is sp:
            //      SP - the end of current word
            if (wordArr[arrPos][wordPos] != ' ' && wordPos != 0)   //   change
            {
                if (wordPos >= wordSize)
                {
                    wordSize = wordSize + 1;
                    wordArr[arrPos] = realloc(wordArr[arrPos], sizeof(char)*wordSize);
                    //  memory allocation failures
                    if (wordArr[arrPos] == NULL)
                    {
                        fprintf(stderr, "Allocation failure.");
                        exit(1);
                    }
                }
                wordArr[arrPos][wordPos] = ' ';
//                wordPos++;
                //  prepare space for the next word
                arrPos++;
                wordPos = 0;
                wordArr[arrPos] = (char*)malloc(sizeof(char)*wordSize);
                if (wordArr[arrPos] == NULL)
                {
                    fprintf(stderr, "Allocation failure.");
                    exit(1);
                }
            }
        }
        readStatus = (int)read(0, &currChar, 1);
    }
    
    
//    if (wordArr[arrPos][wordPos] != ' ')
//    if (wordPos >= wordSize)
//    {
//        wordSize = wordSize + 1;
//        wordArr[arrPos] = realloc(wordArr[arrPos], sizeof(char)*wordSize);
//        if (wordArr[arrPos] == NULL)
//        {
//            fprintf(stderr, "Fail to reallocate this array");
//            exit(1);
//        }
//    }
    
    qsort(wordArr, arrPos, sizeof(char*), frocmp);
    
    //  does not contain words
    if (isEmpty == 1)
        return 0;
    
    int i = 0;
    for ( ; i < arrPos; i++)
    {
        int j = 0;
        while (wordArr[i][j] != ' ')
        {
            write(1, &wordArr[i][j], 1);
            //  check stdout
            if (ferror (stdout))
            {
                fprintf(stderr, "Output failure.");
                exit(1);
            }
            j++;
        }
        write(1, &wordArr[i][j], 1);
        //  check stdout
        if (ferror (stdout))
        {
            fprintf(stderr, "Output failure.");
            exit(1);
        }
    }
    
    int k = 0;
    for ( ; k < arrPos; k++)
        free(wordArr[k]);
    
    free(wordArr);
    fprintf(stderr, "Comparisons: %d\n", cmpNum);
    return 0;
}