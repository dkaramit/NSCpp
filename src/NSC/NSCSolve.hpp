#ifndef SolveRadPhi_included
#define SolveRadPhi_included
#include <iostream>

#include <cmath>
#include <functional>
#include <vector>
#include <array>

using std::exp;
using std::log;
using std::vector;
using std::array;


#include "RKF_class.hpp"
#include "RKF_costructor.hpp"
#include "RKF_calc_k.hpp"
#include "RKF_sums.hpp"
#include "RKF_step_control-simple.hpp"
// #include "RKF_step_control-PI.hpp"
#include "RKF_steps.hpp"
#include "RKF_METHOD.hpp"

#include "Interpolation.hpp"


#include "NSC.hpp"

/*CosmoFuncs stuff*/
#include"src/Cosmo/Cosmo.hpp"
#include"src/AxionMass/AxionMass.hpp"



/*================================*/
#define METHOD_NSC  DormandPrince

#define NSC_minimum_step_size 1e-8
#define NSC_initial_step_size 1e-4
#define NSC_maximum_step_size 1e-3
#define NSC_maximum_No_steps int(1e7)
#define NSC_absolute_tolerance 1e-11
#define NSC_relative_tolerance 1e-11
#define NSC_beta 0.9
#define NSC_fac_max 1.01
#define NSC_fac_min 0.95
/*================================*/

template<class LD>
class NSC{
    using sys=std::function<void (Array<LD> &lhs, Array<LD> &y, LD t)>;
    using Solver=RKF<sys, Neqs, METHOD_NSC<LD>, LD>;
    
    public:

    LD TEND,c,Ti,ratio,umax,TSTOP;
    vector<vector<LD>> results;

    
    LD T_int_stop,logH2_int_stop,dlogH2du_int_stop; 
    LD T_int_ini,logH2_int_ini,dlogH2du_int_ini; 
    LD TE1,TE2,TD1,TD2,aE1,aE2,aD1,aD2;

    NSC(){};
    ~NSC(){};
    NSC(LD TEND, LD c, LD Ti, LD ratio, LD umax, LD TSTOP){
        this->TEND=TEND;
        this->c=c;
        this->Ti=Ti;
        this->ratio=ratio;
        this->umax=umax;
        this->TSTOP=TSTOP;
    };


    void solve();
};



template<class LD>
void NSC<LD>::solve(){ 
    LD Gamma = Hubble(TEND) ;
    LD rhoPhii = rhoR(Ti) * ratio;

    /*================================*/
    Array<LD> y0={0.,0.}; 
    /*================================*/
    
    RadPhi<LD> BE(Gamma, c, Ti,  rhoPhii);
    sys Boltzmann = [&BE](Array<LD> &lhs, Array<LD> &y, LD t){BE(lhs, y, t);};

    Solver System(Boltzmann, y0,-umax,
                    NSC_initial_step_size, NSC_minimum_step_size, NSC_maximum_step_size, NSC_maximum_No_steps,
                    NSC_absolute_tolerance, NSC_relative_tolerance, NSC_beta, NSC_fac_max, NSC_fac_min);


    TE1=Ti,TE2=Ti,TD1=Ti,TD2=Ti;
    aE1=1,aE2=1,aD1=1,aD2=1;
    LD _T,_rhoR,_rhoPhi,_a,_u,_H;
    int pE=0,pD=0;

    results.resize(0);

    
    _H=sqrt( (8*M_PI)/(3*mP*mP)* ( rhoR(Ti)   +  rhoPhii )  );
    results.push_back(vector<LD>{1.,0,Ti,rhoR(Ti),rhoPhii,2*std::log(_H)});

    unsigned int current_step=0;
    

    while (true){
        current_step++;
        if( System.tn>=System.tmax  or current_step == System.max_N  ) {   break ;}

        System.next_step();
        for (int eq = 0; eq < 2; eq++){System.yprev[eq]=System.ynext[eq];}
        // increase time
        System.tn+=System.h;

        _u=System.tn * (-1) ;
        _a=exp( -_u );
        _T=Ti*exp(System.ynext[0]+_u);

        if(_T<TSTOP){break;}

        _rhoPhi = rhoPhii*exp(System.ynext[1]+c*_u);
        
        _rhoR = rhoR(_T);
        _H=sqrt( (8*M_PI)/(3*mP*mP)* ( _rhoR   +  _rhoPhi )  );
        results.push_back(vector<LD>{_a,System.tn,_T,_rhoR,_rhoPhi,2*std::log(_H)}); 

        
        if(pE==0){  if(_rhoR < _rhoPhi){ TE1 = _T; aE1 = _a; pE++;}   }
        if(pE==1){  if(_rhoR > _rhoPhi){ TE2 = _T; aE2 = _a; pE++;}   }
        
        if(pD==0){  if(Gamma/_H*_rhoPhi/_rhoR>4./10. ){ TD1 = _T; aD1 = _a; pD++;}   }
        if(pD==1){  if(Gamma/_H*_rhoPhi/_rhoR<4./10. ){ TD2 = _T; aD2 = _a; pD++;}   }
    }

    N=results.size();
};


#endif