#!/bin/bash

if [ ! -e ../libes.a ]
then
	echo build libes.a first!
	sleep 2
	exit
fi

g++ -Wall -I. -L./.. -D__LINUX__ -O3 -o test_libes test_libes.cpp -s -les -lrt -lasound -lpthread && ./test_libes
read -p "press any key to exit..."
