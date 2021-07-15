#include"NonStandardCosmology/NSC/NSCSolve.hpp"

#ifndef LONG
    #define LONG long
#endif

#ifndef LD
    #define LD LONG double
#endif

typedef void* (*initType)(LD , LD , LD, LD , LD , LD, LD);
typedef void (*solveType)(LD, void*);
typedef unsigned int (*sizeType)(void*);


void* __cc__(LD TEND, LD c, LD Ti, LD ratio, LD umax, LD TSTOP){ 
    return new NSC<LD>(TEND, c, Ti, ratio, umax, TSTOP);
}
// destructor to delete the void*
void delete_ptr(void * nsc){  delete static_cast<NSC<LD>*>(nsc) ; }

void __solve__(LD fa, void* nsc){ 
    static_cast<NSC<LD>*>(nsc) -> solve(fa);
}

unsigned int returnSize(void* nsc){
    return static_cast<NSC<LD>*>(nsc) -> N;
}



extern "C"{
    initType INIT () {return __cc__ ; }
    makeType DEL () {return delete_ptr ; }
    solveType SOLVE () { return __solve__;}
    sizeType getSize(){return returnSize ;}



    void getResults(LD *a, LD *T, LD *rho, LD *rhoPhi, LD *points, void* nsc){
        unsigned int N= (static_cast<NSC<LD>*>(nsc) -> N);
        
        for(unsigned int i=0; i<N; ++i){
            a[i]=static_cast<NSC<LD>*>(nsc) ->results[i][0];
            T[i]=static_cast<NSC<LD>*>(nsc) ->results[i][2];
            rho[i]=static_cast<NSC<LD>*>(nsc) ->results[i][3];
            rhoPhi[i]=static_cast<NSC<LD>*>(nsc) ->results[i][4];
        }


        points[0]=static_cast<NSC<LD>*>(nsc)->TE1;
        points[1]=static_cast<NSC<LD>*>(nsc)->TE2;
        points[2]=static_cast<NSC<LD>*>(nsc)->TD1;
        points[3]=static_cast<NSC<LD>*>(nsc)->TD2;
        points[4]=static_cast<NSC<LD>*>(nsc)->aE1;
        points[5]=static_cast<NSC<LD>*>(nsc)->aE2;
        points[6]=static_cast<NSC<LD>*>(nsc)->aD1;
        points[7]=static_cast<NSC<LD>*>(nsc)->aD2;
    }
}
