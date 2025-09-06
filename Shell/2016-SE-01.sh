  1 #!/bin/bash
  2 if [[ $# -ne 1 ]]; then
  3     echo "1 arg is needed"
  4     exit 1;
  5 fi
  6
  7 if [[ ! -d "$1" ]]; then
  8     echo "Not a dir"
  9     exit 2
 10 fi
 11
 12 find -L "$1" -type l 2>/dev/null -print
