CC = gcc
# CFLAGS = -Wall -Wextra -pthread
CFLAGS = -Wall -pthread -Wno-unused-variable -I./include
# LDFLAGS = -lpthread


TARGET = server app app_terminal admin_app admin_terminal
TARGET_F = $(patsubst %, release/%, $(TARGET))
OBJ_SERVER = $(patsubst %.c, %.o, $(wildcard server/*.c))
OBJ_CLIENT = $(patsubst %.c, %.o, $(wildcard client/include/*.c))
OBJ_ADMIN = $(patsubst %.c, %.o, $(wildcard admin/include/*.c))
OBJ_INCLUDE = $(patsubst %.c, %.o, $(wildcard include/*.c))
OBJ_ALL = $(OBJ_SERVER) $(OBJ_CLIENT) $(OBJ_ADMIN) $(OBJ_INCLUDE)

#_______________________________________________________________________________
all: $(TARGET)

server: server/server.o $(OBJ_SERVER) $(OBJ_INCLUDE)
	$(CC) $(CFLAGS) -o release/$@ $^

client: client/app.o $(OBJ_CLIENT) $(OBJ_INCLUDE)
	$(CC) $(CFLAGS) -o release/$@ $^

client_terminal: client/app_terminal.o $(OBJ_CLIENT) $(OBJ_INCLUDE)
	$(CC) $(CFLAGS) -o release/$@ $^

admin_app: admin/admin_app.o $(OBJ_ADMIN) $(OBJ_INCLUDE)
	$(CC) $(CFLAGS) -o release/$@ $^

admin_terminal: admin/admin_terminal.o $(OBJ_ADMIN) $(OBJ_INCLUDE)
	$(CC) $(CFLAGS) -o release/$@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
#_______________________________________________________________________________
.PHONY: clean clean_obj
clean:
	find . -name "*.o" -type f -delete
# rm -f $(OBJ_ALL) $(TARGET_F)

clean_obj:
	find . -name "*.o" -type f -delete
	find . -name "*.Identifier" -type f -delete