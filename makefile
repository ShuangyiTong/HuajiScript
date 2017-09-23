# Compiler
CC = clang++
# Flags
CFLAGS = -std=c++14 -O3 -Wall -c

all: hjscript.o driver.o
	$(CC) hjscript.o driver.o -o huaji

hjscript.o: hjscript.cpp hjscript.hpp
	$(CC) $(CFLAGS) hjscript.cpp

driver.o: driver.cpp hjscript.hpp
	$(CC) $(CFLAGS) driver.cpp

clean:
	$(RM) huaji *.o *~