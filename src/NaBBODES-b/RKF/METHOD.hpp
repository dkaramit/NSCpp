#ifndef RKF_METHOD
#define RKF_METHOD
#include<array>

template<class LD>
struct DormandPrince{
    static constexpr unsigned int s=7;
    static constexpr unsigned int p=4;
    
    using arr=std::array<LD,s>;
    using arr2=std::array<arr,s>;

    static constexpr arr c={0,1/5.,3/10.,4/5.,8/9.,1.,1.};
    static constexpr arr b={5179/57600.,0,7571/16695.,393/640.,-92097/339200.,187/2100.,1/40.};
    static constexpr arr bstar={ 35/384.,0.,500/1113.,125/192.,-2187/6784.,11/84.,0 };
    static constexpr arr2 a={
        arr{0,0,0,0,0,0,0},
        arr{1/5.,0,0,0,0,0,0},
        arr{3/40.,9/40.,0,0,0,0,0},
        arr{44/45.,-56/15.,32/9.,0,0,0,0},
        arr{19372/6561.,-25360/2187.,64448/6561.,-212/729.,0,0,0},
        arr{9017/3168.,-355/33.,46732/5247.,49/176.,-5103/18656.,0,0},
        arr{35/384.,0,500/1113.,125/192.,-2187/6784.,11/84.,0}

    };
};

template<class LD>
struct CashKarp{
    static constexpr unsigned int s=6;
    static constexpr unsigned int p=4;

    using arr=std::array<LD,s>;
    using arr2=std::array<arr,s>;

    static constexpr arr c={0,1/5.,3/10.,3/5.,1.,7/8.};
    static constexpr arr b={37/378.,0,250/621.,125/594.,0,512/1771.};
    static constexpr arr bstar={2825/27648.,0.,18575/48384.,13525/55296,277/14336.,0.25};
    static constexpr arr2 a={
        arr{0,0,0,0,0,0},
        arr{0.2,0,0,0,0,0},
        arr{3/40.,9/40.,0,0,0,0},
        arr{0.3,-0.9,6/5.,0,0,0},
        arr{-11/54.,2.5,-70/27,35/27.,0,0},
        arr{1631/55296.,175/512.,575/13824.,44275/110592.,253/4096.,0}
    };
};



template<class LD>
struct RKF45{
    static constexpr unsigned int s=6;
    static constexpr unsigned int p=4;

    using arr=std::array<LD,s>;
    using arr2=std::array<arr,s>;

    static constexpr arr c={0,1/4.,3/8.,12/13.,1.,0.5};
    static constexpr arr b={16/135.,0,6656/12825.,28561/56430.,-9/50.,2/55.};
    static constexpr arr bstar={25/216.,0.,1408/2565.,2197/4104,-0.2,0};
    static constexpr arr2 a={
        arr{0,0,0,0,0,0},
        arr{0.25,0,0,0,0,0},
        arr{3/32.,9/32.,0,0,0,0},
        arr{1932/2197.,-7200/2197.,7296/2197.,0,0,0},
        arr{439/216.,-8.,3680/513.,-845/4104.,0,0},
        arr{-8/27.,2.,-3544/2565.,1859/4104.,-11/40.,0}
    };
};
#endif