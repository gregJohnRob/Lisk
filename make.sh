#!/bin/bash

#Checking for correct input
if [ $# != 2 ]; then
  echo "Usage: ./make PROJECT OPTION"
  echo "See Building.txt for more info"
  exit 1
fi

#Checking for project now
#Then check all options and call appriopriate make instructions
if [ $1 == "server" ]; then
  cd server

  if [ $2 == "-n" ]; then
    make

  elif [ $2 == "-d" ]; then
      make debug

  elif [ $2 == "-r" ]; then
      make release

  elif [ $2 == "-c" ]; then
      make clean

  elif [ $2 == "-f" ]; then
      make fullclean
  else
    echo "Invalid option"
  fi

elif [ $1 == "client" ]; then
  echo "Not yet implemented..."
else
  echo "Invalid Project"

fi

exit 0
