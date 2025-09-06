  1 #!/bin/bash
  2
  3 #if [[ $(whoami) != "root" ]]; then
  4 #    echo "must be root">&2
  5 #    exit 1
  6 #fi
  7
  8 table=$(mktemp)
  9
 10 ps -e -o user=,rss=,pid=| sort > $table
 11 last_user=''
 12 last__mem=0
 13 last_cnt=1
 14 while read -r user rss pid; do
 15     if [[ $user != $last_user ]]; then
 16         avg=$(( last_mem * 2 / last_cnt ))
 17         if [[ $maxrss -gt $avg ]]; then
 18             echo "kill $maxpid"
 19         fi
 20         echo "$last_user $last_cnt $last_mem"
 21         last_user=$user
 22         last_mem=0
 23         max_pid=0
 24         maxrss=0
 25     fi
 26     if [[ $rss -gt $maxrss ]]; then
 27         maxrss=$rss
 28         maxpid=$pid
 29     fi
 30     last_mem=$(($last_mem + rss))
 31     last_cnt=$(($last_cnt + 1))
 32 done< $table
 33 rm $table
