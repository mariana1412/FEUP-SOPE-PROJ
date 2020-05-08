CC = gcc
CFLAGS = -Wall -pthread
UOBJ = Client/U2.o
QOBJ = Server/Q2.o Server/Queue.o
AOBJ = Auxiliar/utils.o Auxiliar/args.o 
TARGETS = U2 Q2

all: U2 Q2

%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $<

U2: $(UOBJ) $(AOBJ)
	$(CC) $(CFLAGS) $(AOBJ) $(UOBJ) -o $@

Q2: $(QOBJ) $(AOBJ)
	$(CC) $(CFLAGS) $(AOBJ) $(QOBJ) -o $@

clean:
	rm $(UOBJ) $(QOBJ) $(AOBJ) $(TARGETS) 
