#ifndef COSMO_head
#define COSMO_head

#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include<string>

#include "src/Interpolation/Interpolation.hpp"

namespace mimes{    
    template<class LD>
    class Cosmo {
        using VecLD=std::vector<LD>;
        
        protected:
        CubicSpline<LD> h,g;
        VecLD Ttab,htab,gtab;
        LD TMin, TMax, hMin, hMax, gMin, gMax;
        LD Mp;
        public:

        constexpr static  LD T0= 2.7255 * 8.62e-14; //Today's CMB temperature in GeV
        constexpr static  LD h_hub=0.674; //dimensionless hubble parameter
        constexpr static  LD rho_crit=(0.197*1e-13)*(0.197*1e-13)*(0.197*1e-13)*(1.05*1e-5)*h_hub*h_hub; //critical density today in GeV^4
        constexpr static  LD relicDM_obs=0.12; //DM relic abandance today
        constexpr static LD mP=1.22e19;//Planck mass

        Cosmo(std::string path, LD minT=0, LD maxT=1e10){    
            /*
            path is the path of the data
            minT (maxT) the minimum (maximum) T you need for the interpolation
            */
            // the data are assumed to be: T  h  g
            // the currently used data are taken from arXiv: 2005.03544 
            
            unsigned int N=0;
            LD T,heff,geff;
            std::ifstream data_file(path,std::ios::in);

            LD T_prev=-1;
            while (not data_file.eof()){
                data_file>>T;
                data_file>>heff;
                data_file>>geff;
                
                
                if(T>=minT and T<=maxT){
                    
                    //if there is an empty line theta does not change, so do skip it.
                    if(N>1 and T==T_prev){continue;}

                    Ttab.push_back(T);
                    htab.push_back(heff);
                    gtab.push_back(geff);

                    N++;
                }
                T_prev=T;
            }
            data_file.close();

            TMin=Ttab[0];
            TMax=Ttab[N-1];
            hMin=htab[0];
            hMax=htab[N-1];
            gMin=gtab[0];
            gMax=gtab[N-1];

            this->h=CubicSpline<LD>(&Ttab,&htab);
            this->g=CubicSpline<LD>(&Ttab,&gtab);
        }

        LD heff(LD T){
            if(T>=TMax){return hMax;}
            if(T<=TMin){return hMin;}
            return h(T);
        }

        LD dh(LD T){
            if(T>=TMax){return 1;}
            if(T<=TMin){return 1;}
            return 1.+1/3.*T/h(T)*h.derivative_1(T);    
        }
        
        LD geff(LD T){
            if(T>=TMax){return gMax;}
            if(T<=TMin){return gMin;}
            return g(T);
        }
        LD dgeffdT(LD T){
            if(T>=TMax){return 0;}
            if(T<=TMin){return 0;}
            return g.derivative_1(T);
        }
        LD dheffdT(LD T){
            if(T>=TMax){return 0;}
            if(T<=TMin){return 0;}
            return h.derivative_1(T);
        }

        LD s(LD T){
            //s=\dfrac{2\pi^2}{45} h_{eff} T^3.
            return (2*M_PI*M_PI)/(45.)*heff(T)*T*T*T;
        }

        LD rhoR(LD T){
            //\rho_R=\dfrac{\pi^2}{30} g_{eff} T^4
            return M_PI*M_PI/30.*geff(T)*T*T*T*T ;
        }


        LD Hubble(LD T){
            //H=\sqrt{ \dfrac{8\pi}{3 M_p^2} \rho_R(T) }
            return std::sqrt(8*M_PI/3. *rhoR(T) )/mP ;
        }
    };
};

#endif