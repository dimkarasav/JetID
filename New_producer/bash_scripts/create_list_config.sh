#!/bin/bash

#########################################Config variables that need to be set for set of big trees####################




Output_base_name="MinimumBias__Run2016G-21Feb2020_UL2016-v1__MINIAOD"
path_to_list="lists/UL16_MinBias_nonAPV/"
list_name="RunG"
isUL="true"  #set to "true", anything else will make it 'false' 
isCHS="false" #set to "true", anything else will make it 'false'
era="2016"   # this can be '2016', '2017' or '2018', maybe will crash for anything else.



#####################################################################################################################


usage ()
{
	echo ""
	echo "Arguments for script \"$0\" were given incorrectly or missing! Check the Usage & Example"
	echo ""
	echo "Usage:   $0 -list_No 'Number_of_first_list_to_be_submited' -dir Output_directory -log_dir log_files_directory"

	echo "Example: $0 -list_No 0 -dir /eos/cms/store/user/dkarasav/JetHT_Run2018-reReco_reduced_trees/JetHT__Run2018A-17Sep2018-v1__MINIAOD/ -log_dir Condor_Outputs/JetHT__Run2018A-17Sep2018-v1__MINIAOD/"

	echo ""
	exit 1;
}


#if [ $# -ne 3 ]; then usage; fi;
while [ $# -gt 0 ]; # till there are parameters
do
  case "$1" in
    -list_No) list_No="$2"; shift ;;
    -dir) directory="$2"; shift ;;
    -log_dir) log_directory="$2"; shift ;;
    *) usage ;;
  esac
  shift  # get following parameters
done


	# if a config list with the same name exists in the log directory, it gets deleted to replace it.
	if test -f "$log_directory"/config_file_"$list_No".txt; then
    	rm "$log_directory"/config_file_"$list_No".txt
	fi


	echo "directory=\""$directory"/\"" 									>>	 "$log_directory"/config_file_"$list_No".txt
	echo "Output_base_name=\""$Output_base_name"\"" 					>>	 "$log_directory"/config_file_"$list_No".txt
	echo "list_name=\""$list_name"_"$list_No".txt\"" 					>>	 "$log_directory"/config_file_"$list_No".txt
	echo "isUL=\""$isUL"\"" 											>>	 "$log_directory"/config_file_"$list_No".txt
	echo "isCHS=\""$isCHS"\"" 											>>	 "$log_directory"/config_file_"$list_No".txt
	echo "era=\""$era"\"" 												>>	 "$log_directory"/config_file_"$list_No".txt
	echo "list_No=\""$list_No"\"" 										>>	 "$log_directory"/config_file_"$list_No".txt
	echo "path_to_list=\""$path_to_list"\""			 					>>	 "$log_directory"/config_file_"$list_No".txt

	
