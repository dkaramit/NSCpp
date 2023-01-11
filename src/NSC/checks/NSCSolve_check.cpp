#include <iostream>
#include <iomanip> 
#include <cmath> 
#include <string> 
#include"src/NSC/NSCSolve.hpp"


#define printPoints
#define printError


// macros for the solver
#ifndef SOLVER
    #define SOLVER 1
    #define METHOD RODASPR2
#endif


// macros for the numeric type
#ifndef LONG
    #define LONG 
#endif

#ifndef LD
    #define LD LONG double
#endif

int main(int argc, char **argv){ 

    //model parameters
    LD TEND = 1e-2;
    LD c=2;
    LD Ti=1e7;
    LD ratio=1e-3;

    // solver parameters
    LD umax = 500; //u at which the integration stops 
    LD TSTOP = 1e-4; // temperature at which integration stops
    /*options for the solver*/
    LD initial_step_size=1e-2; //initial step the solver takes. 
    LD minimum_step_size=1e-8; //This limits the sepsize to an upper limit. 
    LD maximum_step_size=1e-2; //This limits the sepsize to a lower limit.
    LD absolute_tolerance=1e-8; //absolute tolerance of the RK solver
    LD relative_tolerance=1e-8; //relative tolerance of the RK solver
    LD beta=0.9; //controls how agreesive the adaptation is. Generally, it should be around but less than 1.
    
    /*
    the stepsize does not increase more than fac_max, and less than fac_min. 
    This ensures a better stability. Ideally, fac_max=inf and fac_min=0, but in reality one must 
    tweak them in order to avoid instabilities.
    */
    LD fac_max=1.2; 
    LD fac_min=0.8;
    unsigned int maximum_No_steps=int(1e7); //maximum steps the solver can take Quits if this number is reached even if integration is not finished.

    nsc::Evolution<LD,SOLVER,METHOD<LD>> BE(TEND,c,Ti,ratio,umax,TSTOP,
    initial_step_size,minimum_step_size, maximum_step_size, absolute_tolerance, relative_tolerance, beta,
    fac_max, fac_min, maximum_No_steps);

    BE.solveNSC(); //solve the system. 



    std::cout<<std::setprecision(5)
    <<TEND<<"\t"<<c<<"\t"<<Ti<<"\t"<<ratio<<"\t"<<BE.TE1<<"\t"<<BE.TE2<<"\t"<<BE.TD1<<"\t"<<BE.TD2<<"\n";


    // print all the points
    #ifdef printPoints
    std::cout<<"---------------------points:---------------------\n";
    std::cout<<"u\tT [GeV]\trho_Phi [GeV^4]"<<std::endl;
    for(size_t i=0; i<BE.pointSize; ++i ){
        std::cout<<std::setprecision(16)<<BE.u[i]<<"\t"<<BE.T[i]<<"\t"<<BE.rhoPhi[i]<<"\n";
    }
    #endif

    // print the local errors
    #ifdef printError
    std::cout<<"---------------------local error:---------------------\n";
    std::cout<<"dT\tdrhoPhi"<<std::endl;
    for(size_t i=0; i<BE.pointSize; ++i ){
        std::cout<<BE.dT[i]<<"\t"<<BE.drhoPhi[i]<<"\n";
    }
    #endif


    return 0;
}
