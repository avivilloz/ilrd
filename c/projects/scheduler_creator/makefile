#general
NAME=$(shell basename $(CURDIR))

#for compiling
CC=gcc

DEBUG_FLAGS=-ansi -pedantic-errors -Wall -Wextra -g
RELEASE_FLAGS=-ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3

#add relative path for external libraries if needed
INCLUDES=-I include/ -I ~/git/ds/uid/include/ -I ~/git/ds/pq/include/ -I ~/git/ds/task/include/ -I ~/git/ds/sorted_list/include/ -I ~/git/ds/dlist/include/ -I ~/git/ds/slist/include/ -I ~/git/ds/scheduler/include/ -I ~/git/ds/queue/include/ 
#add absolute path for external libraries if needed
SHARED_LIBRARIES=-L$(realpath .)/lib -Wl,-rpath=$(realpath .)/lib -L~/git/ds/slist/lib -Wl,-rpath=~/git/ds/slist/lib -L~/git/ds/queue/lib -Wl,-rpath=~/git/ds/queue/lib -L~/git/ds/scheduler/lib -Wl,-rpath=~/git/ds/scheduler/lib -L~/git/ds/uid/lib -Wl,-rpath=~/git/ds/uid/lib -L~/git/ds/task/lib -Wl,-rpath=~/git/ds/task/lib -L~/git/ds/dlist/lib -Wl,-rpath=~/git/ds/dlist/lib -L~/git/ds/sorted_list/lib -Wl,-rpath=~/git/ds/sorted_list/lib -L~/git/ds/pq/lib -Wl,-rpath=~/git/ds/pq/lib

SRC=$(wildcard src/*.c)
OBJ=$(addprefix obj/, $(notdir $(SRC:.c=.o)))
#add relative path for external libraries if needed
LIBS=lib/lib$(NAME).so ../queue/lib/libqueue.so ../slist/lib/libslist.so ../scheduler/lib/libscheduler.so ../uid/lib/libuid.so ../task/lib/libtask.so ../pq/lib/libpq.so ../sorted_list/lib/libsorted_list.so ../dlist/lib/libdlist.so

#for running
VLG_FLAGS=--leak-check=yes --track-origins=yes

###################################################################
#compiling

all: obj1 obj2 obj3 lib debug release

.PHONY: obj1
obj1: src/logger.c
	$(CC) $(DEBUG_FLAGS) -c -o obj/logger.o -fPIC $^ $(INCLUDES) 

.PHONY: obj2
obj2: src/sched_logger.c
	$(CC) $(DEBUG_FLAGS) -c -o obj/sched_logger.o -fPIC $^ $(INCLUDES)

.PHONY: obj3
obj3: src/tasks.c
	$(CC) $(DEBUG_FLAGS) -c -o obj/tasks.o -fPIC $^ $(INCLUDES)

.PHONY: lib
lib: $(OBJ)
	$(CC) $(DEBUG_FLAGS) -o lib/lib$(NAME).so -shared $^ $(INCLUDES)

debug: $(LIBS)
	$(CC) $(DEBUG_FLAGS) $(SHARED_LIBRARIES) -o bin/$(NAME)_debug.out $^ $(INCLUDES)

release: $(LIBS)
	$(CC) $(RELEASE_FLAGS) $(SHARED_LIBRARIES) -o bin/$(NAME)_release.out $^ $(INCLUDES)

.PHONY: clean
clean:
	rm -f obj/* lib/* bin/*

####################################################################
#running/debugging

vlg: bin/$(NAME)_debug.out
	valgrind $(VLG_FLAGS) $^

run: bin/$(NAME)_release.out
	./$^
