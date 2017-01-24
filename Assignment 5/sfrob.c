//  sfrob.c

#include <stdio.h>
#include <stdlib.h>

const int INITIAL_ARR_SIZE = 100;
const int INITIAL_WORD_SIZE = 20;

/*int frocmp(char const * a, char const * b)
{
    const char * aa = a;
    const char * bb = b;
*/
int frocmp(const void * pa, const void * pb)
{
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
    int arrSize = INITIAL_ARR_SIZE;
    int wordSize = INITIAL_WORD_SIZE;
    int isEmpty = 1;
    
    //  Initial allocation
    char** wordArr = (char**)malloc(sizeof(char*)*arrSize);
    //  memory allocation failures
    if (wordArr == NULL)
    {
        fprintf(stderr, "Allocation failure.");
        exit(1);
    }
    
    char currChar = getchar();
    
    //  empty file
    if (currChar == EOF)
        return 0;
    
    //  Check stdin
    if (ferror (stdin))
    {
        fprintf(stderr, "Input failure");
        exit(1);
    }
    
    int arrPos = 0;
    
    while (currChar != EOF)
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
        
        //  If previous part succeeds, inplement the second dimension of the array.
        int wordPos = 0;
        wordArr[arrPos] = (char*)malloc(sizeof(char)*wordSize);
        if (wordArr[arrPos] == NULL)
        {
            fprintf(stderr, "Allocation failure.");
            exit(1);
        }
        
        while (currChar != ' ' && currChar != EOF)
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
            
            currChar = getchar();
            //  Check stdin
            if (ferror (stdin))
            {
                fprintf(stderr, "Input failure");
                exit(1);
            }
            wordPos++;
        }
        
//        if (currChar == ' ' || currChar == EOF)
//        {
        while (currChar == ' ' || currChar == EOF)
        {
            //  If current char is sp:
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
                wordPos++;
            }
            //  continue the loop, check if the char is also a sp.
            if (currChar == EOF)
                break;
            else
            {
                currChar = getchar();
                //  Check stdin
                if (ferror (stdin))
                {
                    fprintf(stderr, "Input failure");
                    exit(1);
                }
            }
        }
        arrPos++;
    }
    
    qsort(wordArr, arrPos, sizeof(char*), frocmp);
    
    //  does not contain words
    if (isEmpty == 1)
        return 0;
    
    if (ferror (stdout))
    {
        fprintf(stderr, "Output failure.");
        exit(1);
    }
    
    for (int i = 0; i < arrPos; i++)
    {
        int j = 0;
        while (wordArr[i][j] != ' ')
        {
            putchar(wordArr[i][j]);
            //  check stdout
            if (ferror (stdout))
            {
                fprintf(stderr, "Output failure.");
                exit(1);
            }
            j++;
        }
        putchar(wordArr[i][j]);
        //  check stdout
        if (ferror (stdout))
        {
            fprintf(stderr, "Output failure.");
            exit(1);
        }
    }
    
    for (int i = 0; i < arrPos; i++)
        free(wordArr[i]);
    
    free(wordArr);
    return 0;
}
