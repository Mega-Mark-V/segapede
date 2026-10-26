#include "defs.h"
#include "types.h"
#include "vars.h"
#include "proto.h"
#include "io.h"
#include "anims.h"
#include "coll.h"
#include "floors.h"

#include "params.h"


#define DAMPING 500
#define TRAIL_DISTANCE 0xc0000

void InitSegments(void){
int i;

  for(i = 1; i < 9; i++){
    things[i].type = (i == 1) ? ENGINE_SEGMENT : SPACER_SEGMENT;
    things[i].Xpos.w = 150;
    things[i].Ypos.w = 20;
    things[i].chBase = PALETTE2;
    things[i].surface = 0;
    things[i].follow = i-1;
    things[i].lead = i+1;
    things[i].behavior = IN_AIR;
    if(i == 1) {
      BeginAnimation(&things[i], ANIM_seg_c_l00);
    } else {
      BeginAnimation(&things[i], ANIM_seg_a_l00);
    }
  }
  things[--i].lead = -1;
  things[i].type = TAILCAP_SEGMENT;
  BeginAnimation(&things[i], ANIM_seg_b_l00);
}

/* This array defines any change in the distance that a following segment should maintain */
static long leadDistance[] = {
    0x00000000,
    0x00080000,
    0x00000000,
    0x00000000,
    0x00000000,
};

/* This array defines any change in the distance at which a segment should follow */
static long followDistance[] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
};

