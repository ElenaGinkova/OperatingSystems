 1 #include <unistd.h>
  2 #include <err.h>
  3 #include <string.h>
  4 #include <stdlib.h>
  5 #include <sys/wait.h>
  6 const int BUFF = 4096;
  7 void write_safe(const char* str) // tuk ne moje sizeof zashtoto e ptr
  8 {
  9     if(write(1, str, strlen(str)) != (ssize_t)strlen(str)) { err(1, "write" ); }
 10 }
 11
 12
 13 int main(void) {
 14     int read_b;
 15     char comm[BUFF];
 16     const char* prompt = "Enter cmd: ";
 17     write_safe(prompt);
 18     while((read_b = read(0, comm, BUFF)) > 0)
 19     {
 20         comm[read_b - 1] = '\0';
 21         if(strcmp(comm, "exit") == 0) { exit(0); }
 22         pid_t child = fork();
 23         if(child == -1) { err(1, "fork"); }
 24         if(child == 0) {
 25             char comm2[BUFF];
 26             strcpy(comm2, "/bin/");
 27             strcat(comm2, comm);
 28             if( execl(comm2, comm, (char*) NULL) == -1) { err(1, "exec"); }
 29         }
 30
 31         int status;
 32         if(wait(&status) == -1) { err(1, "child"); }
 33         if(!WIFEXITED(status)) { errx(1, "child terminated");}
 34         if(WEXITSTATUS(status) != 0) { errx(1, "child exit code"); }
 35         write_safe(prompt);
 36     }
 37     if(read_b <0 ) { err(1, "read"); }
 38     return 0;
 39 }
