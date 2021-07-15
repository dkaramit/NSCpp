#! /bin/bash
# A simple script that runs in a command ("_comm_") in parallel.
# The command runs "cpus" simultateus times, while reading the file 



if [ "$#" -lt "3" ]
then
    echo "Number of parameters passed must be 3 or 4."
    echo "Usage:
            parallel_scan.sh command cpus file_in "
    exit
fi

_comm_="$1"

tot=0
while read -r line; do
    tot=$((tot+1))    
    args[$tot]="$line"     
done < $3





cpus=$2

if [ "$cpus" -gt  "$tot" ]
then
    cpus=$tot
fi


N_batches=`python -c "print(  int( $tot / $cpus)  )"`
R_batches=`python -c "print( $tot - $N_batches * $cpus  )"`


# echo "----------"
# echo $cpus
# echo $N_batches
# echo $R_batches
# echo "----------"





res_file=$4
if [ -z "$4" ] 
then
    res_file="/tmp/_tmp_par"

else
    touch $4"-1"  2>/tmp/err && rm $4"-1" || res_file="/tmp/_tmp_par-" 
fi
res_file=$res_file"_$$"
#the temporary file is res_file_$BASHPID_process_numer


#run the first batch:
total_ran=0
for i in $(seq 1 1 $cpus )
do      
    total_ran=$((total_ran+1))
    $_comm_ ${args[total_ran]} > $res_file"_$i" &
    pid[$i]=$!
done



#Keep launching new porcesses to continuue running "cpus" simultaneously
while true
do
    running=0 

    #check if a process is finnished  
    for i in $(seq 1 1 $cpus )
    do 
        if ps -p ${pid[i]} > /dev/null
        then
            running=$((running +  1))
        else
            #if its finnished, print the result and delete the tmp file (if result file exists)
            if test -f $res_file"_$i" 
            then 
                echo `cat $res_file"_$i"` 
                rm $res_file"_$i"
            fi

            #if the total number of runs (cpus*N_batches) is not finnished, and process $i is finnished, run a new on.
            if [ "$total_ran" -lt "$((cpus * N_batches + R_batches))" ]
            then

                total_ran=$((total_ran+1))
                $_comm_ ${args[total_ran]} > $res_file"_$i" &
                pid[$i]=$!
                running=$((running +  1))
            fi
        fi
    done
    
    
    # echo  "currently running: $running"
    
    #exit when 
    if [ "$running" = "0" ]
    then
        # echo "finnished all $total_ran runs!!" 
        exit
    fi



done 