static void SegmentHSurface(int i){
int j = things[i].follow;
int segTrail = TRAIL_DISTANCE + leadDistance[things[j].type] + followDistance[things[i].type];
int dxr = things[j].Xpos.l - (things[i].Xpos.l + segTrail);
int dxl = (things[i].Xpos.l + segTrail) - things[j].Xpos.l;

  switch(things[i].state){
    case STOPPED:
      if(dxl > 0x00010000){
        things[i].state = MOVING_LEFT;
        things[i].chBase |= HFlip;
      }
      if(dxr > 0x00010000){
        things[i].state = MOVING_RIGHT;
        things[i].chBase &= ~HFlip;
      }
      if(stickEdges1 & JOYBUTTONC){     /* start a jump */
        things[i].behavior = IN_AIR;
        things[i].Yvel -= 2*JUMP_IMPULSE;
        if(things[i].chBase & HFlip){
          things[i].state = JUMPING_LEFT;
        } else {
          things[i].state = JUMPING_RIGHT;
        }
      }
      break;
    case MOVING_LEFT:
      if(dxl > 0x00010000){
        if(things[i].Xvel > -(ZIP_SPEED_LIMIT - ZIP_ACCELERATION)){
          things[i].Xvel -= ZIP_ACCELERATION;
        } else {
          things[i].Xvel = -ZIP_SPEED_LIMIT;
        }
      } else
      if(dxr > 0x00010000){
        things[i].state = BRAKING_LEFT;
      } else {
        things[i].state = COASTING_LEFT;
      }
      if(stickEdges1 & JOYBUTTONC){     /* start a jump */
        things[i].behavior = IN_AIR;
        things[i].state = JUMPING_LEFT;
        things[i].Yvel -= JUMP_IMPULSE;
      }
      break;
    case MOVING_RIGHT:
      if(dxr > 0x00010000){
        if(things[i].Xvel < (ZIP_SPEED_LIMIT - ZIP_ACCELERATION)){
          things[i].Xvel += ZIP_ACCELERATION;
        } else {
          things[i].Xvel = ZIP_SPEED_LIMIT;
        }
      } else
      if(dxl > 0x00010000){
        things[i].state = BRAKING_RIGHT;
      } else {
        things[i].state = COASTING_RIGHT;
      }
      if(stickEdges1 & JOYBUTTONC){     /* start a jump */
        things[i].behavior = IN_AIR;
        things[i].state = JUMPING_RIGHT;
        things[i].Yvel -= JUMP_IMPULSE;
      }
      break;
    case COASTING_LEFT:
      if(things[i].Xvel < -ZIP_DECELERATION){
        things[i].Xvel += ZIP_DECELERATION;
      } else {
        things[i].Xvel = 0;
        things[i].state = STOPPED;
        break;
      }
      if(dxl > 0x00010000){
        things[i].state = MOVING_LEFT;
      } else
      if(dxr > 0x00010000){
        things[i].state = BRAKING_LEFT;
      } else {
        things[i].behavior = IN_AIR;
        things[i].state = JUMPING_LEFT;
        things[i].Yvel -= JUMP_IMPULSE;
      }
      break;
    case COASTING_RIGHT:
      if(things[i].Xvel > ZIP_DECELERATION){
        things[i].Xvel -= ZIP_DECELERATION;
      } else {
        things[i].Xvel = 0;
        things[i].state = STOPPED;
        break;
      }
      if(dxl > 0x00010000){
        things[i].state = BRAKING_RIGHT;
      } else
      if(dxr > 0x00010000){
        things[i].state = MOVING_RIGHT;
      } else {
        things[i].behavior = IN_AIR;
        things[i].state = JUMPING_RIGHT;
        things[i].Yvel -= JUMP_IMPULSE;
      }
      break;
    case BRAKING_LEFT:
      if(things[i].Xvel < -3 * ZIP_DECELERATION){
        things[i].Xvel += 3 * ZIP_DECELERATION;
      } else {
        things[i].Xvel = 0;
        things[i].state = STOPPED;
        break;
      }
      if(dxl > 0x00010000){
        things[i].state = MOVING_LEFT;
      }
      if(!(dxr > 0x00010000)){
        things[i].state = COASTING_LEFT;
      }
      if(stickEdges1 & JOYBUTTONC){     /* start a jump */
        things[i].behavior = IN_AIR;
        things[i].state = JUMPING_LEFT;
        things[i].Yvel -= JUMP_IMPULSE;
      }
      break;
    case BRAKING_RIGHT:
      if(things[i].Xvel > ZIP_DECELERATION){
        things[i].Xvel -= ZIP_DECELERATION;
      } else {
        things[i].Xvel = 0;
        things[i].state = STOPPED;
        break;
      }
      if(dxr > 0x00010000){
        things[i].state = MOVING_RIGHT;
      }
      if(!(dxl > 0x00010000)){
        things[i].state = COASTING_RIGHT;
      }
      if(stickEdges1 & JOYBUTTONC){     /* start a jump */
        things[i].behavior = IN_AIR;
        things[i].state = JUMPING_RIGHT;
        things[i].Yvel -= JUMP_IMPULSE;
      }
      break;
  }

  /* move him */
  things[i].Xpos.l += things[i].Xvel;

  /* Track surface, possibly switch to V_SURFACE or IN_AIR */
  if(things[i].behavior == H_SURFACE){    /* do this unless we've changed behaviors */
    short elev, elev2;
    int h, v;
    int slope;

    if(things[i].chBase & HFlip){
      h = things[i].Xpos.w - (things[i].Frame[3] - things[i].Frame[1]);
    } else {
      h = things[i].Xpos.w + (things[i].Frame[3] - things[i].Frame[1]);
    }
    v = things[i].Ypos.w + (things[i].Frame[4] - things[i].Frame[2]);
    if(v < 0) v = 0;
    elev = GetElevation(h, v);
    {
      int dh;
      dh = elev - v;
      if(dh == -v){
        things[i].behavior = IN_AIR;
        things[i].state = (things[i].chBase & HFlip) ? JUMPING_LEFT : JUMPING_RIGHT;
      } else {
        if((dh >= -4) && (dh <= 4)){
          things[i].Ypos.w += dh;
          things[i].Yvel = dh << 16;
        } else {
          things[i].behavior = IN_AIR;
          things[i].state = (things[i].chBase & HFlip) ? JUMPING_LEFT : JUMPING_RIGHT;
        }
      }
    }

    /* Apply acceleration for hills */
    if(things[i].chBase & HFlip){
      h++;
      elev2 = GetElevation(h, v);
      things[i].Xvel += (elev2 - elev) << 14;
    } else {
      h--;
      elev2 = GetElevation(h, v);
      things[i].Xvel -= (elev2 - elev) << 14;
    }
  } else {
    things[i].Ypos.l += things[i].Yvel;
  }
}

