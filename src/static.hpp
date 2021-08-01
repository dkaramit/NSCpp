#ifndef Static_head
#define Static_head

#include"src/Cosmo/Cosmo.hpp"

#include "src/misc_dir/path.hpp"


/*cosmological parameters*/
template<class LD> static nsc::Cosmo<LD> cosmo(cosmo_PATH,0,nsc::Cosmo<LD>::mP);


#endif