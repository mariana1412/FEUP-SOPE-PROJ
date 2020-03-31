CC = gcc
CFLAGS = -Wall
OBJS = simpledu.o reg.o args.o display.o
XHDRS = simpledu.h reg.h macros.h args.h display.h
EXEC = simpledu

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.c %.h $(XHDRS)
	$(CC) $(CFLAGS) -c $<

.PHONY : clean
clean :
	-rm $(OBJS) $(EXEC)
