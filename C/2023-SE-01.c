  1 typedef struct{
  2  uint8_t info[2];
  3 }record;
  4
  5 int main(int argc, char** argv) {
  6     if(argc != 3) {errx(1, "args");}
  7     int rfd = open(argv[1], O_RDONLY);
  8     if(rfd == -1) {err(1, "r");}
  9     int wrfd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
 10     if(wrfd == -1) {err(1,"wr");}
 11     char buff[1024];
 12     int b;
 13     record rec;
 14     while((b = read(rfd, &rec, 2)) > 0)
 15     {
 16         if(rec.info[0] != 0x55) {errx(1, "format");}
 17
 18         uint8_t curr = 0;
 19         for(int i = 0; i < rec.info[1] - 3; i++)
 20         {
 21             if(read(rfd, &buff[i], 1) < 0) {err(1, "r");}
 22             curr ^= buff[i];
 23         }
 24         uint8_t sum = 0x55 ^ rec.info[1];
 25         if(read(rfd, &sum, 1) < 0) {err(1, "r");}
 26
 27         if(sum == curr)
 28         {
 29             if(write(wrfd, &rec, sizeof(rec))) {err(1, "wr");}
 30             if(write(wrfd, buff, strlen(buff))) {err(1, "wr");}
 31             if(write(wrfd, &sum, sizeof(sum))) {err(1, "wr");}
 32         }
 33     }
 34     if(b < 0) {err(1, "r");}
 35     close(rfd);
 36     close(wrfd);
 37     return 0;
 38 }
