-include Definitions.mk


#---optimization options---#
OPT=O3 #this should be fast and safe
# OPT=O0 #this is generally 2x slower than O3
# OPT=Ofast #this is usually bit faster than O3 but can cause issues (I haven't observed any though)

CC=g++ #compiler. I use g++, and haven't tested anything else. 

#c++ std lib
STD=c++17
# STD=c++11 #this should also work

PATH_INCLUDE= -I$(rootDir)
FLG= -$(OPT) -std=$(STD) $(PATH_INCLUDE) -Wall -DLONG=$(LONG) -DMETHOD=$(METHOD) -Dsolver=$(Solver)


Ros_Headers= $(wildcard $(rootDir)src/NaBBODES/Rosenbrock/*.hpp) $(wildcard $(rootDir)src/NaBBODES/Rosenbrock/LU/*.hpp) $(wildcard $(rootDir)src/NaBBODES/Rosenbrock/Jacobian/*.hpp)
RKF_Headers= $(wildcard $(rootDir)src/NaBBODES/RKF/*.hpp) 
SPLINE_Headers=$(wildcard $(rootDir)src/SimpleSplines/*.hpp)

NSC_Headers= $(wildcard $(rootDir)src/NSC/*.hpp)  

PathHead=$(rootDir)src/misc_dir/path.hpp

Cosmo_Headers= $(wildcard $(rootDir)src/Cosmo/Cosmo.hpp)

Static_Funcs= $(wildcard $(rootDir)src/static.hpp) 

all: NSC.run

NSC.run: NSC.cpp\
					$(NSC_Headers) $(Cosmo_Headers)\
					$(PathHead) $(Static_Funcs)\
					$(Ros_Headers) $(RKF_Headers) $(SPLINE_Headers)

	$(CC) -o $@ $< $(FLG) 


clean:
	rm -f NSC.run