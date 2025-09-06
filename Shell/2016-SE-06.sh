  1 #!/bin/bash
  2
  3 if [[ $# -ne 1 ]]; then
  4    echo args
  5    exit 1
  6 fi
  7
  8 temp=$(mktemp)
  9 count=0
 10 to_repl=''
 11 while read line; do
 12     #s cut ot - natatuk
 13     to_repl=$(echo "${line}" | sed -E "s/^[0-9]+ *г\. - /${count}. /")
 14     echo "$to_repl" >> $temp
 15     count=$(( count + 1 ))
 16 done< "$1"
 17 cat "$temp" | sort -t ' ' -k 2
 18 rm "$temp"
