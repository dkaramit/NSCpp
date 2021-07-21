#include <iostream>
#include <iomanip> 
#include <cmath> 
#include <string> 
#include"src/NSC/NSCSolve.hpp"


#define printResults
// #define printPoints
// #define printRelevant //prints what you need for MiMeS


#ifndef LONG
    #define LONG 
#endif

#ifndef LD
    #define LD LONG double
#endif

int main(int argc, char **argv){ 

    if(argc!=16){
        std::cerr<<"usage: NSC.run TEND c Ti ratio umax TSTOP\n \
        initial_step_size minimum_step_size maximum_step_size absolute_tolerance relative_tolerance beta \n \
        fac_max fac_min maximum_No_steps\n";
        
        std::cerr<<"With:\n\n \
        TEND: TEND [GeV] is defined from Gamma_Phi=H_R(TEND) [H_R is the Hubble rate in RD Universe]\n\n \
        c: characterises the equation of state of Phi, with c=3(1+omega) and p=omega rho_Phi\n\n \
        Ti, ratio: ratio = rho_Phi/rho_R at temperature Ti [GeV]. These are the initial conditions\n\n \
        umax: if u>umax the integration stops (rempember that u=log(a/a_i))\n\n \
        TSTOP: if the temperature drops below this, integration stops.\n\n \
        -----------Optional arguments------------------------\n\n \
        initial_stepsize: initial step the solver takes.\n\n \
        maximum_stepsize: This limits the sepsize to an upper limit.\n\n \
        minimum_stepsize: This limits the sepsize to a lower limit.\n\n \
        absolute_tolerance: absolute tolerance of the RK solver.\n\n \
        relative_tolerance: relative tolerance of the RK solver.\n\n \
        Note:\n \
        Generally, both absolute and relative tolerances should be 1e-8.\n\n \
        In some cases, however, one may need more accurate result (eg if f_a is extremely high,\n \
        the oscillations happen violently, and the ODE destabilizes). Whatever the case, if the\n \
        tolerances are below 1e-8, long doubles *must* be used.\n\n \
        beta: controls how agreesive the adaptation is. Generally, it should be around but less than 1.\n\n \
        fac_max,  fac_min: the stepsize does not increase more than fac_max, and less than fac_min.\n\n \
        This ensures a better stability. Ideally, fac_max=inf and fac_min=0, but in reality one must\n \
        tweak them in order to avoid instabilities.\n\n \
        maximum_No_steps: maximum steps the solver can take Quits if this number is reached even if integration\n \
        is not finished.\n";

        return 1;
    }


    int ar=0;

    //model parameters
    LD TEND = atof(argv[++ar]) ;
    LD c = atof(argv[++ar]);
    LD Ti = atof(argv[++ar]);
    LD ratio = atof(argv[++ar]);
    // solver parameters
    LD umax = atof(argv[++ar]); //u at which the integration stops 
    LD TSTOP = atof(argv[++ar]); // temperature at which integration stops
    
    /*options for the solver (These variables are optional. Yoou can use the Axion class without them).*/
    LD initial_step_size=atof(argv[++ar]); //initial step the solver takes. 
    LD minimum_step_size=atof(argv[++ar]); //This limits the sepsize to an upper limit. 
    LD maximum_step_size=atof(argv[++ar]); //This limits the sepsize to a lower limit.
    LD absolute_tolerance=atof(argv[++ar]); //absolute tolerance of the RK solver
    LD relative_tolerance=atof(argv[++ar]); //relative tolerance of the RK solver
    LD beta=atof(argv[++ar]); //controls how agreesive the adaptation is. Generally, it should be around but less than 1.
    /*
    the stepsize does not increase more than fac_max, and less than fac_min. 
    This ensures a better stability. Ideally, fac_max=inf and fac_min=0, but in reality one must 
    tweak them in order to avoid instabilities.
    */
    LD fac_max=atof(argv[++ar]); 
    LD fac_min=atof(argv[++ar]);
    unsigned int maximum_No_steps=atoi(argv[++ar]); //maximum steps the solver can take Quits if this number is reached even if integration is not finished.
    


    nsc::NSC<LD> BE(TEND,c,Ti,ratio,umax,TSTOP,
    initial_step_size,minimum_step_size, maximum_step_size, absolute_tolerance, relative_tolerance, beta,
    fac_max, fac_min, maximum_No_steps);

    BE.solveNSC();


    #ifdef printResults
    std::cout<<std::setprecision(16)
    <<TEND<<"\t"<<c<<"\t"<<Ti<<"\t"<<ratio<<"\t"<<BE.TE1<<"\t"<<BE.TE2<<"\t"<<BE.TD1<<"\t"<<BE.TD2<<"\n";
    #endif

    // print all the points
    #ifdef printPoints
    std::cout<<"---------------------points:---------------------\n";
    std::cout<<"a/a_i\tT [GeV]\trho_Phi [GeV^4]\tlogH^2"<<std::endl;
    for(size_t i=0; i<BE.pointSize; ++i ){
        for(int j=0; j<4; ++j){
            std::cout<<std::setprecision(16)<<BE.points[i][j];
            if(j==3){std::cout<<"\n";}else{std::cout<<"\t";}
        }
    }
    #endif


    #ifdef printRelevant
    for(size_t i=0; i<BE.pointSize; ++i ){

            std::cout<<std::setprecision(16)
            <<std::log(BE.points[i][0])<<"\t"
            <<BE.points[i][1]<<"\t"
            <<BE.points[i][3]<<"\n";
            
    }
    #endif

    return 0;
}