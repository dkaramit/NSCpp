METHOD=RODASPR2
# METHOD=ROS34PW2
# METHOD=GRK4A
# METHOD=GRK4T
# METHOD=ROS3w


# LONG=  #leave empty to use doubles
LONG=long #set to long , in order to use long doubles


Path=$(PWD)
CC=g++

# path to Jacobian.hpp
JACPATH="$(Path)/Jacobian"


FLG=  -std=c++17  -I "$(Path)" -I $(JACPATH)  -lm  -DMETHOD=$(METHOD) -DLONG=$(LONG)  -O3 -Wall


all: Ros.run Example.run

Ros.run: METHOD.hpp  Ros_calc_k.hpp  Ros_class.hpp\
	Ros_costructor.hpp  Ros.hpp  Ros_step_control-PI.hpp\
	Ros_step_control-simple.hpp  Ros_steps.hpp  Ros_LU.hpp\
	Ros_sums.hpp Ros.cpp LU/LU.hpp Jacobian/Jacobian.hpp makefile
	$(CC) -o "$(Path)/Ros.run" "$(Path)/Ros.cpp" $(FLG) 


Example.run: METHOD.hpp  Ros_calc_k.hpp  Ros_class.hpp\
	Ros_costructor.hpp  Ros.hpp  Ros_step_control-PI.hpp\
	Ros_step_control-simple.hpp  Ros_steps.hpp  Ros_LU.hpp\
	Ros_sums.hpp Example.cpp LU/LU.hpp Jacobian/Jacobian.hpp makefile
	$(CC) -o "$(Path)/Example.run" "$(Path)/Example.cpp" $(FLG) 


clean:
	rm -f *.run	 