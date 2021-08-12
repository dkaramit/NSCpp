#include <iostream>
#include <iomanip> 
#include <cmath> 
#include <string> 
#include"NSCpp.hpp"


#ifndef LONG
    #define LONG 
#endif

#ifndef LD
    #define LD LONG double
#endif

int main(int argc, char **argv){ 

    if(argc!=3){
        std::cerr<<"usage: CosmoExample.run minT maxT\n";
        std::cerr<<"With:\n \
        minT: minimum interpolation tempareture.\n \
        maxT: maximum interpolation tempareture.\n\n \
        Beyond these limits, h_eff and g_eff are taken constant.\n";

        return 1;
    }


    LD minT=atof(argv[1]);
    LD maxT=atof(argv[2]);

    nsc::Cosmo<LD> cosmo(cosmo_PATH,minT,maxT);

    // print N points
    unsigned int N=50;
    
    // take logarithmically spaced points 
    std::vector<LD> T;
    T=nsc::util::logspace<LD>(std::log10(minT), std::log10(maxT),N);
    
    std::cout<<"T[GeV]\th_eff\tg_eff\tdh_effdT[GeV^-1]\tdg_effdT[GeV^-1]\tdh\t";
    std::cout<<"H[GeV]\ts[GeV^3]\n";
    for(unsigned int i = 0; i < N; ++i){
        std::cout<<T[i]<<"\t"<<cosmo.heff(T[i])<<"\t"<<cosmo.geff(T[i])
        <<"\t"<<cosmo.dheffdT(T[i])<<"\t"<<cosmo.dgeffdT(T[i])<<"\t"
        <<"\t"<<cosmo.dh(T[i])<<"\t"<<cosmo.Hubble(T[i])<<"\t"<<cosmo.s(T[i])<<"\n";
    }
    


    return 0;
}
