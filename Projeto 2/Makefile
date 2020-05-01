CC = gcc
CFLAGS = -Wall -pthread
UOBJ = Client/U1.o
QOBJ = Server/Q1.o
AOBJ = Auxiliar/utils.o Auxiliar/args.o
TARGETS = U1 Q1

all: U1 Q1

%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $<

U1: $(UOBJ) $(AOBJ)
	$(CC) $(CFLAGS) $(AOBJ) $(UOBJ) -o $@

Q1: $(QOBJ) $(AOBJ)
	$(CC) $(CFLAGS) $(AOBJ) $(QOBJ) -o $@

clean:
	rm $(UOBJ) $(QOBJ) $(AOBJ) $(TARGETS) 
