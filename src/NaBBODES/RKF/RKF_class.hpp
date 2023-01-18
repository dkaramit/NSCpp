#ifndef RKF_class
#define RKF_class

#include<array>
#include<vector>
#include<functional>

//This is a general implementation of explicit embedded RK solver of
// a system of differential equations in the interval [0,tmax].

/*
diffeq is a class of the system of  equations to be solved 
N_eqs is ten number of equations to be solved
RKF_method is the method (the DormandPrince seems to be the standard here)
*/


template<unsigned int N_eqs, class RK_method, class LD>
class RKF{
    private://There is no reason to make things private (if you break it it's not my fault)... 
        using diffeq=std::function<void(std::array<LD, N_eqs> &lhs, std::array<LD, N_eqs> &y, LD t)>;


        LD hmin, hmax, abs_tol, rel_tol, beta, fac_max, fac_min;
        LD h_old, delta_acc, delta_rej;//these will be initialized at the beginning of next_step
        unsigned int max_N;
        bool h_stop;//h_stop becomes true when suitable stepsize is found.    
    public:
        //Inputs. The initial condition is given as a Array (the type is users choice as long as it can be called with [])
        diffeq dydt;
        
        LD tmax, h, tn;
        std::array<LD, N_eqs> yprev;
        
        
        std::vector<LD> time;
        std::array<std::vector<LD>, N_eqs> solution;
        std::array<std::vector<LD>, N_eqs> error;

        std::array<std::array<LD,RK_method::s>,N_eqs> k;
        // LD k[N_eqs][RK_method::s];
        //these are here to hold the k's, sum_i b_i*k_i, sum_i b_i^{\star}*k_i, and sum_j a_{ij}*k_j 
        std::array<LD,N_eqs> ak, bk, bstark;
        // abs_delta=abs(ynext-ynext_star)
        std::array<LD,N_eqs> abs_delta;

        std::array<LD,N_eqs> ynext;//this is here to hold the prediction
        std::array<LD,N_eqs> ynext_star;//this is here to hold the second prediction

        
        
        RKF(diffeq  dydt, const std::array<LD,N_eqs>& init_cond, LD tmax, 
            LD initial_step_size=1e-5, LD minimum_step_size=1e-11, LD maximum_step_size=1e-3,
            int maximum_No_steps=1000000, LD absolute_tolerance=1e-8,LD relative_tolerance=1e-8,
            LD beta=0.85,LD fac_max=3, LD fac_min=0.3);
        
        ~RKF()=default;

        /*-------------------it would be nice to have a way to define these sums more generaly-----------------*/
        void next_step();

        void calc_k();

        void sum_ak(unsigned int stage); // calculate sum_j a_{ij}*k_j and passit to this->ak
        void sum_bk();// calculate sum_i b_i*k_i and passit to this->bk 
        void sum_bstark();// calculate sum_i b^{\star}_i*k_i and passit to this->bk
        
        void step_control();//adjust stepsize until error is acceptable
        
        void solve();
};



#endif
