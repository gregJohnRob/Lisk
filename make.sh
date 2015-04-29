#!/bin/bash

#Checking for correct input
if [ $# != 1 ]; then
  echo "Usage: ./make OPTION"
  echo "See Building.txt for more info"
  exit 1
fi

#Then check all options and call appriopriate make instructions
if [ $1 == "-s" ]; then
  make server

elif [ $1 == "-c" ]; then
  make client

elif [ $1 == "-r" ]; then
  make clean

elif [ $1 == "-fr" ]; then
  make fullclean

elif [ $1 == "-a" ]; then
  make server
  make client

elif [ $1 == "-rc" ]; then
  make clean_c

elif [ $1 == "-rs" ]; then
  make clean_s

else
  echo "Invalid option"
  echo "See Building.txt for more info"
fi


exit 0
