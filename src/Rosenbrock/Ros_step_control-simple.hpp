#ifndef Ros_step_control
#define Ros_step_control
#include "Ros_class.hpp"

#define max(a,b)  (a <= b) ? b : a


/*-----------------------Begin: step_control---------------------------------*/
Ros_Template
void Ros_Namespace::step_control(){
    LD Delta=0.;
    LD _sc;
    LD fac=beta;
    
    for (int eq = 0; eq < N_eqs; eq++){
        _sc=max(std::abs( ynext[eq] ), std::abs( yprev[eq] ));
        _sc=abs_tol+rel_tol*_sc;
        Delta+= (abs_delta[eq]/_sc)*(abs_delta[eq]/_sc);  
    ;}

    Delta=std::sqrt(1./N_eqs*Delta);
    if(Delta<1) { h_stop=true ; }

    //step size cotrol from "Solving Ordinary Differential Equations I"
    fac*=std::pow( Delta , -1./((LD) RK_method::p + 1.));
    if(fac> fac_max){fac = fac_max;}
    if(fac< fac_min){fac = fac_min;}
    
    h= h*fac ;

    if (h>hmax ){ h=hmax; h_stop=true;}
    if (h<hmin ){ h=hmin; h_stop=true;}

    if(h_stop){delta_acc=Delta;}
    if (tn+h>tmax ){ h=tmax-tn;}
}
/*-----------------------End: step_control---------------------------------*/


#endif
