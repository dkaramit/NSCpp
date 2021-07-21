rootDir=../../../#the root directory of NSCpp
#######################################-data files-#######################################
# relative path to comoslogical data
cosmoDat=src/data/eos2020.dat
#
#######################################-precision-#######################################
# LONG=long : in order to use long doubles. This is slower, but generally more accurate, since we can use tolerances down to 10^-11 (or below that). 
# LONG= : in order to use doubles. This is faster. However the tolerances of the ODE solver cannt be below 1e-8.

# use long doubles
LONG=long
# use doubles
# LONG= 


#compiler. I use g++, but clang seems to work as well.
CC=g++ 
# CC=clang  

#---optimization options---#
OPT=O3 #this should be fast and safe
# OPT=O0 #this is generally 2x slower than O3
# OPT=Ofast #this is usually bit faster than O3 but can cause issues (I haven't observed any though)
