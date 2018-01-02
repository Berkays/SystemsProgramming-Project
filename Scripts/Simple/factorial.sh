#!/bin/bash
n=$1
k=$1
i=$(($n - 1))
if [ $n -lt 0 ]
then
echo "Invalid number"
exit 0
else
while [ $i -gt 0 ]
do
   n=$(($n * $i))
   i=$(( $i - 1 ))
done
echo "Factorial of $k is $n"
exit 1
fi
