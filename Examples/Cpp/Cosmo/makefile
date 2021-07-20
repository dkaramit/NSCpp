-include Definitions.mk

#c++ std lib
STD=c++17
# STD=c++11 #this should also work

PATH_INCLUDE=-I$(rootDir)

FLG= -$(OPT) -std=$(STD) -lstdc++ -lm  $(PATH_INCLUDE) -Wall -DLONG=$(LONG)

SPLINE_Headers=$(wildcard $(rootDir)src/SimpleSplines/*.hpp)
Cosmo_Headers=$(wildcard $(rootDir)src/Cosmo/Cosmo.hpp)

all: Cosmo.run

Cosmo.run: Cosmo.cpp $(Cosmo_Headers) $(SPLINE_Headers)
	$(CC) -o $@ $< $(FLG) 

make clean:
	rm -f Cosmo.run