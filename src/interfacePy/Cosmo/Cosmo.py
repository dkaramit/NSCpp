from  ctypes import CDLL, c_longdouble, c_double

from sys import path as sysPath
from os import path as osPath

sysPath.append(osPath.join(osPath.dirname(__file__), '../../'))

from misc_dir.path import _PATH_
from misc_dir.type import cdouble

#load the library
func = CDLL(_PATH_+'/lib/libCosmo.so')

CppFunc=func.heff,func.geff,func.dgeffdT,func.dheffdT,func.dh,func.rhoR,func.Hubble,func.s  

#specify the argument types
for i,f in enumerate (CppFunc): 
    f.argtypes = cdouble,
    #specify the return type
    f.restype = cdouble
###############################################

# heff(T): h_eff at temperature T [GeV]
heff=func.heff
# geff(T): g_eff at temperature T [GeV]
geff=func.geff
# dgeffdT(T): \dfrac{dg_eff}{dT} at temperature T [GeV]
dgeffdT=func.dgeffdT
# dheffdT(T): \dfrac{dh_eff}{dT} at temperature T [GeV]
dheffdT=func.dheffdT
# dh(T): \delta_h(T)=1+1/3 \frac{d log h_eff}{d log T} at temperature T [GeV]
dh=func.dh

# rhoR(T): \rho_R (energy density of the plasma) at temperature T [GeV]
rhoR=func.rhoR
# Hubble(T): H at temperature T [GeV]
Hubble=func.Hubble
# s(T): s (entropy density of the plasma) at temperature T [GeV]
s=func.s

func.getT0.argtypes =None
func.getT0.restype =cdouble
func.geth_hub.argtypes =None
func.geth_hub.restype =cdouble
func.getrho_crit.argtypes =None
func.getrho_crit.restype =cdouble
func.getrelicDM.argtypes =None
func.getrelicDM.restype =cdouble
func.getMP.argtypes =None
func.getMP.restype =cdouble

# CMB temperature today in GeV
T0=func.getT0()
# critical density today in GeV^4
rho_crit=func.getrho_crit()
# dimensionless hubble parameter
h_hub=func.geth_hub()
# central value of Omega h^2 according to Planck 
relicDM_obs=func.getrelicDM()
# Planck mass
mP=func.getMP()


if __name__=="__main__":
    pass