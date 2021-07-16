#ifndef SolveRadPhi_included
#define SolveRadPhi_included
#include <cmath>
#include <string>
#include <functional>
#include <vector>

#include "src/NSC/NSC.hpp"


//----Solver-----//
#include "src/RKF/RKF_class.hpp"
#include "src/RKF/RKF_costructor.hpp"
#include "src/RKF/RKF_calc_k.hpp"
#include "src/RKF/RKF_sums.hpp"
#include "src/RKF/RKF_step_control-simple.hpp"
// #include "src/RKF/RKF_step_control-PI.hpp"
#include "src/RKF/RKF_steps.hpp"
#include "src/RKF/METHOD.hpp"




//----Solver-----//
#include "src/Rosenbrock/LU/LU.hpp"
#include "src/Rosenbrock/Ros_class.hpp"
#include "src/Rosenbrock/Ros_costructor.hpp"
#include "src/Rosenbrock/Ros_LU.hpp"
#include "src/Rosenbrock/Ros_calc_k.hpp"
#include "src/Rosenbrock/Ros_sums.hpp"
#include "src/Rosenbrock/Ros_step_control-simple.hpp"
// #include "src/Rosenbrock/Ros_step_control-PI.hpp"
#include "src/Rosenbrock/Ros_steps.hpp"
#include "src/Rosenbrock/Jacobian/Jacobian.hpp"
#include "src/Rosenbrock/METHOD.hpp"



/*================================*/
#ifndef METHOD
    #define solver 1 
    #define METHOD RODASPR2
#endif



namespace nsc{

    template<class LD>
    class NSC{
        //-----Function type--------//
        using sys= std::function<void (Array<LD> &lhs, Array<LD> &y, LD u)>;
        
        #if solver==1
        using Solver=Ros<sys, Neqs, METHOD<LD>, Jacobian<sys, Neqs, LD>, LD>;
        #endif

        #if solver==2
        using Solver=RKF<sys, Neqs, METHOD<LD>, LD>;
        #endif

        LD initial_step_size, minimum_step_size, maximum_step_size, absolute_tolerance, relative_tolerance;
        LD beta,fac_max,fac_min;
        int maximum_No_steps;
        
        public:

        LD TEND,c,Ti,ratio,umax,TSTOP;
        std::vector<std::vector<LD>> points;

        unsigned int pointSize;
        LD TE1,TE2,TD1,TD2,aE1,aE2,aD1,aD2;


        NSC(LD TEND, LD c, LD Ti, LD ratio, LD umax, LD TSTOP,
                    LD initial_step_size=1e-2, LD minimum_step_size=1e-8, LD maximum_step_size=1e-2, 
                    LD absolute_tolerance=1e-8, LD relative_tolerance=1e-8,
                    LD beta=0.9, LD fac_max=1.2, LD fac_min=0.8, int maximum_No_steps=int(1e7)){
            this->TEND=TEND;
            this->c=c;
            this->Ti=Ti;
            this->ratio=ratio;
            this->umax=umax;
            this->TSTOP=TSTOP;

            this->initial_step_size=initial_step_size;
            this->minimum_step_size=minimum_step_size;
            this->maximum_step_size=maximum_step_size;
            this->absolute_tolerance=absolute_tolerance;
            this->relative_tolerance=relative_tolerance;
            this->beta=beta;
            this->fac_max=fac_max;
            this->fac_min=fac_min;
            this->maximum_No_steps=maximum_No_steps;
        };

        void solveNSC();
    };



    template<class LD>
    void NSC<LD>::solveNSC(){ 
        LD Gamma = cosmo.Hubble(TEND) ;
        LD rhoPhii = cosmo.rhoR(Ti) * ratio;

        /*================================*/
        Array<LD> y0={0.,0.}; 
        /*================================*/
        
        RadPhi<LD> BE(Gamma, c, Ti,  rhoPhii);
        sys Boltzmann = [&BE](Array<LD> &lhs, Array<LD> &y, LD t){BE(lhs, y, t);};

        Solver System(Boltzmann, y0,umax,
                        initial_step_size, minimum_step_size, maximum_step_size, maximum_No_steps,
                        absolute_tolerance, relative_tolerance, beta, fac_max, fac_min);


        TE1=Ti,TE2=Ti,TD1=Ti,TD2=Ti;
        aE1=1,aE2=1,aD1=1,aD2=1;
        LD _T,_rhoR,_rhoPhi,_a,_u,_H;
        int pE=0,pD=0;

        
        _H=std::sqrt( (8*M_PI)/(3*mP*mP)* ( cosmo.rhoR(Ti)   +  rhoPhii )  );
        points.push_back(std::vector<LD>{1.,Ti,rhoPhii,2*std::log(_H)});

        int current_step=0;
        

        while (true){
            current_step++;
            if( System.tn>=System.tmax  or current_step == System.max_N  ) {   break ;}

            System.next_step();
            for (int eq = 0; eq < 2; eq++){System.yprev[eq]=System.ynext[eq];}
            // increase time
            System.tn+=System.h;

            _u=System.tn * (-1) ;
            _a=std::exp( -_u );
            _T=Ti*std::exp(System.ynext[0]+_u);

            if(_T<TSTOP){break;}

            _rhoPhi = rhoPhii*std::exp(System.ynext[1]+c*_u);
            
            _rhoR = cosmo.rhoR(_T);
            _H=std::sqrt( (8*M_PI)/(3*mP*mP)* ( _rhoR   +  _rhoPhi )  );
            points.push_back(std::vector<LD>{_a,_T,_rhoPhi,2*std::log(_H)}); 

            
            if(pE==0){  if(_rhoR < _rhoPhi){ TE1 = _T; aE1 = _a; pE++;}   }
            if(pE==1){  if(_rhoR > _rhoPhi){ TE2 = _T; aE2 = _a; pE++;}   }
            
            if(pD==0){  if(Gamma/_H*_rhoPhi/_rhoR>4./10. ){ TD1 = _T; aD1 = _a; pD++;}   }
            if(pD==1){  if(Gamma/_H*_rhoPhi/_rhoR<4./10. ){ TD2 = _T; aD2 = _a; pD++;}   }
        }

        pointSize=points.size();
    };
}

#endif