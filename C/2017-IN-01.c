 1 #include <err.h>
  2 #include <sys/stat.h>
  3 #include <unistd.h>
  4 #include <fcntl.h>
  5 #include <stdint.h>
  6
  7 typedef struct{
  8     uint16_t offs;
  9     uint8_t len;
 10     uint8_t save;
 11 } triple;
 12
 13 void writeStr(int f1, triple tr, int f21, int f22, uint16_t newOffs){
 14     uint8_t b;
 15     for(uint8_t i = 0; i < tr.len; i++)
 16     {
 17         if(read(f1, &b, 1) < 0) { err(1, "read"); }
 18         if(write(f21, &b, 1) < 0) { err(1, "write"); }
 19     }
 20     tr.offs = newOffs;
 21     tr.save = 0;
 22     if(write(f22, &tr, sizeof(tr)) < 0) { err(1, "write"); }
 23 }
 24 int main(int argc, char* argv[]) {
 25     if(argc != 5) {errx(1, "args");}
 26
 27     int f11 = open(argv[1], O_RDONLY);
 28     if(f11 < 0) { err(1, "f1 open"); }
 29     int f12 = open(argv[2], O_RDONLY);
 30     if(f12 < 0) { err(1, "f11 open"); }
 31     int f21 = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
 32     if(f21 < 0) { err(1, "f21 open"); }
 33     int f22 = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
 34     if(f22 < 0) { err(1, "f22 open"); }
 35
 36     struct stat st;
 37     if(fstat(f12,&st) < 0) { err(1, "fstat"); }
 38     size_t size = st.st_size;
 39     if(size % sizeof(triple) != 0) { errx(1, "size f12"); }
 40     if(fstat(f11, &st) < 0) { err(1, "fstat");}
 41     size_t f11size = st.st_size;
 42
 43     int readB;
 44     triple tr;
 45     uint16_t newOffs=0;
 46     while((readB = read(f12, &tr, sizeof(tr))) > 0){
 47         if(tr.offs > f11size) { errx(1, "offs");}
 48         if(lseek(f11, tr.offs, SEEK_SET) < 0) { err(1, "lseek"); }
 49         uint8_t b;
 50         if(read(f11, &b,  1) < 0) { err(1, "read"); }
 51         if(b >= 'A' && b <= 'Z') {
 52             if(lseek(f11, tr.offs, SEEK_SET) < 0) { err(1, "lseek"); }
 53             writeStr(f11, tr, f21, f22, newOffs);
 54             newOffs += tr.len;
 55         }
 56     }
 57     if(readB < 0) { err(1, "read f12"); }
 58
 59
 60     close(f11);
 61     close(f12);
 62     close(f21);
 63     close(f22);
 64     return 0;
 65 }

