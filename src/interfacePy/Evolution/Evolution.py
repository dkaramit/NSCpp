from ctypes import  CDLL, c_longdouble, c_double, c_char_p, c_void_p, c_int, c_uint, POINTER, c_bool
from numpy import array as np_array

from time import time

from sys import path as sysPath
from os import path as osPath

sysPath.append(osPath.join(osPath.dirname(__file__), '../../'))

from misc_dir.path import rootDir
from misc_dir.type import cdouble

cint=c_int
cuint=c_uint
void_p=c_void_p


#load the library
NSClib = CDLL(rootDir+'/lib/libNSC.so')

NSClib.INIT.argtypes= None
NSClib.INIT.restype = void_p

NSClib.DEL.argtypes= void_p,
NSClib.DEL.restype = None

NSClib.SOLVE.argtypes= cdouble,cdouble,cdouble,cdouble,cdouble,cdouble,void_p,cdouble,cdouble,cdouble,cdouble,cdouble,cdouble,cdouble,cdouble, cuint, void_p
NSClib.SOLVE.restype = c_bool

NSClib.getResults.argtypes= POINTER(cdouble),void_p,
NSClib.getResults.restype = None

NSClib.getErrors.argtypes= POINTER(cdouble),POINTER(cdouble),void_p,
NSClib.getErrors.restype = None


NSClib.getSize.argtypes= void_p,
NSClib.getSize.restype = cint

NSClib.getPoints.argtypes=POINTER(cdouble),POINTER(cdouble),POINTER(cdouble),void_p
NSClib.getPoints.restype=None




