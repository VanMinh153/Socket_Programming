CC = gcc
# CFLAGS = -Wall -Wextra -pthread
CFLAGS = -Wall -pthread -Wno-unused-variable
# LDFLAGS = -lpthread

TARGET = server app app_terminal admin_app admin_terminal
TARGET_F = $(patsubst %, release/%, $(TARGET))
OBJ_SERVER = $(patsubst %.c, %.o, $(wildcard server/*.c))
OBJ_CLIENT = $(patsubst %.c, %.o, $(wildcard client/*.c))
OBJ_ADMIN = $(patsubst %.c, %.o, $(wildcard admin/*.c))
OBJ_INCLUDE = $(patsubst %.c, %.o, $(wildcard include/*.c))
OBJ_ALL = $(OBJ_SERVER) $(OBJ_CLIENT) $(OBJ_ADMIN) $(OBJ_INCLUDE)

#_______________________________________________________________________________
.PHONY: clean clean_obj

all: $(TARGET)

server: $(OBJ_SERVER) $(OBJ_INCLUDE)
	$(CC) $(CFLAGS) -o release/$@ $^

client: $(OBJ_CLIENT) $(OBJ_INCLUDE)
	$(CC) $(CFLAGS) -o release/$@ $^

client_terminal: $(OBJ_CLIENT) $(OBJ_INCLUDE)
	$(CC) $(CFLAGS) -o release/$@ $^

admin_app: $(OBJ_ADMIN) $(OBJ_INCLUDE)
	$(CC) $(CFLAGS) -o release/$@ $^

admin_terminal: $(OBJ_ADMIN) $(OBJ_INCLUDE)
	$(CC) $(CFLAGS) -o release/$@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
#_______________________________________________________________________________
clean:
	rm -f $(OBJ_ALL) $(TARGET_F)

clean_obj:
	find . -name "*.o" -type f -delete
	find . -name "*.Identifier" -type f -delete