#ifndef SolveRadPhi_included
#define SolveRadPhi_included
#include <cmath>
#include <string>
#include <vector>
#include <type_traits>

#include "src/NSC/RadPhi.hpp"


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


namespace nsc{
    template<bool C, typename T1, typename T2>
    struct IF{using type=T1;};

    template<typename T1, typename T2>
    struct IF<false,T1,T2>{using type=T2;};
}

namespace nsc{

    /*
        A class that holds the arguments of the ODE solvers


        initial_stepsize: initial step the solver takes.
        maximum_stepsize: This limits the sepsize to an upper limit.
        minimum_stepsize: This limits the sepsize to a lower limit.
        absolute_tolerance: absolute tolerance of the RK solver.
        relative_tolerance: relative tolerance of the RK solver.
        Note:
        Generally, both absolute and relative tolerances should be 1e-8.
        In some cases, however, one may need more accurate result. Whatever the case, if the
        tolerances are below 1e-8, long doubles *must* be used.
        beta: controls how agreesive the adaptation is. Generally, it should be around but less than 1.
        fac_max,  fac_min: the stepsize does not increase more than fac_max, and less than fac_min.
        This ensures a better stability. Ideally, fac_max=inf and fac_min=0, but in reality one must
        tweak them in order to avoid instabilities.
        maximum_No_steps: maximum steps the solver can take Quits if this number is reached even if integration
        is not finished.
    */
    template<class LD>
    struct solverArgs{
        LD initial_step_size=1e-2;
        LD minimum_step_size=1e-8;
        LD maximum_step_size=1e-2; 
        LD absolute_tolerance=1e-8;
        LD relative_tolerance=1e-8;
        LD beta=0.9;
        LD fac_max=1.2;
        LD fac_min=0.8;
        unsigned int maximum_No_steps= static_cast<unsigned int>(1e7);
    };


    template<class LD, const int Solver, class Method>
    class Evolution{
        using RKSolver = typename IF<Solver==1,
            Ros<Neqs, Method, Jacobian<Neqs, LD>, LD>,
            typename IF<Solver==2,RKF<Neqs, Method, LD>,void>::type>::type; 
        
        static_assert(std::is_floating_point<LD>::value, "Use only floating point numbers!");

        bool check_run=false;
        
        void reset(){
            u.clear();T.clear();rhoPhi.clear();
            dT.clear();drhoPhi.clear();
            pointSize=u.size();
            TE1=0;TE2=0;TD1=0;TD2=0;uE1=0;uE2=0;uD1=0;uD2=0;
            check_run=false;
        }

        public:

        std::vector<LD> u,T,rhoPhi;
        std::vector<LD> dT, drhoPhi;

        unsigned int pointSize;
        LD TE1=0,TE2=0,TD1=0,TD2=0,uE1=0,uE2=0,uD1=0,uD2=0;

        Evolution()=default;
        ~Evolution()=default;
        Evolution(const Evolution &)=delete;
        Evolution(Evolution &&)=delete;
        Evolution& operator=(const Evolution &)=delete;
        Evolution& operator=(Evolution &&)=delete;

        /*
        solveNSC: solves teh system of equations for NSC. It returns true if the integration seems to have completed correctly (ie last step was taken at a temperature T<TSTOP).
        
        With:
        TEND: TEND [GeV] is defined from Gamma_Phi=H_R(TEND) [H_R is the Hubble rate in RD Universe]
        c: characterises the equation of state of Phi, with c=3(1+omega) and p=omega rho_Phi
        Ti, ratio: ratio = rho_Phi/rho_R at temperature Ti [GeV]. These are the initial conditions
        TSTOP: if the temperature drops below this, integration stops.
        umax: if u>umax the integration stops (rempember that u=log(a/a_i)).
        plasma: pointer to instance of Cosmo class.
        -----------Optional arguments------------------------
        Either a solverArgs instance or a initialized using "Designated initializers"
        For example you can use it as 

        BE.solveNSC(TEND,c,Ti,ratio,TSTOP,umax,&plasma,
                {
                    .initial_step_size=1e-2, .minimum_step_size=1e-5, .maximum_step_size=1e-2,
                    .absolute_tolerance=1e-3, .relative_tolerance=1e-3, .beta=0.95, 
                    .fac_max=1.1, .fac_min=0.8, .maximum_No_steps=100000
                })

        initial_stepsize: initial step the solver takes.
        maximum_stepsize: This limits the sepsize to an upper limit.
        minimum_stepsize: This limits the sepsize to a lower limit.
        absolute_tolerance: absolute tolerance of the RK solver.
        relative_tolerance: relative tolerance of the RK solver.
        Note:
        Generally, both absolute and relative tolerances should be 1e-8.
        In some cases, however, one may need more accurate result. Whatever the case, if the
        tolerances are below 1e-8, long doubles *must* be used.
        beta: controls how agreesive the adaptation is. Generally, it should be around but less than 1.
        fac_max,  fac_min: the stepsize does not increase more than fac_max, and less than fac_min.
        This ensures a better stability. Ideally, fac_max=inf and fac_min=0, but in reality one must
        tweak them in order to avoid instabilities.
        maximum_No_steps: maximum steps the solver can take Quits if this number is reached even if integration
        is not finished.
        */
        bool solveNSC(const LD &TEND, const LD &c, const LD &Ti, const LD &ratio, const LD &TSTOP, const LD &umax,  Cosmo<LD> *plasma, const solverArgs<LD> &args={});
        
