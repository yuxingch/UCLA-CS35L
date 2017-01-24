//  tr2u.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char * argv[])
{
    //  Check the number of operands.
    if (argc != 3)
    {
        fprintf(stderr, "Wrong number of arguments.\n");
        exit(1);
    }
    
    //  Check the length of two arguments.
    int fLength = (int) strlen(argv[1]);
    int tLength = (int) strlen(argv[2]);
    if (fLength != tLength)
    {
        fprintf(stderr, "The lengths of two arguments are different.\n");
        exit(1);
    }
    
    //  Check duplicate bytes
    int i, j;
    for ( i = 0; i < fLength; i++ )
    {
        for ( j = 0; j < fLength; i++ )
        {
            if (argv[1][i] == argv[2][j])
            {
                fprintf(stderr, "FROM has duplicate bytes.\n");
                exit(1);
            }
        }
    }
    
    //  getchar
    char inputCh;
    
    while(read(0, &inputCh, 1) > 0)
    {
        int k;
        for (k = 0; k < fLength; k++)
        {
            if (inputCh == argv[1][k])
            {
                write(1, &argv[2][k], 1);
                if (ferror(stdout))
                {
                    fprintf(stderr, "Fail to output.\n");
                    exit(1);
                }
                break;
            }
        }
        //  cannot find
        if (k == fLength)   //  ??????
        {
            write(1, &inputCh, 1);
            if (ferror(stdout))
            {
                fprintf(stderr, "Fail to output.\n");
                exit(1);
            }
        }
    }
    
    if (ferror(stdin))
    {
        fprintf(stderr, "Fail to get a byte from a stdin stream.\n");
        exit(1);
    }
    
    return 0;
}
