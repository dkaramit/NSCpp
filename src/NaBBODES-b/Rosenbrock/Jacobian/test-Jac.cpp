#include<iostream>
#include<array>
#include"Jacobian.hpp"
// #define _class //run it with class with overloaded operator()
#define _function //run it with function pointer

#define LD double


// this is how the diffeq should look like
#define n_eqs 2 //number of equations
typedef std::array<LD, n_eqs> Array;//define an array type of length n_eqs
typedef std::array<Array, n_eqs> Array2;
//-------------------------------------------------------------------------//



#ifdef _function
typedef void (*diffeq)(Array &lhs, Array &y  , LD t); // define function pointer
void sys(Array &lhs, Array &y  , LD t)
{

    lhs[0]=y[1]*y[0]*t;
    lhs[1]=y[0]*y[1]+t;

}
#endif


#ifdef _class
class Cdiffeq{
    public:
    Cdiffeq(){};
    ~Cdiffeq(){};
    void operator()(Array &lhs, Array &y  , LD t)
    {
        lhs[0]=y[1]*y[0]*t;
        lhs[1]=y[0]*y[1]+t;
    }


};
#endif


using std::cout;
using std::endl;


int main(){
    #ifdef _class
        Cdiffeq dydt;
        Jacobian<n_eqs,LD> jac(dydt);
    #endif

    #ifdef _function
        Jacobian<n_eqs,LD> jac(sys);
    #endif

    Array dfdt;
    // Matrix J={{0,0},{0,0}};
    Array2 J;

    Array y={1,4.2};
    LD t=0.3;

    jac(J,dfdt,y,t);

    cout<<"dfdt=[";
    for(int i=0; i<n_eqs ; i++){  cout<<dfdt[i]; if(i!=n_eqs-1){cout<<",";}   }
    cout<<"]"<<endl;


    cout<<"J=["<<endl;
    
    for (int i = 0; i < n_eqs; i++){
         cout<<"[";
        for (int j = 0; j < n_eqs; j++) {cout<<J[i][j]; if(j!=n_eqs-1){cout<<",";} }
          cout<<"]";if(i!=n_eqs-1){cout<<",";}cout<<endl;
        
        
    }  
    cout<<"]"<<endl;
    





    return 0;
}