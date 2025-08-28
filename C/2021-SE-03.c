 1 #include <stdlib.h>
  2 #include <unistd.h>
  3 #include <err.h>
  4 #include <stdio.h>
  5 #include <string.h>
  6 #include <stdint.h>
  7 #include <sys/wait.h>
  8 #include <sys/stat.h>
  9 #include <fcntl.h>
 10 int main(int argc, char** argv) {
 11     if(argc != 3) {errx(1, "args");}
 12     int rfd = open(argv[1], O_RDONLY);
 13     if(rfd == -1) { err(1, "r");}
 14     int wrfd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
 15     if(wrfd == -1) { err(1, "wr");}
 16
 17
 18     struct stat st;
 19     if(fstat(rfd, &st) < 0) {err(1, "fstat");}
 20     if(st.st_size % sizeof(uint16_t) != 0) {err(1, "size");}
 21
 22     uint32_t size = st.st_size / sizeof(uint16_t);
 23
 24     char buff[1024];
 25     uint32_t i = 0;
 26     snprintf(buff, 1024, "#include <stdint.h>\nuint32_t arrN = %d;\nconst uint16_t arr = {", size);
 27     if(write(wrfd, buff, strlen(buff)) < 0) {err(1, "wr");}
 28
 29     for(; i < size; i++)
 30     {
 31         uint16_t el;
 32         if(read(rfd, &el, sizeof(el)) < 0) {err(1, "r");}
 33
 34         int l;
 35         if(i == 0) { l = snprintf(buff, 1024, "%u", el); }
 36         else if(i < size - 1) { l = snprintf(buff, 1024, ", %u", el); }
 37         else { l = snprintf(buff, 1024, "%u};\n", el); }
 38
 39
 40         if(write(wrfd, buff, l) < 0) {err(1, "wr");}
 41     }
 42     close(rfd);
 43     close(wrfd);
 44     return 0;
 45 }
