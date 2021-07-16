#include <iostream>
#include <iomanip> 
#include <cmath> 
#include <string> 
#include"src/NSC/NSCSolve.hpp"


#define printPoints


#ifndef LONG
    #define LONG 
#endif

#ifndef LD
    #define LD LONG double
#endif

int main(int argc, char **argv){ 

    //model parameters
    LD TEND = 1e-2;
    LD c=2;
    LD Ti=1e7;
    LD ratio=1e-3;

    // solver parameters
    LD umax = 500; //u at which the integration stops 
    LD TSTOP = 1e-4; // temperature at which integration stops


    nsc::NSC<LD> BE(TEND,c,Ti,ratio,umax,TSTOP);

    BE.solveNSC();



    std::cout<<std::setprecision(5)
    <<TEND<<"\t"<<c<<"\t"<<Ti<<"\t"<<ratio<<"\t"<<BE.TE1<<"\t"<<BE.TE2<<"\t"<<BE.TD1<<"\t"<<BE.TD2<<"\n";


    // print all the points
    #ifdef printPoints
    std::cout<<"---------------------points:---------------------\n";
    std::cout<<"a/a_i\tT [GeV]\trho_Phi [GeV^4]\tlogH^2"<<std::endl;
    for(size_t i=0; i<BE.pointSize; ++i ){
        for(int j=0; j<4; ++j){
            std::cout<<BE.points[i][j];
            if(j==3){std::cout<<"\n";}else{std::cout<<"\t";}
        }
    }
    #endif


    return 0;
}
