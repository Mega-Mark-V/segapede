#include "defs.h"
#include "types.h"
#include "vars.h"
#include "proto.h"
#include "vdp.h"
#include "io.h"


#define START_OF_SPRITES 5

#if 1

static short sizes[16] = {
    1,  /* 0 */
    2,  /* 1 */
    3,  /* 2 */
    4,  /* 3 */
    2,  /* 4 */
    4,  /* 5 */
    6,  /* 6 */
    8,  /* 7 */
    3,  /* 8 */
    6,  /* 9 */
    9,  /* a */
    12, /* b */
    4,  /* c */
    8,  /* d */
    12, /* e */
    16, /* f */
};

static unsigned short places[10] = {
    ZIP_SWAP_AREA,
    SEGMENT_1_AREA,
    SEGMENT_2_AREA,
    SEGMENT_3_AREA,
    SEGMENT_4_AREA,
    SEGMENT_5_AREA,
    SEGMENT_6_AREA,
    SEGMENT_7_AREA,
    SEGMENT_8_AREA,
    SEGMENT_9_AREA,
};

static unsigned short numbers[10] = {
    ZIP_CELL_BASE,
    SEGMENT_1_BASE,
    SEGMENT_2_BASE,
    SEGMENT_3_BASE,
    SEGMENT_4_BASE,
    SEGMENT_5_BASE,
    SEGMENT_6_BASE,
    SEGMENT_7_BASE,
    SEGMENT_8_BASE,
    SEGMENT_9_BASE,
};

void DrawCreatures(){
short ty, tx;
short i, y;
short n;
short xoff, yoff;
register CreaturePtr temp;
register unsigned short *st;
register SpriteDefPtr sp;
unsigned short base;
unsigned short targ;
short send;

  temp = &things[0];
  sp = &spriteAttributes[sprite];
  for(i = 0; i < thingCount; i++){
    if(temp->type < 0){
      temp++;
      continue;
    }
    base = temp->chBase + numbers[i];
    targ = places[i];
    send = temp->newanim;
    st = (unsigned short *)temp->Frame;
    xoff = st[1];
    yoff = st[2];
    ty = (temp->Ypos.w - yoff) - screenYPos + 128;
    if((32 < ty) && (ty < 380)){
      n = START_OF_SPRITES;
      tx = temp->Xpos.w - screenXPos + 0x80;
      for(y = 0; y < st[0]; y++){
        if(send){
          SendBlock32(&temp->artset[st[n+2] * 16], targ, sizes[st[n+1]] * 32);
          temp->newanim = 0;
        }
        sp->vp = ty + st[n];
        sp->cell = base;
        base += sizes[st[n+1]];
        targ += sizes[st[n+1]] * 32;
        sp->size = st[n+1];
        sp->link = sprite + 1;
        if(temp->chBase & HFlip){
          sp->hp = tx - (st[n+3] - xoff) - (((sp->size << 1) & 0x18) + 8);
        } else {
          sp->hp = tx + st[n+3] - xoff;
        }
        n += 4;
        if(!(sp->hp & 0x1ff)) continue;     /* don't let it sit at 0 */
        if((sp->hp & 0xfe00)) continue;     /* don't let it wrap */
        sprite++;
        sp++;
      }
    }
    temp++;
  }
  /* terminate sprite list */
  sp = &spriteAttributes[sprite - 1];
  sp->link = 0;
}

#else

void DrawCreatures(){
short ty, tx;
short txsave;
short i, x, y;
short n;
short xoff, yoff;
register CreaturePtr temp;
register short *st;
register SpriteDefPtr sp;

  temp = &things[0];
  sp = &spriteAttributes[sprite];
  for(i = 0; i <thingCount; i++){
    if(temp->type < 0){
      temp++;
      continue;
    }
    st = (short *)temp->Frame;
    xoff = st[1];
    yoff = st[2];
    ty = (temp->Ypos.w - yoff) - screenYPos + 128;
    if((ty > 32) && (ty < 380)){
      n = START_OF_SPRITES;
      tx = temp->Xpos.w - screenXPos + 0x80;
      for(y = 0; y < st[0]; y++){
        sp->vp = ty + st[n];
        sp->cell = temp->chBase + st[n+2];
        sp->size = st[n+1];
        sp->link = sprite + 1;
        if(temp->chBase & HFlip){
          sp->hp = tx - (st[n+3] - xoff) - (((sp->size << 1) & 0x18) + 8);
        } else {
          sp->hp = tx + st[n+3] - xoff;
        }
        n += 4;
        if(!(sp->hp & 0x1ff)) continue;     /* don't let it sit at 0 */
        if((sp->hp & 0xfe00)) continue;     /* don't let it wrap */
        sprite++;
        sp++;
      }
    }
    temp++;
  }
  /* terminate sprite list */
  sp = &spriteAttributes[sprite - 1];
  sp->link = 0;
}

#endif
