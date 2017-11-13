//同时接收普通数据和带外数据

#include <sys/types.h>
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

int main(int argc, char **argv)
{
    if(argc <= 2)
    {
        printf("usage: %s ip_address port_number\n", basename(argv[0]) );
        return 1;
    }

    //获取ip和端口
    const char *ip = argv[1];
    int port = atoi(argv[2]);
    
    //返回值
    int ret = 0;

    //设置socket结构体
    struct sockaddr_in address;
    bzero(&address, sizeof(address));           //清空address结构体
    address.sin_family = AF_INET;               //ipv4
    inet_pton(AF_INET, ip, &address.sin_addr);  //把字符串ip转换为网络二进制值
    address.sin_port = htons(port);             //把端口转换为大端字节序

    //建立socket
    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);

    //绑定socket
    ret = bind(listenfd, (struct sockaddr *)&address, sizeof(address) );
    assert(ret != -1);

    //监听socket
    ret = listen(listenfd, 5);
    assert(ret != -1);

    //定义客户地址
    struct sockaddr_in client_address;
    socklen_t client_addrlength = sizeof(client_address);

    //接受客户连接
    int connfd = accept(listenfd, (struct sockaddr *)&client_address, &client_addrlength);
    if(connfd < 0)
    {
        printf("errno is: %d\n", errno);
        close(listenfd);
        return 1;
    }

    char remote_addr[INET_ADDRSTRLEN];
    printf("connected with ip: %s and port: %d\n", 
            inet_ntop(AF_INET, &client_address.sin_addr, remote_addr, INET_ADDRSTRLEN),
            ntohs(client_address.sin_port)
            );

    char buf[1024];
    fd_set read_fds;
    fd_set exception_fds;

    FD_ZERO(&read_fds);
    FD_ZERO(&exception_fds);

    while(1)
    {
        memset(buf, '\0', sizeof(buf));
        /*每次调用select前都要重新在read_fds和exception_fds中设置文件描述符connfd，因为事件发生之后，文件描述符集合将被内核修改*/
        FD_SET(connfd, &read_fds);
        FD_SET(connfd, &exception_fds);

        ret = select(connfd+1, &read_fds, NULL, &exception_fds, NULL);
        if(ret < 0)
        {
            printf("selection failure\n");
            break;
        }

        //对于可读事件，采用普通的recv函数读取数据
        if( FD_ISSET(connfd, &read_fds) )
        {
            ret = recv(connfd, buf, sizeof(buf)-1, 0);
            if(ret <= 0)
            {
                printf("over\n");
                break;
            }
            printf("get %d bytes of normal data: %s\n", ret, buf);
        }
        /*对于异常事件，采用带MSG_OOB标志的recv函数读取带外数据*/
        else if( FD_ISSET(connfd, &exception_fds) )
        {
            ret = recv(connfd, buf, sizeof(buf)-1, MSG_OOB );
            if(ret <= 0)
            {
                break;
            }
            printf("get %d bytes of oob data: %s\n", ret, buf);
        }

    }// end of while(1)

    close(connfd);
    close(listenfd);

    return 0;
}
