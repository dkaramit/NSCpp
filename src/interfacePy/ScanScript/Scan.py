from sys import path as sysPath
from os import path as osPath
sysPath.append(osPath.join(osPath.dirname(__file__), '../'))
sysPath.append(osPath.join(osPath.dirname(__file__), '../../src'))
from misc_dir.path import _PATH_


from numpy import array, float64, savetxt


from subprocess import check_output as subprocess_check_output
from sys import stdout as sys_stdout
from time import time
from datetime import datetime
from datetime import timedelta

from os import remove as os_remove
from os import system as os_system
from pathlib import Path


parallelScan=_PATH_+r"/src/interfacePy/ScanScript/parallel_scan.sh"


class Scan:
    def __init__(self,cpus,TEND,c,Ti,ratio,umax,TSTOP,
                PathToCppExecutable, break_after=0,break_time=60,break_command='',
                initial_step_size=1e-2, minimum_step_size=1e-8, maximum_step_size=1e-2, 
                absolute_tolerance=1e-8, relative_tolerance=1e-8,
                beta=0.9, fac_max=1.2, fac_min=0.8, maximum_No_steps=int(1e7)):
        '''
        
        Comment: If the scan exits before it finishes, it will continue from the point it stopped.
        In order to start from the beginning, delete the file "count.dat".
        
        TEND:
        
        c:

        Ti: 
        
        ratio:
        
        cpus: number of points to run simultaneously (No of cpus available). 

        umax: if u>umax the integration stops (rempember that u=log(a/a_i))
        TSTOP: if the temperature drops below this, integration stops
        PathToCppExecutable: 


        break_after,break_time: take a break after break_after seconds for break_time seconds
        (optional) break_command: before it takes a break, run this system command (this may be a script to send the results
        to an e-mail, or back them up)

        -----------Optional arguments------------------------
        initial_stepsize: initial step the solver takes. 

        maximum_stepsize: This limits the sepsize to an upper limit. 
        minimum_stepsize: This limits the sepsize to a lower limit. 
        
        absolute_tolerance: absolute tolerance of the RK solver

        relative_tolerance: relative tolerance of the RK solver
        Note:
        Generally, both absolute and relative tolerances should be 1e-8. 
        In some cases, however, one may need more accurate result (eg if f_a is extremely high, 
        the oscillations happen violently, and the ODE destabilizes). Whatever the case, if the  
        tolerances are below 1e-8, long doubles *must* be used.

        beta: controls how agreesive the adaptation is. Generally, it should be around but less than 1.
        
        fac_max,  fac_min: the stepsize does not increase more than fac_max, and less than fac_min. 
        This ensures a better stability. Ideally, fac_max=inf and fac_min=0, but in reality one must 
        tweak them in order to avoid instabilities.

        maximum_No_steps: maximum steps the solver can take Quits if this number is reached even if integration
        is not finished. 
        '''
        
        self.cpus=cpus
        
        self.umax=umax
        self.TSTOP=TSTOP
        
        self.PathToCppExecutable=PathToCppExecutable

        self.break_after=break_after        
        self.break_time=break_time
        self.break_command=break_command
        
        self.initial_step_size=initial_step_size
        self.minimum_step_size=minimum_step_size
        self.maximum_step_size= maximum_step_size
        self.absolute_tolerance=absolute_tolerance
        self.relative_tolerance=relative_tolerance
        self.beta=beta
        self.fac_max=fac_max
        self.fac_min=fac_min
        self.maximum_No_steps=maximum_No_steps

        self.FileDate=datetime.now()
        self.FileName = "{}".format(self.FileDate.strftime('%d-%m-%Y_%H-%M-%S')) 
        self._p = Path(self.FileName+'.dat')
        
        
        self.in_file="in.xtx"
        self.count_file="count.xtx"
    def run_batch(self):
        '''
        run a batch.
        '''
        # get the result     
        points=subprocess_check_output( [parallelScan,  self.PathToCppExecutable, str(self.cpus),self.in_file]).decode(sys_stdout.encoding)
        points=points.split('\n')
        

        points=array([array(i.split(),float64)  for i in points[:-1] ])
        points=array(sorted(points, key=lambda arr: arr[0]))

        File= self._p.open('ab')            
        savetxt(File,points)
        File.close()

    def run(self):

        try:
            with open(self.count_file,'r') as _:
                last_batch=int(_.read())
        except:
            last_batch=0
            
        Total_batches=len(self.Table_fa)-last_batch
            
        totalT=0
        meanT=0
        ETA=0
        batch=0
        
        sleepTimer=0
        for TEND in self.TEND[last_batch:]:
            time0=time()

            file=open(self.in_file , 'w')
            for c,Ti,ratio in self.NSCparams :
                file.write( '{0} {1} {2} {3} {4} {5} {6} {7} {8} {9} {10} {11} {12} {13} {14} {15} {16} {17} {18}\n'.
                format(TEND, c, Ti, ratio, self.umax, self.TSTOP, self.ratio_ini, self.Npeaks, self.conv, self.inputFile,
                self.initial_step_size, self.minimum_step_size, self.maximum_step_size, self.absolute_tolerance, self.relative_tolerance, self.beta,
                self.fac_max, self.fac_min, self.maximum_No_steps) )
            file.close()
            
            self.run_batch()

            
            batch+=1
            totalT+=time()-time0
            meanT=totalT/batch

            if self.break_after>0:
                sleepTimer+=time()-time0
                if sleepTimer>self.break_after:
                    os_system(self.break_command)
                    print("taking a break")
                    time.sleep(self.break_time)
                    
                    sleepTimer=0
                
            ETA=meanT*(Total_batches-batch)

            print('======================================== \n',
            'Completed batches:  ',batch,' out of ', Total_batches ,'\n',
            'Running for:     {0:.50s}'.format( str(timedelta(seconds=totalT))  ), '\n', 
            'ETA:             {0:.50s}'.format(str(timedelta(seconds=ETA) )  ),'\n', 
            'Time per batch:  {0:.3f} sec'.format(meanT),
            '\n========================================' )
            
            
            with open(self.count_file,'w') as _:
                _.write(str(batch+last_batch))

        os_remove(self.in_file)
        os_remove(self.count_file)
        print('Done!')
    