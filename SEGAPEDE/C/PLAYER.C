#include "defs.h"
#include "types.h"
#include "vars.h"
#include "proto.h"
#include "io.h"
#include "anims.h"


#include "params.h"

void InitPlayer(void){
int i;

  for(i = 0; i < thingCount; i++){
    things[i].type = -1;
  }
  Zip.type = HEAD_SEGMENT;
  Zip.Xpos.w = 150;
  Zip.Ypos.w = 20;
  Zip.Xvel = Zip.Yvel = 0;
  Zip.chBase = PALETTE2;
  Zip.surface = -1;
  Zip.follow = -1;
  Zip.lead = 1;
  Zip.behavior = IN_AIR;
  Zip.state = JUMPING_RIGHT;
  BeginAnimation(&Zip, ANIM_zbored);
  InitSegments();
}

static void PlayerHSurface(){
  switch(Zip.state){
    case STOPPED:
      if(currentStick1 & JOYLEFT){
        Zip.state = MOVING_LEFT;
        Zip.chBase |= HFlip;
      }
      if(currentStick1 & JOYRIGHT){
        Zip.state = MOVING_RIGHT;
        Zip.chBase &= ~HFlip;
      }
      if(stickEdges1 & JOYBUTTONC){     /* start a jump */
        Zip.behavior = IN_AIR;
        Zip.Yvel -= 2*JUMP_IMPULSE;
        if(Zip.chBase & HFlip){
          Zip.state = JUMPING_LEFT;
        } else {
          Zip.state = JUMPING_RIGHT;
        }
      }
      break;
    case MOVING_LEFT:
      if(currentStick1 & JOYLEFT){
        if(Zip.Xvel > -(ZIP_SPEED_LIMIT - ZIP_ACCELERATION)){
          Zip.Xvel -= ZIP_ACCELERATION;
        } else {
          Zip.Xvel = -ZIP_SPEED_LIMIT;
        }
      }
      if(currentStick1 & JOYRIGHT){
        Zip.state = BRAKING_LEFT;
      }
      if(!(currentStick1 & (JOYLEFT | JOYRIGHT))){
        Zip.state = COASTING_LEFT;
      }
      if(stickEdges1 & JOYBUTTONC){     /* start a jump */
        Zip.behavior = IN_AIR;
        Zip.state = JUMPING_LEFT;
        Zip.Yvel -= JUMP_IMPULSE;
      }
      break;
    case MOVING_RIGHT:
      if(currentStick1 & JOYRIGHT){
        if(Zip.Xvel < (ZIP_SPEED_LIMIT - ZIP_ACCELERATION)){
          Zip.Xvel += ZIP_ACCELERATION;
        } else {
          Zip.Xvel = ZIP_SPEED_LIMIT;
        }
      }
      if(currentStick1 & JOYLEFT){
        Zip.state = BRAKING_RIGHT;
      }
      if(!(currentStick1 & (JOYLEFT | JOYRIGHT))){
        Zip.state = COASTING_RIGHT;
      }
      if(stickEdges1 & JOYBUTTONC){     /* start a jump */
        Zip.behavior = IN_AIR;
        Zip.state = JUMPING_RIGHT;
        Zip.Yvel -= JUMP_IMPULSE;
      }
      break;
    case COASTING_LEFT:
      if(Zip.Xvel < -ZIP_DECELERATION){
        Zip.Xvel += ZIP_DECELERATION;
      } else {
        Zip.Xvel = 0;
        Zip.state = STOPPED;
        break;
      }
      if(currentStick1 & JOYLEFT){
        Zip.state = MOVING_LEFT;
      }
      if(currentStick1 & JOYRIGHT){
        Zip.state = BRAKING_LEFT;
      }
      if(stickEdges1 & JOYBUTTONC){     /* start a jump */
        Zip.behavior = IN_AIR;
        Zip.state = JUMPING_LEFT;
        Zip.Yvel -= JUMP_IMPULSE;
      }
      break;
    case COASTING_RIGHT:
      if(Zip.Xvel > ZIP_DECELERATION){
        Zip.Xvel -= ZIP_DECELERATION;
      } else {
        Zip.Xvel = 0;
        Zip.state = STOPPED;
        break;
      }
      if(currentStick1 & JOYLEFT){
        Zip.state = BRAKING_RIGHT;
      }
      if(currentStick1 & JOYRIGHT){
        Zip.state = MOVING_RIGHT;
      }
      if(stickEdges1 & JOYBUTTONC){     /* start a jump */
        Zip.behavior = IN_AIR;
        Zip.state = JUMPING_RIGHT;
        Zip.Yvel -= JUMP_IMPULSE;
      }
      break;
    case BRAKING_LEFT:
      if(Zip.Xvel < -3 * ZIP_DECELERATION){
        Zip.Xvel += 3 * ZIP_DECELERATION;
      } else {
        Zip.Xvel = 0;
        Zip.state = STOPPED;
        break;
      }
      if(currentStick1 & JOYLEFT){
        Zip.state = MOVING_LEFT;
      }
      if(!(currentStick1 & JOYRIGHT)){
        Zip.state = COASTING_LEFT;
      }
      if(stickEdges1 & JOYBUTTONC){     /* start a jump */
        Zip.behavior = IN_AIR;
        Zip.state = JUMPING_LEFT;
        Zip.Yvel -= JUMP_IMPULSE;
      }
      break;
    case BRAKING_RIGHT:
      if(Zip.Xvel > ZIP_DECELERATION){
        Zip.Xvel -= ZIP_DECELERATION;
      } else {
        Zip.Xvel = 0;
        Zip.state = STOPPED;
        break;
      }
      if(currentStick1 & JOYRIGHT){
        Zip.state = MOVING_RIGHT;
      }
      if(!(currentStick1 & JOYLEFT)){
        Zip.state = COASTING_RIGHT;
      }
      if(stickEdges1 & JOYBUTTONC){     /* start a jump */
        Zip.behavior = IN_AIR;
        Zip.state = JUMPING_RIGHT;
        Zip.Yvel -= JUMP_IMPULSE;
      }
      break;
  }

  /* move him */
  Zip.Xpos.l += Zip.Xvel;

  /* Track surface, possibly switch to V_SURFACE or IN_AIR */
  if(Zip.behavior == H_SURFACE){    /* do this unless we've changed behaviors */
    short elev, elev2;
    int h, v;
    int slope;

    if(Zip.chBase & HFlip){
      h = Zip.Xpos.w - (Zip.Frame[3] - Zip.Frame[1]);
    } else {
      h = Zip.Xpos.w + (Zip.Frame[3] - Zip.Frame[1]);
    }
    v = Zip.Ypos.w + (Zip.Frame[4] - Zip.Frame[2]);
    if(v < 0) v = 0;
    elev = GetElevation(h, v);
    {
      int dh;
      dh = elev - v;
      if(dh == -v){
        Zip.behavior = IN_AIR;
        Zip.state = (Zip.chBase & HFlip) ? JUMPING_LEFT : JUMPING_RIGHT;
      } else {
        if((dh >= -4) && (dh <= 4)){
          Zip.Ypos.w += dh;
          Zip.Yvel = dh << 16;
        } else {
          Zip.behavior = IN_AIR;
          Zip.state = (Zip.chBase & HFlip) ? JUMPING_LEFT : JUMPING_RIGHT;
        }
      }
    }

    /* Apply acceleration for hills */
    if(Zip.chBase & HFlip){
      h++;
      elev2 = GetElevation(h, v);
      Zip.Xvel += (elev2 - elev) << 14;
    } else {
      h--;
      elev2 = GetElevation(h, v);
      Zip.Xvel -= (elev2 - elev) << 14;
    }
  } else {
    Zip.Ypos.l += Zip.Yvel;
  }
}

