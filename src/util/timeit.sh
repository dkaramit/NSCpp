#!/bin/bash

# This script executes the first argument and prints the time it took (is seconds) to run it.
# 

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

time_out=.time._nscpp_ 
# run the executable ($1) passing as arguments the contents of the file ($2)
time (xargs -a $2 $1 2>$err_out || cat $err_out; rm -f $err_out ) 2> $time_out

# print the time it took in seconds
perl -pe 's/m/ /g; s/\,/./g; s/s//g' $time_out | awk '$1 ~ /real/ {print 60*$2+$3}' >&2
rm -f $time_out