static void SegmentInAir(int i){
int j = things[i].follow;
int segTrail = TRAIL_DISTANCE + leadDistance[things[j].type] + followDistance[things[i].type];
int dx;

  switch(things[i].state){
    case JUMPING_RIGHT:
      dx = things[j].Xpos.l - (things[i].Xpos.l + segTrail);
      if(dx > 0x00010000){
        if(things[i].Xvel < ZIP_SPEED_LIMIT - ZIP_AIR_ACCELERATION){
          things[i].Xvel += ZIP_AIR_ACCELERATION;
        } else {
          things[i].Xvel = ZIP_SPEED_LIMIT;
        }
      }
      if(dx < -0x00010000){
        things[i].Xvel -= ZIP_AIR_ACCELERATION;
        if(things[i].Xvel < 0){
          things[i].chBase &= ~HFlip;
          things[i].state = JUMPING_LEFT;
        }
      }
      break;
    case JUMPING_LEFT:
      dx = (things[i].Xpos.l + segTrail) - things[j].Xpos.l;
      if(dx > 0x00010000){
        if(things[i].Xvel > -(ZIP_SPEED_LIMIT - ZIP_AIR_ACCELERATION)){
          things[i].Xvel -= ZIP_AIR_ACCELERATION;
        } else {
          things[i].Xvel = -ZIP_SPEED_LIMIT;
        }
      }
      if(dx < -0x00010000){
        things[i].Xvel += ZIP_AIR_ACCELERATION;
        if(things[i].Xvel > 0){
          things[i].chBase |= HFlip;
          things[i].state = JUMPING_RIGHT;
        }
      }
      break;
  }
  /* move him */
  things[i].Yvel += GRAVITY;
  if(things[i].Yvel > 2*ZIP_SPEED_LIMIT) things[i].Yvel = 2*ZIP_SPEED_LIMIT;
  if(things[i].Yvel < -2*ZIP_SPEED_LIMIT) things[i].Yvel = -2*ZIP_SPEED_LIMIT;
  things[i].Xpos.l += things[i].Xvel;
  things[i].Ypos.l += things[i].Yvel;

  /* Check for surface, possibly switch to H_SURFACE */
  {
    short elev;
    int h, v;
    int slope;

    if(things[i].chBase & HFlip){
      h = things[i].Xpos.w - (things[i].Frame[3] - things[i].Frame[1]);
    } else {
      h = things[i].Xpos.w + (things[i].Frame[3] - things[i].Frame[1]);
    }
    v = things[i].Ypos.w + (things[i].Frame[4] - things[i].Frame[2]);
    if(v < 0) v = 0;
    elev = GetElevation(h, v);
    if(elev){
      int dh;
      dh = elev - v;
      if((dh <= 4) && (dh >= -16)){
        things[i].Ypos.w += dh;
        things[i].Yvel = 0;
        things[i].behavior = H_SURFACE;
        things[i].state = (things[i].chBase & HFlip) ? COASTING_LEFT : COASTING_RIGHT;
      }
    }
  }
}

