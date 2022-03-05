#!/bin/bash

g++ -Wall -o ./main ./main.cpp

for INPUT_PATH in ./test/input/*; do
	FILE=$(sudo basename $INPUT_PATH)
	GOT=$(./main < $INPUT_PATH)

	echo $FILE:
    echo "$GOT"
    echo
done