 1 #!/bin/bash
  2 nums=$(mktemp)
  3 max_mod=0
  4 line_f=$(mktemp)
  5 while IFS="\n" read line; do
  6     echo "$line" > $line_f
  7     mod=$line
  8     if [[ -n $(grep -E "^[-]?[1-9]+[0-9]*$" $line_f) && -z $(grep -E "^$line$" $nums) ]]; then
  9         if [[ -n $(grep "-" $line_f) ]]; then
 10             mod=$(echo $line | cut -c 2-)
 11         fi
 12         echo mod $mod
 13         if [[ $mod -gt $max_mod ]]; then
 14             echo changing
 15             max_mod=$(echo $mod)
 16             echo $line > $nums
 17         elif [[ $mod -eq $max_mod ]]; then
 18             echo adding
 19             echo $line >> $nums
 20         fi
 21     fi
 22 done
 23 cat $nums
 24 rm $nums $line_f
