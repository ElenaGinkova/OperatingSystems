  1 #!/bin/bash
  2
  3 nums=$(mktemp)
  4 while IFS="\n" read line; do
  5     if [[ $line =~ ^-?([1-9]+[0-9]*|0)$ ]]; then
  6         echo "$line" >> $nums
  7     fi
  8 done
  9 sum=0
 10 while read cur_num; do
 11     cur_sum=$(grep -Eo "[0-9]" <(echo $cur_num) | awk '{sum+=$1} END {print sum}')
 12     if [[ $cur_sum -gt $sum ]]; then
 13         sum=$(echo $cur_sum)
 14         num=$(echo $cur_num)
 15     elif [[ $cur_sum -eq $sum && ($cur_num -lt $num || -z $num) ]]; then
 16         num=$(echo $cur_num)
 17         sum=$(echo $cur_sum)
 18     fi
 19 done< $nums
 20 echo $num
 21 rm $nums
 22
 23 #abs=$(( num < 0 ? -num : num))
 24 #while [[ $abs -gt 0 ]]; do
 25 #    digit=$(( abs % 10 ))
 26 #    sum=$(( sum + digit))
 27 #    abs=$(( abs / 10 ))
 28 #done
 29
