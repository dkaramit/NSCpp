#include"src/static.hpp"

#ifndef LONG
    #define LONG 
#endif

#ifndef LD
    #define LD LONG double
#endif

extern "C"{
    //this functions return the different cosmological parameters we neeed. 
    LD heff(LD T){return cosmo.heff(T);}

    LD geff(LD T){return cosmo.geff(T);}

    LD dgeffdT(LD T){return cosmo.dgeffdT(T);}

    LD dheffdT(LD T){return cosmo.dheffdT(T);}

    LD dh(LD T){return cosmo.dh(T);}

    LD s(LD T){return cosmo.s(T);}

    LD rhoR(LD T){return cosmo.rhoR(T);}

    LD Hubble(LD T){return cosmo.Hubble(T);}
    
    LD getT0(){return T0;}

    LD getrho_crit(){return rho_crit;}
    
    LD geth_hub(){return h_hub;}
    
    LD getrelicDM(){return relicDM_obs;}

    LD getMP(){return mP;}

}