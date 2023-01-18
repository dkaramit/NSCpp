#set PATH variable to working directory


# LONG=  #leave empty to use doubles
LONG=long #set to long , in order to use long doubles

Headers=$(wildcard ../*.hpp)


Path=$(PWD)
Parent=$(shell dirname  $(Path))


CC=g++
FLG=  -std=c++17  -I $(Parent) -lm   -DLONG=$(LONG)  -O3


all:  rdof.run

rdof.run: makefile $(Headers) rdof.cpp
	$(CC) -o "$(Path)/rdof.run" "$(Path)/rdof.cpp" $(FLG) -Wall


clean:
	@[ -f "$(Path)/rdof.run" ] && rm "$(Path)/rdof.run" || true
	 