class Evolution:
    '''
    The Evolution class solves the Boltzmann equations (BEs) of the plasma coupled to a decaying fluid.
    
    Methods: 
        solveNSC(): solves the system of BEs. After running this,  we have:
                    T_{E_1},T_{E_2},T_{D_1},T_{D_2},a_{E_1},a_{E_2},a_{D_1},a_{D_2}
                    in the variables self.TE1, self.TE2, self.TD1, self.TD2, 
                    self.uE1, self.uE2, self.uD1, self.uD2
        
        getPoints(): this stores the points of integration for a/a_i, T (in GeV), \\rho_Phi (in GeV^4), and log H in 
                        the numpy arrays self.u, self.T, self.rhoPhi

        setParams(TEND, c, Ti, ratio, umax, TSTOP): set the parameters of the NSC without rebuilding the 
                                            interpolations. Once this is ran, all the other member variables are reset
                                            intil we run self.solveNSC().

    '''
    def __init__(self):
        '''
        Evolution class default constructor       
        '''
        self.voidpNSC=void_p()
        self.voidpNSC=NSClib.INIT()
    

        self.u=[]
        self.T=[]
        self.rhoPhi=[]
        self.dT=[]
        self.drhoPhi=[]
        
        self.TE1=0
        self.TE2=0
        self.TD1=0
        self.TD2=0
        self.uE1=0
        self.uE2=0
        self.uD1=0
        self.uD2=0

        self.check_run=False

    def reset(self):
        self.u=[]
        self.T=[]
        self.rhoPhi=[]
        self.dT=[]
        self.drhoPhi=[]
        
        self.TE1=0
        self.TE2=0
        self.TD1=0
        self.TD2=0
        self.uE1=0
        self.uE2=0
        self.uD1=0
        self.uD2=0

    def __del__(self):
        '''destructor'''
        NSClib.DEL(self.voidpNSC) #don't forget to deallocated the pointer
        del self.voidpNSC
        
        del self.u
        del self.T
        del self.rhoPhi
        del self.dT
        del self.drhoPhi
        
        del self.TE1
        del self.TE2
        del self.TD1
        del self.TD2
        del self.uE1
        del self.uE2
        del self.uD1
        del self.uD2

    def solveNSC(self, TEND,c, Ti,ratio, TSTOP, umax, plasma,
           initial_step_size=1e-2,minimum_step_size=1e-8,maximum_step_size=1e-2, 
           absolute_tolerance=1e-8,relative_tolerance=1e-8,
           beta=0.9,fac_max=1.2,fac_min=0.8, maximum_No_steps=int(1e7)):
        '''
        Solve the system (returns wheather the integration completed successfully and the time it took to finish).       
        After this is finished, we get TE1, TE2, TD1,and TD2. In order to get the entire evolution
        run getPoints.
        
        Usage:
        solveNSC(TEND, c, Ti, ratio, TSTOP, umax, initial_step_size, minimum_step_size, maximum_step_size,
                absolute_tolerance, relative_tolerance, beta, fac_max, fac_min, maximum_No_steps)

        With:
        TEND: TEND [GeV] is defined from Gamma_Phi=H_R(TEND) [H_R is the Hubble rate in RD Universe]
        c: characterises the equation of state of Phi, with c=3(1+omega) and p=omega rho_Phi
        Ti, ratio: ratio = rho_Phi/rho_R at temperature Ti [GeV]. These are the initial conditions
        TSTOP: if the temperature drops below this, integration stops.
        umax: if u>umax the integration stops (rempember that u=log(a/a_i))
        plasma: instance of Cosmo class.
        -----------Optional arguments------------------------
        initial_stepsize: initial step the solver takes.
        maximum_stepsize: This limits the sepsize to an upper limit.
        minimum_stepsize: This limits the sepsize to a lower limit.
        absolute_tolerance: absolute tolerance of the RK solver.
        relative_tolerance: relative tolerance of the RK solver.
        Note:
        Generally, both absolute and relative tolerances should be 1e-8.
        In some cases, however, one may need more accurate result. Whatever the case, if the
        tolerances are below 1e-8, long doubles *must* be used.
        beta: controls how agreesive the adaptation is. Generally, it should be around but less than 1.
        fac_max,  fac_min: the stepsize does not increase more than fac_max, and less than fac_min.
        This ensures a better stability. Ideally, fac_max=inf and fac_min=0, but in reality one must
        tweak them in order to avoid instabilities.
        maximum_No_steps: maximum steps the solver can take Quits if this number is reached even if integration
        is not finished.

        '''
        self.plasma=plasma
        time0=time()
        if self.check_run:
            self.reset()

        success=NSClib.SOLVE(TEND,c,Ti,ratio,TSTOP,umax, self.plasma.pointer(),
                        initial_step_size,minimum_step_size, maximum_step_size, 
                        absolute_tolerance, relative_tolerance, beta,
                        fac_max, fac_min, maximum_No_steps,self.voidpNSC)
        ArrP = cdouble * 8 
        points=ArrP()

        NSClib.getResults(points,self.voidpNSC)
        self.TE1=points[0]
        self.TE2=points[1]
        self.TD1=points[2]
        self.TD2=points[3]
        self.uE1=points[4]
        self.uE2=points[5]
        self.uD1=points[6]
        self.uD2=points[7]
        
        self.check_run=True

        return success, time()-time0

    def getPoints(self):
        '''
        get all the points of integration: a/a_i T [GeV] rho_Phi [GeV^4] logH^2.
        You must have ran self.solve() before. 
        '''
        Arr = cdouble * NSClib.getSize(self.voidpNSC) 
        self.u=Arr()
        self.T=Arr()
        self.rhoPhi=Arr()

        NSClib.getPoints(self.u, self.T, self.rhoPhi, self.voidpNSC)
        
        self.u=np_array(list(self.u))
        self.T=np_array(list(self.T))
        self.rhoPhi=np_array(list(self.rhoPhi))
        
    def getErrors(self):
        '''
        This function stores the local errors of theta and zeta in
        self.dT and self.drhoPhi

        '''
        Arr = cdouble * NSClib.getSize(self.voidpNSC) 
        self.dT=Arr()
        self.drhoPhi=Arr()

        NSClib.getErrors(self.dT, self.drhoPhi, self.voidpNSC)
        
        self.dT=np_array(list(self.dT))
        self.drhoPhi=np_array(list(self.drhoPhi))
        
