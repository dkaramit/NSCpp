#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>


#include"Interpolation.hpp"

using std::cout;
using std::endl;
using std::vector;


template<class LD>
void logspace(LD min, LD max, unsigned int length, vector<LD> *X){
    X->reserve(length);
    for(unsigned int i = 0; i<length ; ++i){
        X->push_back( pow( 10, min + i*(max-min)/( length-1 )) );
    }
}


#ifndef LONG
#define LONG
#endif 

#define LD LONG double

int main(){
    vector<LD> T;
    vector<LD> h,g;
    unsigned int N;
    LD tmp;
    std::ifstream data_file("eos2020.dat");


    while (true){
        data_file>>tmp;
        T.push_back(tmp);
        data_file>>tmp;
        h.push_back(tmp);
        data_file>>tmp;
        g.push_back(tmp);

        if (data_file.eof()){
            break;
        }
    }
    N=T.size();


    Interpolation<LD> h_int0(&T,&h),g_int0(&T,&g);
    LinearSpline<LD> h_int1(&T,&h),g_int1(&T,&g);
    CubicSpline<LD> h_int3(&T,&h),g_int3(&T,&g);


    #if 1
    for (unsigned int i = 0; i < T.size(); i++){
        cout<<T[i]<<","<<h[i]<<","<<g[i]<<endl;
    }
    cout<<"data"<<endl;

    LD minT=T[0],maxT=T[N-1];
    T=vector<LD>{};
    logspace<LD>(std::log10(minT),std::log10(maxT), N, &T);
    for (unsigned int i = 0; i < T.size(); i++){
        cout<<T[i]<<","<<h_int0(T[i])<<","<<h_int1(T[i])<<","<<h_int3(T[i])<<",";
        cout<<g_int0(T[i])<<","<<g_int1(T[i])<<","<<g_int3(T[i])<<",";
        cout<<1+1/3.*T[i]/h_int0(T[i])*h_int0.derivative_1(T[i])<<","<<1+1/3.*T[i]/h_int1(T[i])*h_int1.derivative_1(T[i])<<","<<1+1/3.*T[i]/h_int3(T[i])*h_int3.derivative_1(T[i]);
    
        cout<<endl;
    }
    cout<<"end"<<endl;
    #endif


    return 0;
}