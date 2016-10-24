#!/bin/bash
read -p "Input your birthday(YYMMDD):" date1
m=`date --date=$date1 +%m`
d=`date --date=$date1 +%d`
date_now=`date +%s`
y=`date +%Y`
birth=`date --date=$y$m$d +%s`
echo "birth = $birth"
echo "date_now = $date_now"
internal=$(($birth-$date_now))
echo "internal = $internal"
if [ "$internal" -lt "0" ];then
	birth=`date --date=$(($y+1))$m$d +%s`
	internal=$(($birth-$date_now))
fi
echo "There is :$((internal/60/60/24)) days ."
