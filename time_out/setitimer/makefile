CC = g++
PROG = setitimer
$(PROG):$(PROG).o
	$(CC) -Wall -o $(PROG) $(PROG).o
$(PROG).o:setitimer.cc
	$(CC) -c setitimer.cc

clean:
	-rm *.o
