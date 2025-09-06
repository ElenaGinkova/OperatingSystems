  1 #!/bin/bash
  2
  3 home_dirs=$(cat /etc/passwd | awk -F ':' '{print $1 " " $6}')
  4
  5 max_user=''
  6 max_time=0
  7 max_file=''
  8
  9 while read -r user dir; do
 10     if [[ -d $dir ]]; then
 11         time_file=$(find "${dir}" -type f -printf "%T@ %p\n" 2>/dev/null | sort -k1nr | head -n1)
 12
 13         if [[ -z "$time_file" ]]; then
 14             continue
 15         fi
 16
 17         time=$(echo "$time_file" | awk '{print $1}')
 18         file=$(echo "$time_file" | awk '{print $2}')
 19
 20         if (( $(echo "$time > $max_time" | bc -l) == 1 )); then
 21             max_user=$user
 22             max_time=$time
 23             max_file=$file
 24         fi
 25     fi
 26 done < <(echo "${home_dirs}")
 27
 28 echo $max_user
 29 echo $max_time
 30 echo $max_file
