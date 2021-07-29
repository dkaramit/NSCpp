#!/bin/bash

# Note: this script adds a tiny overhead. 
# You can use nsc::Timer in NSCpp/src/util/timeit.hpp in order time your program. 

# This script executes the first argument and prints the time it took (is seconds) to run it.
 

# check if the number of arguments is correct
expectedArgs=2
if [ $# -ne $expectedArgs ]; then
    echo "Please provide $expectedArgs arguments." 
    echo "The first should be a path to an executable, the second should be a file that contains its arguments (each argument should be in different line)."
    exit 
fi

# check if the first argument can be executed
if ! command -v $1 &>/dev/null ; then
    echo "$1 is not a valid executable."
    echo "Please provide a valid path to an executable."
    exit 
fi


# check if the file exists
if [ ! -f $2 ]; then
    echo "File $2 does not exist."
    echo "Please provide a valid path to a file."
    exit 
fi

# check if the file is readable
if [ ! -r $2 ]; then
    echo "File $2 is not readable."
    echo "Please provide a valid path to a readable file."
    exit 
fi

err_out=$1._nscpp_ 
touch $err_out 
# run the executable ($1) passing as arguments the contents of the file ($2)

t0=$(date +%s%N)
xargs -a $2 $1 2>$err_out || cat $err_out; rm -f $err_out 
t1=$(date +%s%N)

awk -v t0=$t0 -v t1=$t1 'BEGIN{print (t1-t0)*1e-9}'  >&2