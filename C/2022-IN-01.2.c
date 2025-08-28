  1 #include <sys/wait.h>
  2 #include <fcntl.h>
  3 #include <stdio.h>
  4 #include <err.h>
  5 #include <string.h>
  6 #include <stdlib.h>
  7 #include <stdint.h>
  8 #include <unistd.h>
  9
 10 int main(int argc, char** argv) {
 11     if(argc != 3) {errx(1, "args");}
 12     int N = atoi(argv[1]); // strtol(argv[1], NULL, 10)
 13     int D = atoi(argv[2]);
 14     int pd1[2];
 15     int pd2[2];
 16     if(pipe(pd1) == -1) {err(1,"pipe");}
 17     if(pipe(pd2) == -1) {err(1,"pipe");}
 18     char c =1;
 19
 20     pid_t pid = fork();
 21     if(pid == -1) {err(1, "fork");}
 22     if(pid == 0){
 23         //write to pd1, read from pd2
 24         close(pd1[0]);
 25         close(pd2[1]);
 26         int b;
 27         //how tf to print to stdout
 28         while((b =read(pd2[0], &c, 1)) > 0)//wait
 29         {
 30             if(write(1, "DONG\n", 6) < 0) {err(1, "wr");}
 31             if(write(pd1[1], &c, 1) < 0) {err(1, "wr");}//signal
 32         }
 33         if(b<0){err(1, "r");}
 34         close(pd1[1]);
 35         close(pd2[0]);
 36         _exit(0);
 37     }
 38
 39     //read from pd1, write to pd2
 40     close(pd1[1]);
 41     close(pd2[0]);
 42     for(int i = 0; i < N; i++)
 43     {
 44         if(write(1, "DING ", 6) < 0) {err(1, "wr");}
 45         if(write(pd2[1], &c, 1) < 0) {err(1, "wr");}//signal
 46         if(read(pd1[0], &c, 1) < 0) { err(1, "r");}//wait
 47         sleep(D);


 48     }
 49     close(pd1[0]);
 50     close(pd2[1]);
 51     //waitCH
 52     return 0;
 53 }
