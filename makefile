#######################################-precision-#######################################
# LONG=long : in order to use long doubles. This is slower, but generally more accurate, since we can use tolerances down to 10^-11 (or below that). 
# LONG= : in order to use doubles. This is faster. However the tolerances of the ODE solver cannt be below 1e-8.

# use long doubles in C++
LONG=long
# use doubles in C++
# LONG= 

# use long doubles in python
LONGpy=long
# use doubles in python
# LONGpy=

#######################################-Runge Kutta method-#######################################
#------------------These are Rosenbrock (semi implicit) methods: Generally RECOMMENDED---------------------#
# Solver=1

# RODASPR2 is fairly accurate and fast enough (faster than the other two from NaBBODES), but one 
# can use the others or provide another Butcher tableu and use it.
# METHOD=RODASPR2 
# METHOD=ROS34PW2
# METHOD=ROS3w

#-------------------------These are explicit RK methods: Generally NOT RECOMMENDED--------------------------#
Solver=2

# DormandPrince is fairly fast. It can be better than RODASPR2 at very low tolerances 
# because it is higher order. The other two can't even finish...
METHOD=DormandPrince
# METHOD=CashKarp
# METHOD=RKF45


################################-the data files are here. change them from List.txt-##############
cosmoDat=$(shell cat .cosmoDat) 

DataFiles= $(cosmoDat) 


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

Ros_Headers= $(wildcard src/Rosenbrock/*.hpp) $(wildcard src/Rosenbrock/LU/*.hpp) 
SPLINE_Headers=$(wildcard src/Interpolation/*.hpp)

NSCSolve_Headers= $(wildcard src/NSC/NSCSolve.hpp) 
NSC_Headers= $(wildcard src/NSC/NSC.hpp) 
NSCpy_Cpp= $(wildcard src/NSC/NSC-py.cpp) 

PathHead=src/misc_dir/path.hpp
PathTypePy=src/misc_dir/type.py

Cosmo_Headers=$(wildcard src/Cosmo/Cosmo.cpp) $(wildcard src/Cosmo/Cosmo.hpp)

Static_Headers= $(wildcard src/static.hpp) 

all: lib exec 

lib: lib/libCosmo.so lib/libNSC.so
	
exec: check

#shared libraries that can be used from python
lib/libCosmo.so: $(PathHead) $(PathTypePy) $(cosmoDat) $(SPLINE_Headers) $(Cosmo_Headers) $(Static_Headers) 
	$(CC) -o lib/libCosmo.so src/Cosmo/Cosmo.cpp -fPIC -shared $(FLG) -DLONG=$(LONGpy)

lib/libNSC.so: $(PathHead) $(PathTypePy) $(cosmoDat) $(SPLINE_Headers) $(NSCpy_Cpp) $(NSCSolve_Headers) $(NSC_Headers) $(Cosmo_Headers) $(Static_Headers) 
	$(CC) -o lib/libNSC.so src/NSC/NSC-py.cpp -fPIC -shared $(FLG) -DLONG=$(LONGpy)

 
$(PathTypePy): 
	@echo "from ctypes import c_$(LONGpy)double as cdouble" > $(PathTypePy)


# make the examples in Examples/Cpp
examples: $(PathHead)
	cd Examples/Cpp && $(MAKE)


#cleans whatever make all created
clean:

	rm -rf $(wildcard lib/*)
	rm -rf $(wildcard exec/*)
	rm -rf $(wildcard Examples/Python/*_examplePlot.pdf)
	rm -rf $(PathTypePy)
	cd Examples/Cpp && $(MAKE) clean


#deletes directories that configure.sh made
deepClean: clean

	rm -f $(wildcard Examples/scan/*.xtx)
	rm -rf exec
	rm -rf lib
	rm -rf src/misc_dir
	rm -rf src/Interpolation
	rm -rf src/Rosenbrock
	rm -rf src/RKF



##--------------------------------make checks----------------------------------------##
check: exec/Cosmo_check.run exec/NSCSolve_check.run

Cosmo_cpp=$(wildcard src/Cosmo/checks/Cosmo_check.cpp)
# check anharmonic factor interpolation
exec/Cosmo_check.run: $(PathHead) $(Cosmo_cpp) $(DataFiles) $(SPLINE_Headers) 
	$(CC) -o exec/Cosmo_check.run src/Cosmo/checks/Cosmo_check.cpp $(FLG) 

NSCSolve_cpp=$(wildcard src/NSC/checks/NSCSolve_check.cpp)
# check interpolations of the NSC_eom class 
exec/NSCSolve_check.run: $(NSC_Headers) $(NSCSolve_Headers) $(PathHead) $(NSCSolve_cpp) $(Ros_Headers) $(RKF_Headers) $(DataFiles) $(SPLINE_Headers) $(Static_Headers) 
	$(CC) -o exec/NSCSolve_check.run src/NSC/checks/NSCSolve_check.cpp $(FLG) -DLONG=$(LONG) -DMETHOD=$(METHOD) -Dsolver=$(Solver)
