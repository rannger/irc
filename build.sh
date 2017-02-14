echo "
CC = clang++
LD = Ld 
CFLAGS = -g -c -Wall -std=gnu++11 
LDFLAG = -lc++ -std=gnu++11 -lncurses 
TARGET = rirc 
OBJS = `echo $(ls *.cc|sed 's/.cc/.o/g')`
all: \$(TARGET)

\$(OBJS): %.o: %.cc
	\$(CC) -c \$(CFLAGS) \$< -o \$@

\$(TARGET):\$(OBJS)
	\$(CC) \$(LDFLAG) -o \$(TARGET) \$(OBJS)

.PHONY: clean

clean:
	rm -f \$(OBJS) \$(TARGET)
" > makefile

make
