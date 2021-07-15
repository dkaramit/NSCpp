#include <iostream>
#include <iomanip> 
#include <cmath> 
#include <string> 
#include"src/Cosmo/Cosmo.hpp"
#include"src/misc_dir/path.hpp"


//-- Get a number (length) of log_10-spaced points from 10^min to 10^max. --//
template<class LD>
void logspace(LD min, LD max, int length, std::vector<LD> &X ){
    for(int i = 0; i<length ; ++i){
        X.push_back(pow( 10, min + i*(max-min)/( length-1 )));
    }
}

#ifndef LONG
    #define LONG 
#endif

#ifndef LD
    #define LD LONG double
#endif

int main(int argc, char **argv){ 

    if(argc!=3){
        std::cout<<"usage: CosmoExample.run minT maxT\n";
        std::cout<<"With:\n \
        minT: minimum interpolation tempareture.\n \
        maxT: maximum interpolation tempareture.\n\n \
        Beyond these limits, everything is taken constant, so one can use minT=1e-5 GeV and maxT=3e3 with good accuracy.\n";

        return 0;
    }


    LD minT=atof(argv[1]);
    LD maxT=atof(argv[2]);

    mimes::Cosmo<LD> cosmo(cosmo_PATH,minT,maxT);

    // print N points
    unsigned int N=50;
    
    // take logarithmically spaced points 
    std::vector<LD> T;
    logspace<LD>(std::log10(minT), std::log10(maxT),N,T);
    
    std::cout<<"T[GeV]\th_eff\tg_eff\tdh_effdT[GeV^-1]\tdg_effdT[GeV^-1]\tdh\t";
    std::cout<<"H[GeV]\ts[GeV^3]\n";
    for(unsigned int i = 0; i < N; ++i){
        std::cout<<T[i]<<"\t"<<cosmo.heff(T[i])<<"\t"<<cosmo.geff(T[i])
        <<"\t"<<cosmo.dheffdT(T[i])<<"\t"<<cosmo.dgeffdT(T[i])<<"\t"
        <<"\t"<<cosmo.dh(T[i])<<"\t"<<cosmo.Hubble(T[i])<<"\t"<<cosmo.s(T[i])<<"\n";
    }
    


    return 0;
}
