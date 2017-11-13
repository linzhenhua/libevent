//用户从终端输入任意字符然后统计个数显示，输入end则结束
//使用多线程实现：主线程获取用户输入并判断是否退出，子线程计数

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

char buf[100] = {0};
int flag;
sem_t sem;

//子线程函数，作用是统计buf中的字符个数并打印
void *func(void *arg)
{
    //子线程首先应该有个循环
    //循环中阻塞在等待主线程激活的时候，子线程被激活后就去获取buf中的字符
    //长度，然后打印，完成后再次被阻塞
    sem_wait(&sem);

    while(flag == 0)
    {
        printf("长度为: %d\n", strlen(buf));
        memset(buf, 0, sizeof(buf));
        sem_wait(&sem);
    }

    pthread_exit(NULL);

}

int main(int argc, char **argv)
{
    int ret = -1;
    pthread_t th;

    sem_init(&sem, 0, 0);

    ret = pthread_create(&th, NULL, func, NULL);
    if(ret != 0)
    {
        printf("pthread_create error\n");
        return -1;
    }

    printf("输入一个字符串，以回车结束\n");
    while(scanf("%s", buf))
    {
        //比较用户输入的是不是end，如果是则退出，如果不是则继续
        if(!strncmp(buf, "end", 3))
        {
            printf("输入的字符串为：%s\n", buf);
            flag = 1;
            sem_post(&sem);
            break;
        }
        //主线程在收到用户收入的字符串，并且确认不是end后
        //就去发信号激活子线程来计数
        //子线程被阻塞，主线程可以激活，这就是线程的同步问题
        //信号量就可以用来实现这个线程同步
        sem_post(&sem);
    }

    //回收子线程
    printf("等待回收子线程\n");
    ret = pthread_join(th, NULL);
    if(ret != 0)
    {
        printf("pthread_join error\n");
        exit(-1);
    }
    printf("子线程回收成功\n");

    sem_destroy(&sem);

    return 0;
}

