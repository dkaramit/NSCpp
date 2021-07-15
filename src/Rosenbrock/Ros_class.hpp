#ifndef Ros_class
#define Ros_class

/*
diffeq is a class of the system of  equations to be solved 
N_eqs is ten number of equations to be solved RK_method 
is the method
*/

//This is a general implementation of explicit embedded RK solver of
// a system of differential equations in the interval [0,tmax].
#define Ros_Template template<class diffeq, int N_eqs, class RK_method, class jacobian, class LD> 
#define Ros_Namespace Ros<diffeq, N_eqs, RK_method,  jacobian, LD>


Ros_Template//Note that you can use template to pass the method
class Ros{
    private://There is no reason to make things private (if you break it it's not my fault)... 
        
    public:
        //Inputs. The initial condition is given as a Array (the type is users choice as long as it can be called with [])
        diffeq dydt;
        jacobian Jac;
        LD tmax, h, hmin, hmax, abs_tol, rel_tol, beta, fac_max, fac_min;
        int max_N;
        LD h_old,delta_acc,delta_rej;//these will be initialized at the beginning of next_step

        
        //things that we'll need
        bool h_stop;//h_stop becomes true when suitable stepsize is found.    

        LD tn;
        LD yprev[N_eqs];// previously accepted step. maybe the name is not good.


        std::vector<LD> time;
        std::vector<LD> solution[N_eqs];
        std::vector<LD> error[N_eqs];
        


    
        //these are here to hold the k's, sum_i b_i*k_i, sum_i b_i^{\star}*k_i, and sum_j a_{ij}*k_j 
        LD **k;
        LD ak[N_eqs],gk[N_eqs],Jk[N_eqs], bk[N_eqs],bstark[N_eqs];
        // need this to store the sum over \gammas (see the contructor)
        LD *sum_gamma;
        // abs_delta=abs(ynext-ynext_star)
        LD abs_delta[N_eqs];
        
        

        LD ynext[N_eqs];//this is here to hold the prediction
        LD ynext_star[N_eqs];//this is here to hold the second prediction

        
        
        /*--These are specific to Rosenbrock methods*/
        LD dfdt[N_eqs]; 
        //define the coefficient. This will become (I-\gamma*h*J). _inv is its inverse
        LD _inv[N_eqs][N_eqs];
        // There are for the LUP-decomposition of (I-\gamma*h*J) 
        LD L[N_eqs][N_eqs];
        LD U[N_eqs][N_eqs];
        int P[N_eqs];
        //lu_sol will capture the sulution of (I-\gamma*h*J)* k = rhs (i.e. k = (I-\gamma*h*J)^{-1} rhs)
        LD lu_sol[N_eqs];
        LD J[N_eqs][N_eqs];//this is here to hold values of the Jacobian

        
        


        
        /*----------------------------------------------------------------------------------------------------*/
        Ros(diffeq dydt, LD (&init_cond)[N_eqs], LD tmax,
            LD initial_step_size=1e-5, LD minimum_step_size=1e-11, LD maximum_step_size=1e-3,int maximum_No_steps=1000000, 
            LD absolute_tolerance=1e-8,LD relative_tolerance=1e-8,LD beta=0.85,LD fac_max=3, LD fac_min=0.3);
        
        ~Ros();

        /*-------------------it would be nice to have a way to define these sums more generaly-----------------*/
        void next_step();
        void LU();

        void calc_k();
        void calc_Jk();

        void sum_ak(int stage); // calculate sum_j a_{ij}*k_j and passit to this->ak
        void sum_gk(int stage); // calculate sum_j a_{ij}*k_j and passit to this->ak
        void sum_bk();// calculate sum_i b_i*k_i and passit to this->bk 
        void sum_bstark();// calculate sum_i b^{\star}_i*k_i and passit to this->bk
        

        
        void step_control();//adjust stepsize until error is acceptable
        void solve();


};



#endif
