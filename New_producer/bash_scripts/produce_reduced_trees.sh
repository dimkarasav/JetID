#!/bin/bash


input_config_txt=$1
Full_path=$2
source "$Full_path"/"$input_config_txt"

export DISPLAY=localhost0.0

run_local=$3  


if [ $run_local -ne 1 ] ;
then
	cp "$Full_path"/makefile .
	cp "$Full_path"/ProduceReducedTrees_main.C .
	cp "$Full_path"/"$path_to_list"/"$list_name" .

fi

	cp "$Full_path"/"$path_to_list"/"$list_name" .




#echo ""
#echo 'list No: '$list_No
#echo 'list copied: '$path_to_list 
#echo 'Output directory: '$directory


./makefile
./main -y "$era" -t "$isCHS" -u "$isUL" -n "$Output_base_name" -d "$directory" -l "$list_name" -i "$list_No"


if [ $run_local -ne 1 ] ;
then
	rm makefile
	rm ProduceReducedTrees_main.C
#	rm "$list_name"
fi

#	rm "$list_name"



