// This is how you run RK. 
#include<iostream>
#include<stdio.h>
#include<cmath>
#include"RKF.hpp"
#include"METHOD.hpp"


#ifndef LONG
#define LONG 
#endif


#define LD LONG double


#ifndef METHOD
#define METHOD RKF45
#endif



#define initial_step_size 1e-2
#define minimum_step_size 1e-5
#define maximum_step_size 1e1
#define maximum_No_steps 1000000
#define absolute_tolerance 1e-9
#define relative_tolerance 1e-9
#define beta 0.95
#define fac_max 1.1
#define fac_min 0.8


// this is how the diffeq should look like
#define n_eqs 1 //number of equations
using Array =  std::array<LD,n_eqs>;//define an array type of length n_eqs
//-------------------------------------------------------------------------//

using std::pow;

/*std::function supports any callable object, so we cna simply use this without overloading 
operator=, or defining copy constructor.*/
class diffeq{
    public:
    LD c;
    diffeq(double c):c(c){}; 

    void operator()(Array &lhs, Array &y, LD t){
        lhs[0]=t*c;
    }

};

using SOLVER = RKF<n_eqs,METHOD<LD>,LD>;

int main(int argc, const char** argv) {
    Array y0={0};

    diffeq dydt(2);

    SOLVER System(dydt,y0,1e1, 
    initial_step_size,  minimum_step_size,  maximum_step_size, 
    maximum_No_steps, absolute_tolerance, relative_tolerance, beta, fac_max, fac_min);
    
    System.solve();
    // return 0;

    int step=0;
    for (auto _t: System.time){
        printf("%e ",(double)_t);
        for( unsigned int eq = 0; eq < n_eqs; eq++){ printf("%e ", (double)System.solution[eq][step]);}
        for( unsigned int eq = 0; eq < n_eqs; eq++){ printf("%e " ,(double)System.error[eq][step]);}
        printf("\n");
        step++;
    }




    return 0;
 }