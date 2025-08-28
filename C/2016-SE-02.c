  1 #include <stdint.h>
  2 #include <err.h>
  3 #include <fcntl.h>
  4 #include <unistd.h>
  5 #include <sys/stat.h>
  6 typedef struct{
  7     uint32_t off;
  8     uint32_t len;
  9 } pair;
 10
 11 int main(int argc, char* argv[]) {
 12     if(argc != 4) { errx(1, "args"); }
 13     int f1 = open(argv[1], O_RDONLY);
 14     if(f1 < 0) { err(1, "1 not opened"); }
 15     int f2 = open(argv[2], O_RDONLY);
 16     if(f2 < 0) { err(1, "2 not opened"); }
 17     int f3 = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0644);
 18     if(f3 < 0) { err(1, "3 not opened"); }
 19
 20     struct stat st;
 21     if(fstat(f1, &st) == -1) { err(1, "stat 1"); }
 22     size_t f1_size = st.st_size;
 23     if(fstat(f2, &st) == -1) { err(1, "stat 2"); }
 24     size_t f2_size = st.st_size;
 25     if(f1_size % sizeof(pair) != 0) { errx(1, "size f1"); }
 26
 27     pair p;
 28     int read_b;
 29     while((read_b = read(f1, &p, sizeof(p))) > 0)
 30     {
 31         if((p.off + p.len) * sizeof(uint32_t) > f2_size) { err(1, "wrong pair"); }
 32         if(lseek(f2, p.off*sizeof(uint32_t), SEEK_SET) < 0) { err(1, "lseek"); }
 33         uint32_t toRead;
 34         for(uint32_t i = 0; i < p.len; i++)
 35         {
 36             if(read(f2, &toRead, sizeof(toRead)) < 0) { err(1, "read f2"); }
 37             if(write(f3, &toRead, sizeof(toRead)) < 0) { err(1, "write f3"); }
 38         }
 39     }
 40     if(read_b < 0) { err(1, "f1 reading"); }
 41     close(f1);
 42     close(f2);
 43     close(f3);
 44     return 0;
 45 }
