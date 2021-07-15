import ctypes
import os
import numpy as np
import time

from .Cosmo import *
from .src.misc_dir.path import _PATH_
from .src.misc_dir.type import cdouble


cint=ctypes.c_int
void_p=ctypes.c_void_p

#load the library
NSC = ctypes.CDLL(_PATH_+'/lib/NSC_py.so')

NSC.INIT.argtypes= None
NSC.INIT.restype = ctypes.CFUNCTYPE( void_p, cdouble, cdouble, cdouble, cdouble, cdouble, cdouble, cdouble)

NSC.INIT.argtypes= None
NSC.INIT.restype = ctypes.CFUNCTYPE( void_p, cdouble, cdouble, cdouble, cdouble, cdouble, cdouble, cdouble)

NSC.SOLVE.argtypes= None
NSC.SOLVE.restype = ctypes.CFUNCTYPE( None, cdouble, void_p)

NSC.MAKEINT.argtypes= None
NSC.MAKEINT.restype = ctypes.CFUNCTYPE( None, void_p)

NSC.DEL.argtypes= None
NSC.DEL.restype = ctypes.CFUNCTYPE( None, void_p)


NSC.EVALT.argtypes= None
NSC.EVALT.restype = ctypes.CFUNCTYPE( cdouble, cdouble, void_p)

NSC.EVALLOGH2.argtypes= None
NSC.EVALLOGH2.restype = ctypes.CFUNCTYPE( cdouble, cdouble, void_p)

NSC.EVALDLOGH2DU.argtypes= None
NSC.EVALDLOGH2DU.restype = ctypes.CFUNCTYPE( cdouble, cdouble, void_p)

NSC.getSize.argtypes= None
NSC.getSize.restype = ctypes.CFUNCTYPE( cint,  void_p)


NSC.getResults.argtypes= ctypes.POINTER(cdouble),ctypes.POINTER(cdouble),ctypes.POINTER(cdouble),ctypes.POINTER(cdouble), ctypes.POINTER(cdouble), void_p
NSC.getResults.restype=None




class Boltzmann:
    def __init__(self, TEND, c, Ti, ratio, umax, TSTOP=5e-4):
        self.voidNSC=void_p()
        self.voidNSC=NSC.INIT()(TEND, c, Ti, ratio, umax, TSTOP)
    

    def delete(self):
        NSC.DEL()(self.voidNSC)
        del self.voidNSC

    def __del__(self):
        self.delete()
        
        del self.a_ai
        del self.T
        
        del self.rho
        del self.rhoPhi
        del self.TE1
        del self.TE2
        del self.TD1
        del self.TD2
        del self.aE1
        del self.aE2
        del self.aD1
        del self.aD2


    def solve(self, _timeit=True, fa=mP):
        time0=time.time()
        NSC.SOLVE()(fa,self.voidNSC)

        max_int=NSC.getSize()(self.voidNSC)

        Arr = cdouble * max_int # Define an array of doubles with the same length as _A 
        ArrP = cdouble * 10 # Define an array of doubles with the same length as _A 
        self.a_ai=Arr()
        self.rho=Arr()
        self.T=Arr()
        self.rhoPhi=Arr()
        points=ArrP()

        NSC.getResults(self.a_ai, self.T, self.rho, self.rhoPhi, points, self.voidNSC)
        
        self.a_ai=np.array(list(self.a_ai))
        self.T=np.array(list(self.T))
        self.rho=np.array(list(self.rho))
        self.rhoPhi=np.array(list(self.rhoPhi))
        
        self.TE1=points[0]
        self.TE2=points[1]
        self.TD1=points[2]
        self.TD2=points[3]
        self.aE1=points[4]
        self.aE2=points[5]
        self.aD1=points[6]
        self.aD2=points[7]
        


        if(_timeit):
            print(time.time()-time0)
        return time.time()-time0

