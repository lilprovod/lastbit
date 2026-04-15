CC = gcc
RM = C:/msys64/usr/bin/rm.exe
CFLAGS = -std=c99 -Wall -Wextra -pedantic -O2 -Isrc
LDFLAGS = -lm

SRC = src/main.c src/io.c src/dotprod.c src/sum.c src/mul.c
OBJ = $(SRC:.c=.o)

TARGET = lastbit

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) -f $(OBJ) $(TARGET)

rebuild: clean all