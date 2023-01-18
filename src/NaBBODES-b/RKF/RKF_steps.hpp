#ifndef RKF_steps
#define RKF_steps
#include "RKF_class.hpp"



/*---------------------------------------------------Begin: Get next step-------------------------------------------------------------------------------*/
template<unsigned int N_eqs, class RK_method, class LD>
void RKF<N_eqs, RK_method, LD>::next_step(){
    //set h_stop=false, to start looking for stepsize
    h_stop=false;
    
    h_old=h;//for the PI controller
    delta_rej=delta_acc;//for the PI controller

    //calculate ynext and ynext_star until h_stop=true 
    while (true){
        // calculate \vec{k}:
        calc_k();
    
        // now we can calulate \vec{y}_{n+1}
        // for this we need sum_{i}^{s}b_{i}\vec{k}_i *h. That is, call sum_bk
        sum_bk();
        
        // having bk, we now have \vec{y}_{n+1} \vec{y}^{\star}_{n+1}. 
        for (unsigned int eq = 0; eq < N_eqs; eq++){   
            ynext[eq] =  yprev[eq] + bk[eq]*h;
            ynext_star[eq] =  yprev[eq] + bstark[eq]*h;       
            // calculate the error
            abs_delta[eq]= ynext[eq] - ynext_star[eq] ;   
        }
        
        // call step_control to see if the error is acceptable
        step_control();

        if(h_stop){break;}
    }
}
/*---------------------------------------------------End: Get next step-------------------------------------------------------------------------------*/


/*---------------------------------------------------Begin: solve-------------------------------------------------------------------------------*/
template<unsigned int N_eqs, class RK_method, class LD>
void RKF<N_eqs, RK_method, LD>::solve(){
    unsigned int current_step=0;
    while (true){
        //increase current_step
        current_step++;
        // check if it's done
        if(tn>=tmax  or current_step == max_N){break;}
        // run next step
        next_step();

        // set previous y to last one
        for (unsigned int eq = 0; eq < N_eqs; eq++){yprev[eq]=ynext[eq];}
        // increase time
        tn+=h;

        // store everything
        time.push_back(tn);
        for (unsigned int eq = 0; eq < N_eqs; eq++){ 
            solution[eq].push_back( ynext[eq] ); 
            error[eq].push_back(ynext[eq] - ynext_star[eq]);
        }
    }
} 
/*---------------------------------------------------End: solve-------------------------------------------------------------------------------*/




#endif
