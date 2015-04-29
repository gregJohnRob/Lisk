#LISK SERVER MAKEFILE
# Author: Ewan McCartney
CC=gcc
CFLAGS=-Wall

#Required by Server
S_EXE=bin/LiskServer
S_OBJECTS = obj/Server.o obj/Coms.o obj/Utils.o obj/Map.o obj/Game.o obj/Encode.o

#Required by Client
C_EXE=bin/TestClient
C_OBJECTS = obj/TestClient.o obj/Encode.o


# ------
# Targets
# ------

#DEFAULT
all: $(S_EXE) $(C_EXE)


#SERVER DEFAULT
server: dirs $(S_EXE)

#CLIENT DEFAULT
client: dirs $(C_EXE)


#Make directories we require
dirs:
		@mkdir -p obj
		@mkdir -p bin
		@cp TestMap.txt bin/TestMap.txt

# -----
# Comp
# -----

#Main Server Target
$(S_EXE) : $(S_OBJECTS)
		$(CC) $(CFLAGS) $^ -o $@

#Main Client
$(C_EXE) : $(C_OBJECTS)
		$(CC) $(CFLAGS) $^ -o $@

#Smaller module compliations
obj/%.o : src/%.c
		$(CC) $(CLFAGS) -c $^ -o $@

# -----
# PHONY + CLEAN
# -----

#Extra
.PHONY: clean fullclean dirs server client clean_s clean_c

#Cleans out object files
clean_c:
			rm $(C_OBJECTS)

clean_s:
			rm $(S_OBJECTS)

clean:
			rm $(S_OBJECTS)
			rm obj/TestClient.o

#Clean out bin as well
fullclean: clean
			rm  $(S_EXE)
			rm  $(C_EXE)
			rm  bin/TestMap.txt
