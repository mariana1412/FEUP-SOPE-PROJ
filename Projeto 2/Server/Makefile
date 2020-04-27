CC = gcc
CFLAGS = -Wall -pthread 
OBJS = Q1.o  args.o utils.o
XHDRS = Q1.h utils.h args.h
EXEC = Q1

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.c %.h $(XHDRS)
	$(CC) $(CFLAGS) -c $<

.PHONY : clean
clean :
	-rm $(OBJS) $(EXEC)
