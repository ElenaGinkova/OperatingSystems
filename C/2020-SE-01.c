 1 #include <unistd.h>
  2 #include <fcntl.h>
  3 #include <err.h>
  4 #include <sys/stat.h>
  5 #include <stdint.h>
  6
  7 typedef struct{
  8     uint16_t start;
  9     uint16_t cnt;
 10 } interval;
 11
 12 typedef struct{
 13     interval intr[4];
 14 }complect;
 15
 16 typedef struct{
 17     uint8_t pass[4];
 18     uint16_t count;
 19     uint8_t pass2[10];
 20 } header;
 21
 22 void handle_lseek(int fd, int i, complect c)
 23 {
 24     switch(i){
 25         case 0:
 26             if(lseek(fd[i + 1], c.intr[i].start * sizeof(uint32_t) + sizeof(header), SEEK_SET) < 0) { er
 27             break;
 28         case 1:
 29             if(lseek(fd[i + 1], c.intr[i].start * sizeof(uint8_t) + sizeof(header), SEEK_SET) < 0) { err
 30             break;
 31         case 2:
 32             if(lseek(fd[i +1], c.intr[i].start * sizeof(uint16_t) + sizeof(header), SEEK_SET) < 0) { err
 33             break;
 34         case 3:
 35             if(lseek(fd[i +1], c.intr[i].start * sizeof(uint64_t) + sizeof(header), SEEK_SET) < 0) { err
 36             break;
 37     }
 38 }
 39 void handle_r_wr(int fd, int fdw, int i){
 40     uint8_t b[4];
 41     switch(i)
 42     {
 43         case 0:
 44             if(read(fd, b, sizeof(uint32_t)) < 0) { err(1, "read"); }
 45             if(write(fdw, b, sizeof(uint32_t)) < 0) { err(1, "wr"); }
 46             break;
 47

   48         case 1:
 49             if(read(fd, b, sizeof(uint8_t)) < 0) { err(1, "read"); }
 50             if(write(fdw, b, sizeof(uint8_t)) < 0) { err(1, "wr"); }
 51             break;
 52
 53         case 2:
 54             if(read(fd, b, sizeof(uint16_t)) < 0) { err(1, "read"); }
 55             if(write(fdw, b, sizeof(uint16_t)) < 0) { err(1, "wr"); }
 56             break;
 57
 58         case 3:
 59             if(read(fd, b, sizeof(uint64_t)) < 0) { err(1, "read"); }
 60             if(write(fdw, b, sizeof(uint64_t)) < 0) { err(1, "wr"); }
 61             break;
 62     }
 63
 64 }
 65 void handle_compl(int* fd, header* h, int fdw)
 66 {
 67     complect c;
 68     if(read(fd[0], &c, sizeof(c)) < 0) { err(1, "read"); }
 69     for(int i = 0; i < 4; i++)
 70     {
 71         if(c.intr[i].start + c.intr[i].cnt > h[i + 1].count) { errx(1, "out of bnd"); }
 72         handle_lseek(fd[i], i, c);
 73         for(int j = 0; j < c.intr[i].cnt; j++)
 74         {
 75             handle_r_wr(fd[i], fdw, i);
 76         }
 77     }
 78 }
 79 void open_all(int fd[], char** argv){
 80     for(int i = 0; i < 5; i++)
 81     {
 82         fd[i] = open(argv[i+1], O_RDONLY);
 83         if(fd[i] < 0) { err(1, "open"); }
 84     }
 85 }
 86 void read_h(int* fd, header* h){
 87     for(int i = 0; i < 5; i++)
 88     {
 89         if(read(fd[i], &h[i], sizeof(header)) < 0) { err(1, "read"); }
 90     }
 91 }
 92 void close_all(int* fd, int fdw){
 93     for(int i = 0; i < 5; i++)
 94     {
 95         close(fd[i]);
 96     }
 97     close(fdw);
 98 }
 99 int main(int argc, char* argv[]) {
100     if(argc != 7) { errx(1, "args");}
101
102     int fd[5];
103     open_all(fd, argv);
104     int fdw = open(argv[6], O_WRONLY | O_CREAT | O_TRUNC, 0644);
105     if(fdw < 0) { err(1, "open"); }
106
107     header h[5];
108     read_h(fd, h);
109     //the cnt in fdw should be updated too
110     for(int i = 0; i < h[0].count; i++)
111     {
112         handle_compl(fd, h, fdw);
113     }
114     close_all(fd, fdw);
115     return 0;
116 }

