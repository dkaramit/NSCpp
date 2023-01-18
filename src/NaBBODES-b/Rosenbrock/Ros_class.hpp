#ifndef Ros_class
#define Ros_class

#include<array>
#include<vector>
#include<functional>

/*
N_eqs is ten number of equations to be solved RK_method 
is the method
*/

//This is a general implementation of explicit embedded RK solver of
// a system of differential equations in the interval [0,tmax].




template<unsigned int N_eqs, class RK_method, class jacobian, class LD> 
//Note that you can use template to pass the method
class Ros{
    private:
        using diffeq = std::function<void(std::array<LD, N_eqs> &lhs, std::array<LD, N_eqs> &y  , LD t)>;
        diffeq dydt;
        jacobian Jac;
        LD hmin, hmax, abs_tol, rel_tol, beta, fac_max, fac_min;
        unsigned int max_N;
        LD h_old,delta_acc,delta_rej;//these will be initialized at the beginning of next_step
        bool h_stop;//h_stop becomes true when suitable stepsize is found.    
        
    public:

        LD tmax, h, tn;
        std::array<LD, N_eqs> yprev;// previously accepted step. maybe the name is not good.


        std::vector<LD> time;
        std::array<std::vector<LD>, N_eqs> solution;
        std::array<std::vector<LD>, N_eqs> error;

    
        //these are here to hold the k's, sum_i b_i*k_i, sum_i b_i^{\star}*k_i, and sum_j a_{ij}*k_j 
        std::array<std::array<LD,RK_method::s>,N_eqs> k;
        std::array<LD,N_eqs> ak,gk,Jk, bk,bstark;
        // need this to store the sum over \gammas (see the contructor)
        std::array<LD,RK_method::s> sum_gamma;
        // abs_delta=abs(ynext-ynext_star)
        std::array<LD, N_eqs> abs_delta;
        
        

        std::array<LD, N_eqs> ynext;//this is here to hold the prediction
        std::array<LD, N_eqs> ynext_star;//this is here to hold the second prediction

        
        
        /*--These are specific to Rosenbrock methods*/
        std::array<LD, N_eqs> dfdt; 
        //define the coefficient. This will become (I-\gamma*h*J). _inv is its inverse
        std::array<std::array<LD, N_eqs>, N_eqs> _inv;
        // There are for the LUP-decomposition of (I-\gamma*h*J) 
        std::array<std::array<LD, N_eqs>, N_eqs> L;
        std::array<std::array<LD, N_eqs>, N_eqs> U;
        std::array<int,N_eqs> P;
        //lu_sol will capture the sulution of (I-\gamma*h*J)* k = rhs (i.e. k = (I-\gamma*h*J)^{-1} rhs)
        std::array<LD, N_eqs> lu_sol;
        std::array<std::array<LD, N_eqs>, N_eqs> J;//this is here to hold values of the Jacobian

        
        


        
        /*----------------------------------------------------------------------------------------------------*/
        Ros(diffeq dydt, const std::array<LD, N_eqs> &init_cond, LD tmax,
            LD initial_step_size=1e-5, LD minimum_step_size=1e-11, LD maximum_step_size=1e-3,int maximum_No_steps=1000000, 
            LD absolute_tolerance=1e-8,LD relative_tolerance=1e-8,LD beta=0.85,LD fac_max=3, LD fac_min=0.3);
        
        ~Ros()=default;

        /*-------------------it would be nice to have a way to define these sums more generaly-----------------*/
        void next_step();
        void LU();

        void calc_k();
        void calc_Jk();

        void sum_ak(unsigned int stage); // calculate sum_j a_{ij}*k_j and passit to this->ak
        void sum_gk(unsigned int stage); // calculate sum_j a_{ij}*k_j and passit to this->ak
        void sum_bk();// calculate sum_i b_i*k_i and passit to this->bk 
        void sum_bstark();// calculate sum_i b^{\star}_i*k_i and passit to this->bk
        

        
        void step_control();//adjust stepsize until error is acceptable
        void solve();


};



#endif
