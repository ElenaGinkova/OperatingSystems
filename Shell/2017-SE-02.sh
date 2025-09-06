 1 #!/bin/bash
  2
  3 if [[ $# -ne 3 ]]; then
  4     echo args>&2
  5     exit 1
  6 fi
  7
  8 if [[ ! -d ${1} || ! -d ${2} ]]; then
  9     echo 1 not dir>&2
 10     exit 2
 11 fi
 12
 13 if [[ ! -z "$(find ${2} -mindepth 1)" ]]; then
 14 #    echo $(find ${2} -mindepth 1)
 15     echo not empty dir>&2
 16     exit 3
 17 fi
 18
 19 #if [[ ! $(whoami) == 'root' ]]; then
 20 # exit 4
 21 #fi
 22
 23 find ${1} -name "*$3*" | while read -r file; do
 24     rel_dir=$(echo $file | sed "s|^$1/||") #remove the source
 25     dir=$(dirname $rel_dir) #remove the file
 26     mkdir -p "${2}/${dir}"
 27     mv "${file}" "${2}/${rel_dir}"
 28 done
