  1 #!/bin/bash
  2
  3 if [[ $# -ne 1 ]]; then
  4     exit 1
  5 fi
  6
  7 if [ -z "$1" ]; then
  8   echo "Използване: $0 <потребител>"
  9   exit 1
 10 fi
 11
 12 #if [[ ! $(whoami) == "root" ]]; then
 13 #    exit 2
 14 #fi
 15
 16 table=$(ps -e -o user=,time= | sort)
 17 lines=$(echo "$table" | wc -l)
 18 users_pid=$(echo "$table"| awk '{print $1}' | sort | uniq -c)
 19
 20 pid_c=$(echo "$users_pid" | grep -E "\<$1\>" |awk '{print $1}')
 21 echo "$users_pid" | awk -v count=$pid_c '{if ( $1 > $count )  print $1, $2}'
 22
 23 total_time=$(echo "$table" | awk -F ' ' '{print $2}' | awk -F ':' -v sum=0 '{sum += $1*3600 + $2*60
 24 total_time=$(echo "scale=2; $total_time / $lines" | bc)
 25 sredno=$(echo $total_time | awk -v time=${total_time} '{h = int(time / 3600);
 26                               m = int((time % 3600) / 60);
 27                               s = int( time % 60 );
 28                               printf "%02d:%02d:%02d\n", h, m, s}')
 29
 30 ps -e -o pid=,time=| awk '{print $1, $2}' | while read -r pid time; do
 31     time=$(echo $time |  awk -F ':' '{print ($1*3600 + $2*60 + $3) * 2}')
 32     if (( $(echo "$time > $total_time" | bc -l) )); then
 33         echo "end $pid with $time"
 34     fi
 35 done
