#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//打印机
void printer(char *str)
{
    while(*str != '\0')
    {
        putchar(*str);
        fflush(stdout);
        str++;
        sleep(1);
    }

    printf("\n");
}

//线程1
void *thread_fun_1(void *arg)
{
    char *str = "hello";
    printer(str);
}

//线程2
void *thread_fun_2(void *arg)
{
    char *str = "world";
    printer(str);
}

int main(int argc, char **argv)
{
    pthread_t tid1, tid2;

    pthread_create(&tid1, NULL, thread_fun_1, NULL);
    pthread_create(&tid2, NULL, thread_fun_2, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return 0;
}
