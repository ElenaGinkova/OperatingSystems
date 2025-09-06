  1 #!/bin/bash
  2
  3 if [[ ${#} -ne 2 ]]; then
  4     echo "args"
  5     exit 1
  6 fi
  7
  8 lines_1=$(grep -c "$1" "$1")
  9 lines_2=$(cat ${2} | grep ${2} | wc -l) # ekvivalenti sa
 10
 11 winner=0
 12 if [[ $lines_1 -gt $lines_2 ]]; then
 13     winner=${1}
 14 else
 15     winner=${2}
 16 fi
 17
 18 cat "$winner" | sed -E "s/^[0-9]+г\.\s*${winner}\s*-\s*//" | sort > "${winner}.songs"
 19 cat ""${winner}".songs"
