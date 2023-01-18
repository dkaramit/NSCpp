#ifndef Ros_METHOD
#define Ros_METHOD
#include<array>

template<class LD>
struct ROS3w{
    static constexpr unsigned int s=3;
    static constexpr unsigned int p=2;

    using arr=std::array<LD,s>;
    using arr2=std::array<std::array<LD,s>,s>;

    static constexpr arr c={0,2/3.,4/3.};
    static constexpr arr b={0.25,0.25,0.5 };
    static constexpr arr bstar={ 0.746704703274 , 0.1144064078371 , 0.1388888888888 };
    static constexpr LD gamma=0.4358665215084;
    
    static constexpr arr2 a={
        arr{0,0,0},
        arr{2/3.,0,0},
        arr{2/3.,2/3.,0}

    };

    static constexpr arr2 g={
        arr{0,0,0},
        arr{0.3635068368900,0,0},
        arr{-0.8996866791992,-0.1537997822626,0}

    };
};
/*--------------------------------------------------------------------------------------------------------*/
template<class LD>
struct ROS34PW2{
    static constexpr unsigned int s=4;
    static constexpr unsigned int p=3;

    using arr=std::array<LD,s>;
    using arr2=std::array<std::array<LD,s>,s>;

    static constexpr arr c={0, 0.87173304301691800777, 0.73157995778885237526, 1};// c[i]=sum_{j} a[i][j]
    static constexpr arr b={2.4212380706095346e-1 , -1.2232505839045147 , 1.5452602553351020 , 4.3586652150845900e-1};
    static constexpr arr bstar={ 3.7810903145819369e-1 , -9.6042292212423178e-2 , 0.5 , 2.1793326075422950e-1};
    static constexpr LD gamma=4.3586652150845900e-1; 
    
    static constexpr arr2 a={
        arr{0,0,0,0},
        arr{8.7173304301691801e-1,0,0,0},
        arr{8.4457060015369423e-1,-1.1299064236484185e-1,0,0},
        arr{0,0,1,0}
    };
        
    static constexpr arr2 g={
        arr{0,0,0,0},
        arr{-8.7173304301691801e-1,0,0,0},
        arr{-9.0338057013044082e-1,5.4180672388095326e-2,0,0},
        arr{2.4212380706095346e-1,-1.2232505839045147,5.4526025533510214e-1,0}
    };
};
/*--------------------------------------------------------------------------------------------------------*/
/*
@article{RANG2015128,
	title = {Improved traditional Rosenbrock–Wanner methods for stiff ODEs and DAEs},
	journal = {Journal of Computational and Applied Mathematics},
	volume = {286},
	pages = {128-144},
	year = {2015},
	issn = {0377-0427},
	doi = {https://doi.org/10.1016/j.cam.2015.03.010},
	url = {https://www.sciencedirect.com/science/article/pii/S0377042715001569},
	author = {Joachim Rang}
}
@article{RangAngermann2005,
	title = {New Rosenbrock W-Methods of Order 3 for Partial Differential Algebraic Equations of Index 1},
	journal = "BIT Numerical Mathematics",
	volume = "45",
	pages = "761–787",
	year = "2005",
	doi = "10.1007/s10543-005-0035-y",
	author = "Rang, J. and Angermann, L."
}
*/
template<class LD>
struct RODASPR2{
    static constexpr unsigned int s=6;
    static constexpr unsigned int p=4;
    
    using arr=std::array<LD,s>;
    using arr2=std::array<std::array<LD,s>,s>;

    static constexpr arr c={0, 0.9375, 0.49816697385844987966, 0.68907842168064792343, 0.99999999999999997224, 0.99999999999999991673};// c[i]=sum_{j} a[i][j]
    static constexpr arr b={5.1944159827788361e-1,3.9955867781540699e-2,-4.7356407303732290e-1,9.4907420451383284e-1,-3.4740759753593431e-1,3.125e-1  };
    static constexpr arr bstar={-1.7746585073632790e-1,-5.8241418952602364e-1,6.8180612588238165e-1,7.6557391437996980e-1,3.125e-1,0 };
    static constexpr LD gamma=3.125e-1; 
    
