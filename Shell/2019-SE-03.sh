  1 #!/bin/bash
  2
  3 if [[ $# -ne 1 ]]; then
  4     echo args>&2
  5     exit 1
  6 fi
  7
  8 if [[ ! -d $1 ]]; then
  9     echo 1 needs to be dir >$2
 10     exit 2
 11 fi
 12
 13 if [[ -z $1 ]]; then
 14     echo shouldnt be empty>&2
 15     exit 3
 16 fi
 17
 18 mkdir -p "EXTR_FILES"
 19
 20 touch "HASH_FILE"
 21 touch "temp"
 22
 23 files=$(find $1 -name "*.tgz" | grep -E "^([^/]*/)*[^_]+_report-([1-9]+[0-9]*|0).tgz$")
 24 echo "$files" | while read arch; do
 25     sh=$(sha256sum $arch | awk '{print $1}')
 26     old_sh=$(grep -E "\<$arch\>" "HASH_FILE" | awk '{print $1}')
 27     if [[ $sh != $old_sh ]]; then
 28         echo "saving sth new"
 29         name=$(basename "$arch" | grep -Eo "^[^_]+")
 30         time=$(basename "$arch" | grep -Eo "([1-9]+[0-9]*|0)")
 31         meow_files=$(tar -tf $arch | grep "meow.txt")
 32
 33         echo "$meow_files" | while read file; do
 34             echo $file
 35             tar -xzf "$arch" -O "$file" > "EXTR_FILES/${name}_${time}.txt"
 36         done
 37         awk -v arch=$arch -v sh=$sh '$2 != arch { print $0 } END { print sh, arch }' HASH_FILE > HA
 38         mv HASH_FILE.tmp HASH_FILE
 39     fi
 40     #sha256sum $arch >> temp
 41 done
 42 #mv temp HASH_FILE
 43
