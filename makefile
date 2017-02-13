
CC = clang++
LD = Ld 
CFLAGS = -g -c -Wall -std=gnu++11 
LDFLAG = -lc++ -std=gnu++11 -lncurses 
TARGET = main
OBJS = Command.o Commandbulider.o Message.o PrivateMsg.o cache.o channel.o main.o pkt_queue.o socket.o utils.o
all: $(TARGET)

$(OBJS): %.o: %.cc
	$(CC) -c $(CFLAGS) $< -o $@

$(TARGET):$(OBJS)
	$(CC) $(LDFLAG) -o $(TARGET) $(OBJS)

.PHONY: clean

clean:
	rm -f $(OBJS) $(TARGET)

