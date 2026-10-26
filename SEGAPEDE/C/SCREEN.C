#include "defs.h"
#include "types.h"
#include "vars.h"
#include "proto.h"
#include "io.h"
#include "map.h"
#include "charset.h"
#include "blocks.h"
#include "b_playfl.h"

#define MAX_SCREEN_X (8*10*40)
#define MAX_SCREEN_Y (8*3*28)
#define MAP_WIDTH (11*20)

typedef unsigned short *USP;
typedef unsigned char *UCP;

static void MakeLeftColumn(){
int i = (screenYPos >> 4);
int j;
int k = (screenYPos >> 3) & 30;
int c = (screenXPos >> 2) & 2;

  for(j = k; j < 32;){
    if(i >= 4*28/2) return;
    fieldAColumnUpdate[j++] = *(USP)(((UCP)tileCodes)+(((USP)map)[MAP_WIDTH*(i)+(screenXPos >> 4)])+c);
    fieldAColumnUpdate[j++] = *(USP)(((UCP)tileCodes)+(((USP)map)[MAP_WIDTH*(i++)+(screenXPos >> 4)])+4+c);
  }
  for(j = 0; j < k;){
    if(i >= 4*28/2) return;
    fieldAColumnUpdate[j++] = *(USP)(((UCP)tileCodes)+(((USP)map)[MAP_WIDTH*(i)+(screenXPos >> 4)])+c);
    fieldAColumnUpdate[j++] = *(USP)(((UCP)tileCodes)+(((USP)map)[MAP_WIDTH*(i++)+(screenXPos >> 4)])+4+c);
  }
}

static void MakeRightColumn(){
int i = (screenYPos >> 4);
int j;
int k = (screenYPos >> 3) & 30;
int c = (~screenXPos >> 2) & 2;

  for(j = k; j < 32;){
    if(i >= 4*28/2) return;
    fieldAColumnUpdate[j++] = *(USP)(((UCP)tileCodes)+(((USP)map)[MAP_WIDTH*(i)+((screenXPos+328) >> 4)])+c);
    fieldAColumnUpdate[j++] = *(USP)(((UCP)tileCodes)+(((USP)map)[MAP_WIDTH*(i++)+((screenXPos+328) >> 4)])+4+c);
  }
  for(j = 0; j < k;){
    if(i >= 4*28/2) return;
    fieldAColumnUpdate[j++] = *(USP)(((UCP)tileCodes)+((USP)map)[MAP_WIDTH*(i)+((screenXPos+328) >> 4)]+c);
    fieldAColumnUpdate[j++] = *(USP)(((UCP)tileCodes)+((USP)map)[MAP_WIDTH*(i++)+((screenXPos+328) >> 4)]+4+c);
  }
}

static void MakeTopRow(){
int i = (screenXPos >> 4);
int j;
int k = (screenXPos >> 3) & 62;
int c = (screenYPos >> 1) & 4;

  for(j = k; j < 64;){
    if(i >= 11*40/2) return;
    fieldARowUpdate[j++] = *(USP)(((UCP)tileCodes)+(((USP)map)[(i)+MAP_WIDTH*(screenYPos >> 4)])+c);
    fieldARowUpdate[j++] = *(USP)(((UCP)tileCodes)+(((USP)map)[(i++)+MAP_WIDTH*(screenYPos >> 4)])+2+c);
  }
  for(j = 0; j < k;){
    if(i >= 11*40/2) return;
    fieldARowUpdate[j++] = *(USP)(((UCP)tileCodes)+(((USP)map)[(i)+MAP_WIDTH*(screenYPos >> 4)])+c);
    fieldARowUpdate[j++] = *(USP)(((UCP)tileCodes)+(((USP)map)[(i++)+MAP_WIDTH*(screenYPos >> 4)])+2+c);
  }
}

