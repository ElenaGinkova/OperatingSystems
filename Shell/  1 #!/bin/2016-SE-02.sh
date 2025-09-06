  1 #!/bin/bash
  2
  3 if [[ ${#} -ne 1 ]]; then
  4     echo "only 1 arg"
  5     exit 1
  6 fi
  7
  8 if ! [[ "${1}" =~ ^[a-zA-Z0-9]+$ ]]; then
  9     echo "only num"
 10     exit 2
 11 fi
 12
 13 #if [[ "$(whoami)" != "root" ]]; then
 14 #    echo "u r not root?"
 15 #    exit 3
 16 #fi
 17
 18 table="$(ps -eo uid=,pid=,rss=)"
 19 users="$(echo "${table}" | awk '{print $1}' | sort -u)"
 20
 21 for user in ${users}; do
 22     total="$(echo "${table}" | grep -Ew "${user}" | awk '{sum+=$3} END {print sum}' )"
 23     #echo "$user has "${total}""
 24 if [[ ${total} -gt "${1}" ]]; then
 25     highest="$(echo "${table}" | grep -E "${user}" | sort -k3 | head -n1 | awk '{print $2 " " $3}')
 26     echo "tot ${total} highest ${highest} for user ${user}"
 27     # kill $(echo ${hieghest} | awk '{print $2}')
 28 fi
 29 done
