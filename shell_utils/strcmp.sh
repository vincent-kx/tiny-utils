#!/bin/sh

str=$1
pattern=$2

#if [ -z "${str##*$pattern*}" ];then
if [[ "$str" =~ .*"$pattern".* ]];then
        echo "yes"
else
        echo "no"
fi