#LISK SERVER MAKEFILE
# Author: Ewan McCartney
CC=gcc
CFLAGS=-Wall

EXE=bin/LiskServer

OBJECTS = obj/Server.o obj/Coms.o obj/Encode.o obj/Utils.o obj/Map.o

all: dirs $(EXE)

debug: CFLAGS+=-g
debug: dirs $(EXE)

release: CLFAGS+=-O2
release: dirs $(EXE)

#Main Target
$(EXE) : $(OBJECTS)
		$(CC) $(CFLAGS) $^ -o $@

#Smaller module compliations
obj/%.o : src/%.c
		$(CC) $(CLFAGS) -c $^ -o $@

#Make directories we require
dirs:
		@mkdir -p obj
		@mkdir -p bin
		@cp TestMap.txt bin/TestMap.txt


#Extra
.PHONY: clean fullclean dirs debug release

#Cleans out object files
clean:
			rm $(OBJECTS)

#Clean out bin as well
fullclean: clean
			rm  $(EXE)
