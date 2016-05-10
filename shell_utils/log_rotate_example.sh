#/bin/sh

BASE_PATH=$(dirname `which $0`)
cd ${BASE_PATH}
max_log_file_size=209715200
max_log_file_num=10
#max_log_file_size=2048
#max_log_file_num=3

log_path=../log
logfilename=edu_user_contact_loader
logfile=$log_path/$logfilename.log

if [[ ! -e $logfile ]]; then
	./edu_user_contact_loader >> $logfile 2>&1
else
	filesize=$(ls -l $logfile | awk {'print $5'})
	if [[ $filesize -ge $max_log_file_size ]]; then
		logfilenum=$(ls -l $log_path/ | wc -l)

		if [[ $logfilenum -gt $max_log_file_num ]]; then
			delfile=$(ls -lt ../log/ | awk '{print $9}' | tail -n1)
			rm $log_path/$delfile
		fi

		datestr=$(date +%s)
		mv $log_path/$logfilename.log $log_path/$logfilename.$datestr.log
		./edu_user_contact_loader >> $log_path/$logfilename.log 2>&1
	else
		./edu_user_contact_loader >> $log_path/$logfilename.log 2>&1
	fi
fi
