 1 #include <err.h>
  2 #include <unistd.h>
  3 #include <sys/wait.h>
  4 #include <sys/types.h>
  5 #include <string.h>
  6 const int BUFF = 1024;
  7
  8 void waitStatus(void)
  9 {
 10     int status;
 11     if(wait(&status) < 0) { err(1, "wait");}
 12     if(!WIFEXITED(status)) { err(1, "child terminated");}
 13     if(WEXITSTATUS(status)) { err(1, "child exit status"); }
 14 }
 15 void exComm(char* comm)
 16 {
 17     int read_b;
 18     char buff[2][BUFF];
 19     int idx = 0;
 20     int i = 0;
 21     char c;
 22     while((read_b = read(0, &c, 1)) > 0)
 23     {
 24         if(c != ' ' && c != '\n' && c != '\t')
 25         {
 26             if(i >= 4) { errx(1, "too long arg");}
 27             buff[idx][i] = c;
 28             i++;
 29         }
 30         else
 31         {
 32             buff[idx][i] = '\0';
 33             if(idx == 1){
 34                 pid_t pid = fork();
 35                 if(pid < 0) { err(1, "fork");}
 36                 if(pid == 0) {
 37                     //child
 38                     execlp(comm, comm, buff[0], buff[1], (char*)NULL);
 39                     err(1, "comm");
 40                 }
 41                 waitStatus();
 42             }
 43             idx = (idx+1)%2;
 44             i = 0;
 45         }
 46     }

 47     if(read_b < 0) { err(1, "read"); }
 48     if(idx == 1)
 49     {
 50         pid_t pid = fork();
 51         if(pid < 0) { err(1, "fork");}
 52         if(pid == 0) {
 53             execlp(comm, comm, buff[0], (char*)NULL);
 54             err(1, "comm");
 55         }
 56         waitStatus();
 57     }
 58 }
 59
 60 int main(int argc, char* argv[]) {
 61     if(argc > 2)
 62     {
 63         errx(1, "args");
 64     }
 65     //1
 66     char def[] = "echo";
 67     if(argc == 2) {
 68         exComm(argv[1]);
 69         if(strlen(argv[1]) > 4) { errx(1, "arglen");}
 70     }
 71     else { exComm(def); }
 72
 73     return 0;
 74 }
 75
