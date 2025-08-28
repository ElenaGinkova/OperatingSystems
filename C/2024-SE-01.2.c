 1 #include <stdio.h>
  2 #include <stdlib.h>
  3 #include <unistd.h>
  4 #include <string.h>
  5 #include <sys/wait.h>
  6 #include <signal.h>
  7 #include <errno.h>
  8 #include <err.h>
  9
 10 int main(int argc, char** argv) {
 11     if(argc != 4) { errx(1, "args"); }
 12
 13     uint16_t S;
 14     int rand = open("/dev/urandom", O_RDONLY);
 15     int null = open("/dev/null", O_WRONLY);
 16     if(rand == -1 || null == -1) {err(1, "o");}
 17     char inputs[UINT16_MAX];
 18     int cnt = atoi(argv[2]);
 19
 20     int res = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
 21     if(res == -1) {err(1, "open");}
 22
 23     for(int i = 0; i < cnt; i++)
 24     {
 25         if(read(rand, &S, sizeof(uint16_t)) < 0) {err(1, "r");}
 26         int b = 0;
 27         if((b = read(rand, inputs, S))  < 0) {err(1, "r");}
 28         input[i] = '\0';
 29
 30         pid_t pid = fork();
 31         if(pid ==-1) {err(1, "fork");}
 32         if(pid == 0)
 33         {
 34             if(dup2(null, 1) < 0) {err(1, "dup");}
 35             close(null);
 36             execlp(argv[1], argv[1], inputs, (char*)NULL);
 37             err(1, "exec");
 38         }
 39
 40         int st;
 41         if(wait(&st) < 0) {err(1, "w");}
 42         if(!WIFEXITED(st))
 43         {
 44             if(write(res, input, strlen(input)) <0) {err(1, "wr");}
 45             close(null);
 46             close(rand);
 47             return 42;
