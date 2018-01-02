#!/bin/bash
key=$1
file=$2
replaceKey=$3
if [ -z "$key" ]
then
echo "No search key specified."
exit 1
elif [ -z "$file" ]
then
echo "No file specified."
exit 1
else
count="$(grep -o "$key" < $file | wc -l)"
echo "$count $key words found in $file"
if [ -n "$replaceKey" ]
then
sed -i -e "s/$key/$replaceKey/g" $file
echo "Replaced found words with $replaceKey"
fi
exit 0
fi

