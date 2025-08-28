  1  #include <sys/stat.h>
  2  #include <stdint.h>
  3 #include <stdlib.h>
  4 #include <string.h>
  5 #include <stdio.h>
  6 #include <fcntl.h>
  7 #include <unistd.h>
  8 #include <err.h>
  9
 10
 11
 12 typedef struct
 13 {
 14     uint64_t next; // next - 0 e last
 15     uint8_t user_data[504];
 16 } node;
 17 void fill_file(int fd, size_t size)
 18 {
 19     int flag = 0;
 20     for (size_t i = 0; i < size; i++)
 21     {
 22         if (write(fd, &flag, sizeof(flag)) < 0) { err(1, "w"); }
 23     }
 24 }
 25 int main(int argc, char** argv) {
 26     if (argc != 2) { errx(1,"args"); }
 27     //size check, and use it as count for nodes
 28     int r_file = open(argv[1], O_RDWR);
 29     if (r_file == -1) { err(1, "o"); }
 30     char name[] = "fileXXXXXX";
 31     int tmp = mkstemp(name);
 32     if (tmp == -1) { err(1, "o"); }
 33     node nd;
 34
 35     struct stat st;
 36     if(fstat(r_file, &st) < 0) {err(1, "fs");}
 37     size_t size = st.st_size;
 38     if(size % sizeof(nd) != 0) {err(1, "size");}
 39     size = size / sizeof(nd);
 40
 41     fill_file(tmp, size);
 42     //read check
 43     int reached = 1;
 44     while (read(r_file, &nd, sizeof(nd)) > 0)
 45     {
 46         //check
 47         if (nd.next == 0) { break; }

48         if (nd.next >= size) { errx(1, "out of bounds"); }
 49
 50         //mark
 51         if (lseek(tmp, nd.next * sizeof(int), SEEK_SET) < 0) { err(1, "ls"); }
 52         if (write(tmp, &reached, sizeof(reached)) < 0) { err(1, "w");}
 53
 54         //move
 55         if (lseek(r_file, nd.next * sizeof(nd), SEEK_SET) < 0) { err(1, "ls"); }
 56     }
 57     if (lseek(tmp, 0, SEEK_SET) < 0) { err(1, "ls"); }
 58     int i = 0;
 59     uint8_t empty[sizeof(node)] = {0};//??
 60     while(read(tmp, &reached, sizeof(int)) > 0)
 61     {
 62         if (i == 0) {
 63             i++;
 64             continue;
 65         }
 66         if (!reached)
 67         {
 68             if (lseek(r_file, i * sizeof(nd), SEEK_SET) < 0) { err(1, "ls"); }
 69             if (write(r_file, empty, sizeof(empty)) < 0) { err(1, "w"); }
 70         }
 71         i++;
 72     }
 73     close(tmp);
 74     close(r_file);
 75 }
