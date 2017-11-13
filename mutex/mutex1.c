#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex;

//打印机
void printer(char *str)
{
    pthread_mutex_lock(&mutex);

    while(*str != '\0')
    {
        putchar(*str);
        fflush(stdout);
        str++;
        sleep(1);
    }

    printf("\n");

    pthread_mutex_unlock(&mutex);
}

//线程1
void *thread_fun_1(void *arg)
{
    char *str = "hello";
    
    //pthread_mutex_lock(&mutex);

    printer(str);

    //pthread_mutex_unlock(&mutex);
}

//线程2
void *thread_fun_2(void *arg)
{
    char *str = "world";

    //pthread_mutex_lock(&mutex);

    printer(str);

    //pthread_mutex_unlock(&mutex);
}

int main(int argc, char **argv)
{
    pthread_t tid1, tid2;

    pthread_mutex_init(&mutex, NULL);

    pthread_create(&tid1, NULL, thread_fun_1, NULL);
    pthread_create(&tid2, NULL, thread_fun_2, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    pthread_mutex_destroy(&mutex);

    return 0;
}
