CC = gcc
# CFLAGS = -Wall -Wextra -pthread $(INCLUDE_FLAGS)
CFLAGS = -Wall -pthread -Wno-unused-variable $(INCLUDE_FLAGS)
INCLUDE_FLAGS = -I./include -I./server/include -I./client/include -I./admin/include
# LDFLAGS = -lpthread


TARGET = test test2 server_test client_test client_terminal admin_test admin_terminal server client admin 
TARGET_F = $(patsubst %, release/%, $(TARGET))
OBJ_SERVER = $(patsubst %.c, %.o, $(wildcard server/include/*.c))
OBJ_CLIENT = $(patsubst %.c, %.o, $(wildcard client/include/*.c))
OBJ_ADMIN = $(patsubst %.c, %.o, $(wildcard admin/include/*.c))
OBJ_INCLUDE = $(patsubst %.c, %.o, $(wildcard include/*.c))
OBJ_ALL = $(OBJ_SERVER) $(OBJ_CLIENT) $(OBJ_ADMIN) $(OBJ_INCLUDE)

#_______________________________________________________________________________
all: server client_terminal

t1: test/test.o $(OBJ_INCLUDE)
	$(CC) $(CFLAGS) -o $@ $(patsubst %, obj/%, $(notdir $^))

t2: test/test2.o include/utils.o
	$(CC) $(CFLAGS) -o $@ $(patsubst %, obj/%, $(notdir $^))
	
tf: test/test_function.o
	$(CC) $(CFLAGS) -o $@ $(patsubst %, obj/%, $(notdir $^))

test: test/test.o $(OBJ_ALL)
	$(CC) $(CFLAGS) -o release/$@ $(patsubst %, obj/%, $(notdir $^))

test2: test/test2.o $(OBJ_ALL)
	$(CC) $(CFLAGS) -o release/$@ $(patsubst %, obj/%, $(notdir $^))

server_test: server/server_test.o $(OBJ_SERVER) $(OBJ_INCLUDE)
	$(CC) $(CFLAGS) -o release/$@ $(patsubst %, obj/%, $(notdir $^))

client_test: client/client_test.o $(OBJ_CLIENT) $(OBJ_INCLUDE)
	$(CC) $(CFLAGS) -o release/$@ $(patsubst %, obj/%, $(notdir $^))

admin_test: admin/admin_test.o $(OBJ_ADMIN) $(OBJ_INCLUDE)
	$(CC) $(CFLAGS) -o release/$@ $(patsubst %, obj/%, $(notdir $^))

# ***
server: server/main.o $(OBJ_SERVER) $(OBJ_INCLUDE)
	$(CC) $(CFLAGS) -o release/$@ $(patsubst %, obj/%, $(notdir $^))

client: client/app.o $(OBJ_CLIENT) $(OBJ_INCLUDE)
	$(CC) $(CFLAGS) -o release/$@ $(patsubst %, obj/%, $(notdir $^))

client_terminal: client/app_terminal.o $(OBJ_CLIENT) $(OBJ_INCLUDE)
	$(CC) $(CFLAGS) -o release/$@ $(patsubst %, obj/%, $(notdir $^))

admin: admin/admin_app.o $(OBJ_ADMIN) $(OBJ_INCLUDE)
	$(CC) $(CFLAGS) -o release/$@ $(patsubst %, obj/%, $(notdir $^))

admin_terminal: admin/admin_terminal.o $(OBJ_ADMIN) $(OBJ_INCLUDE)
	$(CC) $(CFLAGS) -o release/$@ $(patsubst %, obj/%, $(notdir $^))

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o obj/$(notdir $@)
#_______________________________________________________________________________
.PHONY: clean clean_all
clean:
	rm -f obj/*.o $(TARGET_F)

clean_all:
	rm -f $(TARGET_F)
	find . -name "*.o" -type f -delete
	find . -name "*.Identifier" -type f -delete

