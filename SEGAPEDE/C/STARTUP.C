#include "types.h"
#include "defs.h"
#include "vars.h"
#include "vdp.h"
#include "proto.h"


extern char patchbank[];
extern char envbank[];
extern char seqbank[];
extern char sampbank[];


short vdpRegisters[]= {
  0x8004,
  0x8174,
  0x8200+(FG>>10),
  0x8300+(SIDEBAR>>10),
  0x8400+(BG>>13),
  0x8500+(SPRITE_ATTRIBUTES>>9),
  0x8600,
  0x8700,
  0x8800,
  0x8900,
  0x8a00,
  0x8b08,
  0x8c81,
  0x8d00+(HSCROLL>>10),
  0x8e00,
  0x8f02,
  0x9001,
  0x9100,
  0x9200,
};


#pragma noreturn start
#pragma noreturn main

void start(){
  asm(" move.w #$2700,SR",4);
  if(*(unsigned char*)VERSION & 0x0f)
    *((unsigned long*)WATCHDOG) = 0x53454741;
  asm(" moveq #$40,d0");
  asm(" move.b d0,$a10009");
  asm(" move.b d0,$a1000b");
  asm(" move.b #$9f,$a1000d");
  asm(" move.b #$7f,$a10007");

  /* initialize the work RAM */
  {
    int i;
    for(i = 0; i < 16384; i++){
      ((long*)0x00ff0000)[i] = 0;
    }
  }
  InitVDP();
  gemsinit(patchbank,envbank,seqbank,sampbank);
  asm(" move.w #$2500,SR");
  main();
}

/* this is to circumvent the compiler's preference for using the clear
instruction, which screws up vdp access */
short clearvalue;

void InitVDP(void){
register int i;

  for(i = 0; i < 19; i++){
    VDPPort.control.w = vdpRegisters[i];
  }
  VDPPort.control.l = VRAMWR;
  for(i = 0; i < 32768; i++){
    VDPPort.data.w = clearvalue;
  }
  VDPPort.control.l = CRAMWR;
  for(i = 0; i < 64; i++){
    VDPPort.data.w = clearvalue;
  }
  VDPPort.control.l = VSRAMWR;
  for(i = 0; i < 40; i++){
    VDPPort.data.w = clearvalue;
  }
}

#pragma interrupt VBLHandler

void VBLHandler(){
  tickCounter++;
  vblFlag = 1;
  VDPPort.control.w = vdpRegisters[1] & ~0x20;
#if TIMING
 *(volatile unsigned long *)0xc00004 = 0xc0000000;
 *(volatile unsigned short *)0xc00000 = 0x0ff;
#endif
  DoIO();
#if TIMING
 *(volatile unsigned long *)0xc00004 = 0xc0000000;
 *(volatile unsigned short *)0xc00000 = 0x000;
#endif
  VDPPort.control.w = vdpRegisters[1];
}

#pragma interrupt HBLHandler

void HBLHandler(){
  asm(" and.w #$f0ff,20(sp)",4);
  asm(" or.w #$0500,20(sp)",4);
}

#pragma interrupt ErrorHandler
#pragma noreturn ErrorHandler

void ErrorHandler(){
  while(1);
}

void WaitTOF(){
  vblFlag = 0;
  while(!vblFlag)
    asm(" stop #$2500");
}

void ReadHWControllers(){
  asm(" move.w  #$2700,SR");
  gemsholdz80();
  asm(" lea $a10003,a0");
  asm(" move.b #0,(a0)");
  asm(" nop");
  asm(" nop");
  asm(" move.b (a0),d0");
  asm(" lsl.b #2,d0");
  asm(" andi.b #$c0,d0");
  asm(" move.b #$40,(a0)");
  asm(" nop");
  asm(" nop");
  asm(" move.b (a0),d1");
  asm(" andi.b #$3f,d1");
  asm(" or.b d1,d0");
  asm(" not.b d0");
  asm(" move.b d0,_currentStick1");
  asm(" lea $a10005,a0");
  asm(" move.b #0,(a0)");
  asm(" nop");
  asm(" nop");
  asm(" move.b (a0),d0");
  asm(" lsl.b #2,d0");
  asm(" andi.b #$c0,d0");
  asm(" move.b #$40,(a0)");
  asm(" nop");
  asm(" nop");
  asm(" move.b (a0),d1");
  asm(" andi.b #$3f,d1");
  asm(" or.b d1,d0");
  asm(" not.b d0");
  asm(" move.b d0,_currentStick2");
  asm(" move.w #$2500,SR");
  gemsreleasez80();
  stickEdges1 = (currentStick1 ^ lastStick1) & currentStick1;
  lastStick1 = currentStick1;
  stickEdges2 = (currentStick2 ^ lastStick2) & currentStick2;
  lastStick2 = currentStick2;
}
