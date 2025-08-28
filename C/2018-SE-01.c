  1 #include <string.h>
  2 #include <stdio.h>
  3 #include <fcntl.h>
  4 #include <unistd.h>
  5 #include <err.h>
  6 int is_in_set(char c, char* set)
  7 {
  8     for (size_t i = 0; i < strlen(set); i++)
  9     {
 10         if (c == set[i]) return i;
 11     }
 12     return -1;
 13 }
 14 int main(int argc, char** argv) {
 15     if(argc < 3) {errx(1, "args");}
 16     if (strcmp(argv[1], "-d") == 0)
 17     {
 18         int b;
 19         char c;
 20         while ((b = read(0, &c, 1)) > 0)
 21         {
 22             if (is_in_set(c, argv[2]) == -1)
 23             {
 24                 if (write(1, &c, 1) < 0) { err(1, "wr"); }
 25             }
 26         }
 27         if (b < 0) { err(1, "r"); }
 28
 29     }
 30     else if(strcmp(argv[1], "-s") == 0)
 31     {
 32         int b;
 33         char c1;
 34         if (read(0, &c1, 1) < 0) { err(1, "r"); }
 35         char c2;
 36         while ((b = read(0, &c2, 1)) > 0)
 37         {
 38             if (c1 != c2)
 39             {
 40                 if (write(1, &c1, 1) < 0) { err(1, "wr"); }
 41             }
 42             c1 = c2;
 43         }
 44         if (b < 0) { err(1, "r"); }
 45     }
 46     else
 47     {
      //sravni razmer
 49         if (strlen(argv[1]) != strlen(argv[2])) { errx(1, "l");}
 50         int b;
 51         char c1;
 52         while ((b = read(0, &c1, 1)) > 0)
 53         {
 54             int i;
 55             if (( i =is_in_set(c1, argv[1])) != -1)
 56             {
 57                 if (write(1, &argv[2][i], 1) < 0) { err(1, "wr"); }
 58             }
 59             else
 60             {
 61                 if (write(1, &c1, 1) < 0) { err(1, "wr"); }
 62             }
 63         }
 64         if (b < 0) { err(1, "r"); }
 65     }
 66 }

