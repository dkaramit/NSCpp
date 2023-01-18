#ifndef InterpolationBase_head
#define InterpolationBase_head

#include<iostream>
#include<cmath>
#include<vector>

template<class LD>
class Interpolation{
    /*
    Base class for spline interpolation
    The class itself is the 0th order spline
    */

    using VecLD=std::vector<LD>;
    using un_int=unsigned int;
    
    protected:
    VecLD *X;
    VecLD *Y;
    un_int N;

    LD maxX,minX;

    public:
    // constructor
    Interpolation()=default;

    Interpolation(VecLD *x,VecLD *y){
        /*data to be used in interpolation*/
        try{//abort is data_x are in 
            this->X=x;
            this->Y=y;
            this->N=this->X->size();

            for(un_int i=1; i<N; ++i){ 
                if((*X)[i-1]>=(*X)[i]){ throw "x must be in increasing order!!!";}
            }
        } catch(const char* errMsg){
            std::cout<<errMsg<<std::endl;
            abort();
        }

        this->maxX=this->X->operator[](this->N-1);
        this->minX=this->X->operator[](0);

    }

    un_int bSearch(LD x){
        /*
        This function takes a value (x) and finds which polynomial needs to be executed.
        That is it returns i, for x between X[i] and X[i+1]. 
        */
        un_int L=0;
        un_int R=N-1;
        un_int m;

        // abort if the values are beyond the bounds
        try{
            if(x>maxX){ throw "value beyond upper bound";}
            if(x<minX){ throw "value beyond lower bound";}
        } catch(const char* errMsg){
            std::cout<<errMsg<<std::endl;
            abort();
        }

        // binary search 
        while( R-L>1){
            m=(un_int)(L+R)/2; 
            if(x>(*X)[m]) L=m; else R=m;
        }
        return L;
    }


    LD operator()(LD x){
        /*
        0th order spline
        this is not good because the binary search is defined in such way that if x=X[N-1] 
        this function returns Y[N-2].
        */
        un_int i=bSearch(x);
        return (*Y)[i];
    }

    virtual LD derivative_1(LD x){
        /*0^th order spline of the first derivative*/
        un_int i=bSearch(x);
        return ( (*Y)[i]-(*Y)[i+1])/((*X)[i]-(*X)[i+1]);
    }
    virtual LD derivative_2(LD x)=0;

};

#endif
