#!/bin/bash

usage ()
{
	echo ""
	echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!ERROR!!!!!!!!!!!!!!!!!!!!!!!!!!!"
	echo ""
	echo "Arguments for script \"$0\" were given incorrectly or missing (3 arguments required) ! Check the Usage & Example"
	echo ""
	echo "Usage:   $0 -i 'list_to_be_submited' -dir 'Output_directory' -log_dir log_file_directory "

	echo "Example: $0 -i 0  -dir /eos/cms/store/user/dkarasav/JetHT_Run2018-reReco_reduced_trees/JetHT__Run2018A-17Sep2018-v1__MINIAOD/ -log_dir Condor_Outputs/JetHT__Run2018A-17Sep2018-v1__MINIAOD "

	echo ""
	exit 1;
}



#if [ $# -ne 5 ]; then usage; fi;
while [ $# -gt 0 ]; # till there are parameters
do
  case "$1" in
    -i) list_No="$2"; shift ;;
    -dir) output_directory="$2"; shift ;;
    -log_dir) log_directory="$2"; shift ;;
    *) usage ;;
  esac
  shift  # get following parameters
done

Full_path_to_DijetRootTreeAnalyzer=$(pwd)

if [ -z "$list_No" ]  || [ -z "$output_directory" ] || [ -z "$log_directory" ] || [ -z "$Full_path_to_DijetRootTreeAnalyzer" ];  #true if any argument is empty
then
	usage;
fi

		mkdir "$log_directory"
		mkdir "$output_directory"


		echo ""
		echo "Running list number:""$list_No""..."
		
		source bash_scripts/create_list_config.sh -list_No "$list_No" -dir "$output_directory" -log_dir "$log_directory"
		source bash_scripts/produce_reduced_trees.sh "$log_directory"/config_file_"$list_No".txt  "$Full_path_to_DijetRootTreeAnalyzer" 1
		
		list_No=$(( list_No+1 ))



