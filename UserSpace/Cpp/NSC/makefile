-include Definitions.mk

#c++ std lib
STD=c++17
# STD=c++11 #this should also work

PATH_INCLUDE= -I$(rootDir)
FLG= -$(OPT) -std=$(STD) -lm  $(PATH_INCLUDE) -Wall -DLONG=$(LONG) -DMETHOD=$(METHOD) -DSOLVER=$(SOLVER)


Ros_Headers= $(wildcard $(rootDir)src/NaBBODES/Rosenbrock/*.hpp) $(wildcard $(rootDir)src/NaBBODES/Rosenbrock/LU/*.hpp) $(wildcard $(rootDir)src/NaBBODES/Rosenbrock/Jacobian/*.hpp)
RKF_Headers= $(wildcard $(rootDir)src/NaBBODES/RKF/*.hpp) 
SPLINE_Headers=$(wildcard $(rootDir)src/SimpleSplines/*.hpp)

NSC_Headers= $(wildcard $(rootDir)src/NSC/*.hpp)  

Cosmo_Headers= $(wildcard $(rootDir)src/Cosmo/Cosmo.hpp)

all: NSC.run

NSC.run: NSC.cpp\
					$(NSC_Headers) $(Cosmo_Headers)\
					$(Ros_Headers) $(RKF_Headers) $(SPLINE_Headers)

	$(CC) -o $@ $< $(FLG) 


clean:
	rm -f NSC.run