  1 #!/bin/bash
  2
  3 #proverka na formata na failovete yy-mm..
  4 if [[ $# -ne 1 ]]; then
  5     echo args>&2
  6     exit 1
  7 fi
  8
  9 # 1 -logdir
 10 if [[ ! -d "$1" ]]; then
 11     echo not valid dir >&2
 12     exit 2
 13 fi
 14
 15 friends=$(find "$1" -mindepth 3 -maxdepth 3 -type d | xargs -I{} basename {} | sort | uniq )
 16 fr_rows=$(mktemp)
 17 echo "$friends" | while read -r friend; do
 18     lines=0
 19     echo $friend
 20     while read file; do
 21         echo $file
 22         cur_lines=$(wc -l < "$file")
 23         lines=$(($lines + $cur_lines))
 24     done< <(find "$1" -type f -mindepth 4 -maxdepth 4 | grep -E "^([^/]+/){4}$friend/[^/]+$")
 25     echo "$friend $lines" >> $fr_rows
 26 done
 27 cat $fr_rows | sort -k2 -nr | head -n 10
 28
 29 rm $fr_rows
