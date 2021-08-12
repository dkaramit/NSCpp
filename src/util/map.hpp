#ifndef map_included
#define map_included
#include<vector>
#include<functional>
#include<cmath>


namespace nsc{ namespace util {
    // give a list of numbers and get the value of a function on each item of the list 


    /*The list is pushed in a vector that is pass by reference*/
    template<class LD>
    void map(const std::vector<LD> &X, std::function<LD(LD)> func,  std::vector<LD> &Y ){
        Y.clear();
        for(unsigned i = 0; i<X.size() ; ++i){
            Y.push_back(func(X[i]));
        }
    }



    /*The list returned as a vector*/
    template<class LD>
    std::vector<LD> map(const std::vector<LD> &X, std::function<LD(LD)> func){
        std::vector<LD> Y;
        for(unsigned i = 0; i<X.size() ; ++i){
            Y.push_back(func(X[i]));
        }
        return Y;
    }



}}

#endif