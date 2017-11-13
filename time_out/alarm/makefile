CC = g++
PROG = alarm
$(PROG):$(PROG).o
	$(CC) -Wall -o $(PROG) $(PROG).o
$(PROG).o:alarm.cc
	$(CC) -c alarm.cc

clean:
	-rm *.o
