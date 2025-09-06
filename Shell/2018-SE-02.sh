 1 #!/bin/bash
  2
  3 if [[ $# -ne 2 ]]; then
  4     echo args>&2
  5     exit 1
  6 fi
  7
  8 if [[ ! -f $1 || ! -d $2 ]]; then
  9     echo invalid input>&2
 10     exit 2
 11 fi
 12
 13 if [[ -n $(find $1 -mindepth 1) ]]; then
 14     echo dir should be empty>&2
 15     exit 3
 16 fi
 17
 18 if [[ ! -e "$2/dict.txt" ]]; then
 19     touch $2/dict.txt
 20 fi
 21
 22 cat "$1" | tr -s ' ' | grep -Eo "^[A-Za-z]+ [A-Za-z]+" | sort | uniq | awk -v cnt=0 '{print $0 ";"
 23
 24 cat $2/dict.txt | while IFS=";" read -r ime nomer; do
 25     touch $2/$nomer.txt 2>/dev/null
 26     IFS=" " read name1 name2 <<< "${ime}"
 27     #name1=$(echo "$ime" | awk '{print $1}')
 28     #name2=$(echo "$ime" | awk '{print $2}')
 29     grep -E "^$name1 +$name2" $1 > $2/$nomer.txt
 30     cat $2/$nomer.txt
 31 done
