#include"src/NSC/NSCSolve.hpp"

// macros for the solver
#ifndef SOLVER
    #define SOLVER 1
    #define METHOD RODASPR2
#endif


// macros for the numeric type
#ifndef LONG
    #define LONG 
#endif

#ifndef LD
    #define LD LONG double
#endif

#define Cast(BE) static_cast<nsc::NSC<LD,SOLVER,METHOD<LD>>*>(BE)

extern "C"{
    void* INIT(LD TEND, LD c, LD Ti, LD ratio, LD umax, LD TSTOP, 
                    LD initial_step_size, LD minimum_step_size, LD maximum_step_size, 
                    LD absolute_tolerance, LD relative_tolerance,
                    LD beta, LD fac_max, LD fac_min, int maximum_No_steps){ 
        
        return new nsc::NSC<LD,SOLVER,METHOD<LD>>(TEND, c, Ti, ratio, umax, TSTOP,
                            initial_step_size,minimum_step_size, maximum_step_size, 
                            absolute_tolerance, relative_tolerance, beta,
                            fac_max, fac_min, maximum_No_steps);
    }

    void DEL(void* BE){  delete Cast(BE) ; }
    
    void setParams(LD TEND, LD c, LD Ti, LD ratio, LD umax, LD TSTOP, void* BE){
        Cast(BE)->setParams(TEND, c, Ti, ratio, umax, TSTOP);
    }

    void SOLVE(void* BE){ 
        Cast(BE) -> solveNSC();
    }

    unsigned int getSize(void* BE){ return Cast(BE) -> pointSize;}




    void getPoints(LD *a, LD *T, LD *rhoPhi, LD *logH2, void* BE){
        unsigned int N= (Cast(BE) -> pointSize);

        for(unsigned int i=0; i<N; ++i){
            a[i]=Cast(BE) ->points[i][0];
            T[i]=Cast(BE) ->points[i][1];
            rhoPhi[i]=Cast(BE) ->points[i][2];
            logH2[i]=Cast(BE) ->points[i][3];
        }
    }

    void getResults(LD *points, void* BE){
        points[0]=Cast(BE)->TE1;
        points[1]=Cast(BE)->TE2;
        points[2]=Cast(BE)->TD1;
        points[3]=Cast(BE)->TD2;
        points[4]=Cast(BE)->aE1;
        points[5]=Cast(BE)->aE2;
        points[6]=Cast(BE)->aD1;
        points[7]=Cast(BE)->aD2;
    }
}
