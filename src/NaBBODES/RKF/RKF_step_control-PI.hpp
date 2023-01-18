#ifndef RKF_step_control
#define RKF_step_control
#include "RKF_class.hpp"

#define max(a,b)  (a <= b) ? b : a


/*-----------------------Begin: step_control---------------------------------*/

template<unsigned int N_eqs, class RK_method, class LD>
void RKF<N_eqs, RK_method, LD>::step_control(){
    LD Delta=0.;
    LD _sc=0;
    LD fac=beta;
    
    for (unsigned int eq = 0; eq < N_eqs; eq++){
        _sc=max(std::abs( ynext[eq] ), std::abs( yprev[eq] ));
        _sc=abs_tol+rel_tol*_sc;
        Delta+= (abs_delta[eq]/_sc)*(abs_delta[eq]/_sc);
        
    ;}
    Delta=std::sqrt(1./N_eqs*Delta);
    if(Delta==0){Delta=abs_tol*rel_tol;}

    // I use the step control from 
    // https://www.sciencedirect.com/science/article/pii/S147466701751767X
    if(Delta<=1) { 
        if(delta_rej<=1){fac*=h/h_old; }
        fac*=std::pow(Delta, -0.65/( (LD) RK_method::p + 1.) );   
        fac*=std::pow( delta_acc/Delta, 0.3/ ( (LD) RK_method::p + 1. ) );   
        // fac*=std::pow(Deltas[current_step-1]/Delta/Delta, 1/((LD) RK_method::p+1.) ) ;
        h_stop=true ;
    }else{
        fac*=std::pow( Delta , -1./( static_cast<LD>(RK_method::p+1) ));
    }
    
    //this is an alternative. Not very good for some reason. 
    // fac*=h/h_old*std::pow(Deltas[current_step-1]/Delta/Delta, 1/((LD) RK_method::p+1.) ) ;
    
    if(fac> fac_max){fac = fac_max;}
    if(fac< fac_min){fac = fac_min;}
    h=h*fac ;

    if(Delta<=1){h_stop=true;}
    if (h>hmax ){ h=hmax; h_stop=true;}
    if (h<hmin ){ h=hmin; h_stop=true;}
    delta_rej=Delta;

    if(h_stop){delta_acc=Delta;}
    if (tn+h>tmax ){ h=tmax-tn;  }
}
/*-----------------------End: step_control---------------------------------*/

#undef max

#endif
