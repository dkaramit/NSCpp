#ifndef Ros_LU
#define Ros_LU
#include "Ros_class.hpp"



/*--------Calculate the LU decomposition of (1-h*gamma*J) for this step--------------------------*/
Ros_Template
void Ros_Namespace::LU(){
    //initialize coefficient to 0
    LD coeff[N_eqs][N_eqs]={0};
   
    Jac(J,dfdt,yprev,tn);

    // Find the LUP decomposition of   (I-\gamma*h*J)     
    for(int i=0; i<N_eqs ; i++){
        coeff[i][i]=1;
        for(int j=0; j<N_eqs ; j++){
            coeff[i][j]+=-h*RK_method::gamma*J[i][j];
        }
    }
    LUP<N_eqs,LD>(coeff,L,U,P); //LU decomposition of (1-h*gamma*J)
    Inverse_LU<N_eqs,LD>(L,U,P,_inv); // the inverse of (1-h*gamma*J)
}
/*---------------------------------------------------------------------------------------------*/








#endif