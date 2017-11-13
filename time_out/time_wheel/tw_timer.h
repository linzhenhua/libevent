#ifndef TIME_WHEEL_TIMER
#define TIME_WHEEL_TIMER

#include <time.h>
#include <netinet/in.h>
#include <stdio.h>

#define BUFFER_SIZE 64

class tw_timer;

/*绑定socket和定时器*/
struct client_data
{
    sockaddr_in address;
    int sockfd;
    char buf[BUFFER_SIZE];
    tw_timer *timer;
};

/*定时器类*/
class tw_timer
{
    public:
        tw_timer(int rot, int ts):next(NULL), prev(NULL), rotation(rot), time_slot(ts){}

    public:
        int rotation; /*记录定时器在时间轮转多少圈后生效*/
        int time_slot; /*记录定时器属于时间轮上哪个槽（对应的链表，下同）*/
        void (*cb_func)(client_data*); /*定时器回调函数*/
        client_data *user_data; /*客户数据*/
        tw_timer *next; /*指向下一个定时器*/
        tw_timer *prev; /*指向前一个定时器*/
};

class time_wheel
{
    public:
        time_wheel():cur_slot(0)
        {
            for(int i = 0; i < N; ++i)
            {
                slots[i] = NULL; /*初始化每个槽的头结点*/
            }
        }

        ~time_wheel()
        {
            /*遍历每个槽，并销毁其中的定时器*/
            for(int i = 0; i < N; ++i)
            {
                tw_timer *tmp = slots[i];
                while(tmp)
                {
                    slots[i] = tmp->next;
                    delete tmp;
                    tmp = slots[i];
                }
            }
        }

        /*根据定时值timeout创建一个定时器，并把它插入合适的槽中*/
        tw_timer* add_timer(int timeout)
        {
            if(timeout < 0)
            {
                return NULL;
            }
            int ticks = 0;
            /*下面根据待插入定时器的超时值计算它将在时间轮转动多少滴答后触发，
             * 并将该滴答数存储于变量ticks中，如果待插入定时器的超时值小于时间
             * 轮的槽间隔SI，则将ticks向上折合为1，否则就将ticks向下折合为timeout/SI*/
            if(timeout < SI)
            {
                ticks = 1;
            }
            else
            {
                ticks = timeout / SI;
            }
            /*计算待插入的定时器在时间轮转动多少圈后被触发*/
            int rotation = ticks / N;
            /*计算待插入的定时器应该被插入哪个槽中*/
            int ts = (cur_slot + (ticks % N)) % N;

        }
};

#endif