    static constexpr arr2 a={
        arr{0,0,0,0,0,0},
        arr{9.375e-1,0,0,0,0,0},
        arr{-4.7145892646261345e-2,5.4531286650471122e-1,0,0,0,0},
        arr{4.6915543899742240e-1,4.4490537602383673e-1,-2.2498239334061121e-1 ,0,0,0},
        arr{1.0950372887345903,6.3223023457294381e-1,-8.9232966090485821e-01,1.6506213759732410e-1,0,0},
        arr{-1.7746585073632790e-1,-5.8241418952602364e-1,6.8180612588238165e-1,7.6557391437996980e-1,3.125e-1,0}
    };

    static constexpr arr2 g={
        arr{0,0,0,0,0,0},
        arr{-9.3750000000000000e-01,0,0,0,0,0},
        arr{-9.7580572085994507e-02,-5.8666328499964138e-01,0,0,0,0},
        arr{-4.9407065013256957e-01,-5.6819726428975503e-01,5.0318949274167679e-01,0,0,0},
        arr{-1.2725031394709183,-1.2146444240989676,1.5741357867872399,6.0051177678264578e-01,0,0},
        arr{6.9690744901421153e-01,6.2237005730756434e-01,-1.1553701989197045,1.8350029013386296e-01,-6.5990759753593431e-01,0}
    };
};
/*--------------------------------------------------------------------------------------------------------*/
/*
@article{Rentrop1979,
author = {Rentrop, P., Kaps, P.},
journal = {Numerische Mathematik},
keywords = {autonomous initial value problem; Rosenbrock-Wanner method; generalized Runge-Kutta method; error estimation; step-size control; test results; stiff initial value problems},
pages = {55-68},
title = {Generalized Runge-Kutta Methods of Order Four with Stepsize Control for Stiff Ordinary Differential Equations.},
url = {http://eudml.org/doc/132628},
volume = {33},
year = {1979},
}
*/
template<class LD>
struct GRK4A{
    static constexpr unsigned int s=4;
    static constexpr unsigned int p=4;

    using arr=std::array<LD,s>;
    using arr2=std::array<std::array<LD,s>,s>;

    static constexpr arr c={0, 0.438, 0.87, 0};// c[i]=sum_{j} a[i][j]
    static constexpr arr b={0.199293275701,0.482645235674,0.0680614886256, 0.25};
    static constexpr arr bstar={0.346325833758,0.285693175712,0.367980990530,0};
    static constexpr LD gamma=0.395; 
    
    static constexpr arr2 a={
        arr{0,0,0,0},
        arr{0.438,0,0,0},
        arr{0.796920457938,0.0730795420615,0,0},
        arr{0,0,0,0}
    };
        
    static constexpr arr2 g={
        arr{0,0,0,0},
        arr{-0.767672395484,0,0,0},
        arr{-0.851675323742,0.522967289188,0,0},
        arr{0.288463109545,0.0880214273381,-0.337389840627,0}
    };
};
template<class LD>
struct GRK4T{
    static constexpr unsigned int s=4;
    static constexpr unsigned int p=4;

    using arr=std::array<LD,s>;
    using arr2=std::array<std::array<LD,s>,s>;

    static constexpr arr c={0,0.462,0.8802083333333,0};// c[i]=sum_{j} a[i][j]
    static constexpr arr b={0.217487371653 , 0.486229037990,0, 0.296283590357};
    static constexpr arr bstar={ -0.717088504499,  1.77617912176, -0.590906172617e-1  };
    static constexpr LD gamma=0.231; 
    
    static constexpr arr2 a={
        arr{0,0,0,0},
        arr{0.462,0,0,0},
        arr{ -0.815668168327e-1,0.961775150166,0,0},
        arr{0,0,0,0}
    };
        
    static constexpr arr2 g={
        arr{0,0,0,0},
        arr{-0.270629667752,0,0,0},
        arr{0.311254483294 ,0.852445628482e-2,0,0},
        arr{0.282816832044,-0.457959483281, -0.111208333333,0}
    };
};

#endif