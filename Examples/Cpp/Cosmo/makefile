rootDir=../../../#the root directory of MiMeS

#######################################-precision-#######################################
# LONG=long : in order to use long doubles. This is slower, but generally more accurate, since we can use tolerances down to 10^-11 (or below that). 
# LONG= : in order to use doubles. This is faster. However the tolerances of the ODE solver cannt be below 1e-8.

# use long doubles in C++
LONG=long
# use doubles in C++
# LONG= 


#---optimization options---#
OPT=O3 #this should be fast and safe
# OPT=O0 #this is generally 2x slower than O3
# OPT=Ofast #this is usually bit faster than O3 but can cause issues (I haven't observed any though)

CC=g++ #compiler. I use g++, and haven't tested anything else. 

#c++ std lib
STD=c++17
# STD=c++11 #this should also work

PATH_INCLUDE=-I$(rootDir)

FLG= -$(OPT) -std=$(STD) $(PATH_INCLUDE) -Wall -DLONG=$(LONG)

SPLINE_Headers=$(wildcard $(rootDir)src/Interpolation/*.hpp)
cosmoDat=$(shell cat .cosmoDat) 
PathHead=$(rootDir)src/misc_dir/path.hpp
Cosmo_Headers=$(wildcard $(rootDir)src/Cosmo/Cosmo.hpp)

all: Cosmo.run

Cosmo.run: Cosmo.cpp makefile $(Cosmo_Headers) $(DataFiles) $(PathHead) $(SPLINE_Headers)

	$(CC) -o Cosmo.run Cosmo.cpp $(FLG)


make clean:
	rm -f Cosmo.run