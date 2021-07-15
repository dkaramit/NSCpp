#ifndef RKF_calc_k
#define RKF_calc_k
#include "RKF_class.hpp"

/*-----------------------Begin: calc_k---------------------------------*/
RKF_Template
void RKF_Namespace::calc_k(){
    LD yn[N_eqs];//thi i here to hold ynext + sum a*k
    LD fyn[N_eqs];//this is here to get dydt in each step


    // Or for the shake of simplicity, calculae all of them in one loop (shouldn't be slower since the sum_ak for stage=0 should'n realy do anything).
    // claculate the \vec{k}_i
    for (int stage = 0; stage < RK_method::s; stage++){
        // first we need the sum_{j}^{stage-1}a_{stage,j}\vec{k}_j
        sum_ak(stage);
        // then we need \vec{y}+sum_{j}^{stage-1}a_{stage,j}\vec{k}_j (so fill yn with this)
        for (int eq = 0; eq < N_eqs ; eq++){yn[eq]=yprev[eq]+ak[eq]*h;}
        // then calculate f(\vec{y}+sum_{j}^{stage-1}a_{stage,j}\vec{j}, tn + c[stage]*h )
        dydt(fyn, yn,tn+h*(RK_method::c)[stage] );
        // now we can fill \vec{k}[stage]
        for( int eq = 0; eq < N_eqs; eq++ ){ k[eq][stage]=fyn[eq]; }
    }
}
/*-----------------------End: calc_k---------------------------------*/
#endif