static void MakeBottomRow(){
int i = (screenXPos >> 4);
int j;
int k = (screenXPos >> 3) & 62;
int c = (~screenYPos >> 1) & 4;

  for(j = k; j < 64;){
    if(i >= 11*40/2) return;
    fieldARowUpdate[j++] = *(USP)(((UCP)tileCodes)+(((USP)map)[(i)+MAP_WIDTH*((screenYPos+232) >> 4)])+c);
    fieldARowUpdate[j++] = *(USP)(((UCP)tileCodes)+(((USP)map)[(i++)+MAP_WIDTH*((screenYPos+232) >> 4)])+2+c);
  }
  for(j = 0; j < k;){
    if(i >= 11*40/2) return;
    fieldARowUpdate[j++] = *(USP)(((UCP)tileCodes)+(((USP)map)[(i)+MAP_WIDTH*((screenYPos+232) >> 4)])+c);
    fieldARowUpdate[j++] = *(USP)(((UCP)tileCodes)+(((USP)map)[(i++)+MAP_WIDTH*((screenYPos+232) >> 4)])+2+c);
  }
}

void InitScreen(){
int i;

  WaitTOF();        /* ensure that the I/O queue is empty */
#if 1
  SendBlock32(gameCells, 0, sizeof(gameCells));
#else
  VDPDEST(0);
  for(i = 0; i < sizeof(gameCells) / 2; i++){
    VDPPort.data.l = gameCells[i];
  }
#endif
  screenXPos = 0;
  screenYPos = 31 * 8;
  for(i = 31; i >= 0; i--){
    MakeTopRow();
    memcpy((char *)(0xff0000+128*i), fieldARowUpdate, 128);
    screenYPos -= 8;
  }
  screenYPos = 0;
  SendBlock32(0xff0000, 0xc000, 4096);
  WaitTOF();
  for(i = 0; i < 32; i++){
    memcpy((char *)(0xff0000+128*i), &(((unsigned short *)b_field)[32*i]), 64);
    memcpy((char *)(0xff0000+128*i+64), &(((unsigned short *)b_field)[32*i]), 64);
  }
  SendBlock32(0xff0000, 0xe000, 4096);
  WaitTOF();
}

void UpdateScreen(short x, short y){
int sp;
int dx, dy;

  if(x < screenXPos + 160 - 30){
    dx = (screenXPos + 160 - 30) - x;
    sp = (dx <= 8) ? dx : 8;
    if(screenXPos < sp) sp = screenXPos;
    if(sp){
      unsigned int oldx = screenXPos;

      screenXPos -= sp;
      if((screenXPos & ~7) != (oldx & ~7)){
        MakeLeftColumn();
        SendColumn(fieldAColumnUpdate, 0xc000+(((screenXPos >> 2) & ~1) % 128));
      }
    }
  }
  if(x > screenXPos + 160 + 30){
    dx = x - (screenXPos + 160 + 30);
    sp = (dx <= 8) ? dx : 8;
    if(screenXPos > MAX_SCREEN_X - sp) sp = MAX_SCREEN_X - screenXPos;
    if(sp){
      unsigned int oldx = screenXPos;

      screenXPos += sp;
      if((screenXPos & ~7) != (oldx & ~7)){
        MakeRightColumn();
        SendColumn(fieldAColumnUpdate, 0xc000+((((screenXPos + 328) >> 2) & ~1) % 128));
      }
    }
  }
  if(y < screenYPos + 112 - 30){
    dy = (screenYPos + 112 - 30) - y;
    sp = (dy <= 8) ? dy : 8;
    if(screenYPos < sp) sp = screenYPos;
    if(sp){
      unsigned int oldy = screenYPos;
      screenYPos -= sp;
      if((screenYPos & ~7) != (oldy & ~7)){
        MakeTopRow();
        SendBlock32(fieldARowUpdate, 0xc000+128 * ((screenYPos >> 3) % 32), 128);
      }
    }
  }
  if(y > screenYPos + 112 + 30){
    dy = y - (screenYPos + 112 + 30);
    sp = (dy <= 8) ? dy : 8;
    if(screenYPos > MAX_SCREEN_Y - sp) sp = MAX_SCREEN_Y - screenYPos;
    if(sp){
      unsigned int oldy = screenYPos;

      screenYPos += sp;
      if((screenYPos & ~7) != (oldy & ~7)){
        if(screenYPos < MAX_SCREEN_Y - 8){
          MakeBottomRow();
          SendBlock32(fieldARowUpdate, 0xc000+128 * (((screenYPos + 232) >> 3) % 32), 128);
        }
      }
    }
  }
  SendScrolls(screenYPos, screenYPos>>1, -screenXPos, -screenXPos>>1);
}
