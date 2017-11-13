#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

static int count = 0;

void printMes(int signo)
{
    printf("Get a SIGALRM, %d counts!\n", ++count);
}

int main(int argc, char **argv)
{
    int res = 0;

    struct itimerval tick;
    memset(&tick, 0, sizeof(tick));

    //signal(SIGALRM, printMes);

    struct sigaction sa;
    sa.sa_handler = printMes;
    sa.sa_flags = 0;
    sigaction(SIGALRM, &sa, NULL);

    tick.it_value.tv_sec = 4;
    tick.it_value.tv_usec = 0;

    tick.it_interval.tv_sec = 1;
    tick.it_interval.tv_usec = 0;

    if( setitimer(ITIMER_REAL, &tick, NULL) < 0 )
    {
        printf("set timer failed!\n");
    }

    while(1)
    {
        pause();
    }
    
    return 0;
}
