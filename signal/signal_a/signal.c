//忽略，屏蔽信号
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>

int main(int argc, char **argv)
{
    pid_t pid = fork();   //创建一个进程
    if(pid == -1)
    {
        printf("fork() failed! error message:%s\n", strerror(errno));
        return -1;
    }
    //注册信号，屏蔽SIGCHLD信号，子进程退出，将不会给父进程发送信号，因此也不会出现僵尸进程
    signal(SIGCHLD, SIG_IGN);
    if(pid > 0)
    {
        printf("father is running!\n");
        sleep(10);
    }
    if(pid == 0)
    {
        printf("i am child\n");
        exit(0);      //退出之后，父进程不处理的话会变成僵尸进程
    }
    printf("game over!\n");

    return 0;
}
