
 1 #include <stdio.h>
  2 #include <err.h>
  3 #include <fcntl.h>
  4 #include <unistd.h>
  5 #include <sys/stat.h>
  6 #include <stdint.h>
  7
  8 const int MAX_SIZE = 16384;
  9 const int MAX_USERS = 2048;
 10
 11 typedef struct {
 12     uint32_t uid;
 13     uint16_t s1;
 14     uint16_t s2;
 15     uint32_t start;
 16     uint32_t end;
 17 } users;
 18
 19 int main(int argc, char* argv[]) {
 20     users arr[MAX_SIZE];
 21     uint32_t usersArr[MAX_USERS]; // longests session
 22     if(argc != 2) { errx(1, "args");}
 23     int fd = open(argv[1], O_RDONLY);
 24     if(fd == -1) { err(1, "read"); }
 25     int i = 0;
 26     int b;
 27     uint32_t sum = 0;
 28
 29     //stat check size
 30     struct stat st;
 31     if(fstat(fd, &st) < 0) { err(1, "stat"); }
 32     size_t size = st.st_size;
 33     if(size % sizeof(users) != 0) { errx(1, "wrong file size"); }
 34
 35     while((b = read(fd, &arr[i], sizeof(users))) > 0)
 36     {
 37         //save all people
 38         sum += arr[i].end - arr[i]. start;
 39         i++;
 40     }
 41     if(b < 0) { err(1, "read"); }
 42     close(fd);
 43
 44     sum /= i; //sredna
 45     uint32_t D =0;
 46     //dispersiq
 47     for(int j = 0; j < i; j++)

 48     {
 49         uint32_t sess = arr[i].end - arr[i].start;
 50         D += (sess - sum)*(sess - sum);
 51         if(!usersArr[arr[i].uid] || sess > usersArr[arr[i].uid]) { usersArr[arr[i].uid] = sess; }
 52     }
 53     D /= i;
 54     //ppl with sess ^ 2 > D
 55     for(int j = 0; j < i; j++)
 56     {
 57         //stdout longest sess
 58         uint32_t sess = arr[i].end - arr[i].start;
 59         if(sess*sess > D)
 60         {
 61             char buff[256];
 62             int len = snprintf(buff, sizeof(buff), "%d %d\n", arr[i].uid, usersArr[arr[i].uid]);
 63             if( write(1, buff, len)  < 0) { err(1, "write"); }
 64         }
 65     }
 66     return 0;
 67 }
