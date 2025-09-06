 1 #!/bin/bash
  2
  3 if [[ $# -ne 2 ]]; then
  4     echo wrong args>&2
  5     exit 1
  6 fi
  7
  8 dir="$1" #proverka za validna dir
  9 str="$2"
 10 #niz-V.majV.minV-platf
 11 find "$dir" -mindepth 1 -maxdepth 1 -type f -printf "%f\n" | grep -Eo "[^-]+-[^.]+.[^.]+.[^-]+-$str
