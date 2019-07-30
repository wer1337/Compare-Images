#! /bin/bash
# execute a C program

if [ $# -eq 0 ]
then
	echo "Invalid argument list"
	exit 1
elif [ $# -eq 1 ]
then
	echo "Invalid argument list"
	exit 1
elif [$# -eq 2 ]
then
	#source path
	srcpath=$1

	#root path
	rootpath=$2
	
	cd "$rootpath"

	find -name "*.tga" -exec compareFunc
	
	
	compareFunc() {
		
	}	
