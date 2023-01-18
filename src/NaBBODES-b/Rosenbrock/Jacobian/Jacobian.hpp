#ifndef Jac_head
#define Jac_head
#include<functional>
#include<array>
// This is an example Jacobian class.


template<int N_eqs, class LD>
class Jacobian{
    using diffeq = std::function<void(std::array<LD, N_eqs> &lhs, std::array<LD, N_eqs> &y  , LD t)>;
    public:
    LD h;
    diffeq dydt;

    std::array<LD, N_eqs> y0,y1,dydt0,dydt1;

    Jacobian()=default;

    // It is good to have a copy contructor is needed here, in case you need it. 
    // Rosenbrock works without it, but may be useful in the future.
    Jacobian(Jacobian &Jac){
        this->dydt=Jac.dydt;
        this->h=Jac.h;
    };

    Jacobian(diffeq dydt, LD h=1e-10){
        this->dydt=dydt;
        this->h=h;

    };

    Jacobian& operator=(const Jacobian &Jac){
        this->dydt=Jac.dydt;
        this->h=Jac.h;
        return *this;
    }

    void operator()(std::array<std::array<LD, N_eqs>, N_eqs> &J, std::array<LD, N_eqs> &dfdt, std::array<LD, N_eqs> &y  , LD t ){
        // you can use something like this to scale the stepsize according to the scale of t
        LD a=this->h+this->h*t;
        // take the time derivative
        dydt(dydt0,y,t-a);
        dydt(dydt1,y,t+a);
        for (int i = 0; i < N_eqs; i++){ dfdt[i]=(dydt1[i]-dydt0[i])/(2*a); }

        // take the derivatives over y
        for (int i = 0; i < N_eqs; i++){
            for (int j = 0; j < N_eqs; j++){
                for(int _d = 0; _d < N_eqs; _d++){y0[_d]=y[_d]; y1[_d]=y[_d]; }
                // you can use something like this to scale the stepsize according to the scale of y[j]
                a=this->h+this->h*std::abs(y0[j]);
                
                y0[j]=y0[j]-a;
                y1[j]=y1[j]+a;
                dydt(dydt0,y0,t);
                dydt(dydt1,y1,t);

                J[i][j]=(dydt1[i]-dydt0[i])/(2*a);
                // if(isnan(J[i][j])){J[i][j]=0;}
            }
        }




    };





};


#endif
