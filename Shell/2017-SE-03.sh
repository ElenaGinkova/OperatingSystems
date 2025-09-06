  1 #!/bin/bash
  2
  3 #if [[ $(whoami) != "root" ]]; then
  4 #    echo "must be root">&2
  5 #    exit 1
  6 #fi
  7
  8 table=$(mktemp)
  9 res=$(mktemp)
 10 users=$(mktemp)
 11
 12 ps -e -o user=,rss=| sort > $table
 13 cat $table | awk '{print $1}' | uniq > $users
 14 user=''
 15 used_mem=0
 16 while read -r user; do
 17     mem=$(awk -v us="$user" '{if($1 == us) sum+=$2} END {print sum}' "$table")
 18     echo "user $user mem $mem" >> $res
 19 done< $users
 20
 21
 22 rm $table $res $users
