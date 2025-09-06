 1 #!/bin/bash
  2
  3 if [[ ${#} -ne 1 && ${#} -ne 2 ]]; then
  4     echo "args!!"
  5     exit 1
  6 fi
  7
  8 if [[ $# -eq 2 ]]; then
  9     echo 2 arg
 10     while read num file;do
 11         if [[ $num -gt $2 ]]; then
 12          echo $file
 13         fi
 14     done< <(find $1 -printf "%n %p\n")
 15 else
 16     echo 1 arg
 17    # find ${1} -mindepth 1 -type l 2>/dev/null -exec test ! -e {} \; -print
 18     find ${1} -L -type l 2>/dev/null
 19 fi
