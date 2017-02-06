
CC = clang++
LD = Ld 
CFLAGS = -g -c -Wall -std=gnu++11 
LDFLAG = -lc++ -lncurses -std=gnu++11 -g
TARGET = main
OBJS = Command.o Commandbulider.o Message.o main.o socket.o cache.o pkt_queue.o
all: $(TARGET)

$(OBJS): %.o: %.cc
	$(CC) -c $(CFLAGS) $< -o $@

$(TARGET):$(OBJS)
	$(CC) $(LDFLAG) -o $(TARGET) $(OBJS)

.PHONY: clean

clean:
	rm -f $(OBJS) $(TARGET)

