from  ctypes import CDLL, c_longdouble, c_double,c_void_p,c_char_p



from sys import path as sysPath
from os import path as osPath

sysPath.append(osPath.join(osPath.dirname(__file__), '../../'))

from misc_dir.path import _PATH_
from misc_dir.type import cdouble

void_p=c_void_p
char_p=c_char_p



#load the library
CosmoLib = CDLL(_PATH_+'/lib/libCosmo.so')

CppFunc=CosmoLib.heff,CosmoLib.geff,CosmoLib.dgeffdT,CosmoLib.dheffdT,CosmoLib.dh,CosmoLib.rhoR,CosmoLib.Hubble,CosmoLib.s  

for i,f in enumerate (CppFunc): 
    f.argtypes = cdouble, void_p
    f.restype = cdouble
###############################################

CosmoLib.INIT.argtypes=char_p, cdouble, cdouble
CosmoLib.INIT.restype=void_p

CosmoLib.DEL.argtypes= void_p,
CosmoLib.DEL.restype = None

CosmoLib.getT0.argtypes =None
CosmoLib.getT0.restype =cdouble
CosmoLib.geth_hub.argtypes =None
CosmoLib.geth_hub.restype =cdouble
CosmoLib.getrho_crit.argtypes =None
CosmoLib.getrho_crit.restype =cdouble
CosmoLib.getrelicDM.argtypes =None
CosmoLib.getrelicDM.restype =cdouble
CosmoLib.getMP.argtypes =None
CosmoLib.getMP.restype =cdouble


# CMB temperature today in GeV
T0=CosmoLib.getT0()
# critical density today in GeV^4
rho_crit=CosmoLib.getrho_crit()
# dimensionless hubble parameter
h_hub=CosmoLib.geth_hub()
# central value of Omega h^2 according to Planck 
relicDM_obs=CosmoLib.getrelicDM()
# Planck mass in GeV
mP=CosmoLib.getMP()

class Cosmo:
    '''Class that contains various cosmological parameters and functions.
    The available functions are:
    heff, geff, dgeffdT, dheffdT, dh, rhoR, Hubble, s. 
    All are functions of the temperature (in GeV), and correspond to the standard comsological scenario.
    The available variables are:
    T0: CMB temperature today in GeV
    rho_crit: critical density today in GeV^4
    h_hub: dimensionless hubble parameter
    relicDM_obs: central value of Omega h^2 according to Planck
    mP: Planck mass in GeV
    '''
    def __init__(self, cosmo_PATH, minT=0, maxT=mP):
        '''
        The constructor:
        cosmo_PATH: Relative path to a file that contains T [GeV] h_eff g_eff, with increasing T.
        minT: Minimum interpolation temperature (in GeV)(default 0) 
        maxT: Maximum interpolation temperature (in GeV) (default 1.22e19 GeV)
        Beyond the interpolation limits, the h_eff and g_eff are taken to be constant.
        '''
        _file_=char_p(bytes(cosmo_PATH, encoding='utf-8'))
        self.voidCosmo=CosmoLib.INIT(_file_,minT,maxT)

        # CMB temperature today in GeV
        self.T0=T0
        # critical density today in GeV^4
        self.rho_crit=rho_crit
        # dimensionless hubble parameter
        self.h_hub=rho_crit
        # central value of Omega h^2 according to Planck 
        self.relicDM_obs=relicDM_obs
        # Planck mass in GeV
        self.mP=mP


    # def __del__(self):
        # CosmoLib.DEL(self.voidCosmo)
    def heff(self,T):
        '''h_eff at temperature T [GeV]'''
        return CosmoLib.heff(T,self.voidCosmo)

    def geff(self,T):
        '''g_eff at temperature T [GeV] '''
        return CosmoLib.geff(T,self.voidCosmo)

    def dgeffdT(self,T):
        '''\\dfrac{dg_eff}{dT} at temperature T [GeV]'''
        return CosmoLib.dgeffdT(T,self.voidCosmo)

    def dheffdT(self,T):
        '''\\dfrac{dh_eff}{dT} at temperature T [GeV]'''
        return CosmoLib.dheffdT(T,self.voidCosmo)

    def dh(self,T):
        '''\\delta_h(T)=1+1/3 \frac{d log h_eff}{d log T} at temperature T [GeV]'''
        return CosmoLib.dh(T,self.voidCosmo)

    def rhoR(self,T):
        '''energy density of the plasma at temperature T [GeV]'''
        return CosmoLib.rhoR(T,self.voidCosmo)

    def Hubble(self,T): 
        '''H at temperature T [GeV]'''
        return CosmoLib.Hubble(T,self.voidCosmo)

    def s(self,T): 
        '''s (entropy density of the plasma) at temperature T [GeV]'''
        return CosmoLib.s(T,self.voidCosmo)


if __name__=="__main__":
    pass