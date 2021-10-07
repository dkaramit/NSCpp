from numpy import logspace


from sys import path as sysPath
from os import path as osPath
sysPath.append(osPath.join(osPath.dirname(__file__), '../../src'))

#load the module
from interfacePy import Cosmo 

cosmo=Cosmo('../../src/data/eos2020.dat',0,1e5)

for T in logspace(-5,5,50):
    print(
        'T=',T,'GeV\t',
        'H=',cosmo.Hubble(T),'GeV\t',
        'h_eff=',cosmo.heff(T),'\t',
        'g_eff=',cosmo.geff(T),'\t',
        's=',cosmo.s(T),'GeV^3\t',
    )



if False:
    import matplotlib.pyplot as plt

    #########-----g_eff and h_eff-----#########
    fig=plt.figure(figsize=(9,4))
    fig.subplots_adjust(bottom=0.15, left=0.15, top = 0.95, right=0.9,wspace=0.0,hspace=0.0)
    fig.suptitle('')

    sub = fig.add_subplot(1,1,1)

    T=logspace(-5,5,500)
    gt=[cosmo.geff(i) for i in T]
    ht=[cosmo.heff(i) for i in T]

    sub.plot(T,gt,linestyle='--',c='xkcd:red',label=r"$g_{\rm eff} (T)$")
    sub.plot(T,ht,linestyle=':',c='xkcd:black',label=r"$h_{\rm eff} (T)$")

    sub.set_xlabel(r'$T ~ [{\rm GeV}]$')
    sub.set_ylabel(r'rel. dof')

    sub.legend(bbox_to_anchor=(1, 0.0),borderaxespad=0., 
            borderpad=0.05,ncol=1,loc='lower right',fontsize=14,framealpha=0)
    sub.set_yscale('log')
    sub.set_xscale('log')

    fig.savefig('rdofs-T_examplePlot.pdf',bbox_inches='tight')



    #########-----dg_effdT and dh_effdT-----#########

    fig=plt.figure(figsize=(9,4))
    fig.subplots_adjust(bottom=0.15, left=0.15, top = 0.95, right=0.9,wspace=0.0,hspace=0.0)
    fig.suptitle('')

    sub = fig.add_subplot(1,1,1)

    T=logspace(-5,5,500)
    dg=[cosmo.dgeffdT (i) for i in T]
    dh=[cosmo.dheffdT(i) for i in T]

    sub.plot(T,dg,linestyle='--',c='xkcd:red',label=r"$\dfrac{d g_{\rm eff}}{dT} (T)$")
    sub.plot(T,dh,linestyle=':',c='xkcd:black',label=r"$\dfrac{d h_{\rm eff}}{dT} (T)$")

    sub.set_xlabel(r'$T ~ [{\rm GeV}]$')

    sub.legend(bbox_to_anchor=(1, 0.5),borderaxespad=0., 
            borderpad=0.05,ncol=1,loc='lower right',fontsize=14,framealpha=0)
    sub.set_yscale('symlog')
    sub.set_xscale('log')

    fig.savefig('drdofsdT-T_examplePlot.pdf',bbox_inches='tight')


    #########-----dh-----#########

    fig=plt.figure(figsize=(9,4))
    fig.subplots_adjust(bottom=0.15, left=0.15, top = 0.95, right=0.9,wspace=0.0,hspace=0.0)
    fig.suptitle('')

    sub = fig.add_subplot(1,1,1)

    T=logspace(-5,5,500)
    dht=[cosmo.dh(i) for i in T]
    sub.plot(T,dht,linestyle='-',c='xkcd:black') 
    sub.set_xlabel(r'$T ~ [{\rm GeV}]$')
    sub.set_ylabel(r'$\delta_h = 1 + \dfrac{1}{3} \dfrac{d \log h_{\rm eff} }{d \log T}$')
    sub.set_yscale('linear')
    sub.set_xscale('log')

    fig.savefig('dh-T_examplePlot.pdf',bbox_inches='tight')
