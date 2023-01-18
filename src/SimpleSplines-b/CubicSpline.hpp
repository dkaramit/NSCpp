#ifndef CubicSpline_head
#define CubicSpline_head

#include<cmath>
#include<vector>
#include<array>
#include"Interpolation.hpp"

template<class LD>
class CubicSpline: public Interpolation<LD>{
    using VecLD=std::vector<LD>;
    using un_int=unsigned int;

    protected:
    VecLD y2;

    public:
    CubicSpline()=default;

    CubicSpline(VecLD *x,VecLD *y): Interpolation<LD>(x,y){
        y2.reserve(this->N);
        for(un_int i=0; i<this->N-1; ++i){y2.push_back(0);}
        paramCalc();
    };

    void paramCalc();




    LD operator()(LD x){
        /**/
        un_int i=this->bSearch(x);
        un_int klo=i;
        un_int khi=i+1;

        LD h=(*this->X)[khi]-(*this->X)[klo];
        
        LD a=((*this->X)[khi]-x)/h;
        LD b=(x-(*this->X)[klo])/h;

        LD y=a*(*this->Y)[klo]+b*(*this->Y)[khi];
        y+=(a*a*a-a)*y2[klo]*(h*h)/6.;
        y+=(b*b*b-b)*y2[khi]*(h*h)/6.;
        
        return y;
    }

    LD derivative_1(LD x){
        /*The first derivative of  operator() obtained analytically.*/
        un_int i=this->bSearch(x);
    
        un_int klo=i;
        un_int khi=i+1;

        LD h=(*this->X)[khi]-(*this->X)[klo];
        
        LD a=((*this->X)[khi]-x)/h;
        LD b=(x-(*this->X)[klo])/h;

        LD dydx=((*this->Y)[khi]-(*this->Y)[klo])/h;
        dydx+=-(3*a*a-1)*y2[klo]*h/6.;
        dydx+=(3*b*b-1)*y2[khi]*h/6.;
        
        return dydx;
    }

    
    LD derivative_2(LD x){
        /*        
        The second derivative of  operator() obtained analytically.
        */
        un_int i=this->bSearch(x);
        LD a=((*this->X)[i+1]-x)/((*this->X)[i+1]-(*this->X)[i]);
        return a*y2[i]+(1-a)*y2[i+1];
    }


};





template<class LD>
void CubicSpline<LD>::paramCalc(){
    VecLD u(this->N-1,0);
    LD sig,p;
    
    y2[0]=0;
    y2[this->N-1]=0;

    for(un_int i=1; i<this->N-1; ++i){
        sig=((*this->X)[i]-(*this->X)[i-1])/((*this->X)[i+1]-(*this->X)[i-1]);
        p=sig*y2[i-1]+2;
        y2[i]=(sig-1)/p;
        u[i]=((*this->Y)[i+1]-(*this->Y)[i])/((*this->X)[i+1]-(*this->X)[i]) - ((*this->Y)[i]-(*this->Y)[i-1])/((*this->X)[i]-(*this->X)[i-1]);
        u[i]=(6*u[i]/((*this->X)[i+1]-(*this->X)[i-1])-sig*u[i-1])/p;
    }
    
    // we need k=N-2,N-1,...0
    for(int k=this->N-2; k>=0; --k){ 
        y2[k]=y2[k]*y2[k+1]+u[k];
    }

}



#endif