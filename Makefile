CC = gcc
RM = rm
CFLAGS = -std=c99 -Wall -Wextra -pedantic -O2 -Isrc
LDFLAGS = -lm -lgmp

SRC = src/main.c src/io.c src/dotprod.c src/sum.c src/mul.c src/demo.c
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