void OperateSegments(void){
int i, j;

#if 1
  for(i = 1; i < thingCount; i++){
    things[i].oldX = things[i].Xpos.l;
    things[i].oldY = things[i].Ypos.l;
    things[i].oldBehavior = things[i].behavior;
    things[i].oldState = things[i].state;
  }
  for(i = 1; i < thingCount; i++){
    if(things[i].type > 0){
      switch(things[i].behavior){
        case DEAD:
          things[i].type = -1;
          if(things[i].lead != -1){
            things[things[i].lead].follow = things[i].follow;
          }
          if(things[i].follow != -1){
            things[things[i].follow].lead = things[i].lead;
          }
          break;
        case BOUNCING_FREE:
          /* move him */
          things[i].Yvel += GRAVITY;
          if(things[i].Yvel > ZIP_SPEED_LIMIT) things[i].Yvel = ZIP_SPEED_LIMIT;
          if(things[i].Yvel < -ZIP_SPEED_LIMIT) things[i].Yvel = -ZIP_SPEED_LIMIT;
          things[i].Xpos.l += things[i].Xvel;
          things[i].Ypos.l += things[i].Yvel;

          /* Check for surface, possibly switch to H_SURFACE */
          {
            short elev;
            int h, v;

            if(things[i].chBase & HFlip){
              h = things[i].Xpos.w - (things[i].Frame[3] - things[i].Frame[1]);
            } else {
              h = things[i].Xpos.w + (things[i].Frame[3] - things[i].Frame[1]);
            }
            v = things[i].Ypos.w + (things[i].Frame[4] - things[i].Frame[2]);
            if(v < 0) v = 0;
            elev = GetElevation(h, v);
            if(elev){
              int dh;
              dh = elev - v;
              if((dh <= 4) && (dh >= -16)){
                things[i].Ypos.w += dh;
                things[i].Yvel = 0;
                things[i].behavior = FREE_ON_SURFACE;
              }
            }
          }
          things[i].Xpos.l += things[i].Xvel;
          things[i].Ypos.l += things[i].Yvel;
          break;
        case H_SURFACE:
          SegmentHSurface(i);
          break;
        case IN_AIR:
          SegmentInAir(i);
          break;
        case FREE_ON_SURFACE:
          if(things[i].Xpos.w > Zip.Xpos.w - 20){
            if(things[i].Xpos.w < Zip.Xpos.w + 20){
              if(things[i].Ypos.w > Zip.Ypos.w - 20){
                if(things[i].Ypos.w < Zip.Ypos.w + 20){
                  things[i].lead = Zip.lead;
                  things[i].follow = 0;
                  things[i].behavior = Zip.behavior;
                  things[i].state = Zip.state;
                }
              }
            }
          }
          break;
        case V_SURFACE:
        case INVERTED_SURFACE:
        case INVERTED_IN_AIR:
          break;
      }
    }
  }

#else
  i = Zip.lead;
  if(i < 0) return;
  j = things[i].follow;

  /* Update them from front to back */
  do{
    long t, t1, t2, dx, dy, ay;
    things[i].Xvel_old = things[i].Xvel;
    things[i].Yvel_old = things[i].Yvel;
    t1 = things[j].Xpos.l - things[i].Xpos.l;
    t2 = things[j].Ypos.l - things[i].Ypos.l;
    dx = TRAIL_DISTANCE + leadDistance[things[j].type] + followDistance[things[i].type];

    ay = abs(t2);
    if(ay > dx){
      dy = dx;
      dx = 0;
    } else {
      if(ay < (70 * dx) / 100){
        dx = dx - (ay / 2);
      } else {
        dx = 2 * (dx - ay);
      }
      dy = ay;
    }
    if(things[j].surface >= 0){
      if(things[i].surface < 0){
        dy = 0 << 16;
        dx = 8 << 16;
      }
    }

    /* apply damping */
    things[i].Xvel = DoDamping(things[i].Xvel, things[j].Xvel_old);

    /* figure and apply the acceleration */
    if(things[j].chBase & HFlip){
      t = t1 + dx;
    } else {
      t = t1 - dx;
    }
    things[i].Xvel += ((long)t) >> 2;

    /* limit it and apply it forward */
#if 1
    if(t > 0x20000) t = 0x20000;
    if(t < -0x20000) t = -0x20000;
#endif
    things[j].Xvel -= ((long)t) >> 3;

    /* limit horizontal speed */
    if(things[i].Xvel > segmentSpeedLimit << 16) things[i].Xvel = segmentSpeedLimit << 16;
    if(things[i].Xvel < -segmentSpeedLimit << 16) things[i].Xvel = -segmentSpeedLimit << 16;
    things[i].Xpos.l += things[i].Xvel;

/* now do it vertically */

    /* apply damping */
    things[i].Yvel = DoDamping(things[i].Yvel, things[j].Yvel_old);

    /* figure and apply the acceleration */
    if(things[j].chBase & VFlip){
      t = t2 - dy;
    } else {
      t = t2 + dy;
    }
    things[i].Yvel += ((long)t) >> 2;

    /* limit it and apply it forward */
    if(t > 0x40000) t = 0x40000;
    if(t < -0x40000) t = -0x40000;
    things[j].Yvel -= ((long)t) >> 2;

    /* propagate it around a corner */
    if(things[j].surface >= 0){
      if(things[i].surface < 0){
        if(things[j].chBase & HFlip){
          things[j].Xvel -= t << 1;
        } else {
          things[j].Xvel += t << 1;
        }
      }
    }

    /* limit vertical speed */
    if(things[i].Yvel > segmentSpeedLimit << 17) things[i].Yvel = segmentSpeedLimit << 17;
    if(things[i].Yvel < -segmentSpeedLimit << 17) things[i].Yvel = -segmentSpeedLimit << 17;
    things[i].Ypos.l += things[i].Yvel;

    /* stop him before the edge of the screen */
    if(things[i].Xpos.w > (11*40*8) - 32){
      things[i].Xpos.w = (11*40*8) - 32;
      things[i].Xvel = 0;
    }
    if(things[i].Xpos.w < 32){
      things[i].Xpos.w = 32;
      things[i].Xvel = 0;
    }
    if(things[i].Xpos.w < things[j].Xpos.w){
      things[i].chBase &= ~HFlip;
    }
    if(things[i].Xpos.w > things[j].Xpos.w){
      things[i].chBase |= HFlip;
    }
    SetSegmentShape(i, j);
    AnimateCreature(&things[i]);
    FollowTerrain(&things[i]);
    j = i;
    i = things[i].lead;
  } while (i >= 0);
#endif
}

int DoDamping(int v1, int v2){
#if 1
  v1 *= (1000-DAMPING);
  v1 /= 1000;
  return v1;
#else
int dv;

  dv = (DAMPING * (v2 - v1)) / 1000;
  return v1 + dv;
#endif
}

#define MAX_ANGLES 9

