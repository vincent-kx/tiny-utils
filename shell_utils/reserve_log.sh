#!/bin/sh

echo "arg num:$# ,args:$*"

if [ $# == 1 ];then
    path=$1
elif [ $# == 0 ];then
    path=.
else
    echo "error args"
    exit 1
fi

reserve_len=7

now=$(date +%Y%m%d)
one_day_ago=$(date -d -1day +%Y%m%d)

for ((i=0; i<$reserve_len; i++));do
    date_arr[$i]=$(date -d -${i}day +%Y%m%d)
done

echo ${date_arr[@]}

isfound=0

for log_file in $(ls $path | egrep *.log);do

    #for d in ${date_arr[@]};do
    if [[ "$log_file" =~ .*"$d".*log ]];then
        if [ -z "${log_file##*$d*}" ];then
            isfound=1
        fi
    done
    if [ $isfound == 0 ];then
        rm $path/$log_file
        echo "$log_file removed"
    fi
    isfound=0

done

echo "clear done."