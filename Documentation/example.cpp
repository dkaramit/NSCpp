#include<iostream>
#include<iomanip>

//Include everything you need from NSC++
#include"NSCpp.hpp"
    
int main(){
    //Use cosmo_PATH to interpolate ?$\heff$? and ?$\geff$? from T=0 to T=mP.
    nsc::Cosmo<long double> plasma(cosmo_PATH, 0, nsc::Cosmo<long double>::mP);
    
    //Declare Evolution instance using the Rosenbrock method RODASPR2
    nsc::Evolution<long double,1,RODASPR2<long double>> BE;
    
    //Declare parameters
    long double TEND=1e-2, c=3, Ti=1e7, ratio=1e-2, TSTOP=1e-4, umax=200;
    
    bool check=BE.solveNSC(TEND, c, Ti, ratio, TSTOP, umax, &plasma,
                {
                    .initial_step_size=1e-2, .minimum_step_size=1e-8, .maximum_step_size=1e-2, 
                    .absolute_tolerance=1e-11, .relative_tolerance=1e-11, .beta=0.9, 
                    .fac_max=1.2, .fac_min=0.8, .maximum_No_steps=10000000
                });
    
    if(check){
        // If the solver returns true, the solver probably worked.
        std::cout<<std::setprecision(5);
        std::cout<<BE.TE1<<"\t"<<BE.TE2<<"\t"<<BE.TD1<<"\t"<<BE.TD2<<"\n";
        
        //print the results and the errors
        for(size_t i=0; i<BE.pointSize; ++i ){
            std::cout<<std::left<<std::setw(15)<<BE.u[i]<<std::setw(15);
            std::cout<<std::left<<BE.T[i]<<std::setw(15)<<BE.dT[i]<<std::setw(15);
            std::cout<<std::left<<BE.rhoPhi[i]<<std::setw(15)<<BE.drhoPhi[i]<<"\n";
        }
    }else{
        // If the solver returns false, you may need a larger umax.
        std::cerr<<"Something went wrong. Try using larger value for umax\n";
        exit(1);
    }
    
    return 0;
}