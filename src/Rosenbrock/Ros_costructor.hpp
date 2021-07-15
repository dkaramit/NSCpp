#ifndef Ros_constructor
#define Ros_constructor
#include "Ros_class.hpp"



//The constructor. Remember that N has default value
Ros_Template
Ros_Namespace::Ros(diffeq dydt, LD (&init_cond)[N_eqs] , LD tmax,
    LD initial_step_size, LD minimum_step_size, LD maximum_step_size,int maximum_No_steps, 
    LD absolute_tolerance,LD relative_tolerance,LD beta,LD fac_max, LD fac_min){

    // Initialize inputs    
    this->dydt=dydt;
    this->tmax=tmax;
    this->Jac=jacobian(dydt);
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
    //define yprev[N_eqs]. It is also good to initialize ynext.
    (this->time).push_back(0);
    for(int i = 0; i < N_eqs ;++i) {
        this->yprev[i]=init_cond[i];
        this->ynext[i]=init_cond[i];
        (this->solution)[i].push_back( init_cond[i]);
        (this->error)[i].push_back(0);   
    }

    // ---------------------------------------------------------------------------------- //

    // define k[N_eqs][RK_method::s]. Initialize also k=0.
    this->k=new LD*[N_eqs];
    for(int i = 0; i < N_eqs ;++i) {
        this->k[i] = new LD[ RK_method::s];
        for(int j =0 ; j<RK_method::s; j++ ){this->k[i][j] =0;  }
    } 


    // calculate sums over gamma for all stages 
    this->sum_gamma=new LD[RK_method::s];
    for(int stage = 0; stage < RK_method::s; stage++){
        this->sum_gamma[stage]=0;
        for(int j =0 ; j<=RK_method::s-1; j++ ){ this->sum_gamma[stage]+=RK_method::g[stage][j];}
    }


    //initialize tn
    this->tn=0;
    //initialize delta_acc
    delta_acc=1.;
};

//The destructor
Ros_Template
Ros_Namespace::~Ros(){
    delete[] this->k;
    delete[] this->sum_gamma;
};

#endif