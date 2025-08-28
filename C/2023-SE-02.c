  1 #include <fcntl.h>
  2 #include <unistd.h>
  3 #include <err.h>
  4 #include <stdint.h>
  5 #include <sys/stat.h>
  6 #include <stdbool.h>
  7 #include <stdio.h>
  8 #include <string.h>
  9
 10 void readWord(int f, char* word){
 11     int readB;
 12     uint8_t b;
 13     int i = 0;
 14     while((readB = read(f, &b, sizeof(b))) > 0) {
 15         if(b == '\n') { break; }
 16         if(i > 63) { err(1, "word too long"); }
 17         word[i++] = b;
 18     }
 19     if(readB < 0) { err(1, "read"); }
 20     word[i] = '\0';
 21 }
 22
 23 int main(int argc, char* argv[]) {
 24     if(argc != 4) { errx(1, "args"); }
 25     int dic = open(argv[2], O_RDONLY);
 26     if(dic < 0) { err(1, "open"); }
 27     int indx = open(argv[3], O_RDONLY);
 28     if(indx < 0) { err(1, "open"); }
 29
 30     struct stat st;
 31     if(fstat(indx, &st) < 0) { err(1, "stat"); }
 32     uint32_t size = st.st_size;
 33     if(size % sizeof(uint32_t) != 0) { err(1, "size indx"); }
 34     uint32_t end = size / sizeof(uint32_t) - 1;
 35     uint32_t mid = end /2;
 36     uint32_t start = 0;
 37     char curr[65];
 38     bool found = false;
 39     //int i = 0;
 40
 41     while(start <= end){
 42         //new mid
 43         mid = start + (end - start) / 2;
 44         //lseek in indx
 45         if(lseek(indx, mid * sizeof(uint32_t), SEEK_SET) < 0) { err(1, "lseek dic"); }
 46         //read pos in dic
 47         uint32_t pos;

48         if(read(indx, &pos, sizeof(pos)) < 0) { err(1, "read"); }
 49         //check pos > dicSize ...
 50         //lseek
 51         if(lseek(dic, pos + 1, SEEK_SET) < 0) { err(1, "lseek"); }
 52         //read
 53         readWord(dic, curr);
 54         //printf("read word %s\n", curr);
 55         //cmp
 56         int diff = strcmp(argv[1], curr);
 57         if (diff == 0)
 58         {
 59             found = true;
 60             break;
 61         } else if (diff < 0){
 62             end = mid - 1;
 63         } else {
 64             start = mid +1;
 65         }
 66     }
 67     if (!found) { printf("not found"); } //
 68     else {
 69         printf("found %s\n", curr);
 70         uint8_t byte;
 71         while(read(dic, &byte, sizeof(byte)) > 0){
 72             if(byte == '\0') {break;}
 73             if(write(1, &byte, sizeof(byte)) < 0) { err(1, "writing"); }
 74         }
 75     }
 76     close(dic);
 77     close(indx);
 78 }
