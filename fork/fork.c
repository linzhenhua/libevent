#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
    int a = 1;

    int i;
    for(i = 0; i < 3; ++i)
    {
        printf("\ni = %d\n", i);
        pid_t pid;
        pid = fork();
        if(pid > 0)
        {
            printf("a = %d\n", a);
            sleep(1);
            a++;
        }
        else
        {
            printf("A = %d\n", a);
            break;
        }
    }

    return 0;
}

