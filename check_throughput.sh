#!/bin/sh

# check throughput data script

echo ONE,TWO,THREE

filename=$1
flag123="0"

cat $filename |
while read data
do
{
  	if [ "$flag123" == "0" ] 
	then
  		has123=`echo "$data" | sed -n -e 's/^.*] ONE.*$/1/p' -e 's/^.*] TWO.*$/2/p' -e 's/^.*] THREE.*$/3/p'`
 
  		if [ "$has123" == "1" ] || [ "$has123" == "2" ] || [ "$has123" == "3" ] 
		then
    		flag123=$has123
  		fi
	fi

	if [ "$flag123" == "1"  ] || [ "$flag123" == "2" ] || [ "$flag123" == "3" ] 
	then
  		lineFps=`echo "$data" | grep "Throughput" | cut -b57-66`
  		if [ "$flag123" == "1" ] && [ "x$lineFps" != "x" ] 
		then
    		line_result=$lineFps
			flag123="0"
  		elif [ "$flag123" == "2" ] && [ "x$lineFps" != "x" ] 
		then
    		line_result=$line_result,$lineFps
			flag123="0"
  		elif [ "$flag123" == "3" ] && [ "x$lineFps" != "x" ] 
		then
    		line_result=$line_result,$lineFps
    		echo $line_result
    		flag123="0"
  		fi
	fi

} done
