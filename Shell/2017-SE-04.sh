  1 #!/bin/bash
  2
  3 if [[ "${#}" -lt 1 ]]; then
  4     echo "not enough" >&2
  5     exit 1
  6 fi
  7
  8 if [[ ! $1 -d ]]; then
  9     echo "...">&2
 10     exit 2
 11 fi
 12
 13 while read file; do
 14    if [[ -e ${file} ]]; then
 15         echo "$(basename "${file}")" -> "$(file)"
 16    else
 17         broken=$(($broken+1))
 18    fi
 19
 20 done< <(find "${1}" -type l &2>/dev/null)
 21
 22 echo "Broken syml : ${broken}"
