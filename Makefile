CC = gcc
CFLAGS = -Wall -Wextra -pthread
LDFLAGS = -lpthread

TARGET = event_management_app
SOURCES = src/main.c src/server.c src/user.c src/event.c src/logger.c
OBJECTS = $(SOURCES:.c=.o)

$(TARGET): $(OBJECTS)
    $(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

clean:
    rm -f $(OBJECTS) $(TARGET)