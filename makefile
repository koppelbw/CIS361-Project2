CFLAGS = -Wall -lpthread
EFILE = proj2
DEPS = proj2.h  
OBJS = proj2.o  thread1.o thread2.o

$(EFILE): $(OBJS)
	gcc $(CFLAGS) -o $(EFILE) $(OBJS)

%.o: %.c $(DEPS)
	gcc $(CFLAGS) -c $<

