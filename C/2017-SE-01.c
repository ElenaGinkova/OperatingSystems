1 #include <unistd.h>
  2 #include <err.h>
  3 #include <fcntl.h>
  4 #include <stdint.h>
  5 #include <sys/stat.h>
  6
  7 typedef struct{
  8     uint16_t offs;
  9     uint8_t old;
 10     uint8_t new;
 11 } diff;
 12
 13 int main(int argc, char* argv []) {
 14     if(argc != 4 ) { errx(1, "args"); }
 15     int f1 = open(argv[1], O_RDONLY);
 16     if(f1 == -1) { err(1, "f1 open");}
 17     int f2 = open(argv[2], O_RDONLY);
 18     if(f2 == -1) { err(1, "f2 open");}
 19     int f3 = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0644);
 20     if(f3 == -1) { err(1, "f3 open");}
 21
 22     struct stat st;
 23     if(fstat(f1, &st) == -1) { err(1, "stat f1");}
 24     size_t f1_size = st.st_size;
 25     if(fstat(f2, &st) == -1) { err(1, "stat f2");}
 26     size_t f2_size = st.st_size;
 27     if(f1_size != f2_size) { errx(1, "files diff size"); }
 28     if(f1_size > UINT16_MAX) { errx(1, "file too big"); }
 29
 30     uint8_t b1;
 31     uint8_t b2;
 32     int read_b;
 33     uint16_t off = 0;
 34     diff d;
 35     while((read_b = (read(f1, &b1, sizeof(b1)))) > 0){
 36         if(read(f2, &b2, sizeof(b2)) < 0) { err(1, "f2 read");}
 37         if(b1 != b2) {
 38             d.offs= off;
 39             d.old = b1;
 40             d.new = b2;
 41             if(write(f3, &d, sizeof(d)) < 0) { err(1, "write f3"); }
 42         }
 43         off++;
 44     }
 45     if(read_b < 0){ err(1, "read f1"); }
 46
47     close(f1);
 48     close(f2);
 49     close(f3);
 50     return 0;
 51 }