int shapes[MAX_ANGLES][SEGMENT_TYPE_COUNT] = {
 {
   0, ANIM_zipwalk_01, ANIM_seg_a_d90, ANIM_seg_b_d90, ANIM_seg_c_d90,
 }, {
   0, ANIM_zipwalk_01, ANIM_seg_a_u66, ANIM_seg_b_u66, ANIM_seg_c_u66,
 }, {
   0, ANIM_zipwalk_01, ANIM_seg_a_u45, ANIM_seg_b_u45, ANIM_seg_c_u45,
 }, {
   0, ANIM_zipwalk_01, ANIM_seg_a_u22, ANIM_seg_b_u22, ANIM_seg_c_u22,
 }, {
   0, ANIM_zipwalk_01, ANIM_seg_a_l00, ANIM_seg_b_l00, ANIM_seg_c_l00,
 }, {
   0, ANIM_zipwalk_01, ANIM_seg_a_d22, ANIM_seg_b_d22, ANIM_seg_c_d22,
 }, {
   0, ANIM_zipwalk_01, ANIM_seg_a_d45, ANIM_seg_b_d45, ANIM_seg_c_d45,
 }, {
   0, ANIM_zipwalk_01, ANIM_seg_a_d66, ANIM_seg_b_d66, ANIM_seg_c_d66,
 }, {
   0, ANIM_zipwalk_01, ANIM_seg_a_d90, ANIM_seg_b_d90, ANIM_seg_c_d90,
 },
};

void SetSegmentShape(int i, int j){
int x, y;
int theta;
int t;

  if(things[i].lead >= 0){
    x = things[j].Xpos.l - things[things[i].lead].Xpos.l;
    y = things[j].Ypos.l - things[things[i].lead].Ypos.l;
  } else {
    x = (things[j].Xpos.l - things[i].Xpos.l) << 1;
    y = (things[j].Ypos.l - things[i].Ypos.l) << 1;
  }
  theta = atan2(-y, -x);
  if(theta > 180) theta = 360 - theta;
  t = (((theta * 4) / 45) + 1) >> 1;
  if(things[i].angle != t){
    things[i].angle = t;
    BeginAnimation(&things[i], shapes[t][things[i].type]);
    if(t == 0){
      things[i].chBase |= VFlip;
    } else {
      things[i].chBase &= ~VFlip;
    }
  }
}



extern unsigned short map[];

int GetElevation(int h, int v){
int n;
short chunk;
short block;
short e;
short elev;

  n = (11*20) * (v >> 4) + (h >> 4);
  block = map[n];
  chunk = *((unsigned short *)(&floors[block >> 1]));
  e = collisions[chunk + (h & 15)];

  /* deal with the case of the surface in an adjacent block */
  if(e == 0){
    elev = NO_SURFACE;
    if((v & 15) < 8){
      n = (11*20) * ((v >> 4) - 1) + (h >> 4);
      block = map[n];
      chunk = *((unsigned short *)(&floors[block >> 1]));
      e = collisions[chunk + (h & 15)];
      if(e != 0){
        elev = ((v-16) & ~15) + e - 1;
      }
    } else if((v & 15) >= 8){
      n = (11*20) * ((v >> 4) + 1) + (h >> 4);
      block = map[n];
      chunk = *((unsigned short *)(&floors[block >> 1]));
      e = collisions[chunk + (h & 15)];
      if(e != 0){
        elev = ((v+16) & ~15) + e - 1;
      }
    }
  } else {
    elev = (v & ~15) + e - 1;
  }
  return elev;
}


void FollowTerrain(CreaturePtr c){
int h, v;
short elev;

  if(c->chBase & HFlip){
    h = c->Xpos.w - (c->Frame[3] - c->Frame[1]);
  } else {
    h = c->Xpos.w + (c->Frame[3] - c->Frame[1]);
  }
  v = c->Ypos.w + (c->Frame[4] - c->Frame[2]);
  elev = GetElevation(h, v);
  {
    int dh;
    dh = elev - v;
    if(dh == -v){
      c->Ypos.l += c->Yvel;
      c->Yvel += 0x2000;
      c->surface = -1;     /* no surface */
    } else {
      if(dh <= 0){         /* below surface */
        c->Ypos.w += dh;
        c->Yvel = dh << 16;
        c->surface = 1;
      } else {             /* above surface */
        if(0*dh > 4){
          c->Ypos.l += c->Yvel;
          c->Yvel += 0x2000;
          c->surface = -1;
        } else {
          c->Ypos.w += dh;
          c->Yvel = dh << 16;
          c->surface = 1;
        }
      }
    }
  }
}
