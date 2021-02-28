#!/bin/bash

if [ "$1" == "" ]; then
    printf  "ERROR: First Arg is empty, Please declare script name"
else  
	answer = 0

	printf  "Building Script Please wait...\n"
	gcc $1 -o ca1_script_$2
	printf  "Running Script now....\n"
	./ca1_script_$2
	printf "\n\n"
	printf "Script End\n"
fi

