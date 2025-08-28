  1 uint8_t encode(uint8_t in){
  2     uint8_t = 0;
  3     for(int i = 0; i < 4; i++)
  4     {
  5         mask = 1 << i;
  6         mask &= in;
  7         if(mask == 0)
  8         {
  9             out |= 1 << (i * 2);
 10         }
 11         else {
 12             out = 2 << (i * 2);
 13         }
 14     }
 15     return out;
 16 }
 17
 18 int main(int argc, char** argv) {
 19     if(argc != 3) { errx(1, "args"); }
 20     int fdr = open(argv[1], O_RDONLY);
 21     if(fdr == -1) { err(1, "read"); }
 22     int fdw = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
 23     if(fdw == -1) { err(1, "wr");}
 24     //1->10, 0->01
 25     uint8_t in;
 26     uint16_t out;
 27     int b;
 28     while((b = read(fdr, &in, 1)) > 0)
 29     {
 30         uint8_t answ[2] = {0,0};
 31         answ[0] = encode(in >> 4);
 32         answ[1] = encode(in && 15); // 0x0F
 33         if(write(fdw, &answ, sizeof(answ)) < 0) { err(1, "wr"); }
 34     }
 35     if(b < 0) {err(1,"r");}
 36     close(fdr);
 37     close(fdw);
 38     return 0;
 39 }
