  1 #!/bin/bash
  2
  3 #if [[ $(whoami) != "root" ]]; then
  4 #    echo you r not root > &2
  5 #    exit 1
  6 #fi
  7
  8 table=$( ps -eo user=,pid=,rss= )
  9 max_mem=$(echo "$table" | grep -E "^[ ]*\<root\>" | awk '{sum+=$3} END {print sum}')
 10 echo max $max_mem
 11 cat /etc/passwd | grep -vE "\<root\>" | awk -F ':' '{print $1 " " $6}' | while read -r user home; d
 12     #echo iser $user with $home
 13     if [[ ! -d "$home" ]] || [[ $(stat -c %U "$home") != "$user" ]] || [[ $(stat -c %a "$home" | cu
 14         mem=$(echo "$table" | grep -E "^[ ]*\<$user\>" | awk '{sum+=$3} END {print sum}')
 15         if [[ -z $mem ]]; then
 16             mem=0
 17         fi
 18         echo $user mem is $mem
 19         if [[ $mem -gt $max_mem ]]; then
 20             echo "$table" | grep -E "\<$user\>" | awk '{print $2}' | xargs -I{} echo kill {} of $us
 21         fi
 22     fi
 23 done
