#ifndef RKF_constructor
#define RKF_constructor
#include "RKF_class.hpp"

//The constructor. Remember that N has default value
RKF_Template
RKF_Namespace::RKF(diffeq dydt, LD (&init_cond)[N_eqs] , LD tmax,
    LD initial_step_size, LD minimum_step_size, LD maximum_step_size,int maximum_No_steps, 
    LD absolute_tolerance,LD relative_tolerance,LD beta,LD fac_max, LD fac_min){
    // Initialize inputs
    this->dydt=dydt;
    this->tmax=tmax;
    this->h=initial_step_size;
    this->hmin=minimum_step_size;
    this->hmax=maximum_step_size;
    this->max_N=maximum_No_steps;
    this->abs_tol=absolute_tolerance;
    this->rel_tol=relative_tolerance;
    this->beta=beta;
    this->fac_max=fac_max;
    this->fac_min=fac_min;

    // ---------------------------------------------------------------------------------- //
    (this->time).push_back(0);
    //define yprev[N_eqs]. It is also good to initialize ynext.
    for(int i = 0; i < N_eqs ;++i) {
        this->yprev[i]=init_cond[i];
        this->ynext[i]=init_cond[i];
        (this->solution)[i].push_back( init_cond[i]);
        (this->error)[i].push_back(0);
    }

    // ---------------------------------------------------------------------------------- //

    // define k[N_eqs][method.s]. Also put k=0 for definiteness.
    this->k=new LD*[N_eqs];
    for(int i = 0; i < N_eqs ;++i) {
        this->k[i] = new LD[ RK_method::s];
        for(int j =0 ; j<(RK_method::s)-1; j++ ){ this->k[i][j]=0; }
    } 

    //initialize tn, current_step, and End
    this->tn=0;
    //initialize delta_acc
    delta_acc=1.;

};

//The destructor
RKF_Template
RKF_Namespace::~RKF(){
    delete[] this->k;
};

#endif