#include<iostream>
#include<fstream>


#include"src/Cosmo/Cosmo.hpp"
#include"src/misc_dir/path.hpp"


/*uncomment to print difference between interpolation and data*/
#define printDiff

/*uncomment to print interpolation for different values of theta_max*/
#define printVal


#ifndef LONG
    #define LONG 
#endif

#ifndef LD
    #define LD LONG double
#endif

int main(){
    static mimes::Cosmo<LD> cosmo(cosmo_PATH,1e-5,1e4);

    #ifdef printDiff
    {
        LD T,heff,geff;
        std::cout<<"relative difference between interpolation and data"<<std::endl;
        std::cout<<"Line No.\tT[GeV]\tdiff h_eff\tdiff g_eff"<<std::endl;
        
        std::ifstream data_file(cosmo_PATH);
        int i=0;
        while(true){
            data_file>>T; 
            data_file>>heff;
            data_file>>geff;


            std::cout<<++i<<"\t"<<T<<"\t"<<std::abs((heff-cosmo.heff(T))/heff)<<"\t"<<std::abs((geff-cosmo.geff(T))/geff)<<"\n";

            if(data_file.eof()){data_file.close();break;}

        }
    }
    #endif

    #ifdef printVal
    {
        LD logT;
        std::cout<<"print rel. dofs for 1e-5 GeV <=T<=1e5 GeV"<<std::endl;
        std::cout<<"T[GeV]\theff\tgeff"<<std::endl;
        logT=-5;
        int N=150;
        for(int i=0; i<N; ++i){
            std::cout<<std::pow(10,logT)<<"\t"<<cosmo.heff(std::pow(10,logT))<<"\t"<<cosmo.geff(std::pow(10,logT))<<"\n";
            logT=logT+10./(N-1.);
        }
    }    
    #endif
    
    
      
    return 0;
}