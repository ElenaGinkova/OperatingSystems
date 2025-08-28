  1 #include <unistd.h>
  2 #include <fcntl.h>
  3 #include <stdlib.h>
  4 #include <sys/wait.h>
  5 #include <err.h>
  6
  7 char list[3][5] = {"tic ","tac ","toe\n"};
  8
  9 int main(int argc, char** argv) {
 10     if(argc != 3) {errx(1, "args");}
 11
 12     int pr_cnt = atoi(argv[1]);
 13     int wr_cnt = atoi(argv[2]);
 14
 15     if(pr_cnt < 1 || pr_cnt > 7) {errx(1, "args");}
 16     if(wr_cnt < 1 || wr_cnt > 35) {errx(1, "args");}
 17
 18     int pipes[8][2];
 19     int id = 0;
 20
 21
 22     for(int i = 0; i < pr_cnt + 1; i++)
 23     {
 24         if(pipe(pipes[i]) == -1) {err(1, "pipe");}
 25     }
 26
 27     for(int i = 0; i < pr_cnt; i++)
 28     {
 29         pid_t pid = fork();
 30         if(pid == -1) {err(1, "fork");}
 31         if(pid == 0) { id = i + 1; break; }
 32     }
 33
 34     if(id == 0) {
 35         close(pipes[0][0]);
 36         close(pipes[pr_cnt][1]);
 37         for(int i = 1; i < pr_cnt; i++)
 38         {
 39             close(pipes[i][0]);
 40             close(pipes[i][1]);
 41         }
 42     }
 43     else
 44     {
 45         for(int i = 0; i <= pr_cnt; i++)
 46         {
 47             if(i == id - 1)
 48             {
 49                 close(pipes[i][1]);
 50                 close(pipes[id][0]);
 51                 i++;
 52             }
 53             else
 54             {
 55                 close(pipes[i][0]);
 56                 close(pipes[i][1]);
 57             }
 58         }
 59     }
 60     int s = 1;
 61     if(id == 0)
 62     {
 63         if(write(1, list[0], 4) < 0) {err(1, "wr");}
 64         if(write(pipes[0][1], &s, sizeof(int)) < 0) {err(1, "wr");}
 65     }
 66     int pipe = id - 1;
 67     if(pipe < 0) { pipe = pr_cnt;}
 68     while(read(pipes[pipe][0], &s, sizeof(int)) > 0)
 69     {
 70         if(s >= wr_cnt)
 71         {
 72             if(write(pipes[id][1], &s, sizeof(int)) < 0) {err(1, "wr");}
 73             close(pipes[pipe][0]);
 74             close(pipes[id][1]);
 75             exit(0);
 76         }
 77         if(write(1, list[s % 3], 4) < 0) {err(1, "wr");}
 78         s++;
 79         if(write(pipes[id][1], &s, sizeof(int)) < 0) {err(1, "wr");}
 80     }
 81     close(pipes[pipe][0]);
 82     close(pipes[0][1]);
 83     return 0;
 84 }


