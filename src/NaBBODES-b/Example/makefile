# METHOD=ROS34PW2
METHOD=RODASPR2
# METHOD=ROS3w


# LONG=  #leave empty to use doubles
LONG=long #set to long , in order to use long doubles

 

Path=$(PWD)
PathRos=`bash -c "dirname $(Path)"`

CC=g++
FLG=  -std=c++17  -I "$(PathRos)/Rosenbrock/Jacobian"  -I "$(Path)" -lm  -DMETHOD=$(METHOD) -DLONG=$(LONG)  -O3


all: TwoState.run

TwoState.run: makefile TwoState.cpp
	$(CC) -o "$(Path)/TwoState.run" "$(Path)/TwoState.cpp" $(FLG) 


clean:
	@[ -f "$(Path)/TwoState.run" ] && rm "$(Path)/TwoState.run" || true
	 