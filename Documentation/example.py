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