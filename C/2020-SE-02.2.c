 1 #include <stdlib.h>
  2 #include <unistd.h>
  3 #include <err.h>
  4 #include <fcntl.h>
  5 #include <stdbool.h>
  6 #include <stdint.h>
  7 #include <sys/wait.h>
  8 #include <stdbool.h>
  9 #include <stdio.h>
 10 uint8_t decode(uint8_t byte)
 11 {
 12     return byte ^ 0x20;
 13 }
 14 bool check(uint8_t b){
 15     return b == 0x00 || b == 0xFF || b == 0x55 || b == 0x7D;
 16 }
 17 int main(int argc, char** argv) {
 18     if(argc != 3) { errx(1, "args");}
 19     int wr = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
 20     if(wr ==-1) {err(1,"wr");}
 21
 22     int pd[2];
 23     if(pipe(pd) ==-1) {err(1, "pipe");}
 24
 25     pid_t pid = fork();
 26     if(pid == -1) {err(1, "fork");}
 27     if(pid == 0) {
 28         close(pd[0]);
 29         if(dup2(pd[1], 1) < 0) {err(1, "dup");}
 30         close(pd[1]);
 31         execlp("cat", "cat", argv[1], (char*)NULL);
 32         err(1, "exec");
 33     }
 34     close(pd[1]);
 35
 36     int status;
 37     if(wait(&status) < 0) {err(1,"wait");}
 38     if(!WIFEXITED(status)) {err(1, "killed");}
 39     if(WEXITSTATUS(status)) {err(1, "status");}
 40
 41     int b;
 42     uint8_t byte;
 43     int flag = 0;
 44     while((b = read(pd[0], &byte, 1)) > 0)
 45     {
 46         if(byte == 0x55) {continue;}
 47

    48         if(flag)
 49         {
 50             byte = decode(byte);
 51             if(!check(byte))
 52             {
 53                 printf("%c", byte);
 54                 //errx(1, "wrong format");
 55             }
 56             flag = 0;
 57         }
 58         else if(byte == 0x7D)
 59         {
 60             flag = 1;
 61             continue;
 62         }
 63         else
 64         {
 65             flag = 0;
 66         }
 67
 68         if(write(wr, &byte, 1) < 0) {err(1, "wr");}
 69     }
 70     if(b < 0) {err(1, "r");}
 71
 72     close(pd[0]);
 73     close(wr);
 74     return 0;
 75 }
