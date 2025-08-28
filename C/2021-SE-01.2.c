  1 #include <sys/time.h>
  2 #include <unistd.h>
  3 #include <sys/types.h>
  4 #include <pwd.h>
  5 #include <fcntl.h>
  6 #include <err.h>
  7 #include <time.h>
  8 #include <stdlib.h>
  9 #include <string.h>
 10 #include <stdint.h>
 11 #include <stdio.h>
 12
 13 int read_pid(int fd, char* buff){
 14     buff[0] = '\0';
 15     int b;
 16     char c;
 17     int i =0;
 18     while((b = read(fd, &c, 1)) > 0){
 19         if(c == '\n') { buff[i] = '\0'; return 1; }
 20         buff[i++] = c;
 21     }
 22     if(b < 0) { err(1, "read");}
 23     return b;
 24 }
 25 void ps(char* uname){
 26     int pd[2];
 27     if(pipe(pd) == -1) { err(1, "pipe"); }
 28     pid_t pid = fork();
 29     if(pid == -1) { err(1, "fork"); }
 30     if(pid == 0) {
 31         close(pd[0]);
 32         if(dup2(pd[1], 1) < 0) { err(2, "dup2"); }
 33         execlp("ps", "ps", "-u", uname, "-o", "pid=",(char*)NULL);
 34         err(1, "exec");
 35     }
 36     close(pd[1]);
 37     char buff[200];
 38     //work the pids
 39     while(read_pid(pd[0], buff))
 40     {
 41         //echo kill(buff);
 42     }
 43     close(pd[0]);
 44     //ps -u uid -o pid=
 45 }
 46 void write_file(const char* buff, int fd){
 47     if(write(fd, buff, strlen(buff)) < 0) { err(1, "write"); }

 48 }
 49
 50 int main(int argc, char** argv) {
 51     if(argc < 3) { errx(1, "args");}
 52     int fd = open("foo.log", O_WRONLY | O_CREAT | O_TRUNC, 0644);
 53     if(fd == -1){ err(1, "open");}
 54     //data, potr, danni
 55
 56     struct timeval tv;
 57     int t = gettimeofday(&tv, NULL);
 58     if(t < 0) { err(1, "tv");}
 59     struct tm* tmp = localtime(&tv.tv_sec);
 60     if(tmp == NULL) { err(1, "tm"); }
 61     char buff[200];
 62     if(strftime(buff, sizeof(buff), "%F %T.", tmp) == 0) { err(1, "time"); }
 63
 64     //lockUser(pwd->pw_name);
 65
 66     //potr
 67     uid_t uid  = getuid();
 68     struct passwd* pwd = getpwuid(uid);
 69     if(pwd == NULL) { err(1, "pwd"); }
 70     write_file(buff, fd);
 71     write_file(" ", fd);
 72     write_file(pwd->pw_name, fd);
 73     write_file(" ", fd);
 74     for(int i = 1; i < argc; i++)
 75     {
 76         write_file(argv[i], fd);
 77     }
 78     //kill all pr
 79     ps(pwd->pw_name);
 80     close(fd);
 81     return 0;
 82 }