        /*Alternative definition of solveNSC without aggregation*/
        // bool solveNSC(const LD &TEND, const LD &c, const LD &Ti, const LD &ratio, const LD &TSTOP, const LD &umax, Cosmo<LD> *plasma,
        //               const LD &initial_step_size = 1e-2,const LD &minimum_step_size = 1e-8,
        //               const LD &maximum_step_size = 1e-2,const LD &absolute_tolerance = 1e-8,
        //               const LD &relative_tolerance = 1e-8,const LD &beta = 0.9,
        //               const LD &fac_max = 1.2,const LD &fac_min = 0.8,
        //               const unsigned int &maximum_No_steps = static_cast<unsigned int>(1e7)
        //              ){
        //                 return solveNSC(TEND, c, Ti, ratio, TSTOP, umax, plasma,
        //                                 {
        //                                     .initial_step_size=initial_step_size, .minimum_step_size=minimum_step_size,
        //                                     .maximum_step_size=maximum_step_size, .absolute_tolerance=absolute_tolerance, 
        //                                     .relative_tolerance=relative_tolerance, .beta=beta, 
        //                                     .fac_max=fac_max, .fac_min=fac_min, .maximum_No_steps=maximum_No_steps
        //                                 }                       
        //                                );
        //             }
    };


    template<class LD, const int Solver, class Method>
    bool Evolution<LD,Solver,Method>::solveNSC(const LD &TEND, const LD &c, const LD &Ti, const LD &ratio, const LD &TSTOP, const LD &umax, Cosmo<LD> *plasma, const solverArgs<LD> &args){ 
        
        LD Gamma = plasma->Hubble(TEND) ;
        LD rhoPhii = plasma->rhoR(Ti) * ratio;

        /*================================*/
        Array<LD> y0={0.,0.}; 
        /*================================*/

        if(check_run){this->reset();}

        RKSolver System(RadPhi<LD>(Gamma, c, Ti,  rhoPhii, plasma), y0, umax,
                        args.initial_step_size, args.minimum_step_size, args.maximum_step_size, args.maximum_No_steps,
                        args.absolute_tolerance, args.relative_tolerance, args.beta, args.fac_max, args.fac_min);


        TE1=Ti,TE2=Ti,TD1=Ti,TD2=Ti;
        uE1=0,uE2=0,uD1=0,uD2=0;
        LD _T,_rhoR,_rhoPhi,_u,_H;
        int pE=0,pD=0;

        
        _H=std::sqrt( (8*M_PI)/(3*Cosmo<LD>::mP*Cosmo<LD>::mP)* ( plasma->rhoR(Ti)   +  rhoPhii )  );


        u.push_back(0);
        T.push_back(Ti);
        rhoPhi.push_back(rhoPhii);

        dT.push_back(0);
        drhoPhi.push_back(0);

        unsigned int current_step=0;
        

        while (true){
            current_step++;
            if( System.tn>=System.tmax  or current_step == args.maximum_No_steps  ) { break ;}

            System.next_step();


            for (int eq = 0; eq < 2; eq++){System.yprev[eq]=System.ynext[eq];}
            // increase time
            System.tn+=System.h;

            _u=System.tn ;
            _T=Ti*std::exp(System.ynext[0]-_u);
            


            if(_T<TSTOP){break;}

            _rhoPhi = rhoPhii*std::exp(System.ynext[1]-c*_u);
            

            u.push_back(_u);
            T.push_back(_T);
            rhoPhi.push_back(_rhoPhi);

            dT.push_back(_T*std::abs(System.ynext[0] - System.ynext_star[0]));
            drhoPhi.push_back(_rhoPhi*std::abs(System.ynext[1] - System.ynext_star[1]));
            
            
            _rhoR = plasma->rhoR(_T);

            _H=std::sqrt( (8*M_PI)/(3*Cosmo<LD>::mP*Cosmo<LD>::mP)* ( _rhoR   +  _rhoPhi )  );
            

            /*Find the points where the behaviour changes. Notice that for c>4, there is at most one point of equality.*/
            if(c<=4){
                if(pE==0){  if(_rhoR < _rhoPhi){ TE1 = _T; uE1 = _u; pE++;}   }//the first time \rho_R = \rho_\Phi
                if(pE==1){  if(_rhoR > _rhoPhi){ TE2 = _T; uE2 = _u; pE++;}   }//the second time \rho_R = \rho_\Phi
                
            }

            if(c>4){
                if(pE==0){  if(_rhoR > _rhoPhi){ TE1 = _T; uE1 = _u; pE++;}   }//the first time \rho_R = \rho_\Phi
            }

            if(pD==0){  if(Gamma/_H*_rhoPhi/_rhoR>4./10. ){ TD1 = _T; uD1 = _u; pD++;}   }// the first time \rho_R/H = 0.4* \rho_\Phi/\Gamma
            if(pD==1){  if(Gamma/_H*_rhoPhi/_rhoR<4./10. ){ TD2 = _T; uD2 = _u; pD++;}   }// the second time \rho_R/H = 0.4* \rho_\Phi/\Gamma
        
        }

        pointSize=u.size();
        check_run=true;

        return _T<=TSTOP;
    };
}

#endif