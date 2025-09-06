 1 #!/bin/bash
  2
  3 if [[ ${#} -ne 2 ]]; then
  4     echo "2 args"
  5     exit 1
  6 fi
  7
  8 if [[ ! "$1" =~ ^[[1-9][0-9]*|0]$ || ! "$2" =~ ^[[1-9][0-9]*|0]$ ]]; then
  9     echo "nums"
 10     exit 2
 11 fi
 12 $(mkdir ~/2016-SE-04/a)
 13 $(mkdir ~/2016-SE-04/b)
 14 $(mkdir ~/2016-SE-04/c)
 15
 16 while read file; do
 17     line_c=$(cat $file | wc -l)
 18     if [[ ${line_c} -lt ${1} ]]; then
 19         echo "${line_c} moving to a"
 20         $(mv ${file}  ~/2016-SE-04/a)
 21     elif [[ ${line_c} -gt ${2} ]]; then
 22          echo "${line_c} moving to c"
 23         $(mv ${file}  ~/2016-SE-04/c)
 24     else
 25          echo "${line_c} moving to b"
 26         $(mv ${file}  ~/2016-SE-04/b)
 27     fi
 28 done< <(find ~/2016-SE-04 -mindepth 1 -type f)
