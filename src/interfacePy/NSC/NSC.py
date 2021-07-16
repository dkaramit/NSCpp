from ctypes import  CDLL, c_longdouble, c_double, c_void_p, c_int, POINTER
from numpy import array as np_array

from time import time

from sys import path as sysPath
from os import path as osPath

sysPath.append(osPath.join(osPath.dirname(__file__), '../../'))

from misc_dir.path import _PATH_
from misc_dir.type import cdouble

cint=c_int
void_p=c_void_p


#load the library
NSClib = CDLL(_PATH_+'/lib/libNSC.so')

NSClib.INIT.argtypes= cdouble,cdouble,cdouble,cdouble,cdouble,cdouble,cdouble,cdouble,cdouble,cdouble,cdouble,cdouble,cdouble,cdouble, cint
NSClib.INIT.restype = void_p

NSClib.DEL.argtypes= void_p,
NSClib.DEL.restype = None


NSClib.SOLVE.argtypes= void_p,
NSClib.SOLVE.restype = None

NSClib.getResults.argtypes= POINTER(cdouble),void_p,
NSClib.getResults.restype = None


NSClib.getSize.argtypes= void_p,
NSClib.getSize.restype = cint

NSClib.getPoints.argtypes=POINTER(cdouble),POINTER(cdouble),POINTER(cdouble),POINTER(cdouble),void_p
NSClib.getPoints.restype=None




class NSC:
    def __init__(self, TEND,c, Ti,ratio,umax, TSTOP,
           initial_step_size=1e-2,minimum_step_size=1e-8,maximum_step_size=1e-2, 
           absolute_tolerance=1e-8,relative_tolerance=1e-8,
           beta=0.9,fac_max=1.2,fac_min=0.8, maximum_No_steps=int(1e7)):
        
        self.voidpNSC=void_p()
        self.voidpNSC=NSClib.INIT(TEND,c,Ti,ratio,umax,TSTOP,
                        initial_step_size,minimum_step_size, maximum_step_size, 
                        absolute_tolerance, relative_tolerance, beta,
                        fac_max, fac_min, maximum_No_steps)
    
        self.a_ai=[]
        self.T=[]
        self.rhoPhi=[]
        self.logH2=[]
        self.TE1=Ti
        self.TE2=Ti
        self.TD1=Ti
        self.TD2=Ti
        self.aE1=1
        self.aE2=1
        self.aD1=1
        self.aD2=1

    def delete(self):
        NSClib.DEL(self.voidpNSC)
        del self.voidpNSC

    def __del__(self):
        self.delete()
        
        del self.a_ai
        del self.T
        del self.rhoPhi
        self.logH2
        
        del self.TE1
        del self.TE2
        del self.TD1
        del self.TD2
        del self.aE1
        del self.aE2
        del self.aD1
        del self.aD2


    def solve(self):
        time0=time()
        NSClib.SOLVE(self.voidpNSC)
        ArrP = cdouble * 8 
        points=ArrP()

        NSClib.getResults(points,self.voidpNSC)
        self.TE1=points[0]
        self.TE2=points[1]
        self.TD1=points[2]
        self.TD2=points[3]
        self.aE1=points[4]
        self.aE2=points[5]
        self.aD1=points[6]
        self.aD2=points[7]

        return time()-time0

    def getPoints(self):

        Arr = cdouble * NSClib.getSize(self.voidpNSC) 
        self.a_ai=Arr()
        self.T=Arr()
        self.rhoPhi=Arr()
        self.logH2=Arr()

        NSClib.getPoints(self.a_ai, self.T, self.rhoPhi, self.logH2, self.voidpNSC)
        
        self.a_ai=np_array(list(self.a_ai))
        self.T=np_array(list(self.T))
        self.rhoPhi=np_array(list(self.rhoPhi))
        self.logH2=np_array(list(self.logH2))
        
