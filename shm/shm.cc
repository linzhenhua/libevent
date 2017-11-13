#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define USER_LIMIT 5
#define BUFFER_SIZE 1024
#define FD_LIMIT 65535
#define MAX_EVENT_NUMBER 1024
#define PROCESS_LIMIT 65536

/*处理一个客户连接必要的数据*/
struct client_data
{
    sockaddr_in address;    //客户端的socket地址
    int connfd;             //socket文件描述符
    pid_t pid;              //处理这个连接的子进程的PID
    int pipefd[2];          //和父进程通信用的管道
};

static const char *shm_name = "/my_shm";
int sig_pipefd[2];
int epollfd;
int listenfd;
int shmfd;
char *share_mem = 0;
/*客户连接数组，进程用客户连接的编号来索引这个数组，即可取得相关的客户连接数据*/
client_data *users = 0;
/*子进程和客户连接的映射关系表，用进程的PID来索引这个数组，即可取得该进程所处理
 * 的客户连接的编号*/
int *sub_process = 0;
/*当前客户数量*/
int user_count = 0;
bool stop_child = false;

int setnonblocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);

    return old_option;
}


