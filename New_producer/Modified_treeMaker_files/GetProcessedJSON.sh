#!/bin/bash

#example:
#./GetProcessedJSON.sh Output_UL16RunH/ JSON_Certified/ JSON_RunH

input_dir=$1
output_dir=$2
name=$3

#grep "Task name" "$input_dir"/*/workdir/crab*/crab.log

current_path=$(pwd) 

Output_of_grep=$(grep "Task name" "$input_dir"/*/workdir/crab*/crab.log) #find the line that has the task name

#echo "Output of grep"
#echo $Output_of_grep



substringBefore="UTC: 	 Task name: " #substring to separate the task name from grep output
substringAfter="INFO "

temp=${Output_of_grep#*$substringBefore}


#echo "temp1 = "
#echo $temp

task_name=${temp%$substringAfter*}
task_name=${task_name%$substringAfter*}


while [[ $task_name == *"INFO"* ]]
do
   task_name=${task_name%$substringAfter*}
   task_name=${task_name%$substringAfter*}
done

echo "task_name = "

echo $task_name


cd $input_dir

crab remake --task="$task_name"

crab report crab_*

cp crab_*/results/processedLumis.json "$current_path"/"$output_dir"/processedLumis_"$name".json
