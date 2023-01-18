#set PATH variable to working directory


# LONG=  #leave empty to use doubles
LONG=long #set to long , in order to use long doubles

Headers=$(wildcard ./*.hpp)


Path=$(PWD)
CC=g++
FLG=  -std=c++17  -I "$(Path)" -lm   -DLONG=$(LONG)  -O3


all: spline_example.run 

spline_example.run: makefile $(Headers) spline_example.cpp
	$(CC) -o "$(Path)/spline_example.run" "$(Path)/spline_example.cpp" $(FLG) -Wall


clean:
	rm "$(Path)/spline_example.run" || true
	 