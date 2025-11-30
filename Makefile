CC=gcc
CFLAGS=-Wall -Wextra -O2 -Iinclude -Ilib/stb

# List of source files
SRC=src/main.c src/image.c

OBJ=$(SRC:.c=.o)

TARGET=LineVision

$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
