 1 #include <err.h>
  2 #include <unistd.h>
  3 #include <stdint.h>
  4 #include <sys/wait.h>
  5 #include <fcntl.h>
  6 #include <sys/stat.h>
  7 #include <stdlib.h>
  8 #include <stdio.h>
  9 typedef struct {
 10     char name[8];
 11     uint32_t offs;
 12     uint32_t len;
 13 } triple;
 14
 15 const int MAX = 8;
 16
 17 void waitpr(void)
 18 {
 19     int status;
 20     if(wait(&status) < 0){ err(1, "wait"); }
 21     if(!WIFEXITED(status)) { err(1, "child didnt exit"); }
 22     if(WEXITSTATUS(status) != 0) { err(1, "child didnt exit with 0"); }
 23 }
 24
 25 int main(int argc, char* argv[]) {
 26     if(argc != 2) { errx(1,"args");}
 27
 28     int fd = open(argv[1], O_RDONLY);
 29     if( fd == -1 ) { err(1, "open");}
 30
 31     struct stat st;
 32     if(fstat(fd, &st) < 0) { err(1, "stat");}
 33     int size = st.st_size;
 34     if(size % sizeof(triple) != 0) { errx(1, "size");}
 35     int cnt = size / sizeof(triple);
 36     if(cnt > MAX ) { errx(1, "too many tr");}
 37
 38     triple tr;
 39
 40     uint16_t res = 0;
 41     int pfd[2];
 42     if(pipe(pfd) < 0) { err(1,"pipe");}
 43
 44     for(int j = 0; j < cnt; j++)
 45     {
 46         if(read(fd, &tr, sizeof(triple)) > 0){ err(1, "read"); }
 4748         pid_t pid = fork();
 49         if(pid < 0) { err(1,"pid");}
 50
 51         if( pid == 0 )
 52         {
 53             close(pfd[0]);
 54             int file = open(tr.name, O_RDONLY);
 55             if(file < 0) { err(1, "open");}
 56             //stat check
 57
 58             uint16_t temp = 0;
 59             uint16_t curr;
 60
 61             if(lseek(file, tr.offs * sizeof(uint16_t), SEEK_SET) < 0) { err(1,"lseek");}
 62
 63             for(uint32_t jj = 0; jj < tr.len; jj++)
 64             {
 65                 if(read(file, &curr, sizeof(curr)) < 0) { err(1, "read");}
 66
 67                 temp ^= curr;
 68             }
 69             close(file);
 70
 71             if(write(pfd[1], &temp, sizeof(temp)) < 0) { err(1, "write"); }
 72             close(pfd[1]);
 73             exit(0); // !!!!!
 74         }
 75     }
 76     close(pfd[1]);
 77
 78     uint16_t temp;
 79     for(int i = 0; i < cnt; i++)
 80     {
 81         if(read(pfd[0], &temp, sizeof(temp)) < 0) { err(1, "read");}
 82         res ^= temp;
 83     }
 84     close(pfd[0]);
 85     close(fd);
 86     if(write(1, "res: ", 5) < 0) { err(1, "write");}
 87     if(write(1, &res, sizeof(res) < 0)) { err(1, "write");}
 88
 89     for(int i = 0; i < cnt; i++)
 90     {
 91         waitpr();
 92     }
 93
 94     return 0;


   
