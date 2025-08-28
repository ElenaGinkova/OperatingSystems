  1 #include <unistd.h>
  2 #include <err.h>
  3 #include <time.h>
  4 #include <sys/wait.h>
  5 #include <fcntl.h>
  6 #include <stdlib.h>
  7 #include <stdio.h>
  8 int main(int argc, char* argv[]) {
  9     if (argc < 3) { err(1, "args"); }
 10     int file = open("run.log", O_CREAT | O_TRUNC | O_WRONLY, 0644); //!
 11     char** args = argv + 1;
 12     for(int i = 0; i < argc-1; i++)
 13     {
 14         args[i] = args[i + 1];
 15     }
 16     args[argc-1] = NULL;
 17
 18     int duration = atoi(argv[1]); //!!
 19     if (duration < 0 || duration > 10) { errx(1, "duration not 1-9"); }
 20     int flag = 0;
 21
 22     while (1)
 23     {
 24         time_t start = time(NULL);
 25         pid_t pid = fork();
 26         if (pid < 0) { err(1, "fork"); }
 27         if(pid == 0) {
 28             //child
 29             execvp(argv[2], args); // args are vector
 30             err(1, "comm");
 31         }
 32
 33         int st;
 34         if(wait(&st) < 0) { err(1, "cant wait"); }
 35         time_t end = time(NULL);
 36
 37         int exit_code;
 38         if (WIFEXITED(st))
 39         {
 40             exit_code = WEXITSTATUS(st);
 41         }
 42         else
 43         {
 44             exit_code = 129;
 45         }
 46         char buff[256];
 47         int len = snprintf(buff, sizeof(buff), "%ld %ld %d\n", start, end, exit_code);
 48         if(write(file, buff, len) < ) { err(1, "write"); }
 49
 50         if (exit_code != 0 && end - start < duration)
 51         {
 52             if (flag == 1) { break; }
 53             else { flag = 1; }
 54         }
 55         else
 56         {
 57             flag = 0;
 58         }
 59     }
 60     close(file);
 61 }


