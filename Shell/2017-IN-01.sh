  1 #!/bin/bash
  2
  3 if [[ $# -ne 3 ]]; then
  4     echo ...
  5     exit 1
  6 fi
  7
  8 #     1-file, 2-key_1, 3-key_2
  9 #1 cepim po redove
 10 #2 cepim na kluch i stoinost
 11 #3 cepim argumentite po interval
 12 #xor na arg ot $2 s $1
 13
 14 key_1=$(grep -E "^"$2"=" "$1" | cut -d '=' -f1)
 15 values_1=$(grep -E "^"$2"=" "$1"| cut -d '=' -f2)
 16 key_2=$(grep -E "^"$3"=" "$1" | cut -d '=' -f1)
 17 values_2=$(grep -E "^"$3"=" "$1"| cut -d '=' -f2)
 18 new_values=''
 19
 20 for value in $values_2; do
 21     if ! grep -w "${value}" <(echo "$values_1") > /dev/null; then #watching only the exit code
 22         #if not contained in val2
 23         new_values="${new_values} ${value}"
 24     fi
 25 done
 26 new_values=$(echo $new_values | cut -c 1-)
 27 sed -iE "s/${values_2}/${new_values}/" "${1}"
 28 cat $1
