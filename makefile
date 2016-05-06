
CC = clang++
LD = Ld 
CFLAGS = -g -c -Wall -std=gnu++11 
LDFLAG = -lc++ -std=gnu++11 
TARGET = main
OBJS = Command.o Commandbulider.o Message.o main.o socket.o
all: $(TARGET)

$(OBJS): %.o: %.cc
	$(CC) -c $(CFLAGS) $< -o $@

$(TARGET):$(OBJS)
	$(CC) $(LDFLAG) -o $(TARGET) $(OBJS)

.PHONY: clean

clean:
	rm -f $(OBJS) $(TARGET)

