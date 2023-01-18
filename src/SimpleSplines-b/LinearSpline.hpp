#ifndef LinearSpline_head
#define LinearSpline_head

#include<cmath>
#include<vector>
#include"Interpolation.hpp"


template<class LD>
class LinearSpline: public Interpolation<LD>{
    using VecLD=std::vector<LD>;
    using un_int=unsigned int;


    public:
    LinearSpline()=default;
    
    LinearSpline(VecLD *x,VecLD *y): Interpolation<LD>(x,y){};

    LD operator()(LD x){
        /*
        1st order spline
        */
        un_int i=this->bSearch(x);
        LD a=((*this->X)[i+1]-x)/((*this->X)[i+1]-(*this->X)[i]);
        return a*(*this->Y)[i]+(1-a)*(*this->Y)[i+1];
    }

    virtual LD derivative_1(LD x){
        /*
        1st order spline of the first derivative.
        It is better than taking the derivative of operator()
        */
        un_int i=this->bSearch(x);
        LD xi=(*this->X)[i],yi=(*this->Y)[i];
        LD xj=(*this->X)[i+1],yj=(*this->Y)[i+1];
        
        if (i==this->N-2) {return (yi-yj)/(xi-xj);}
        
        LD a=(xj-x)/(xj-xi);
        LD dydx0=(yi-yj)/(xi-xj);
        LD dydx1=(yj-(*this->Y)[i+2])/(xj-(*this->X)[i+2]);
    
        return a*dydx0+(1-a)*dydx1;
    }

    virtual LD derivative_2(LD x){return 0;}
     



};


#endif