#!/bin/bash

for file in ./tests/*; do
	OUTPUT=$(./main < $file)
	echo "$file -> $OUTPUT"
done