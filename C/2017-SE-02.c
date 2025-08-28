 1 #include <string.h>
  2 #include <stdio.h>
  3 #include <fcntl.h>
  4 #include <unistd.h>
  5 #include <err.h>
  6 const int BUFF_SIZE = 1024;
  7 int read_line(int fd, char* buff) {
  8     char b;
  9     int i = 0;
 10     buff[i] = '\0';
 11     while (read(fd, &b, 1) > 0)
 12     {
 13         if (b == '\n')
 14         {
 15             buff[i++] = '\n';
 16             buff[i] = '\0';
 17             return 1;
 18         }
 19         buff[i] = b;
 20         i++;
 21     }
 22     //read check
 23     return 0;
 24 }
 25
 26 int main(int argc, char** argv)
 27 {
 28
 29     int n = 0;
 30     int j = 1;
 31     if (argc > 1){
 32         if (strcmp(argv[1], "-n") == 0) { n = 1; j = 2; } }
 33
 34     //filenames
 35     char buff[BUFF_SIZE];
 36     if ((argc == 2 &&  n) || argc == 1)
 37     {
 38         //read from io
 39         while (read_line(0, buff) > 0)
 40         {
 41             if (n) {
 42                 char buff_num[BUFF_SIZE];
 43                 snprintf(buff_num, BUFF_SIZE, "%d ", n);
 44                 if (write(1, buff_num, strlen(buff_num)) < 0) { err(1, "wr"); }
 45                 n++;
 46             }

 47             if (write(1, buff, strlen(buff)) < 0) { err(1, "wr"); }
 48         }
 49     }
 50     else
 51     {
 52         //read from files in args
 53         for (; j < argc; j++)
 54         {
 55             int fd;
 56             if (strcmp(argv[j], "-") == 0)
 57             {
 58                 fd = 0;
 59             }
 60             else
 61             {
 62                 fd = open(argv[j], O_RDONLY);
 63                 if (fd == -1) { err(1,"o"); }
 64             }
 65             while (read_line(fd, buff) > 0)
 66             {
 67                 if (n) {
 68                     char buff_num[BUFF_SIZE];
 69                     snprintf(buff_num, BUFF_SIZE, "%d ", n);
 70                     if (write(1, buff_num, strlen(buff_num)) < 0) { err(1, "wr"); }
 71                     n++;
 72                 }
 73                 if (write(1, buff, strlen(buff)) < 0) { err(1, "wr"); }
 74             }
 75             if (fd != 0) { close(fd); }
 76         }
 77     }
 78 }
