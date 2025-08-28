  1 #include <err.h>
  2 #include <unistd.h>
  3 #include <fcntl.h>
  4 #include <stdlib.h>
  5 #include <sys/wait.h>
  6
  7 int main(int argc, char* argv[]) {
  8     if(argc != 2) {errx(1, "args");}
  9     int pfd[2];
 10     if(pipe(pfd) == -1) { err(1, "pipe"); }
 11     pid_t pid1 = fork();
 12     if(pid1 == -1) { err(1, "fork"); }
 13     if(pid1 == 0){
 14         //child
 15         close(pfd[0]);
 16
 17         if(dup2(pfd[1], 1) == -1) { err(1, "dup2");}
 18         close(pfd[1]);
 19
 20         execlp("cat", "cat", argv[1], (char*)NULL);
 21         err(1, "cat");
 22     }
 23     close(pfd[1]);
 24     int status;
 25     if(wait(&status) == -1){err(1,"wait");}
 26     if(!WIFEXITED(status))
 27     {
 28         close(pfd[0]);
 29         err(1,"terminated child");
 30     }
 31     if(WEXITSTATUS(status) != 0)
 32     {
 33         close(pfd[0]);
 34         err(1, "exit status child");
 35     }
 36     if(dup2(pfd[0], 0) == -1) { err(1, "dup");}
 37     execlp("sort", "sort", (char*)NULL);
 38     err(1, "sort");
 39     exit(0);
 40     return 0;
 41 }
