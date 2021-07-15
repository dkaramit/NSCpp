#ifndef RKF_sums
#define RKF_sums
#include "RKF_class.hpp"

/*-----------------------Begin: sum_ak---------------------------------*/
RKF_Template
void RKF_Namespace::sum_ak(int stage){
    // this function stores sum_{j}^{stage-1}a_{stage,j}\vec{k}_j in ak, so we first need to make all elements zero, and then take the sum for each component
    // for (int eq = 0; eq <N_eqs ; eq++){ak[eq]=0.;  }//again redundant but it is more clear this way
    for (int eq = 0; eq <N_eqs ; eq++){
        ak[eq]=0.;  //you cloud initialize it here for example.
        for (int j = 0; j <= stage-1; j++){ ak[eq]+=RK_method::a[stage][j]*k[eq][j];  }
        // std::cout<<stage<<"   "<<eq<<"    "<<ak[eq]<<"\n";std::cin.get();
    }
    
}
/*-----------------------End: sum_ak---------------------------------*/

/*-----------------------Begin: sum_bk---------------------------------*/
RKF_Template
void RKF_Namespace::sum_bk(){
    // this function stores sum_{i}^{s}b_{i}\vec{k}_i in bk and sum_{i}^{s}b_{i}^{\star}\vec{k}_i in bstark  
    for (int eq = 0; eq <N_eqs ; eq++){
        bk[eq]=0.;
        bstark[eq]=0.; 
        for (int i = 0; i < RK_method::s; i++){ 
            bk[eq]+=RK_method::b[i]*k[eq][i];  
            bstark[eq]+=RK_method::bstar[i]*k[eq][i];  
            }
    }

}
/*-----------------------End: sum_bk---------------------------------*/




#endif