 1 #include <stdlib.h>
  2 #include <unistd.h>
  3 #include <err.h>
  4 #include <errno.h>
  5 #include <fcntl.h>
  6 #include <sys/types.h>
  7 #include <stdint.h>
  8 #include <sys/stat.h>
  9
 10 int main(int argc, char** argv) {
 11     if(argc != 4) {errx(1, "args");}
 12
 13     int scl = open(argv[1], O_RDONLY);
 14     if(scl == -1) {err(1, "r");}
 15     int sdl = open(argv[2], O_RDONLY);
 16     if(sdl == -1) {err(1, "r");}
 17     int wr = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
 18     if(wr == -1) {err(1, "r");}
 19
 20     uint8_t B;
 21     int b;
 22
 23     struct stat st1, st2;
 24     if(fstat(scl,&st1) < 0 || fstat(sdl,&st2) < 0) {err(1, "st");}
 25     if(st2.st_size % sizeof(uint16_t) != 0 ) {err(1, "stat");}
 26     if(st1.st_size * 8 != st2.st_size / 2) {err(1, "f");}
 27
 28     while((b=read(scl, &B, sizeof(B))) >0)
 29     {
 30         for(int i = 7; i >= 0; i--)
 31         {
 32             uint16_t B2;
 33             uint8_t mask = 1 << i;
 34             if(read(sdl, &B2, sizeof(B2)) < 0) {err(1, "r");}
 35             mask &= B;
 36             if(mask)
 37             {
 38                 if(write(wr, &B2, sizeof(B2)) < 0) {err(1, "wr");}
 39             }
 40         }
 41     }
 42     if(b<0){err(1, "r");}
 43     close(scl);
 44     close(sdl);
 45     close(wr);
 46     return 0;
 47 }

