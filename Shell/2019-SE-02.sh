 1 #!/bin/bash
  2
  3 if [[ $# -eq 0 ]]; then
  4     echo args>&2
  5     exit 1
  6 fi
  7
  8 n=10
  9 if [[ $1 == "-n" ]]; then
 10     n=$2
 11     shift 2
 12 fi
 13
 14
 15 result=$(mktemp)
 16 while read file; do
 17     IDF=$(basename ${file} | sed "s|.log||" )
 18     lines=$(cat $file | tail -n ${n})
 19     echo "$lines" | while read line; do
 20        date=$(echo $line | awk '{print $1}')
 21        time=$(echo $line | awk '{print $2}')
 22        data=$(echo $line | awk '{for (i=3; i<=NF; i++) printf "%s ", $i}')
 23        echo "$date $time $IDF $data" >> $result
 24     done
 25 done< <(echo "$@" | tr ' ' '\n')
 26
 27 cat $result | sort -t ' ' -k1,2
 28 rm $result
