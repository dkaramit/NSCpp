#ifndef Static_head
#define Static_head

#include"src/Cosmo/Cosmo.hpp"

#include "src/misc_dir/path.hpp"

#ifndef LONG
    #define LONG 
#endif

#ifndef LD
    #define LD LONG double
#endif

/*cosmological parameters*/
//it is better not to use all the available data h_PATH, because there are a lot of points.
//interpolating up to T=3GeV should be enough (the difference is less than 1%)...
static nsc::Cosmo<LD> cosmo(cosmo_PATH,1e-5,3e3);

static const LD T0=  nsc::Cosmo<LD>::T0;
static const LD h_hub= nsc::Cosmo<LD>::h_hub;
static const LD rho_crit= nsc::Cosmo<LD>::rho_crit;
static const LD relicDM_obs=nsc::Cosmo<LD>::relicDM_obs;
static const LD mP=nsc::Cosmo<LD>::mP;



#undef LD

#endif