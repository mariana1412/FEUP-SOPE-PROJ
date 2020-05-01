CC = gcc
CFLAGS = -Wall -pthread 
OBJS = U1.o	../Server/args.o ../Server/utils.o
XHDRS = U1.h ../Server/args.h ../Server/utils.h
EXEC = U1

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.c %.h $(XHDRS)
	$(CC) $(CFLAGS) -c $<

.PHONY : clean
clean :
	-rm $(OBJS) $(EXEC)
