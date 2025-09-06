  1 #!/bin/bash
  2
  3 if [[ $# -ne 2 ]]; then
  4     echo args>&2
  5     exit 1
  6 fi
  7
  8 if [[ -z $(find $2 -mindepth 1) ]]; then
  9     echo dir should not be empty>&2
 10     exit 2
 11 fi
 12
 13 if [[ ! -d $2 ]]; then
 14     echo 2 must be dir>&2
 15     exit 3
 16 fi
 17
 18 touch "$1"
 19
 20 echo "hostname,phy,vlans,hosts,failover,VPN-3DES-AES,peers,VLAN Trunk Ports,license,SN,key" > $1
 21
 22 extr() {
 23     v=$(echo "$1" | grep -E "$2" | sed "s|$2||")
 24     echo "$v"
 25 }
 26 find $2 -type f -name "*.log" | while read file; do
 27 echo "$file" | sed "s|.log||" > $1
 28 f=$(cat $file | tr -s ' ')
 29 echo $1
 30 v1=$(extr "${f}" " Maximum Physical Interfaces : ")
 31 echo $v1
 32 done
