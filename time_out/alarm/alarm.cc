#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void sig_alarm(int x)
{
    printf("hello\n");
}

int main(int argc, char **argv)
{
    signal(SIGALRM, sig_alarm);
    alarm(2);

    sleep(10);

    printf("ok\n");

    return 0;
}
