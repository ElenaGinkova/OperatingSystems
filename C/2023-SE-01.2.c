  1 #include <fcntl.h>
  2 #include <unistd.h>
  3 #include <err.h>
  4 #include <stdint.h>
  5 #include <sys/stat.h>
  6 #include <sys/wait.h>
  7 #include <stdlib.h>
  8 #include <string.h>
  9
 10 int read_file(int fd, char* buff){
 11     char c;
 12     buff[0] = '\0';
 13     int i =0;
 14     while(read(fd, &c, 1) > 0){
 15         if(c == '\n' || c == ' ')
 16         {
 17             buff[i] = '\0';
 18             return 1;
 19         }
 20         buff[i] = c;
 21         i++;
 22     }
 23     //read check
 24     return 0;
 25 }
 26 int main(int argc, char** argv) {
 27     if(argc != 2) {errx(1, "args");}
 28     int pd[2];
 29     if(pipe(pd) == -1) {err(1, "pipe");}
 30     pid_t pid = fork();
 31     if(pid==-1){err(1, "f");}
 32     if(pid ==0){
 33         close(pd[0]);
 34         if(dup2(pd[1], 1) == -1) { err(1, "dup"); }
 35         close(pd[1]);
 36         execlp("find", "find", argv[1], "!",  "-name", "*.hash", "-type", "f",(char*)NULL);
 37         err(1, "exec");
 38     }
 39     close(pd[1]);
 40     char buff[200];
 41     int workers = 0;
 42     while(read_file(pd[0], buff))
 43     {
 44         int process = fork();
 45         if(process == -1) {err(1, "fork");}
 46         if(process == 0){
 47             close(pd[0]);

 48             int pd2[2];
 49             if(pipe(pd2) == -1) {err(1, "pipe");}
 50             pid_t pid2 = fork();
 51             if(pid2 == -1) { err(1, "fork"); }
 52             if(pid2 == 0){
 53                 close(pd2[0]);
 54                 if(dup2(pd2[1],1) ==-1){err(1, "d");}
 55                 close(pd2[1]);//!!
 56                 execlp("md5sum", "md5sum", buff, (char*)NULL);
 57                 err(1, "exec");
 58             }
 59             close(pd2[1]);
 60
 61             char md5[200];
 62             if(read_file(pd2[0], md5) == 0) {err(1, "r");}//cut!!
 63             strncat(buff, ".hash", 200 - strlen(buff) - 1);//!!
 64             //\0
 65             int hash_file = open(buff, O_WRONLY | O_CREAT | O_TRUNC, 0644);
 66             if(hash_file ==-1) {err(1, "o");}
 67             if(write(hash_file, md5, strlen(md5)) < 0) {err(1, "wr");}
 68             close(pd2[0]);
 69             close(hash_file);
 70             _exit(0);
 71         }
 72         workers++;
 73     }
 74     close(pd[0]);
 75
 76     for(int i =0; i < workers; i++)
 77     {
 78         int st;
 79         if(wait(&st) == -1) { err(1, "w");}
 80         if(!WIFEXITED(st)) { err(1, "child killed");}
 81         if(WEXITSTATUS(st)) {err(1, "exit st");}
 82     }
 83     return 0;
 84 }
