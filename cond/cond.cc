#include <iostream>
#include <queue>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

using std::cout;
using std::endl;
using std::queue;

#define N 100
#define ST 10

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ready = PTHREAD_COND_INITIALIZER;

queue<int> que;

void *threadProducer(void *arg)
{
    while(true)
    {
        sleep(rand() % ST);

        cout << "Produce try in... \n";
        pthread_mutex_lock(&lock);
        cout << "Produce in! \n";
        int source = rand() % N;
        cout << "Produce " << source << endl;
        que.push(source);
        pthread_mutex_unlock(&lock);
        cout << "Produce out \n";

        pthread_cond_signal(&ready);
    }
}

void *threadConsumer(void *arg)
{
    while(true)
    {
        sleep(rand() % ST);

        cout << "Consum try in...\n";
        pthread_mutex_lock(&lock);
        cout << "Consum in!\n";
        while(que.empty())
        {
            pthread_cond_wait(&ready, &lock);
            cout << "Consum form sleep\n";
        }
        cout << "Consum " << que.front() << endl;
        que.pop();
        pthread_mutex_unlock(&lock);
        cout << "Consum out \n\n";
    }
}

int main(int argc, char **argv)
{
    pthread_t tProducer, tConsumer;

    pthread_create(&tProducer, NULL, threadProducer, NULL);
    pthread_create(&tConsumer, NULL, threadConsumer, NULL);

    pthread_join(tProducer, NULL);
    pthread_join(tConsumer, NULL);

    exit(0);
}
