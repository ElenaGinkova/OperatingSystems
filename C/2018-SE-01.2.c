 1 #include <unistd.h>
  2 #include <fcntl.h>
  3 #include <err.h>
  4 #include <sys/wait.h>
  5 void waitChild(void)
  6 {
  7     int st;
  8     if(wait(&st) < 0) { err(1, "could not wait)");}
  9     if(!WIFEXITED(st)) { err(1, "child terminated"); }
 10     if(WEXITSTATUS(st) != 0) { err(1, "exit st"); }
 11
 12 }
 13 int main(int argc, char* argv[]) {
 14     if(argc != 2) { errx(1, "args"); }
 15     //find dir - type f - mindepth 1 -printf "%T@ %f\n" | sort -nr | head -n1 | cut -f2 -d ' '
 16     int fd[2];
 17     if(pipe(fd) < 0) { err(1, "pipe"); }
 18     pid_t pid = fork();
 19     if(pid < 0) { err(1, "fork");}
 20
 21     if(pid == 0)
 22     {
 23         close(fd[0]);
 24         if(dup2(fd[1], 1) < 0) { err(1, "dup2");} // we want to write in pipe 1->pipe
 25
 26         execlp("find", "find", argv[1], "-mindepth", "1" ,"-type", "f", "-printf",  "%T@ %f\n", (cha
 27         err(1, "find");
 28     }
 29     close(fd[1]);
 30     waitChild();
 31
 32     int fd2[2];
 33     if(pipe(fd2) == -1) { err(1, "pipe");}
 34     pid = fork();
 35     if(pid == -1) { err(1, "fork");}
 36
 37     if(pid==0)
 38     {
 39         close(fd2[0]);
 40         if(dup2(fd[0], 0) == -1) { err(1,"dup2");}
 41         if(dup2(fd2[1], 1) == -1) { err(1, "dup2");}
 42         execlp("sort", "sort", "-nr", (char*)NULL);
 43         err(1, "sort");
 44     }
 45     close(fd[0]);
 46     close(fd2[1]);
 47     waitChild();


 48
 49     int fd3[2];
 50     if(pipe(fd3) == -1) { err(1, "pipe");}
 51     pid = fork();
 52     if(pid == -1) { err(1, "fork");}
 53
 54     if(pid==0)
 55     {
 56         close(fd3[0]);
 57         if(dup2(fd2[0], 0) == -1) { err(1,"dup2");}
 58         if(dup2(fd3[1], 1) == -1) { err(1, "dup2");}
 59         execlp("head", "head", "-n1", (char*)NULL);
 60         err(1, "head");
 61     }
 62
 63     close(fd2[0]);
 64     close(fd3[1]);
 65     waitChild();
 66
 67     if(dup2(fd3[0], 0) == -1) { err(1,"dup2");}
 68     execlp("cut", "cut", "-f2", "-d", " ", (char*)NULL);
 69     err(1, "cut");
 70
 71     return 0;
 72 }
