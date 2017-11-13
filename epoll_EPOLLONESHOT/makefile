CC = g++
epoll_EPOLLONESHOT:epoll_EPOLLONESHOT.o
	$(CC) -Wall -o epoll_EPOLLONESHOT epoll_EPOLLONESHOT.o -lpthread
epoll_EPOLLONESHOT.o:epoll_EPOLLONESHOT.cc
	$(CC) -c epoll_EPOLLONESHOT.cc

clean:
	-rm *.o
