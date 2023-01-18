// This is how you run Rosenbrock, using a functor
#include<iostream>
#include<fstream>
#include<cmath>
#include"Ros.hpp"
#include "Jacobian/Jacobian.hpp"//this is treated as user input, since one may have an analytic form.
#include "METHOD.hpp"

using std::cout;
using std::endl;
/*--------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------*/
#ifndef LONG
#define LONG 
#endif


#define LD LONG double


#ifndef METHOD
#define METHOD ROS34PW2
#endif


#define initial_step_size 1e-2
#define minimum_step_size 1e-8
#define maximum_step_size 1e3
#define maximum_No_steps 1000000
#define absolute_tolerance 1e-8
#define relative_tolerance 1e-8
#define beta 0.5
#define fac_max 1.01
#define fac_min 0.9

// this is how the diffeq should look like
#define n_eqs 1 //number of equations
using Array =  std::array<LD, n_eqs>;//define an array type of length n_eqs
//-------------------------------------------------------------------------//

using std::pow;


// you can use a function, but with a class you can also hold data that can be useful.
class diffeq{
    public:
    LD c;
    diffeq(LD c):c(c){};

    void operator()(Array &lhs, Array &y  , LD t){
        lhs[0]=t*c;
    }

};



using SOLVER = Ros<n_eqs, METHOD<LD> ,Jacobian<n_eqs,LD> , LD>;

int main(int argc, const char** argv) {
    
    Array y0 = {2};
    diffeq dydt(2);


    SOLVER System(dydt,y0, 1e4,
    initial_step_size,  minimum_step_size,  maximum_step_size, maximum_No_steps, 
    absolute_tolerance, relative_tolerance, beta, fac_max, fac_min);
    
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