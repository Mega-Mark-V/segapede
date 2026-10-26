#include "defs.h"
#include "types.h"
#include "vars.h"
#include "proto.h"
#include "io.h"
#include "anims.h"
#include "gems.h"

void main(void){
  gemsstartsong(16);
  for(;;){
    WaitTOF();
    asm(" move.w #$2700,SR");
    InitVDP();
    asm(" move.w #$2500,SR");
    if(SegaLogo() || Title()){
      Game();
    }
  }
}

static short palette[64] = {
  0x000,0x602,0x802,0xa02,0xc02,0x200,0x400,0x202,
  0x000,0x026,0x068,0x28a,0x6ce,0x024,0x046,0x4ac,

  0x200,0x020,0x040,0x060,0x080,0x0c0,0x4e6,0xeee,
  0x000,0xe44,0xe82,0xea8,0xeee,0x024,0x046,0x4ac,

  0x000,0x002,0x422,0x644,0x866,0xcaa,0xeee,0x202,
  0x024,0x048,0x06c,0x0ae,0x0ee,0xe00,0xe60,0xe0e,

  0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
  0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
};

void Game(void){
int i, j;
short workpal[64];
int fadeTime = 30;

  InitGameVariables();
  /* blow down the initial playfield */
  InitScreen();
  thingCount = 9;

  /* fade in while running game */
  for(i = 1; i <= fadeTime; i++){
    for(j = 0; j < 64; j++){
      workpal[j] = palette[j];
    }
    InterpolateFullPalette(0, i, fadeTime, workpal);
    SendPalettes(workpal);
    GameCore();
  }

  /* run game until done */
  while(GameCore());

  /* fade out while running game */
  for(i = fadeTime-1; i >= 0; i--){
    for(j = 0; j < 64; j++){
      workpal[j] = palette[j];
    }
    InterpolateFullPalette(0, i, fadeTime, &workpal);
    SendPalettes(workpal);
    GameCore();
  }
}

int GameCore(void){
int done;

#if TIMING
 *(volatile unsigned long *)0xc00004 = 0xc0000000;
 *(volatile unsigned short *)0xc00000 = 0x00f;
#endif

  /* move Zip */
  OperatePlayer();
#if TIMING
 *(volatile unsigned long *)0xc00004 = 0xc0000000;
 *(volatile unsigned short *)0xc00000 = 0xf0f;
#endif
/*OperateSegments(); */

  /* move enemies */

  /* move screen */
#if TIMING
 *(volatile unsigned long *)0xc00004 = 0xc0000000;
 *(volatile unsigned short *)0xc00000 = 0x0f0;
#endif
  UpdateScreen(things[0].Xpos.w, things[0].Ypos.w);

  /* draw everybody */
#if TIMING
 *(volatile unsigned long *)0xc00004 = 0xc0000000;
 *(volatile unsigned short *)0xc00000 = 0xf00;
#endif
  DrawCreatures();
#if TIMING
 *(volatile unsigned long *)0xc00004 = 0xc0000000;
 *(volatile unsigned short *)0xc00000 = 0xff0;
#endif
  SendSpriteControl(spriteAttributes);
  ReadControllers();

#if TIMING
 *(volatile unsigned long *)0xc00004 = 0xc0000000;
 *(volatile unsigned short *)0xc00000 = 0x000;
#endif
  WaitTOF();
  sprite = 0;
  done = (stickEdges1 & JOYSTART);
  return !done;
}

void InitGameVariables(void){
  screenXPos = 0;
  screenYPos = 0;
  currentStick1 = 0;
  lastStick1 = 0;
  stickEdges1 = 0;
  currentStick2 = 0;
  lastStick2 = 0;
  stickEdges2 = 0;
  InitPlayer();
}
