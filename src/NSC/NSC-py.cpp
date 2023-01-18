#include"src/NSC/NSCSolve.hpp"
#include"src/Cosmo/Cosmo.hpp"

// macros for the solver
#ifndef SOLVER
    #define SOLVER 1
    #define METHOD RODASPR2
#endif


// macros for the numeric type
#ifndef LONGpy
    #define LONGpy 
#endif

#ifndef LD
    #define LD LONGpy double
#endif

#define Cast(BE) static_cast<nsc::Evolution<LD,SOLVER,METHOD<LD>>*>(BE)

extern "C"{
    void* INIT(){ return new nsc::Evolution<LD,SOLVER,METHOD<LD>>(); }
    void DEL(void* BE){  delete Cast(BE) ; }
    
    bool SOLVE(LD TEND, LD c, LD Ti, LD ratio, LD TSTOP, LD umax, void *plasma,
                    LD initial_step_size, LD minimum_step_size, LD maximum_step_size, 
                    LD absolute_tolerance, LD relative_tolerance,
                    LD beta, LD fac_max, LD fac_min, unsigned int maximum_No_steps, void* BE){ 
        return Cast(BE) -> solveNSC(TEND, c, Ti, ratio, TSTOP, umax, static_cast<nsc::Cosmo<LD>*>(plasma),
                            {.initial_step_size=initial_step_size, .minimum_step_size=minimum_step_size, .maximum_step_size=maximum_step_size, 
                            .absolute_tolerance=absolute_tolerance, .relative_tolerance=relative_tolerance, .beta=beta,
                            .fac_max=fac_max, .fac_min=fac_min, .maximum_No_steps=maximum_No_steps});
    }

    unsigned int getSize(void* BE){ return Cast(BE) -> pointSize;}

    void getPoints(LD *u, LD *T, LD *rhoPhi, void* BE){
        unsigned int N= (Cast(BE) -> pointSize);

        for(unsigned int i=0; i<N; ++i){
            u[i]=Cast(BE) ->u[i];
            T[i]=Cast(BE) ->T[i];
            rhoPhi[i]=Cast(BE) ->rhoPhi[i];
        }
    }

    void getErrors(LD *dT,LD *drhoPhi, void* BE){
        unsigned int N= (Cast(BE) -> pointSize);

        for(unsigned int i=0; i<N; ++i){
            dT[i]=Cast(BE) ->dT[i];
            drhoPhi[i]=Cast(BE) ->drhoPhi[i];
        }
    }

    void getResults(LD *points, void* BE){
        points[0]=Cast(BE)->TE1;
        points[1]=Cast(BE)->TE2;
        points[2]=Cast(BE)->TD1;
        points[3]=Cast(BE)->TD2;
        points[4]=Cast(BE)->uE1;
        points[5]=Cast(BE)->uE2;
        points[6]=Cast(BE)->uD1;
        points[7]=Cast(BE)->uD2;
    }
}
