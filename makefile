-include Definitions.mk 

#you can inlude more things here
PATH_INCLUDE= -I$(rootDir)

#c++ std
STD=c++17


FLG= -$(OPT) -std=$(STD) -lm  $(PATH_INCLUDE) -Wall

Ros_Headers= $(wildcard $(rootDir)src/NaBBODES/Rosenbrock/*.hpp) $(wildcard $(rootDir)src/NaBBODES/Rosenbrock/LU/*.hpp) $(wildcard $(rootDir)src/NaBBODES/Rosenbrock/Jacobian/*.hpp)
RKF_Headers= $(wildcard $(rootDir)src/NaBBODES/RKF/*.hpp) 

SPLINE_Headers=$(wildcard $(rootDir)src/SimpleSplines/*.hpp)

NSCSolve_Headers= $(wildcard $(rootDir)src/NSC/NSCSolve.hpp) 
NSC_Headers= $(wildcard $(rootDir)src/NSC/NSC.hpp) 
NSCpy_Cpp= $(wildcard $(rootDir)src/NSC/NSC-py.cpp)

Cosmo_Headers=$(wildcard $(rootDir)src/Cosmo/Cosmo.hpp)
Cosmopy_cpp=$(wildcard $(rootDir)src/Cosmo/Cosmo.cpp) 

Static_Funcs= $(wildcard $(rootDir)src/static.hpp) 

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

 


# make the examples in UserSpace/Cpp
examples:
	cd UserSpace/Cpp && $(MAKE)


#cleans whatever make all created
clean:

	rm -rf $(wildcard lib/*)
	rm -rf $(wildcard exec/*)
	rm -rf $(wildcard UserSpace/Python/*_examplePlot.pdf)
	cd UserSpace/Cpp && $(MAKE) clean


#deletes directories that configure.sh made
deepClean: clean
	rm -rf exec
	rm -rf lib
	rm -rf $(rootDir)src/misc_dir



##--------------------------------make checks----------------------------------------##
check: exec/Cosmo_check.run exec/NSCSolve_check.run

Cosmo_cpp=$(wildcard $(rootDir)src/Cosmo/checks/Cosmo_check.cpp)
# check anharmonic factor interpolation
exec/Cosmo_check.run: $(Cosmo_cpp) $(Cosmo_Headers) $(SPLINE_Headers) 
	$(CC) -o $@ $< $(FLG) -DLONG=$(LONG)

NSCSolve_cpp=$(wildcard $(rootDir)src/NSC/checks/NSCSolve_check.cpp)
# check interpolations of the NSC_eom class 
exec/NSCSolve_check.run: $(NSC_Cpp) $(NSCSolve_cpp) $(NSCSolve_Headers) $(NSC_Headers)\
						 $(Cosmo_Headers) $(Static_Funcs)\
					     $(Ros_Headers) $(RKF_Headers) $(SPLINE_Headers)   
	$(CC) -o $@ $< $(FLG) -DLONG=$(LONG) -DMETHOD=$(METHOD) -Dsolver=$(Solver)
