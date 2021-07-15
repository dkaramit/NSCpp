#ifndef Ros_steps
#define Ros_steps
#include "Ros_class.hpp"



/*---------------------------------------------------Begin: Get next step-------------------------------------------------------------------------------*/
Ros_Template
void Ros_Namespace::next_step(){
    //set h_stop=false, to start looking for stepsize
    h_stop=false;
    
    h_old=h;//for the PI controller
    delta_rej=delta_acc;//for the PI controller
    //calculate the LU decomposition of (1-\gamma*h*J) and find its inverse before you enter the loop. 
    LU();
    //calculate ynext and ynext_star until h_stop=true 
    while (true){
        // calculate \vec{k}:
        calc_k();

        // now we can calulate \vec{y}_{n+1}
        // for this we need sum_{i}^{s}b_{i}\vec{k}_i *h. That is, call sum_bk
        sum_bk();

        // having bk, we now have \vec{y}_{n+1} \vec{y}^{\star}_{n+1}. 
        for (int eq = 0; eq < N_eqs; eq++){   
            ynext[eq] =  yprev[eq] + bk[eq];
            ynext_star[eq] =  yprev[eq] + bstark[eq];       
            // calculate the error
            abs_delta[eq]= ynext[eq] - ynext_star[eq] ;
        }
        
        // call step_control to see if the error is acceptable
        step_control();
        
        if(h_stop){break; }
    }
}
/*---------------------------------------------------End: Get next step-------------------------------------------------------------------------------*/



/*---------------------------------------------------Begin: solve-------------------------------------------------------------------------------*/

Ros_Template
void Ros_Namespace::solve(){
    int current_step=0;
    while (true){
        //increase current_step
        current_step++;
        // check if it's done
        if(tn>=tmax  or current_step == max_N){break;}
        // run next step
        next_step();

        // set previous y to last one
        for (int eq = 0; eq < N_eqs; eq++){yprev[eq]=ynext[eq];}
        // increase time
        tn+=h;

        // store everything
        time.push_back(tn);
        for (int eq = 0; eq < N_eqs; eq++){ 
            solution[eq].push_back( ynext[eq] ); 
            error[eq].push_back(ynext[eq] - ynext_star[eq]);
        }
    }
} 
/*---------------------------------------------------End: solve-------------------------------------------------------------------------------*/




#endif
