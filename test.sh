#!/bin/bash

PTH=$1

g++ -Wall -o $PTH/main $PTH/main.cpp

TEST_DIR="test"
INPUT_DIR="input"
OUTPUT_DIR="output"

for INPUT_PATH in $PTH/$TEST_DIR/$INPUT_DIR/*; do
	FILE=$(sudo basename $INPUT_PATH)
	GOT=$($PTH/main < $INPUT_PATH)
	EXPECTED=$(sudo cat "$PTH/$TEST_DIR/$OUTPUT_DIR/$FILE")

	if [ "$GOT" != "$EXPECTED" ]
	then
		echo $FILE: got \"$GOT\", expected \"$EXPECTED\"
	fi
done