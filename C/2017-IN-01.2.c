  1 #include <err.h>
  2 #include <sys/types.h>
  3 #include <unistd.h>
  4 #include <sys/wait.h>
  5 int main(void) {
  6     //cat
  7     int catfd[2];
  8     if(pipe(catfd) < 0) { err(1, "pipe");}
  9
 10     pid_t pid = fork();
 11     if(pid == -1) { err(1, "fork");}
 12
 13     if(pid == 0){
 14         //child
 15         close(catfd[0]);
 16         if(dup2(catfd[1], 1) < 0) { err(1, "dup2");}
 17
 18         execlp("cat", "cat", "/etc/passwd", (char*)NULL);
 19         err(1, "couldnt cat");
 20     }
 21     close(catfd[1]);
 22
 23     //cut
 24     int cutfd[2];
 25     if(pipe(cutfd) < 0) { err(1, "pipe");}
 26     pid = fork();
 27     if(pid < 0) { err(1, "fork");}
 28
 29     if(pid == 0){
 30         //child
 31         close(cutfd[0]);
 32         if(dup2(catfd[0], 0) < 0) { err(1, "dup2"); }
 33         if(dup2(cutfd[1], 1) < 0) { err(1, "dup2"); }
 34
 35         execlp("cut", "cut", "-d", ":", "-f", "7", (char*)NULL);
 36         err(1, "cut");
 37     }
 38     close(catfd[0]);
 39     close(cutfd[1]);
 40
 41     //uniq
 42     int uniqfd[2];
 43     if(pipe(uniqfd) < 0) { err(1, "pipe");}
 44     pid = fork();
 45     if(pid < 0) { err(1, "fork");}
 46     if(pid == 0){
 47         close(uniqfd[0]);
 48         if(dup2(cutfd[0], 0) < 0) { err(1, "dup2"); }
 49         if(dup2(uniqfd[1], 1) < 0) { err(1, "dup2"); }
 50
 51         execlp("uniq", "uniq", "-c", (char*)NULL);
 52         err(1, "uniq");
 53     }
 54     close(cutfd[0]);
 55     close(uniqfd[1]);
 56
 57     while(wait(NULL) > 0) {}
 58
 59     //sort
 60     if(dup2(uniqfd[0], 0) < 0) { err(1, "dup2"); }
 61
 62     execlp("sort","sort", "-n", (char*)NULL);
 63     err(1, "sort");
 64
 65     return 0;
 66 }
