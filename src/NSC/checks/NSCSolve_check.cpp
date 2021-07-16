#include <iostream>
#include <iomanip> 
#include <cmath> 
#include <string> 
#include"src/NSC/NSCSolve.hpp"


#define preintPoints


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
    LD umax = 500; //t at which the integration stops 
    LD TSTOP = 1e-4; // temperature at which integration stops


    NSC<LD> nsc(TEND,c,Ti,ratio,umax,TSTOP);

    nsc.solveNSC();



    std::cout<<std::setprecision(5)
    <<TEND<<"\t"<<c<<"\t"<<Ti<<"\t"<<ratio<<"\t"<<nsc.TE1<<"\t"<<nsc.TE2<<"\t"<<nsc.TD1<<"\t"<<nsc.TD2<<"\n";


    // print all the points
    #ifdef printPoints
    std::cout<<"---------------------points:---------------------\n";
    std::cout<<"a/a_i\tT [GeV]\ttheta\tzeta\trho_a [GeV^4]"<<std::endl;
    for(size_t i=0; i<Ax.pointSize; ++i ){
        for(int j=0; j<5; ++j){
            std::cout<<Ax.points[i][j];
            if(j==4){std::cout<<"\n";}else{std::cout<<"\t";}
        }
    }
    #endif


    return 0;
}
