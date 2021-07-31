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
static nsc::Cosmo<LD> cosmo(cosmo_PATH,0,nsc::Cosmo<LD>::mP);

static const LD T0=  nsc::Cosmo<LD>::T0;
static const LD h_hub= nsc::Cosmo<LD>::h_hub;
static const LD rho_crit= nsc::Cosmo<LD>::rho_crit;
static const LD relicDM_obs=nsc::Cosmo<LD>::relicDM_obs;
static const LD mP=nsc::Cosmo<LD>::mP;



#undef LD

#endif