#include<iostream>
#include<array>
#include<cmath>
#include "LU.hpp"

using std::cout;
using std::endl;

#define LD double

// #define indmax // run ind_max test

// #define swap //run index_swap test

// #define perm //run permutation test

// #define lup //run LUP test

// #define _rand //run random tests of Solve_LU

// #define inv_test // random tests of Inverse_LU




#ifdef _rand
    #include <cstdlib>
#endif

int main(){


    #ifdef indmax
        std::array<LD,5> x={2,1,-1,2,50};
        cout<<ind_max<5,LD>(x)<<endl;
    #endif


    
    #ifdef swap
        std::array<LD,5> x={2,1,-1,2,50};
        index_swap<5,LD>(x,4,1);
        for( LD i : x ){ cout<<i<<endl;}
    #endif
    

    #ifdef perm
        const unsigned int N=5;
        std::array<LD,N> A={1,2,5,8,3};
        std::array<int,N>  P={2,4,0,3,1};

        std::array<LD,N> Ap{0};

        apply_permutations_vector<N,LD>(A,P,Ap);
        for( int i =0 ; i<5 ; i++){ cout<<A[i]<<" ";}
        cout<<endl;
        for( int i =0 ; i<5 ; i++){ cout<<Ap[i]<<" ";}
        cout<<endl;
    #endif




    #ifdef lup
    const unsigned int N=5;
    std::array<std::array<LD,N> ,N>  M={{
    { 0,  2,  2 , 3 , 5},
    {-3, -1,  1 , 5 , 9},
    { 1, -1,  1 , 4 , 7},
    { 1, -1,  1 , 0 , 2},
    { 1, -1,  1 , 0 , 3}
    }};


    std::array<int,N> P;
    std::array<std::array<LD,N>,N> L, U;

    LUP<N,LD>(M,L,U,P);

    for( LD i : P ){ cout<< i<<' ';}
    cout<<endl;
    cout<<endl;
    cout<<endl;

    for (int i = 0; i < N; i++) {
    
    for (int j = 0; j < N; j++) {
        cout<< U[i][j]<<"\t";

    }
    cout<<endl;
    }
    cout<<endl;
    cout<<endl;

    for (int i = 0; i < N; i++) {
    
    for (int j = 0; j < N; j++) {
        cout<< L[i][j]<<"\t";

    }
    cout<<endl;
    }
    #endif


    #ifdef _rand
        /* 
            Random tests for Solve_LU. Basically run "runs" tests of Solve_LU with N number 
            of equations (with random coefficients of magnitude up to 100), and if 
            (M*x-b)/b > 1e-11, print it.
        */    
        const unsigned int runs=100000;
        std::array<LD,runs> err;


        const unsigned int N=10;
        std::array<std::array<LD,N>,N> M,U,L;
        std::array<LD,N> b,x;
        std::array<int,N> P;
        
        std::array<LD,N> tmp;
        
        LD tmpE;

        for(unsigned int _r=0; _r<runs ; _r++){
            for (unsigned int i = 0; i < N; i++) { for (int j = 0; j < N; j++)  {
                M[i][j]= ( rand()/ ((LD) RAND_MAX ) -0.5 ) *100 ;  } 
                b[i]= (rand()/ ((LD) RAND_MAX ) -0.5 ) *100 ;  
            } 
        
            LUP<N,LD>(M,L,U,P);
            Solve_LU<N,LD>(L,U,P,b,x);

            err[_r]=0;
            for (int i = 0; i < N; i++){
                dot<N,LD>(M,x,tmp);
                tmpE= std::abs((tmp[i] - b[i])/b[i]) ;
                if(tmpE>err[_r] ) {err[_r] = tmpE ;}

            }
        
        
        }

        for(LD _err: err){  
            if(_err>1e-11){ cout<<_err<<endl;} 
        }
        

    #endif



    #ifdef inv_test 
    /*
    run tests for Inverse_LU. Basically run "runs" random tests of Inverse_LU of N*N matrix 
    (with random entries of magnitude up to 100), and if 
    (M*M^{-1}-1)>1e-12, print it.
    */
    
    
    const unsigned int N=10;
    std::array<std::array<LD,N>,N> M,L,U,invM,R;

    std::array<std::array<LD,N>,N> Unit;
    for (int i = 0; i < N; i++){ 
        for (int j = 0; j < N; j++){
            Unit[i][j]=0;
        }
    } 


    std::array<int,N> P;
    LD tmp;
    
    for(int _run=0 ; _run<1000; ++_run)
    {
        for (int i = 0; i < N; i++) 
        { 
            // Unit is initialized as zero matrix. So put 1 at Unit[i][i].
            Unit[i][i]=1;
            for (int j = 0; j < N; j++)  
            {        
                // fil M with random numbers
                M[i][j]= ( rand()/ ((LD) RAND_MAX ) -0.5 ) *100 ;  

            }
        } 

        // LUP decomposition of M
        LUP<N,LD>(M,L,U,P);
        // Given LUP decomposition you can calculate the inverse. 
        Inverse_LU<N,LD>(L,U,P,invM);

        // calculate M*M^{-1}
        dot<N,LD>(M,invM,R);


        // print if M*M^{-1} - 1 > 10^{-10}
        for(int i=0; i<N; ++i){
            for(int j=0; j<N; ++j){
                tmp=fabs(R[i][j]-Unit[i][j]);
                if(tmp>1e-12){ cout<< tmp << endl;}
            }
        }
    }
    #endif



    return 0;
}