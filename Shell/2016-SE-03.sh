  1 #if [[ ! "$(whoami)" = "root"]]; then
  2 #    echo "not root"
  3 #    exit 1
  4 #fi
  5
  6 while read line; do
  7     dir=$(echo ${line} | awk -F ':' '{print $6}')
  8     user=$(echo ${line} | awk -F ':' '{print $1}')
  9
 10     if [[ ! -d "${dir}" || -z $(echo ${dir} | grep -Eo "/home/" ) ]]; then
 11         echo ""${user}" no homedir"
 12     else
 13      wr_perm=$(stat -c "%a" ${dir} | cut -c 1)
 14         if [[ ${wr_perm} -eq 4 || ${wr_perm} -eq 1 || ${wr_perm} -eq 5 ]]; then
 15         echo "no write perm"
 16         fi
 17     fi
 18 done < /etc/passwd
