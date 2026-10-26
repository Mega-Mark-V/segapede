/*
1+byte
00+ 1 bit (=0 => 2 byte run, =1 => 3 byte run)
01+ 3 bits high order offset+1byte low order offset+11+1 byte count
                                                   +2 bits(count-3)
01000+zero byte + 11 + zero byte => end
01000+zero byte + 11 + byte = 1 => 4k break, eat and emit nothing
*/

#define SHIFT_BIT {shifter <<= 1;shift_count++;if(shift_count==8){shifter |= *in++;shift_count=0;}}

int ExpandLZ(unsigned char *in, unsigned char *out){
unsigned char *bytes;
unsigned char *temp;
unsigned long shifter;
int offset;
int count;
int shift_count = 0;
int done = 0;

    bytes = in + *(unsigned short *)in;
    bytes += 2;
    in += 2;
    shifter = *(unsigned long *)in;
    in += 4;
    while(!done){
        if(0x80000000 & shifter){
            SHIFT_BIT;
            *out++ = *bytes++;
        } else {
            SHIFT_BIT;
            if(0x80000000 & shifter){
                SHIFT_BIT;
                offset = (shifter & 0xe0000000) >> 21;
                SHIFT_BIT;
                SHIFT_BIT;
                SHIFT_BIT;
                offset += *bytes++;
                switch(shifter & 0xc0000000){
                    case 0x00000000:
                        count = 3;
                        break;
                    case 0x40000000:
                        count = 4;
                        break;
                    case 0x80000000:
                        count = 5;
                        break;
                    case 0xc0000000:
                        count = *bytes++;
                        if((count == 0) && (offset == 0)) done = 1;
                        if((count == 1) && (offset == 0)) count = 0;
                        break;
                }
                SHIFT_BIT;
                SHIFT_BIT;
                temp = out - offset;
                while(count--) *out++ = *temp++;
            } else {
                SHIFT_BIT;
                temp = out - *bytes++;
                if(0x80000000 & shifter){
                    *out++ = *temp++;
                }
                *out++ = *temp++;
                *out++ = *temp++;
                SHIFT_BIT;
            }
        }
    }
}
