#ifndef COSMO_head
#define COSMO_head

#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include<string>

#include "src/SimpleSplines/Interpolation.hpp"

namespace nsc{    

    template<class LD>
    class Cosmo {
        private:
        using VecLD=std::vector<LD>;
        CubicSpline<LD> log_h,log_g;
        VecLD log_Ttab,log_htab,log_gtab;
        LD TMin, TMax, hMin, hMax, gMin, gMax;

        public:

        Cosmo()=delete;
        ~Cosmo()=default;
        Cosmo(const Cosmo &)=default;
        Cosmo(Cosmo &&)=default;
        Cosmo& operator=(const Cosmo &)=default;
        Cosmo& operator=(Cosmo &&)=default;

        constexpr static  LD T0= 2.7255 * 8.62e-14; //Today's CMB temperature in GeV
        constexpr static  LD h_hub=0.674; //dimensionless hubble parameter
        constexpr static  LD rho_crit=(0.197*1e-13)*(0.197*1e-13)*(0.197*1e-13)*(1.05*1e-5)*h_hub*h_hub; //critical density today in GeV^4
        constexpr static  LD relicDM_obs=0.12; //DM relic abandance today
        constexpr static LD mP=1.22e19;//Planck mass

        Cosmo(std::string path, LD minT, LD maxT){    
            /*
            path is the path of the data
            minT (maxT) the minimum (maximum) T you need for the interpolation
            */
            // the data are assumed to be: T [GeV] h g
            
            unsigned int N=0;
            LD T,heff,geff;
            std::ifstream data_file(path,std::ios::in);
            
            if(not data_file.good()) {
                std::cerr << path << " does not exist.";
                std::cerr <<" Unable to interpolate plasma relavistic degrees of freedom.\n"; 
                exit(1);
            }
            LD T_prev=-1;
            while (not data_file.eof()){
                data_file>>T;
                data_file>>heff;
                data_file>>geff;
                
                
                if(T==0){continue;}// T=0 is not valid
                if(T>=minT and T<=maxT){
                    
                    if(N>1 and T==T_prev){continue;}

                    log_Ttab.push_back(std::log(T));
                    log_htab.push_back(std::log(heff));
                    log_gtab.push_back(std::log(geff));

                    N++;
                }
                T_prev=T;
            }
            data_file.close();

            TMin=std::exp(log_Ttab[0]);
            TMax=std::exp(log_Ttab[N-1]);
            hMin=std::exp(log_htab[0]);
            hMax=std::exp(log_htab[N-1]);
            gMin=std::exp(log_gtab[0]);
            gMax=std::exp(log_gtab[N-1]);

            this->log_h=CubicSpline<LD>(&log_Ttab,&log_htab);
            this->log_g=CubicSpline<LD>(&log_Ttab,&log_gtab);
        }

        LD heff(LD T){
            if(T>=TMax){return hMax;}
            if(T<=TMin){return hMin;}
            return std::exp(log_h(std::log(T)));
        }

        LD dh(LD T){
            if(T>=TMax){return 1;}
            if(T<=TMin){return 1;}
            return 1.+1/3.*log_h.derivative_1(std::log(T));    
        }
        
        LD geff(LD T){
            if(T>=TMax){return gMax;}
            if(T<=TMin){return gMin;}
            return std::exp(log_g(std::log(T)));
        }
        LD dgeffdT(LD T){
            if(T>=TMax){return 0;}
            if(T<=TMin){return 0;}
            return geff(T)/T*log_g.derivative_1(std::log(T));
        }
        LD dheffdT(LD T){
            if(T>=TMax){return 0;}
            if(T<=TMin){return 0;}
            return heff(T)/T*log_h.derivative_1(std::log(T));
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