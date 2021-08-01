#include"src/static.hpp"

#ifndef LONG
    #define LONG 
#endif

#ifndef LD
    #define LD LONG double
#endif

extern "C"{
    //this functions return the different cosmological parameters we neeed. 
    LD heff(LD T){return cosmo<LD>.heff(T);}

    LD geff(LD T){return cosmo<LD>.geff(T);}

    LD dgeffdT(LD T){return cosmo<LD>.dgeffdT(T);}

    LD dheffdT(LD T){return cosmo<LD>.dheffdT(T);}

    LD dh(LD T){return cosmo<LD>.dh(T);}

    LD s(LD T){return cosmo<LD>.s(T);}

    LD rhoR(LD T){return cosmo<LD>.rhoR(T);}

    LD Hubble(LD T){return cosmo<LD>.Hubble(T);}
    
    LD getT0(){return nsc::Cosmo<LD>::T0;}

    LD getrho_crit(){return nsc::Cosmo<LD>::rho_crit;}
    
    LD geth_hub(){return nsc::Cosmo<LD>::h_hub;}
    
    LD getrelicDM(){return nsc::Cosmo<LD>::relicDM_obs;}

    LD getMP(){return nsc::Cosmo<LD>::mP;}

}