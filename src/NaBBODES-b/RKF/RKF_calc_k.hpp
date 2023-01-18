#ifndef RKF_calc_k
#define RKF_calc_k
#include "RKF_class.hpp"

/*-----------------------Begin: calc_k---------------------------------*/
template<unsigned int N_eqs, class RK_method, class LD>
void RKF<N_eqs, RK_method, LD>::calc_k(){
    std::array<LD,N_eqs> yn;//thi i here to hold ynext + sum a*k
    std::array<LD,N_eqs> fyn;//this is here to get dydt in each step


    // Or for the shake of simplicity, calculae all of them in one loop (shouldn't be slower since the sum_ak for stage=0 should'n realy do anything).
    // claculate the \vec{k}_i
    for (unsigned int stage = 0; stage < RK_method::s; stage++){
        // first we need the sum_{j}^{stage-1}a_{stage,j}\vec{k}_j
        sum_ak(stage);
        // then we need \vec{y}+sum_{j}^{stage-1}a_{stage,j}\vec{k}_j (so fill yn with this)
        for (unsigned int eq = 0; eq < N_eqs; eq++){yn[eq]=yprev[eq]+ak[eq]*h;}
        // then calculate f(\vec{y}+sum_{j}^{stage-1}a_{stage,j}\vec{j}, tn + c[stage]*h )
        dydt(fyn, yn,tn+h*RK_method::c[stage] );
        // now we can fill \vec{k}[stage]
        for(unsigned int eq = 0; eq < N_eqs; eq++){ k[eq][stage]=fyn[eq]; }
    }
}
/*-----------------------End: calc_k---------------------------------*/
#endif