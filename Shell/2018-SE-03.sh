  1 #!/bin/bash
  2
  3 if [[ $# -ne 2 ]]; then
  4     echo args>&2
  5     exit 1
  6 fi
  7
  8 if [[ ! -f $1 || ! -f $2 ]]; then
  9     echo not files>$2
 10     exit 2
 11 fi
 12
 13 file=$(cat $1 | sed "s|,|;|")
 14 if [[ -e $2 ]]; then rm $2; fi
 15 touch $2
 16 echo "$file" | while IFS=";" read num line; do
 17
 18     similar=$(echo "$file" | grep -E "^[^$num];"$line"$")
 19
 20     if [[ -n $similar ]]; then
 21         num=$(echo "$similar" | awk -F ';' -v num=$num '{if ($1 < num) num=$1} END {print num}')
 22     fi
 23
 24     if [[ -z $(grep -E ",$line$" "$2") ]]; then
 25         echo "$num,$line" >> $2
 26     fi
 27 done
 28 cat $2
