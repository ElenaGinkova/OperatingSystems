 1 #include <stdint.h>
  2 #include <err.h>
  3 #include <fcntl.h>
  4 #include <unistd.h>
  5 #include <sys/stat.h>
  6 #include <stdio.h>
  7 typedef struct {
  8     uint16_t offs;
  9     uint8_t old;
 10     uint8_t new;
 11 } patch;
 12
 13 int main(int argc, char* argv[]) {
 14     //open
 15     if(argc != 4){ errx(1, "args"); }
 16     int pf = open(argv[1], O_RDONLY);
 17     if(pf < 0) { err(1, "p1 open"); }
 18     int f1 = open(argv[2], O_RDONLY);
 19     if(f1 < 0) { err(1, "f1 open"); }
 20     int f2 = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
 21     if(f2 < 0) { err(1, "f2 open"); }
 22
 23     //check size
 24     struct stat st;
 25     if(fstat(pf, &st) < 0) { err(1, "stat"); }
 26     size_t psize = st.st_size;
 27     if(psize % sizeof(patch) != 0) { errx(1, "patch size"); }
 28     if(fstat(f1,&st) < 0) { err(1, "f1 stat");}
 29     size_t f1size = st.st_size;
 30
 31     //cpy f1 to f2
 32     uint8_t b;
 33     int read_b;
 34     while((read_b = read(f1, &b, sizeof(b))) > 0){
 35         if(write(f2, &b, sizeof(b)) < 0) { err(1, "err write"); }
 36     }
 37     if(read_b < 0)  { err(1, "patch read"); }
 38
 39     //patch
 40     patch p;
 41     while((read_b = read(pf, &p, sizeof(p))) > 0){
 42         if(p.offs > f1size) { errx(1, "offs too big"); }
 43
 44         printf("offs %d \n", p.offs);
 45         if(lseek(f1, p.offs * sizeof(uint8_t), SEEK_SET) < 0) { err(1, "lseek"); }
 46         uint8_t old;

 47         if(read(f1, &old, sizeof(old)) < 0) { err(1, "read"); }
 48         printf("old %x \n", p.old);
 49
 50         if(lseek(f2, p.offs * sizeof(uint8_t), SEEK_SET) < 0) { err(1, "lseek"); }
 51         if(old != p.old) { errx(1, "old is not the same old"); }
 52
 53         if(write(f2, &p.new, sizeof(p.new)) < 0) { err(1, "write"); }
 54     }
 55     if(read_b < 0) { err(1, "patch read"); }
 56
 57     close(f1);
 58     close(f2);
 59     close(pf);
 60     return 0;
 61 }
