// This is how you run RK. 
#include<iostream>
#include<fstream>
#include<cmath>
#include<complex>
#include<array>


#include"../Rosenbrock/Ros.hpp"
#include "../Rosenbrock/Jacobian/Jacobian.hpp"//this is treated as user input, since one may have an analytic form.
#include "../Rosenbrock/METHOD.hpp"

/*--------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------*/


#ifndef METHOD
    #define METHOD ROS34PW2
#endif

#ifndef LONG
    #define LONG
#endif

#define LD LONG double


// this is how the diffeq should look like
#define n_eqs 4 //number of equations
typedef std::array<LD,n_eqs> Array;//define an array type of length n_eqs
//-------------------------------------------------------------------------//


class TwoStateSystem{
    public:
        std::complex<LD>  H[2][2];
        std::complex<LD>  C[2], HC[2];
        std::complex<LD>  Imag;

        LD tmax;


        TwoStateSystem(std::complex<LD> H[2][2], LD tmax);
        ~TwoStateSystem(){};
        void operator()( Array &lhs, Array &c  , LD t );
        std::complex<LD> Hamiltonian(int i, int j, LD t );//this is to allow the hamiltoninan to be time-dependent

};

TwoStateSystem::TwoStateSystem(std::complex<LD> H[2][2], LD tmax){
    this->tmax=tmax;


    this->Imag.real(0);
    this->Imag.imag(1);


    for(int i =0; i<2 ;i++){
        for(int j =0; j<2 ;j++){
            this->H[i][j]=H[i][j];
            }
    }
 
};

std::complex<LD> TwoStateSystem::Hamiltonian(int i, int j, LD t ){

    if (i!=j){
        return (std::complex<LD>)( 10*exp(- 5.*pow(t-tmax/2.,2.) ) )*H[i][j];
    }
    else{

        return H[i][j];
    }


}

void TwoStateSystem::operator()( Array &lhs, Array &c  , LD t )
        {
            C[0].real(c[0]);
            C[0].imag(c[1]);
            C[1].real(c[2]);
            C[1].imag(c[3]);

            for(int i=0 ; i<2 ; i++){
                HC[i]=0;
                for(int j=0 ; j<2 ; j++){
                    HC[i]+=Hamiltonian(i,j,t)*C[j];
                }   
                HC[i]*=-(this->Imag);
            }

            lhs[0]=HC[0].real() ;
            lhs[1]=HC[0].imag() ;
            lhs[2]=HC[1].real() ;
            lhs[3]=HC[1].imag() ;


        };


#define initial_step_size 1e-4
#define minimum_step_size 1e-8
#define maximum_step_size 1e0
#define maximum_No_steps 1000000
#define absolute_tolerance 1e-9
#define relative_tolerance 1e-9
#define beta 0.85
#define fac_max 5.
#define fac_min 1/6.

//you can also define then using the -D flag ( as -DMETHOD=ROS34PW2 for example)
// #define METHOD ROS3w //2nd order
// #define METHOD ROS34PW2 //3rd order
// #define METHOD RODASPR2  //4th order
using namespace std;

std::complex<LD> Imag(0,1);//define imaginary unit


int main(int argc, const char** argv) {
    
    LD tmax = (LD)atof(argv[1]);
    
    Array lhs;
    Array c0;
    c0[0]=1/sqrt(2.);
    c0[1]=0;
    c0[2]=0;
    c0[3]=-1/sqrt(2.);
    
    std::complex<LD> H[2][2]={{5,.5},{.5,3}};

    TwoStateSystem schrodinger(H,tmax);
    
    Ros<n_eqs, METHOD<LD> ,Jacobian<n_eqs,LD>, LD > System(schrodinger,c0,tmax, 
     initial_step_size,  minimum_step_size,  maximum_step_size, maximum_No_steps, 
     absolute_tolerance, relative_tolerance, beta, fac_max, fac_min);
    System.solve();

    LD P1,P2;
    LD t;
    std::complex<LD> Emean,C[2];
    for(int i=0; i< System.time.size() ; ++i) {  
        P1=pow(System.solution[0][i],2 )+pow(System.solution[1][i],2 );
        P2=pow(System.solution[2][i],2 )+pow(System.solution[3][i],2 );
        t=System.time[i];
        printf("%e ",(double)(t) ) ;
        printf("%e ", (double)P1);
        printf("%e ",(double)P2); 
        
        C[0].real(System.solution[0][i]);
        C[0].imag(System.solution[1][i]);
        C[1].real(System.solution[2][i]);
        C[1].imag(System.solution[3][i]);
        
        Emean=conj(C[0])*schrodinger.Hamiltonian(0,0,t)*C[0]; 
        Emean+=conj(C[0])*schrodinger.Hamiltonian(0,1,t)*C[1]; 
        Emean+=conj(C[1])*schrodinger.Hamiltonian(1,0,t)*C[0];
        Emean+=conj(C[1])*schrodinger.Hamiltonian(1,1,t)*C[1];

        printf("%e ",(double)Emean.real()); 
        printf("%e ",(double)Emean.imag()); 
        
        for( int eq = 0; eq < n_eqs; eq++){ printf("%e ", (double)System.solution[eq][i]);    }
        for( int eq = 0; eq < n_eqs; eq++){ printf("%e ", (double)System.error[eq][i]);    }
        printf("\n");
        }


    return 0;
 }