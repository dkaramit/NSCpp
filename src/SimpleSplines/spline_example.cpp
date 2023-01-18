#include<iostream>
#include<vector>
#include<cmath>


#include"Interpolation.hpp"

using std::cout;
using std::endl;
using std::vector;
using std::sin;
using std::cos;
using std::exp;


template<class LD>
void linspace(LD min, LD max, unsigned int length, vector<LD> *X){
    X->reserve(length);
    for(unsigned int i = 0; i<length ; ++i){
        X->push_back( min + i*(max-min)/( length-1 ) );
    }
}



template<class LD>
void map(vector<LD> *X , LD(* func)(LD), vector<LD> *Y){
    unsigned int length=X->size();
    Y->reserve(length);
    
    for(unsigned int i = 0; i < length ; ++i){
        Y->push_back (func((*X)[i])) ;
    }
}


#ifndef LONG
#define LONG
#endif 

#define LD LONG double

int main(){
    vector<LD>x,x_int;
    vector<LD>y,yp,ypp;
    unsigned int N=50;


    linspace<LD>(-M_PI,M_PI,N,&x);
    map<LD>(&x,sin,&y);
    map<LD>(&x,cos,&yp);
    for (unsigned int i = 0; i < x.size(); i++){ypp.push_back(-y[i]);}
    


    Interpolation<LD> intrp0(&x,&y);
    LinearSpline<LD> intrp1(&x,&y);
    CubicSpline<LD> intrp3(&x,&y);


    #if 1
    for (unsigned int i = 0; i < x.size(); i++){
        cout<<x[i]<<","<<y[i]<<","<<yp[i]<<","<<ypp[i]<<endl;
    }
    cout<<"data"<<endl;

    linspace<LD>(-M_PI,M_PI,N*30,&x_int);
    for (unsigned int i = 0; i < x_int.size(); i++){
        cout<<x_int[i]<<","<<intrp0(x_int[i])<<","<<intrp1(x_int[i])<<","<<intrp3(x_int[i])<<",";
        cout<<intrp0.derivative_1(x_int[i])<<","<<intrp1.derivative_1(x_int[i])<<","<<intrp3.derivative_1(x_int[i])<<",";
        cout<<intrp0.derivative_2(x_int[i])<<","<<intrp1.derivative_2(x_int[i])<<","<<intrp3.derivative_2(x_int[i]);
    
        cout<<endl;
    }
    cout<<"end"<<endl;
    #endif


    return 0;
}