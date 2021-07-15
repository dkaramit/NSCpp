#ifndef Ros_calc_k
#define Ros_calc_k
#include "Ros_class.hpp"




/*-----------------------Begin: calc_Jk---------------------------------*/
Ros_Template
void Ros_Namespace::calc_Jk(){
    /*
    Calculate product of matrix with vector. 
    We use it to get J*gk.
    */
    for(int i=0; i<N_eqs ; i++){
        Jk[i]=0;
        for(int j=0; j<N_eqs ; j++){ Jk[i]+=J[i][j]*gk[j]; }
    }         
}
/*-----------------------End: calc_Jk---------------------------------*/

/*-----------------------Begin: calc_k---------------------------------*/
Ros_Template
void Ros_Namespace::calc_k(){
    // since LU decomposition is not updated as you try to find suitable step, put it ouside the step control loop (should be faster)!
    // LU();
    // calculate k for the other stages
    LD yn[N_eqs];
    // rhs is the rhs side of the equation to be solved by LU.  
    LD rhs[N_eqs];
    // to make it more clear, we are going to separate the rhs in three different parts
    LD rhs1[N_eqs],rhs2[N_eqs];

    for(int stage = 0; stage < RK_method::s; stage++){
        sum_ak(stage);
        sum_gk(stage);
        // setup the argument for dydt (it is evaluated at y_n+\sum a*k )
        for(int eq=0; eq<N_eqs ; eq++){yn[eq]=yprev[eq]+ak[eq];}
        /*--- Get the rhs terms ---*/
        // first term
        dydt(rhs1, yn, tn+RK_method::c[stage]*h);
        // second term
        for(int eq=0; eq<N_eqs ; eq++) { rhs2[eq] =  (RK_method::gamma+sum_gamma[stage])*dfdt[eq];}
        // third term
        calc_Jk();
        // then the rhs becomes
        for(int eq=0; eq<N_eqs ; eq++) { rhs[eq] = h*rhs1[eq] + h*h*rhs2[eq] + h*Jk[eq];}
        /*------------------------*/


        // Solving  L*U*P*lu_sol=rhs is equivalent. Using the inverse, should be a bit faster. 
        // Solve_LU<N_eqs>(L,U,P,rhs, lu_sol);
        
        dot<N_eqs,LD>(_inv,rhs, lu_sol);
        for( int eq = 0; eq < N_eqs; eq++ ){ k[eq][stage]=lu_sol[eq]; }
    }
}
/*-----------------------End: calc_k---------------------------------*/
#endif