

SRC :=msg_pkt.c
BIN := list_demo

CC :=gcc
CFLAGS :=-g

all:$(BIN)

$(BIN):msg_pkt.o
	$(CC) $^ -o $@
msg_pkt.o:msg_pkt.c msg_pkt.h
	$(CC) $< $(CFLAGS) -c 

clean:
	rm -r $(BIN) *.o
	


