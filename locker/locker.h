#ifndef LOCKER_H
#define LOCKER_H

#include <exception>
#include <pthread.h>
#include <semaphore.h>

/*封装信号量的类*/
class sem
{
    public:
        /*创建并初始化信号量*/
        sem()
        {
            if( 0 != sem_init(&m_sem, 0, 0) )
            {
                /*构造函数没有返回值，可以通过抛出异常来报告错误*/
                throw std::exception();
            }
        }

        /*销毁信号量*/
        ~sem()
        {
            sem_destroy(&m_sem);
        }

        /*等待信号量*/
        bool wait()
        {
            return 0 == sem_wait(&m_sem);
        }

        /*增加信号量*/
        bool post()
        {
            return 0 == sem_post(&m_sem);
        }

    private:
        sem_t m_sem;
};

/*封装条件变量的类*/
class cond
{
    public:
        /*创建并初始化条件变量*/
        cond()
        {
            if( 0 != pthread_mutex_init(&m_mutex, NULL) )
            {
                throw std::exception();
            }
            if( 0 != pthread_cond_init(&m_cond, NULL) )
            {
                /*构造函数中一旦出现问题，就应该立即释放已经成功分配了的资源*/
                pthread_mutex_destroy(&m_mutex);
                throw std::exception();
            }
        }

        /*销毁条件变量*/
        ~cond()
        {
            pthread_mutex_destroy(&m_mutex);
            pthread_cond_destroy(&m_mutex);
        }

        /*等待条件变量*/
        bool wait()
        {
            int ret = 0;
            pthread_mutex_lock(&m_mutex);
            ret = pthread_cond_wait(&m_cond, &m_mutex);
            pthread_mutex_unlock(&m_mutex);

            return 0 == ret;
        }

        /*唤醒等待条件变量的线程*/
        bool signal()
        {
            return 0 == pthread_cond_signal(&m_cond);
        }

    private:
        pthread_mutex_t m_mutex;
        pthread_cond_t m_cond;
};

#endif