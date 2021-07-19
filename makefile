-include Definitions.mk 


#---optimization options---#
OPT=O3 #this should be fast and safe
# OPT=O0 #this is generally 2x slower than O3
# OPT=Ofast #this is usually bit faster than O3 but can cause issues (I haven't observed any though)

#you can inlude more things here
PATH_INCLUDE= -I./

#compiler. I use g++, and haven't tested anything else.
CC=g++ 

#c++ std
STD=c++17


FLG= -$(OPT) -std=$(STD) $(PATH_INCLUDE) -Wall

Ros_Headers= $(wildcard src/NaBBODES/Rosenbrock/*.hpp) $(wildcard src/NaBBODES/Rosenbrock/LU/*.hpp) $(wildcard src/NaBBODES/Rosenbrock/Jacobian/*.hpp)
RKF_Headers= $(wildcard src/NaBBODES/RKF/*.hpp) 

SPLINE_Headers=$(wildcard src/SimpleSplines/*.hpp)

NSCSolve_Headers= $(wildcard src/NSC/NSCSolve.hpp) 
NSC_Headers= $(wildcard src/NSC/NSC.hpp) 
NSCpy_Cpp= $(wildcard src/NSC/NSC-py.cpp)

Cosmo_Headers=$(wildcard src/Cosmo/Cosmo.hpp)
Cosmopy_cpp=$(wildcard src/Cosmo/Cosmo.cpp) 

Static_Funcs= $(wildcard src/static.hpp) 

all: lib exec examples

lib: lib/libCosmo.so lib/libNSC.so
	
exec: check

#shared libraries that can be used from python
lib/libCosmo.so: $(Cosmopy_cpp) $(Cosmo_Headers) $(Static_Funcs) $(SPLINE_Headers) 
	$(CC) -o $@ $< -fPIC -shared $(FLG) -DLONG=$(LONGpy)

lib/libNSC.so: $(NSCpy_Cpp) $(NSCSolve_Headers) $(NSC_Headers)\
			   $(Cosmo_Headers) $(Static_Funcs)\
			   $(Ros_Headers) $(RKF_Headers) $(SPLINE_Headers)   
	$(CC) -o $@ $< -fPIC -shared $(FLG) -DLONG=$(LONGpy) -DMETHOD=$(METHOD) -Dsolver=$(Solver)

 


# make the examples in Examples/Cpp
examples:
	cd Examples/Cpp && $(MAKE)


#cleans whatever make all created
clean:

	rm -rf $(wildcard lib/*)
	rm -rf $(wildcard exec/*)
	rm -rf $(wildcard Examples/Python/*_examplePlot.pdf)
	cd Examples/Cpp && $(MAKE) clean


#deletes directories that configure.sh made
deepClean: clean
	rm -rf exec
	rm -rf lib
	rm -rf src/misc_dir



##--------------------------------make checks----------------------------------------##
check: exec/Cosmo_check.run exec/NSCSolve_check.run

Cosmo_cpp=$(wildcard src/Cosmo/checks/Cosmo_check.cpp)
# check anharmonic factor interpolation
exec/Cosmo_check.run: $(Cosmo_cpp) $(Cosmo_Headers) $(SPLINE_Headers) 
	$(CC) -o $@ $< $(FLG) -DLONG=$(LONG)

NSCSolve_cpp=$(wildcard src/NSC/checks/NSCSolve_check.cpp)
# check interpolations of the NSC_eom class 
exec/NSCSolve_check.run: $(NSC_Cpp) $(NSCSolve_cpp) $(NSCSolve_Headers) $(NSC_Headers)\
						 $(Cosmo_Headers) $(Static_Funcs)\
					     $(Ros_Headers) $(RKF_Headers) $(SPLINE_Headers)   
	$(CC) -o $@ $< $(FLG) -DLONG=$(LONG) -DMETHOD=$(METHOD) -Dsolver=$(Solver)
