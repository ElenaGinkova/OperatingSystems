 1 #include <unistd.h>
  2 #include <err.h>
  3 #include <fcntl.h>
  4 #include <sys/wait.h>
  5 #include <stdlib.h>
  6 #include <string.h>
  7 #include <signal.h>
  8 #include <stdio.h>
  9
 10 int main(int argc, char** argv) {
 11     if(argc < 2) { errx(1, "args");}
 12
 13     int pd[argc - 1][2];
 14     pid_t children[argc -1]; // ne moje taka ppc
 15
 16     for(int i = 1; i < argc; i++)
 17     {
 18         if(pipe(pd[i-1]) == -1) {err(1, "pipe");}
 19
 20          pid_t pid = fork();
 21          if(pid == -1) { err(1, "fork");}
 22          if(pid == 0)
 23          {
 24             close(pd[i-1][0]);
 25             if(dup2(pd[i-1][1], 1) == -1) {err(1, "dup");}
 26             for(int j =0;j<argc-1;j++){close(pd[j][1]);}
 27             execlp(argv[i], argv[i], (char*)NULL);
 28             err(26, "exec");
 29         }
 30         children[i -1] = pid;
 31     }
 32     for(int j = 1; j < argc;j++) {   close(pd[j-1][1]); }
 33
 34 //    char buff[200];
 35     char ch;
 36     int i =0;
 37     int flag = 0;
 38     const char* text = "found it!";
 39     for(int j = 1; j < argc; j++)
 40     { i =0;
 41     while(read(pd[j-1][0], &ch, 1) > 0)
 42     {
 43         if(ch != text[i])
 44         {
 45             i = 0;
 46 //            buff[0] = '\0';
 47             continue;

 48         }
 49         i++;
 50         if((size_t)i == strlen(text)) {
 51             flag = 1;
 52             for(int jj = 0; jj < argc - 1; jj++)
 53             {
 54                 if(kill(children[jj], SIGTERM) < 0) { err(26, "kill fail"); }
 55             }
 56             break;
 57         }
 58     }
 59     if(flag) break;
 60     }
 61     for(int j = 1;j < argc; j++)
 62     {
 63         int st;
 64         if(wait(&st) < 0)  {err(1, "w");}
 65         close(pd[j-1][0]);
 66     }
 67     if(flag)
 68     {
 69         printf("found it");
 70         exit(0);
 71     }
 72     exit(1);
 73
 74     return 0;
 75 }
