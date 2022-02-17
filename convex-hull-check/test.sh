#!/bin/bash

TEST_DIR="test"
INPUT_DIR="input"
OUTPUT_DIR="output"

for INPUT_PATH in ./$TEST_DIR/$INPUT_DIR/*; do
	FILE=$(sudo basename $INPUT_PATH)
	GOT=$(./main < $INPUT_PATH)
	EXPECTED=$(sudo cat "./$TEST_DIR/$OUTPUT_DIR/$FILE")

	if [ "$GOT" != "$EXPECTED" ]
	then
		echo $FILE: got $GOT, expected $EXPECTED
	fi
done