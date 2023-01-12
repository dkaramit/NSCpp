#ifndef SYSTEM_RadPhi
#define SYSTEM_RadPhi
#include <cmath>
#include <array>

#include"src/Cosmo/Cosmo.hpp"
#include "src/misc_dir/path.hpp"


namespace nsc{
    constexpr unsigned int Neqs=2;
    template<class LD> using  Array = std::array<LD,Neqs>;

    template<class LD>
    class RadPhi{
        LD Gamma, c;
        LD Ti, rhoPhii;
    public:
        RadPhi()=default;
        ~RadPhi()=default;
        Cosmo<LD> *plasma;
        
        RadPhi(LD Gamma, LD c, LD Ti,  LD rhoPhii, Cosmo<LD> *plasma){
            this->Gamma = Gamma;
            this->c = c;
            this->Ti=Ti;
            this->rhoPhii=rhoPhii;
            this->plasma=plasma;
        };

        void operator()(Array<LD> &lhs, Array<LD> &y, LD u)
        {
            LD _T, _H, _rhoR,  _s, _dh, _rhoPhi;

            // T=T_i e^{-u} f_R(u)
            _T=Ti*std::exp(y[0]-u);
    
            // \rho_\Phi=\rho_{\Phi,i} e^{-c u} f_\Phi(u)
            _rhoPhi=rhoPhii*exp(y[1]-c*u);

            _s=plasma->s(_T);
            _rhoR = plasma->rhoR(_T);
            _dh=plasma->dh(_T);
            _H = std::sqrt(8. * M_PI / (3. * Cosmo<LD>::mP * Cosmo<LD>::mP) * (_rhoR + _rhoPhi));
            
            lhs[0] = 1 -1/_dh +  1/3.*Gamma*_rhoPhi/(_H*_s*_T*_dh); // dlogf_R/du  
            lhs[1] = - Gamma / _H; // dlogf_Phi/du
        };
    };
}

#endif