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
#define minimum_step_size 1e-8
#define maximum_step_size 1e2
#define maximum_No_steps 1000000
#define absolute_tolerance 1e-9
#define relative_tolerance 1e-9
#define beta 0.8
#define fac_max 1.5
#define fac_min 0.5


// this is how the diffeq should look like
#define n_eqs 3 //number of equations
using Array =  std::array<LD,n_eqs>;//define an array type of length n_eqs
//-------------------------------------------------------------------------//

using std::pow;

// you can use a function, but with a class you can also hold data that can be useful.
class diffeq{
    public:
    diffeq()=default;
    ~diffeq()=default;

    void operator()(Array &lhs, Array &y, LD t){
        lhs[0]=-20*y[0]*pow(t,2) ;
        lhs[1]=5*y[0]*pow(t,2)+2*(-pow( y[1],2  )+pow( y[2],2 ) )*pow(t,1);
        lhs[2]=15*y[0]*pow(t,2)+2*(pow( y[1],2  )-pow( y[2],2 ) )*pow(t,1);
    }

};


using SOLVER = RKF<n_eqs,METHOD<LD>,LD>;

int main(int argc, const char** argv) {
    Array y0 = {8,12,4};

    diffeq dydt;

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