static void PlayerInAir(){
  switch(Zip.state){
    case JUMPING_LEFT:
      if(currentStick1 & JOYLEFT){
        if(Zip.Xvel > -(ZIP_SPEED_LIMIT - ZIP_AIR_ACCELERATION)){
          Zip.Xvel -= ZIP_AIR_ACCELERATION;
        } else {
          Zip.Xvel = -ZIP_SPEED_LIMIT;
        }
      }
      if(currentStick1 & JOYRIGHT){
        Zip.Xvel += ZIP_AIR_ACCELERATION;
        if(Zip.Xvel > 0){
          Zip.chBase |= HFlip;
          Zip.state = JUMPING_RIGHT;
        }
      }
      break;
    case JUMPING_RIGHT:
      if(currentStick1 & JOYRIGHT){
        if(Zip.Xvel < ZIP_SPEED_LIMIT - ZIP_AIR_ACCELERATION){
          Zip.Xvel += ZIP_AIR_ACCELERATION;
        } else {
          Zip.Xvel = ZIP_SPEED_LIMIT;
        }
      }
      if(currentStick1 & JOYLEFT){
        Zip.Xvel -= ZIP_AIR_ACCELERATION;
        if(Zip.Xvel < 0){
          Zip.chBase &= ~HFlip;
          Zip.state = JUMPING_LEFT;
        }
      }
      break;
  }
  /* move him */
  Zip.Yvel += GRAVITY;
  if(Zip.Yvel > 2*ZIP_SPEED_LIMIT) Zip.Yvel = 2*ZIP_SPEED_LIMIT;
  if(Zip.Yvel < -2*ZIP_SPEED_LIMIT) Zip.Yvel = -2*ZIP_SPEED_LIMIT;
  Zip.Xpos.l += Zip.Xvel;
  Zip.Ypos.l += Zip.Yvel;

  /* Check for surface, possibly switch to H_SURFACE */
  {
    short elev;
    int h, v;
    int slope;

    if(Zip.chBase & HFlip){
      h = Zip.Xpos.w - (Zip.Frame[3] - Zip.Frame[1]);
    } else {
      h = Zip.Xpos.w + (Zip.Frame[3] - Zip.Frame[1]);
    }
    v = Zip.Ypos.w + (Zip.Frame[4] - Zip.Frame[2]);
    if(v < 0) v = 0;
    elev = GetElevation(h, v);
    if(elev){
      int dh;
      dh = elev - v;
      if((dh <= 4) && (dh >= -16)){
        Zip.Ypos.w += dh;
        Zip.Yvel = 0;
        Zip.behavior = H_SURFACE;
        Zip.state = (Zip.chBase & HFlip) ? COASTING_LEFT : COASTING_RIGHT;
      }
    }
  }
}


void OperatePlayer(void){
int newAnim = 0;
  Zip.Xvel_old = Zip.Xvel;
  Zip.Yvel_old = Zip.Yvel;
  switch(Zip.behavior){
    case H_SURFACE:
      PlayerHSurface();
      break;
    case IN_AIR:
      PlayerInAir();
      break;
    case V_SURFACE:
    case INVERTED_SURFACE:
    case INVERTED_IN_AIR:
    case DEAD:
      break;
  }
  /* stop him before the edge of the screen */
  if(Zip.Xpos.w > (11*40*8) - 32){
    Zip.Xpos.w = (11*40*8) - 32;
    Zip.Xvel = 0;
  }
  if(Zip.Xpos.w < 32){
    Zip.Xpos.w = 32;
    Zip.Xvel = 0;
  }

  /* get his next frame */
  if(!newAnim) AnimateCreature(&Zip);
}

