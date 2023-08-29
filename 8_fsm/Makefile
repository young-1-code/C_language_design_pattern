CC := gcc
LDFLAGS :=
CFLAGS := -g
TARGET := mainApp
SRCS := $(wildcard *.c)

OBJ:
	$(CC) $(SRCS) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

.PHONY:clean
clean:
	@rm $(TARGET)