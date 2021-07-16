rootDir=../../../#the root directory of MiMeS


#######################################-precision-#######################################
# LONG=long : in order to use long doubles. This is slower, but generally more accurate, since we can use tolerances down to 10^-11 (or below that). 
# LONG= : in order to use doubles. This is faster. However the tolerances of the ODE solver cannt be below 1e-8.

# use long doubles in C++
LONG=long
# use doubles in C++
# LONG= 

#######################################-Runge Kutta method-#######################################
#------------------These are Rosenbrock (semi implicit) methods: Generally RECOMMENDED---------------------#
Solver=1

# RODASPR2 is fairly accurate and fast enough (faster than the other two from NaBBODES), but one 
# can use the others or provide another Butcher tableu and use it.
METHOD=RODASPR2 
# METHOD=ROS34PW2
# METHOD=ROS3w

#-------------------------These are explicit RK methods: Generally NOT RECOMMENDED--------------------------#
# Solver=2

# DormandPrince is fairly fast. It can be better than RODASPR2 at very low tolerances 
# because it is higher order. The other two can't even finish...
# METHOD=DormandPrince
# METHOD=CashKarp
# METHOD=RKF45


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

cosmoDat=$(shell cat .cosmoDat) 
DataFiles= $(cosmoDat)  

DataFiles=$(wildcard $(rootDir)src/data/*.dat)
Ros_Headers= $(wildcard $(rootDir)src/Rosenbrock/*.hpp) $(wildcard $(rootDir)src/Rosenbrock/LU/*.hpp) $(wildcard $(rootDir)src/Rosenbrock/Jacobian/*.hpp)
RKF_Headers= $(wildcard $(rootDir)src/RKF/*.hpp) 
SPLINE_Headers=$(wildcard $(rootDir)src/Interpolation/*.hpp)

NSC_Headers= $(wildcard $(rootDir)src/NSC/*.hpp)  

PathHead=$(rootDir)src/misc_dir/path.hpp

Cosmo_Headers=$(wildcard $(rootDir)src/Cosmo/Cosmo.cpp) $(wildcard $(rootDir)src/Cosmo/Cosmo.hpp)

Static_Funcs= $(wildcard $(rootDir)src/static.hpp) 

all: NSC.run

NSC.run: NSC.cpp\
					$(NSC_Headers) $(Cosmo_Headers)\
					$(DataFiles) $(PathHead) $(Static_Funcs)\
					$(Ros_Headers) $(RKF_Headers) $(SPLINE_Headers)

	$(CC) -o NSC.run NSC.cpp $(FLG) 


clean:
	rm -f NSC.run