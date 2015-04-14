#LISK GENERIC MAKEFILE
# Author: Ewan McCartney
#
#Just palms off the make to the other project specific makefiles

.PHONY: server-n server-d server-r server-c server-f help


#Server Makefile options
#-----------------------
server-n:
		@cd server
		@make

server-d:
		@cd server
		@make debug
		@exit 0

server-r:
		@cd server
		@make release

server-c:
		@cd server
		@make clean

server-f:
		@cd server
		@make fullclean

help:
		@echo "Usage: make <PROJECT>-<OPTION>"
		@echo "<PROJECT> = server|client"
		@echo "<OPTION> = n|d|r|c|f"
		@echo "For more info view MAKE_README"
