#include "defs.h"
#include "types.h"
#include "vars.h"
#include "proto.h"
#include "io.h"
#include "vdp.h"



#include "anims.h"



void AnimateCreature(CreaturePtr t){
register short a;
  if(!(--t->anim_time)){
    a = t->anim_next;
    t->anim_time = animations[a].duration;
    t->Frame = animations[a].frame;
    t->anim_next = animations[a].next;
    t->artset = animations[a].artset;
    t->newanim = 1;
  }
}

void BeginAnimation(CreaturePtr t, int activity){
    t->anim_time = animations[activity].duration;
    t->Frame = animations[activity].frame;
    t->anim_next = animations[activity].next;
    t->artset = animations[activity].artset;
    t->newanim = 1;
}

#if 0
unsigned long *CompressedDataPointers[] = {
    0,
};

int CreatureShapeLength[] = {
    0,
};


int LoadCreature(int a, int b){
  ExpandLZ(CompressedDataPointers[a], 0xff0000);
  SendBlock32(0xff0000, (ENEMY_AREA+32*b), CreatureShapeLength[a]*32);
  WaitTOF();
  return CreatureShapeLength[a];
}
#endif

