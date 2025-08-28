  1 #include <err.h>
  2 #include <string.h>
  3 #include <unistd.h>
  4 #include <fcntl.h>
  5 #include <stdio.h>
  6 #include <stdlib.h>
  7 int main(int argc, char* argv[]) {
  8     if(argc != 3 && argc != 5) { errx(1, "args"); }
  9     int num1 = -1;
 10     int num2 = -1;
 11     char del = ' ';
 12     if(argc == 3)
 13     {
 14         if(strlen(argv[2]) == 1){
 15             num1 = atoi(argv[2]);
 16             num2 = num1;
 17         } else if (strlen(argv[2]) == 3 && argv[2][1] == '-'){
 18             num1 = atoi(argv[2]);
 19             num2 = atoi(argv[2]+2);
 20             if(num1 > num2) { errx(1, "range"); }
 21         }else{
 22             errx(1, "wrong input");
 23         }
 24     }
 25     else if(argc == 5)
 26     {
 27         if(strcmp(argv[1],"-d") == 0 && strcmp(argv[3], "-f") == 0){
 28             del = argv[2][0];
 29             if(strlen(argv[4]) == 1)
 30             {
 31                 num1 = atoi(argv[4]);
 32                 num2= num1;
 33             }else if (strlen(argv[4]) == 3){
 34                 num1 = atoi(argv[4]);
 35                 num2 = atoi(argv[4] +2);
 36                 if(num1 > num2) {errx(1, "num1>2");}
 37             }
 38             else
 39             {
 40                 errx(1,"num1,2");
 41             }
 42         }else{
 43             errx(1, "args val");
 44         }
 45     }
 46
 47     int curP = 0;
 48     char b;
 49     int readB;
 50     while((readB = read(0, &b, 1)) > 0){
 51         if(argc == 3){
 52             if(b == '\n')
 53             {
 54                 curP = 1;
 55                 if(write(1, &b, 1) < 0) { err(1, "write"); }
 56                 continue;
 57             }
 58             else if(curP >= num1 && curP <= num2)
 59             {
 60                if(write(1, &b, 1) < 0) {err(1,"write");}
 61             }
 62             curP++;
 63         }
 64         else
 65         {
 66             if( b == '\n')
 67             {
 68                 curP = 1;
 69                 if(write(1, &b, 1) < 0) { err(1, "write"); }
 70                 continue;
 71             }
 72             if(b == del)
 73             {
 74                 curP++;
 75                 if(curP > num1 && curP <= num2)
 76                 {
 77                     if(write(1, &b, 1) < 0) { err(1, "write"); }
 78                 }
 79                 continue;
 80             }
 81             if(curP >= num1 && curP <= num2)
 82             {
 83                 if(write(1, &b, 1) < 0) { err(1, "write"); }
 84             }
 85         }
 86     }
 87     if(readB < 0) { errx(1,"read");}
 88     return 0;
 89 }




