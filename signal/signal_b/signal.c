//恢复信号
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>

void catch_signal(int sign)
{
    switch(sign)
    {
        case SIGINT:
            printf("ctrl+C 被执行了！\n");
            break;
    }
}

int main(int argc, char **argv)
{
    //注册终端中断信号
    signal(SIGINT, catch_signal);

    char tempc = 0;

    while( (tempc = getchar()) != 'a' )
    {
        printf("tempc=%c\n", tempc);
    }

    //恢复信号
    signal(SIGINT, SIG_DFL);
    while(1)
    {
        pause();
    }

    printf("game over!\n");

    return 0;
}
