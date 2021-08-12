#ifndef logspace_included
#define logspace_included
#include<vector>
#include<cmath>


namespace nsc{ namespace util {
    //-- Get a number (length) of log_10-spaced points from 10^min to 10^max. --//
    
    
    /*logspace where the list is pushed in a vector that is pass by reference*/
    template<class LD>
    void logspace(LD min, LD max, unsigned int length, std::vector<LD> &X ){
        X.clear();
        for(unsigned i = 0; i<length ; ++i){
            X.push_back(std::pow( 10, min + i*(max-min)/( length-1 )));
        }
    }



    /*logspace where the list returned as a vector*/
    template<class LD>
    std::vector<LD> logspace(LD min, LD max, unsigned int length){
        std::vector<LD> X;
        for(unsigned i = 0; i<length ; ++i){
            X.push_back(std::pow( 10, min + i*(max-min)/( length-1 )));
        }
        return X;
    }



}}

#endif