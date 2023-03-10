# `NSC++`
Non-Standard Cosmologies in C++

Get the stable (recommended) version by running

```
    git clone -b stable https://github.com/dkaramit/NSCpp
```

The most recent (assumed under development) version by running

```
    git clone https://github.com/dkaramit/NSCpp.git
	cd NSCpp
	git submodule init
	git submodule update --remote
```

Once everything is downloaded, run `bash configure.sh` in the root directory of `NSC++`. This creates a few things that `NSC++` may need.

## Relativistic degrees of freedom of the plasma
`NSC++` comes with tabulated RDOF as calculated in [2005.03544](https://arxiv.org/abs/2005.03544) (in `NSCpp/src/data/eos_2020.dat`). However, this one can use another file when calling the `Cosmo` class costructor (see below). Ideally, the RDOF we choose should be changed by assigning a path to another file to the `cosmo_PATH` variable in the file `NSCpp/Paths.mk` before running `bash configure.sh`. The main reason is that this script makes sure that the data file complies with the requirements of `NSC++`; that is, the file must be in format:

|$T$ (in GeV) | $h_{\rm eff}(T)$ | $g_{\rm eff}(T)$| 
|-|-|-|

with increasing $T$.

## Using `NSC++`

`NSC++` is a header-only library with no external dependencies appart from a `c++` compiler that supports the `c++17` standard. That is, `NSC++` only needs to be included in a `c++` source file. Then, at compilation one only needs to specify the path to the `NSC++` root directory using the `-I` flag.

`NSC++` can also be used directly from `python` scripts. To do this, one needs to run 

```
    make lib
```

in the root directory of `NSC++`. Then, the `NSC++` modules can be used in a script as:

```
    from sys import path as sysPath
    sysPath.append('path_to_src')
    from interfacePy.Evolution import Evolution 
    from interfacePy.Cosmo import Cosmo 
```

with `'path_to_src'` the path to `NSCpp/src` relative to the tocation of the script.

## Examples
In [arXiv:2301.08253](https://arxiv.org/abs/2301.08253) you can find description the classes and function in detail. However, it is worth showing an example in `c++` and `python` here.  

### `C++`


```c++
#include<iostream>
#include<iomanip>

//Include everything you need from NSC++
#include"NSCpp.hpp"
    
int main(){
    //Use cosmo_PATH to interpolate heff and geff from T=0 to T=mP.
    nsc::Cosmo<long double> plasma(cosmo_PATH, 0, nsc::Cosmo<long double>::mP);
    
    //Declare Evolution instance using the Rosenbrock method RODASPR2
    nsc::Evolution<long double,1,RODASPR2<long double>> BE;
    
    //Declare parameters
    long double TEND=1e-2, c=3, Ti=1e7, ratio=1e-2, TSTOP=1e-4, umax=200;
    
    bool check=BE.solveNSC(TEND, c, Ti, ratio, TSTOP, umax, &plasma,
                {
                    .initial_step_size=1e-2, .minimum_step_size=1e-8, .maximum_step_size=1e-2, 
                    .absolute_tolerance=1e-11, .relative_tolerance=1e-11, .beta=0.9, 
                    .fac_max=1.2, .fac_min=0.8, .maximum_No_steps=10000000
                });
    
    if(check){
        // If the solver returns true, the solver probably worked.
        std::cout<<std::setprecision(5);
        std::cout<<BE.TE1<<"\t"<<BE.TE2<<"\t"<<BE.TD1<<"\t"<<BE.TD2<<"\n";
        
        //print the results and the errors
        for(size_t i=0; i<BE.pointSize; ++i ){
            std::cout<<std::left<<std::setw(15)<<BE.u[i]<<std::setw(15);
            std::cout<<std::left<<BE.T[i]<<std::setw(15)<<BE.dT[i]<<std::setw(15);
            std::cout<<std::left<<BE.rhoPhi[i]<<std::setw(15)<<BE.drhoPhi[i]<<"\n";
        }
    }else{
        // If the solver returns false, you may need a larger umax.
        std::cerr<<"Something went wrong. Try using larger value for umax\n";
        exit(1);
    }
    
    return 0;
}
```

Assuming that this code is in `NSCpp/UserSpace` and has a name `example.cpp`, it can be simply compiled by 

```
g++ -I../ -O3 -std=c++17 example.cpp -o example
```

It can also be compiled using `clang` as

```
clang -lstdc++ -lm -I../ -O3 -std=c++17 example.cpp -o example
```

### `Python`

```py
# append the path to the src directory 
from sys import path as sysPath
sysPath.append('../src')

#load the NSC module
from interfacePy.Evolution import Evolution 

# load Cosmo and the Planck mass
from interfacePy.Cosmo import Cosmo,mP 

#This gives you access to the path of the rdof file.
from misc_dir.path import cosmo_PATH

#Instance of the Cosmo class. Interpolate from T=0 to T=mP
plasma=Cosmo(cosmo_PATH,0,mP)

# Evolution instance
BE=Evolution()

# solve the system
check,time=BE.solveNSC(TEND=1e-2, c=3, Ti=1e7, ratio=1e-2, umax=500, TSTOP=1e-4, plasma=plasma,
    initial_step_size=1e-2, minimum_step_size=1e-8, maximum_step_size=1e-2, 
    absolute_tolerance=1e-11, relative_tolerance=1e-11, beta=0.9, fac_max=1.2, 
    fac_min=0.8,maximum_No_steps=10000000)

if check:
    # get points 
    BE.getPoints()
    # get errors
    BE.getErrors()

    print(BE.TE1,BE.TE2,BE.TD1,BE.TD2)

    for i,u in enumerate(BE.u):
        print(u,BE.T[i],BE.dT[i],BE.rhoPhi[i],BE.drhoPhi[i])
else:
    print("Something went wrong. Try using larger value for umax")
    exit(1)

#run the destructors
del BE
del plasma
```

This script can be ran after `make lib` has finished. Here we assume that the path of this script is `NSCpp/UserSpace`. If it is located in another directory, we only need to change `sysPath.append('../src')` to `sysPath.append('path_to_src')` as described before.



## If you have trouble using the library I would be happy to help. 

Contact e-mail: <dkaramit@yahoo.com>, <dimitrios.karamitros@manchester.ac.uk>.

## Citation

If you use `NSC++` please cite it as 
```
@article{Karamitros:2023uak,
    author = "Karamitros, Dimitrios",
    title = "{NSC++: Non-Standard Cosmologies in C++}",
    eprint = "2301.08253",
    archivePrefix = "arXiv",
    primaryClass = "astro-ph.CO",
    month = "1",
    year = "2023"
}
```
## Accepted for publication in Computer Physics Communications!


Enjoy,\
Dimitris

# Note:
 
You can use `NSC++` to produce the non-standard cosmological input that `MiMeS` needs. [`MiMeS`](https://arxiv.org/abs/2201.10886) is an axion/ALP simulator. The stable version of `MiMeS` can be found: [github.com/dkaramit/MiMeS/tree/stable](https://github.com/dkaramit/MiMeS/tree/stable).  
