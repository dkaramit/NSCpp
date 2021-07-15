#ifndef SYSTEM_RadPhi
#define SYSTEM_RadPhi
#include <cmath>

#include"src/Cosmo/Cosmo.hpp"

using std::exp;
using std::log;

const static unsigned int Neqs=2;
template<class LD> using  Array = LD[Neqs];

template<class LD>
class RadPhi{
public:
    LD Gamma, c;
    LD Ti, rhoPhii;
    RadPhi(LD Gamma, LD c, LD Ti,  LD rhoPhii){
        this->Gamma = Gamma;
        this->c = c;
        this->Ti=Ti;
        this->rhoPhii=rhoPhii;

    };
    ~RadPhi(){};


    void operator()(Array<LD> &lhs, Array<LD> &y, LD t)
    {
        LD _T, _H, _rhoR,  _s, _dh, _rhoPhi;
        LD LogfR,LogfPhi;

        LogfR=y[0]-t;
        _T=Ti*exp(LogfR);
        LogfPhi=y[1]-c*t;
        _rhoPhi=rhoPhii*exp(LogfPhi);

        _s=s(_T);
        _rhoR = rhoR(_T);
        _dh=dh(_T);

        _H = sqrt(8. * M_PI / (3. * mP * mP) * (_rhoR + _rhoPhi));
        
        lhs[0] = 1/_dh - 1 -  1/3.*Gamma*_rhoPhi/(_H*_s*_T*_dh) ;        
        lhs[1] =  Gamma / _H;
        
        lhs[0] *=  -1.;
        lhs[1] *=  -1.;
    };
};

#endif