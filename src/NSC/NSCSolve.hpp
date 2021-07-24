#ifndef SolveRadPhi_included
#define SolveRadPhi_included
#include <cmath>
#include <string>
#include <functional>
#include <vector>

#include "src/NSC/NSC.hpp"


//----Solver-----//
#include "src/NaBBODES/RKF/RKF_class.hpp"
#include "src/NaBBODES/RKF/RKF_costructor.hpp"
#include "src/NaBBODES/RKF/RKF_calc_k.hpp"
#include "src/NaBBODES/RKF/RKF_sums.hpp"
#include "src/NaBBODES/RKF/RKF_step_control-simple.hpp"
// #include "src/NaBBODES/RKF/RKF_step_control-PI.hpp"
#include "src/NaBBODES/RKF/RKF_steps.hpp"
#include "src/NaBBODES/RKF/METHOD.hpp"




//----Solver-----//
#include "src/NaBBODES/Rosenbrock/LU/LU.hpp"
#include "src/NaBBODES/Rosenbrock/Ros_class.hpp"
#include "src/NaBBODES/Rosenbrock/Ros_costructor.hpp"
#include "src/NaBBODES/Rosenbrock/Ros_LU.hpp"
#include "src/NaBBODES/Rosenbrock/Ros_calc_k.hpp"
#include "src/NaBBODES/Rosenbrock/Ros_sums.hpp"
#include "src/NaBBODES/Rosenbrock/Ros_step_control-simple.hpp"
// #include "src/NaBBODES/Rosenbrock/Ros_step_control-PI.hpp"
#include "src/NaBBODES/Rosenbrock/Ros_steps.hpp"
#include "src/NaBBODES/Rosenbrock/Jacobian/Jacobian.hpp"
#include "src/NaBBODES/Rosenbrock/METHOD.hpp"



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
        unsigned int maximum_No_steps;
        
        public:

        LD TEND,c,Ti,ratio,umax,TSTOP;
        std::vector<std::vector<LD>> points;

        unsigned int pointSize;
        LD TE1,TE2,TD1,TD2,aE1,aE2,aD1,aD2;

        /*
        NSC class constructor:
        nsc::NSC(TEND,c,Ti,ratio,umax,TSTOP, initial_step_size, minimum_step_size, maximum_step_size,
                absolute_tolerance, relative_tolerance, beta, fac_max, fac_min, maximum_No_steps)
        
        With:
        TEND: TEND [GeV] is defined from Gamma_Phi=H_R(TEND) [H_R is the Hubble rate in RD Universe]
        c: characterises the equation of state of Phi, with c=3(1+omega) and p=omega rho_Phi
        Ti, ratio: ratio = rho_Phi/rho_R at temperature Ti [GeV]. These are the initial conditions
        umax: if u>umax the integration stops (rempember that u=log(a/a_i))
        TSTOP: if the temperature drops below this, integration stops.
        -----------Optional arguments------------------------
        initial_stepsize: initial step the solver takes.
        maximum_stepsize: This limits the sepsize to an upper limit.
        minimum_stepsize: This limits the sepsize to a lower limit.
        absolute_tolerance: absolute tolerance of the RK solver.
        relative_tolerance: relative tolerance of the RK solver.
        Note:
        Generally, both absolute and relative tolerances should be 1e-8.
        In some cases, however, one may need more accurate result (eg if f_a is extremely high,
        the oscillations happen violently, and the ODE destabilizes). Whatever the case, if the
        tolerances are below 1e-8, long doubles *must* be used.
        beta: controls how agreesive the adaptation is. Generally, it should be around but less than 1.
        fac_max,  fac_min: the stepsize does not increase more than fac_max, and less than fac_min.
        This ensures a better stability. Ideally, fac_max=inf and fac_min=0, but in reality one must
        tweak them in order to avoid instabilities.
        maximum_No_steps: maximum steps the solver can take Quits if this number is reached even if integration
        is not finished.
        */

        NSC(LD TEND, LD c, LD Ti, LD ratio, LD umax, LD TSTOP,
            LD initial_step_size=1e-2, LD minimum_step_size=1e-8, LD maximum_step_size=1e-2, 
            LD absolute_tolerance=1e-8, LD relative_tolerance=1e-8,
            LD beta=0.9, LD fac_max=1.2, LD fac_min=0.8, unsigned int maximum_No_steps=10000000){
            
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

        Solver System(BE, y0,umax,
                        initial_step_size, minimum_step_size, maximum_step_size, maximum_No_steps,
                        absolute_tolerance, relative_tolerance, beta, fac_max, fac_min);


        TE1=Ti,TE2=Ti,TD1=Ti,TD2=Ti;
        aE1=1,aE2=1,aD1=1,aD2=1;
        LD _T,_rhoR,_rhoPhi,_a,_u,_H;
        int pE=0,pD=0;

        
        _H=std::sqrt( (8*M_PI)/(3*mP*mP)* ( cosmo.rhoR(Ti)   +  rhoPhii )  );
        points.push_back(std::vector<LD>{1.,Ti,rhoPhii,std::log(_H)});

        unsigned int current_step=0;
        

        while (true){
            current_step++;
            if( System.tn>=System.tmax  or current_step == maximum_No_steps  ) {   break ;}

            System.next_step();
            for (int eq = 0; eq < 2; eq++){System.yprev[eq]=System.ynext[eq];}
            // increase time
            System.tn+=System.h;

            _u=System.tn ;
            _a=std::exp( _u );
            _T=Ti*std::exp(System.ynext[0]-_u);

            if(_T<TSTOP){break;}

            _rhoPhi = rhoPhii*std::exp(System.ynext[1]-c*_u);
            
            _rhoR = cosmo.rhoR(_T);
            _H=std::sqrt( (8*M_PI)/(3*mP*mP)* ( _rhoR   +  _rhoPhi )  );
            points.push_back(std::vector<LD>{_a,_T,_rhoPhi,std::log(_H)}); 

            
            if(pE==0){  if(_rhoR < _rhoPhi){ TE1 = _T; aE1 = _a; pE++;}   }//the first time \rho_R = \rho_\Phi
            if(pE==1){  if(_rhoR > _rhoPhi){ TE2 = _T; aE2 = _a; pE++;}   }//the second time \rho_R = \rho_\Phi
            
            if(pD==0){  if(Gamma/_H*_rhoPhi/_rhoR>4./10. ){ TD1 = _T; aD1 = _a; pD++;}   }// the first time \rho_R/H = 0.4* \rho_\Phi/\Gamma
            if(pD==1){  if(Gamma/_H*_rhoPhi/_rhoR<4./10. ){ TD2 = _T; aD2 = _a; pD++;}   }// the second time \rho_R/H = 0.4* \rho_\Phi/\Gamma
        }

        pointSize=points.size();
    };
}

#endif