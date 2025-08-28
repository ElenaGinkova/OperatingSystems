 1 #include <fcntl.h>
  2 #include <unistd.h>
  3 #include <err.h>
  4 #include <stdint.h>
  5 #include <stdio.h>
  6 #include <sys/wait.h>
  7 #include <sys/stat.h>
  8 typedef struct {
  9     uint16_t ram;
 10     uint16_t reg_cnt;
 11     char filename[8];
 12 } file;
 13
 14 typedef struct {
 15     uint8_t regs[32];
 16     uint8_t rams[512];
 17     uint8_t instr[4];
 18 } content;
 19
 20 void handleInstr(int fd2, content* c){
 21     while(read(fd2, c->instr, sizeof(c->instr)) > 0){
 22         uint8_t op1 = c->instr[1];
 23         uint8_t op2 = c->instr[2];
 24         uint8_t op3 = c->instr[3];
 25         switch(c->instr[0]){
 26             case 0: c->regs[op1] = (c->regs[op2] & c->regs[op3]); break;
 27             case 1: c->regs[op1] = (c->regs[op2] | c->regs[op3]); break;
 28             case 2: c->regs[op1] = (c->regs[op2] + c->regs[op3]);  break;
 29             case 3: c->regs[op1] = (c->regs[op2] * c->regs[op3]); break;
 30             case 4: c->regs[op1] = (c->regs[op2] ^ c->regs[op3]); break;
 31             case 5: dprintf(1, "%d\n", c->regs[op1]); break;
 32             case 6: sleep(c->regs[op1]); break;
 33             case 7: c->regs[op1] = c->rams[c->regs[op2]]; break;
 34             case 8: c->rams[c->regs[op2]] = c->regs[op1]; break;
 35             case 9: if(c->regs[op1] != c->regs[op2])
 36                     {
 37                         if(lseek(fd2, op3 * 4 ,SEEK_SET) == -1) { err(1, "lseek"); }
 38                     }
 39                     break;
 40             case 10: c->regs[op1] = op2; break;
 41             case 11: c->rams[c->regs[op1]] = op2; break;
 42         }
 43     }
 44 }
 45
 46 void save(char filename[8], const content* c, uint16_t reg_cnt, uint16_t ram){
 47     int fd2_write = open(filename, O_WRONLY);
 48     if(lseek(fd2_write, 0, SEEK_SET) == -1) { err(1, "cant seek"); }
 49     if(write(fd2_write, c->regs, reg_cnt) < 0) { err(1, "write"); }
 50     if(write(fd2_write, c->rams, ram) < 0) { err(1, "write"); }
 51     close(fd2_write);
 52 }
 53 void waitPr(void){
 54     int st;
 55     if(wait(&st) < 0) { err(1, "couldnt wait");}
 56     if(!WIFEXITED(st)) { err(1, "didnt exit"); }
 57     if(WEXITSTATUS(st)) { err(1, "child exit status"); }
 58 }
 59 int main(int argc, char** argv) {
 60     if(argc != 2) { errx(1, "args"); }
 61     int fd1 = open(argv[1], O_RDONLY);
 62     if(fd1 == -1) { err(1, "open"); }
 63
 64     struct stat st;
 65     if(fstat(fd1, &st) == -1) { err(1, "fstat"); }
 66     size_t size = st.st_size;
 67     if(size % sizeof(file) != 0) { errx(1, "file format"); }
 68
 69     int read_b;
 70     file f1;
 71     content c;
 72     int cnt = 0;
 73     while((read_b = read(fd1, &f1, sizeof(f1))) > 0)
 74     {
 75         if(f1.ram > 512 || f1.reg_cnt > 32) { errx(1, "too big nums"); }
 76         pid_t pid = fork();
 77         if(pid == -1) { err(1, "fork"); }
 78         if(pid == 0){
 79             int fd2 = open(f1.filename, O_RDONLY);
 80             if(fd2 == -1) { err(1, "open"); }
 81             if(read(fd2, &c.regs, sizeof(uint8_t) * f1.reg_cnt) < 0) { err(1, "read"); }
 82             if(read(fd2, &c.rams, sizeof(uint8_t) * f1.ram) < 0) { err(1, "read"); }
 83             handleInstr(fd2, &c);
 84             close(fd2);
 85             save(f1.filename, &c, f1.reg_cnt, f1.ram);
 86         }
 87         cnt++;
 88     }
 89     if(read_b < 0) { err(1, "read"); }
 90
 91     for(int i = 0; i < cnt; i++)
 92     {
 93         waitPr();
 94     }

95     close(fd1);
 96     return 0;
 97 }
 98


