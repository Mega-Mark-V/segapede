#if 0
#include "defs.h"
#include "types.h"

#endif

long tanTable[] = {
  0,            /*  0 */
  4,            /*  1 */
  9,            /*  2 */
  13,           /*  3 */
  18,           /*  4 */
  22,           /*  5 */
  27,           /*  6 */
  31,           /*  7 */
  36,           /*  8 */
  41,           /*  9 */
  45,           /* 10 */
  50,           /* 11 */
  54,           /* 12 */
  59,           /* 13 */
  64,           /* 14 */
  69,           /* 15 */
  73,           /* 16 */
  78,           /* 17 */
  83,           /* 18 */
  88,           /* 19 */
  93,           /* 20 */
  98,           /* 21 */
  103,          /* 22 */
  109,          /* 23 */
  114,          /* 24 */
  119,          /* 25 */
  125,          /* 26 */
  130,          /* 27 */
  136,          /* 28 */
  142,          /* 29 */
  148,          /* 30 */
  154,          /* 31 */
  160,          /* 32 */
  166,          /* 33 */
  173,          /* 34 */
  179,          /* 35 */
  186,          /* 36 */
  193,          /* 37 */
  200,          /* 38 */
  207,          /* 39 */
  215,          /* 40 */
  223,          /* 41 */
  231,          /* 42 */
  239,          /* 43 */
  247,          /* 44 */
  256,          /* 45 */
  265,          /* 46 */
  275,          /* 47 */
  284,          /* 48 */
  294,          /* 49 */
  305,          /* 50 */
  316,          /* 51 */
  328,          /* 52 */
  340,          /* 53 */
  352,          /* 54 */
  366,          /* 55 */
  380,          /* 56 */
  394,          /* 57 */
  410,          /* 58 */
  426,          /* 59 */
  443,          /* 60 */
  462,          /* 61 */
  481,          /* 62 */
  502,          /* 63 */
  525,          /* 64 */
  549,          /* 65 */
  575,          /* 66 */
  603,          /* 67 */
  634,          /* 68 */
  667,          /* 69 */
  703,          /* 70 */
  743,          /* 71 */
  788,          /* 72 */
  837,          /* 73 */
  893,          /* 74 */
  955,          /* 75 */
  1027,         /* 76 */
  1109,         /* 77 */
  1204,         /* 78 */
  1317,         /* 79 */
  1452,         /* 80 */
  1616,         /* 81 */
  1822,         /* 82 */
  2085,         /* 83 */
  2436,         /* 84 */
  2926,         /* 85 */
  3661,         /* 86 */
  4885,         /* 87 */
  7331,         /* 88 */
  14666,        /* 89 */
  2147483647,   /* 90 */
};

static short bsearchtan(long d){
short j, k, m;

  j = 90;
  k = 0;
  for(;;){
    m = (j+k)/2;
    if(j <= k+1) break;
    if(tanTable[m] >= d){
      j = m;
    } else {
      k = m;
    }
  }
  return m;
}

short atan2(long x, long y){
short t;

  x /= 256;
  if(x < 0){
    if(y < 0){
      t = 180 + bsearchtan(y/x);
    } else {
      if(y == 0) return 180;
      t = 180 - bsearchtan(-y/x);
    }
  } else {
    if(y < 0){
      if(x == 0) return 270;
      t = 360 - bsearchtan(-y/x);
    } else {
      if(y == 0) return 0;
      if(x == 0) return 90;
      t = bsearchtan(y/x);
    }
  }
  return t;
}

long toothScores[4] = {
    MEDSCORE_1, MEDSCORE_2, HISCORE_1, HISCORE_2
};

short toothMessages[4] = {
    2, 2, 3, 3
};

long ballInHeadScores[5] = {
    0, BONUS_2, HISCORE_2, MEDSCORE_2, 0
};

#define HITS_PER_TOOTH 2

void CheckRobotnikCollision(CreaturePtr t1){
register CreaturePtr t2;
long vxc, vyc, vx1, vx2, vy1, vy2;
long dx, dy;
long adx, ady;
long ax, ay;
long dist;
long dot;
int theta;

  t2 = &things[ballCount+6];
  if(t2->type < 0) return;
  /* determine the separation of the objects */
  dx = t2->Xpos.l - t1->Xpos.l;
  adx = abs(dx);
  dy = t2->Ypos.l - t1->Ypos.l;
  ady = abs(dy);
  dist = Distance(adx, ady) >> 16;
  if(dist <= BALL_RADIUS + ROBOTNIK_RADIUS){
    vx1 = t1->Xvel - t2->Xvel;
    vy1 = t1->Yvel;
    dot = (dx>>8) * (vx1>>8) + (dy>>8) * (vy1>>8);
    if(dot <= 0) return;
    if(dist == 0) dist = 1;
    ax = (dx>>8) * (dot>>8) / (dist * dist);
    ay = (dy>>8) * (dot>>8) / (dist * dist);
    t1->Xvel -= (ax<<1) - (ax>>4);
    t1->Yvel -= (ay<<1) - (ay>>4);
    t1->behavior = 0;       /* not from flipper */
    if(!powerOff && !robotnikHit){
      robotnikHitTimer = 4;
      theta = atan2(dx, dy);
      if((theta < 20) || (theta > 160)){
        t1->Xvel -= dx / 10 + (Random >> 8);
        t1->Yvel -= dy / 10 + (Random >> 8);
        AddScore(MINSCORE);
        StartSound(56);
      } else {
        if(dy < (60 << 16)){
          /* hit back tooth */
          if(dx < 0){
            /* hit right tooth */
            if(roboParts[3].state <= HITS_PER_TOOTH){
              robotnikHitTimer = 15;
              robotnikHit = 1;
              if(roboParts[3].state < HITS_PER_TOOTH){
                roboParts[3].chBase |= HFlip;
                BeginAnimation(&roboParts[3], OuterToothSpin);
                AddScore(LOWSCORE_1);
                StartMessage(1);
                StartSound(42);
                robotnikHitTimer = 15;
              } else {
                BeginAnimation(&roboParts[3], OuterToothBreak);
                StartExplosion(roboParts[3].screenX+128, roboParts[3].screenY+128);
                AddScore(toothScores[teethDestroyed]);
                StartMessage(toothMessages[teethDestroyed]);
                teethDestroyed++;
                StartSound(43);
                StartSound(106);
                StartSound(44);
                robotnikHitTimer = 25;
              }
            } else {
              t1->behavior = 1;     /* ball in head */
              t1->Xpos.l = -0x02000000;  /* get it off screen */
              t1->control_timer = 45;  /* stay for 0.75 seconds */
              t2->behavior = 6;
              t2->control_timer = 45;
              AddScore(ballInHeadScores[teethDestroyed]);
              StartMessage(5);
              StartSound(45);
            }
            roboParts[3].state++;
          } else {
            /* hit left tooth */
            if(roboParts[2].state <= HITS_PER_TOOTH){
              robotnikHit = 1;
              if(roboParts[2].state < HITS_PER_TOOTH){
                roboParts[2].chBase &= ~HFlip;
                BeginAnimation(&roboParts[2], OuterToothSpin);
                AddScore(LOWSCORE_1);
                StartMessage(1);
                StartSound(42);
                robotnikHitTimer = 15;
              } else {
                BeginAnimation(&roboParts[2], OuterToothBreak);
                StartExplosion(roboParts[2].screenX+128, roboParts[2].screenY+128);
                AddScore(toothScores[teethDestroyed]);
                StartMessage(toothMessages[teethDestroyed]);
                teethDestroyed++;
                StartSound(43);
                StartSound(106);
                StartSound(44);
                robotnikHitTimer = 25;
              }
            } else {
              t1->behavior = 4;     /* ball in head */
              t1->Xpos.l = -0x02000000;  /* get it off screen */
              t1->control_timer = 45;  /* stay for 0.75 seconds */
              t2->behavior = 4;
              t2->control_timer = 45;
              AddScore(ballInHeadScores[teethDestroyed]);
              StartMessage(5);
              StartSound(45);
            }
            roboParts[2].state++;
          }
        } else {
          /* hit front tooth */
          if(dx < 0){
            /* hit right tooth */
            if(roboParts[1].state <= HITS_PER_TOOTH){
              robotnikHit = 1;
              if(roboParts[1].state < HITS_PER_TOOTH){
                roboParts[1].chBase |= HFlip;
                BeginAnimation(&roboParts[1], InnerToothSpin);
                AddScore(LOWSCORE_1);
                StartMessage(1);
                StartSound(42);
                robotnikHitTimer = 15;
              } else {
                BeginAnimation(&roboParts[1], InnerToothBreak);
                StartExplosion(roboParts[1].screenX+128, roboParts[1].screenY+128);
                AddScore(toothScores[teethDestroyed]);
                StartMessage(toothMessages[teethDestroyed]);
                teethDestroyed++;
                StartSound(43);
                StartSound(106);
                StartSound(44);
                robotnikHitTimer = 25;
              }
            } else {
              t1->behavior = 2;     /* ball in head */
              t1->Xpos.l = -0x02000000;  /* get it off screen */
              t1->control_timer = 45;  /* stay for 0.75 seconds */
              t2->behavior = 6;
              t2->control_timer = 45;
              AddScore(ballInHeadScores[teethDestroyed]);
              StartMessage(5);
              StartSound(45);
            }
            roboParts[1].state++;
          } else {
            /* hit left tooth */
            if(roboParts[0].state <= HITS_PER_TOOTH){
              robotnikHit = 1;
              if(roboParts[0].state < HITS_PER_TOOTH){
                roboParts[0].chBase &= ~HFlip;
                BeginAnimation(&roboParts[0], InnerToothSpin);
                AddScore(LOWSCORE_1);
                StartMessage(1);
                StartSound(42);
                robotnikHitTimer = 15;
              } else {
                BeginAnimation(&roboParts[0], InnerToothBreak);
                StartExplosion(roboParts[0].screenX+128, roboParts[0].screenY+128);
                AddScore(toothScores[teethDestroyed]);
                StartMessage(toothMessages[teethDestroyed]);
                teethDestroyed++;
                StartSound(43);
                StartSound(106);
                StartSound(44);
                robotnikHitTimer = 25;
              }
            } else {
              t1->behavior = 3;     /* ball in head */
              t1->Xpos.l = -0x02000000;  /* get it off screen */
              t1->control_timer = 45;  /* stay for 0.75 seconds */
              t2->behavior = 4;
              t2->control_timer = 45;
              AddScore(ballInHeadScores[teethDestroyed]);
              StartMessage(5);
              StartSound(45);
            }
            roboParts[0].state++;
          }
        }
      }
      if(teethDestroyed >= 4){
        t1->type = -1;
      }
    }
  }
}

void CheckDropCollision(CreaturePtr t1){
short temp;

  if(!powerOff){
    if(t1->Ypos.w > worldLength - BALL_RADIUS - 5){
      temp = t1->Xpos.w - 146;
      if(temp < 0) temp = -temp;
      if(temp < 35){
        if(!(targetsDown & 1)){
          BeginAnimation(&roboParts[5], DropTargetFall);
          StartSound(92);
          AddScore(MINSCORE);
          targetsDown |= 1;
        }
      }
      if((temp > 65) && (temp < 135)){
        if(t1->Xpos.w > 146){
          if(!(targetsDown & 2)){
            BeginAnimation(&roboParts[9], DropTargetFall);
            StartSound(92);
            AddScore(MINSCORE);
            targetsDown |= 2;
          }
        } else {
          if(!(targetsDown & 4)){
            BeginAnimation(&roboParts[7], DropTargetFall);
            StartSound(92);
            AddScore(MINSCORE);
            targetsDown |= 4;
          }
        }
      }
    }
  }
}

void CheckEggCollision(CreaturePtr t1){
register CreaturePtr t2;
long vxc, vyc, vx1, vx2, vy1, vy2;
long dx, dy;
long adx, ady;
long ax, ay;
long dist;
long dot;
int theta;
int i;

  t2 = &things[ballCount+6];
  if(t2->type < 0) return;
  /* determine the separation of the objects */
  dx = t2->Xpos.l - t1->Xpos.l;
  adx = abs(dx);
  dy = t2->Ypos.l - t1->Ypos.l;
  ady = abs(dy);
  dist = Distance(adx, ady) >> 16;
  if(dist <= BALL_RADIUS + EGG_RADIUS){
    vx1 = t1->Xvel;
    vy1 = t1->Yvel;
    dot = (dx>>8) * (vx1>>8) + (dy>>8) * (vy1>>8);
    if(dot <= 0) return;
    if(dist == 0) dist = 1;
    ax = (dx>>8) * (dot>>8) / (dist * dist);
    ay = (dy>>8) * (dot>>8) / (dist * dist);
    t1->Xvel -= (ax<<1) - (ax>>4);
    t1->Yvel -= (ay<<1) - (ay>>4);
    if(!powerOff){
      theta = atan2(dx, dy);
      if((theta < 20) || (theta > 160)){
        t1->Xvel -= dx / 8 + (Random >> 8);
        t1->Yvel -= dy / 8 + (Random >> 8);
      } else {
        t1->Xvel -= dx / 20 + (Random >> 8);
        t1->Yvel -= dy / 20 + (Random >> 8);
      }
      StartSound(53);
      switch(++t2->damage){
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
          roboParts[5].anim_next = EggShard1;
          roboParts[5].anim_time = 0;
          AddScore(LOWSCORE_2);
          break;
        case 10:
          roboParts[5].anim_next = Cage1;
          roboParts[5].anim_time = 0;
          AddScore(LOWSCORE_2);
          StartSound(106);
          break;
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
          roboParts[4].anim_next = EggShard2;
          roboParts[4].anim_time = 0;
          AddScore(LOWSCORE_2);
          break;
        case 16:
          roboParts[4].anim_next = Cage2;
          roboParts[4].anim_time = 0;
          AddScore(LOWSCORE_2);
          StartSound(106);
          break;
        case 17:
        case 18:
        case 19:
          roboParts[3].anim_next = EggShard3;
          roboParts[3].anim_time = 0;
          AddScore(LOWSCORE_2);
          break;
        case 20:
          roboParts[3].anim_next = Cage3;
          roboParts[3].anim_time = 0;
          AddScore(LOWSCORE_2);
          StartSound(106);
          break;
        case 21:
        case 22:
        case 23:
          roboParts[2].anim_next = EggShard4;
          roboParts[2].anim_time = 0;
          AddScore(MEDSCORE_2);
          break;
        case 24:
          roboParts[2].anim_next = Cage4;
          roboParts[2].anim_time = 0;
          AddScore(MEDSCORE_2);
          StartSound(106);
          break;
        case 25:
        case 26:
          roboParts[1].anim_next = EggShard5;
          roboParts[1].anim_time = 0;
          AddScore(MEDSCORE_2);
          break;
        case 27:
          roboParts[1].anim_next = Cage5;
          roboParts[1].anim_time = 0;
          AddScore(HISCORE_2);
          StartMessage(34);
          StartSound(106);
          break;
        case 34:
          roboParts[1].next = NULL;
          for(i = 0; i < 8; i++){
            things[ballCount+8+i].type = -1;
          }
          t1->type = -1;        /* ][][][][][][ 9/8/93 kr */
          BeginAnimation(&roboParts[1], EggWrecked);
          AddScore(JACKPOT);
          StartMessage(35);
          StartSound(107);
          StartSound(65);
          StartSound(65);
          break;
        default:
          break;
      }
    }
    t1->behavior = 0;       /* not from flipper */
  }
}

short cluckHitScore[] = {
    MINSCORE,
    LOWSCORE_1,
    LOWSCORE_2,
    LOWSCORE_2,
    MEDSCORE_1,
    MEDSCORE_2,
    MEDSCORE_2,
    HISCORE_1,
};

short cluckDestroyedScore[] = {
    MEDSCORE_2,
    MEDSCORE_2,
    HISCORE_1,
    HISCORE_1,
    HISCORE_2,
    HISCORE_2,
    BONUS_1,
    BONUS_2,
};

short cluckDestroyedMessage[] = {
    39,39,40,40,41,41,42,42
};

void CheckCluckCollisions(CreaturePtr t1){
register CreaturePtr t2;
long vxc, vyc, vx1, vx2, vy1, vy2;
long dx, dy;
long adx, ady;
long ax, ay;
long dist;
long dot;
int i;

  for(t2 = &things[ballCount+8], i = 0; i < CLUCK_COUNT; i++, t2++){
    if(t2->type < 0) continue;
    /* determine the separation of the objects */
    dx = t2->Xpos.l - t1->Xpos.l;
    adx = abs(dx);
    dy = t2->Ypos.l - t1->Ypos.l;
    ady = abs(dy);
    dist = Distance(adx, ady) >> 16;
    if(dist <= BALL_RADIUS + 10){
      vx1 = t1->Xvel - t2->Xvel;
      vy1 = t1->Yvel - t2->Yvel;
      dot = (dx>>8) * (vx1>>8) + (dy>>8) * (vy1>>8);
      if(dot <= 0) continue;
      if(dist == 0) dist = 1;
      ax = (dx>>8) * (dot>>8) / (dist * dist);
      ay = (dy>>8) * (dot>>8) / (dist * dist);
      t1->Xvel -= (ax<<1) - (ax>>4);
      t1->Yvel -= (ay<<1) - (ay>>4);
      if(!powerOff){
        t1->Xvel -= dx / 8 + (Random >> 8);
        t1->Yvel -= dy / 8 + (Random >> 8);
        if(++t2->damage == 4){
          StartExplosion(t2->dl->screenX+128, t2->dl->screenY+128-15);
          t2->type = -1;
          StartSound(120);
          StartSound(106);
          AddScore(cluckDestroyedScore[clucksDestroyed]);
          StartMessage(cluckDestroyedMessage[clucksDestroyed++]);
          if(clucksDestroyed == 8) gemsstopsong(97);
        } else {
          StartSmallExplosion(t2->dl->screenX+128, t2->dl->screenY+128-15);
          StartSound(117);
          AddScore(cluckHitScore[clucksDestroyed]);
        }
      }
      t1->behavior = 0;       /* not from flipper */
    }
  }
}

AnimStatePtr ChamberCreatures[7] = {
    Tails,
    Saru,
    Flicky,
    Muttski,
    Rotor,
    Sally,
    Bunny,
};

short ChamberMessages[7] = {
    10,3,5,14,15,14,15
};

short ChamberScores[7] = {
    MEDSCORE_2,
    MEDSCORE_2,
    MEDSCORE_2,
    HISCORE_1,
    HISCORE_1,
    HISCORE_1,
    HISCORE_1,
};

void CheckChamberCollision(CreaturePtr t1){
register CreaturePtr t2;
long vxc, vyc, vx1, vx2, vy1, vy2;
long dx, dy;
long adx, ady;
long ax, ay;
long dist;
long dot;
int i;

  for(t2 = &things[ballCount+8], i = 0; i < 7; i++, t2++){
    if(t2->type < 0) continue;
    if(t2->behavior) continue;
    /* determine the separation of the objects */
    dx = t2->Xpos.l - t1->Xpos.l;
    adx = abs(dx);
    dy = t2->Ypos.l - t1->Ypos.l;
    ady = abs(dy);
    dist = Distance(adx, ady) >> 16;
    if(dist <= BALL_RADIUS + 10){
      vx1 = t1->Xvel;
      vy1 = t1->Yvel;
      dot = (dx>>8) * (vx1>>8) + (dy>>8) * (vy1>>8);
      if(dot <= 0) continue;
      if(dist == 0) dist = 1;
      ax = (dx>>8) * (dot>>8) / (dist * dist);
      ay = (dy>>8) * (dot>>8) / (dist * dist);
      t1->Xvel -= (ax<<1) - (ax>>4);
      t1->Yvel -= (ay<<1) - (ay>>4);
      t1->behavior = 0;       /* not from flipper */
      if(!powerOff){
        t1->Xvel -= dx / 10 + (Random >> 8);
        t1->Yvel -= dy / 10 + (Random >> 8);
        AddScore(MINSCORE);
        StartSound(121);
#if 0
        switch(++t2->dl->state){
          case 1:
            BeginAnimation(t2->dl, ChamberFirstHit);
            break;
          case 2:
            BeginAnimation(t2->dl, ChamberSecondHit);
            break;
          case 3:
            BeginAnimation(t2->dl, ChamberThirdHit);
            break;
          case 4:
            BeginAnimation(t2->dl, ChamberFourthHit);
            break;
          case 5:
            StartExplosion(t2->dl->screenX+128, t2->dl->screenY+128 - 15);
            BeginAnimation(t2->dl, ChamberCreatures[i]);
            holdEnd++;
            StartSound(106);
            StartSound(65);
            StartMessage(ChamberMessages[chambersDestroyed]);
            AddScore(ChamberScores[chambersDestroyed]);
            t2->behavior = i+1;
            break;
          default:
            break;
        }
#else
        switch(++t2->dl->state){
          case 1:
            BeginAnimation(t2->dl, ChamberFirstHit);
            break;
          case 2:
            BeginAnimation(t2->dl, ChamberThirdHit);
            break;
          case 3:
            StartExplosion(t2->dl->screenX+128, t2->dl->screenY+128 - 15);
            BeginAnimation(t2->dl, ChamberCreatures[i]);
            holdEnd++;
            StartSound(106);
            StartSound(65);
            StartMessage(ChamberMessages[chambersDestroyed]);
            AddScore(ChamberScores[chambersDestroyed]);
            t2->behavior = i+1;
            break;
          default:
            break;
        }
#endif
      }
    }
  }
}

void CheckThumperCollision(CreaturePtr t1){
register CreaturePtr t2;
long vxc, vyc, vx1, vx2, vy1, vy2;
long dx, dy;
long adx, ady;
long ax, ay;
long dist;
long dot;
int i;
int theta;

  for(t2 = &things[ballCount+8], i = 0; i < 2; i++, t2++){
    if(t2->type < 0) continue;
    /* determine the separation of the objects */
    dx = t2->Xpos.l - t1->Xpos.l;
    adx = abs(dx);
    dy = t2->Ypos.l - t1->Ypos.l;
    ady = abs(dy);
    dist = Distance(adx, ady) >> 16;
    if(dist <= BALL_RADIUS + 32){
      vx1 = t1->Xvel;
      vy1 = t1->Yvel;
      dot = (dx>>8) * (vx1>>8) + (dy>>8) * (vy1>>8);
      if(dot <= 0) continue;
      if(dist == 0) dist = 1;
      ax = (dx>>8) * (dot>>8) / (dist * dist);
      ay = (dy>>8) * (dot>>8) / (dist * dist);
      t1->Xvel -= (ax<<1) - (ax>>4);
      t1->Yvel -= (ay<<1) - (ay>>4);
      if(!powerOff){
        theta = atan2(dx, dy);
        if((theta < 20) || (theta > 160)){
          t1->Xvel -= dx / 10 + (Random >> 8);
          t1->Yvel -= dy / 10 + (Random >> 8);
        } else {
          t1->Xvel -= dx / 20 + (Random >> 8);
          t1->Yvel -= dy / 20 + (Random >> 8);
        }
        if(++t2->damage % 3){
          AddScore(LOWSCORE_2);
/*        StartMessage(5); */
        } else {
          AddScore(MEDSCORE_1);
/*        StartMessage(2); */
        }
        StartSound(52);
        BeginAnimation(t2->dl, Chicken);
        StartSmallExplosion(t1->dl->screenX+128, t1->dl->screenY+128 - 7);
      }
      t1->behavior = 0;       /* not from flipper */
    }
  }
}

void CheckSlidingThumperCollision(CreaturePtr t1){
register CreaturePtr t2;
long vxc, vyc, vx1, vx2, vy1, vy2;
long dx, dy;
long adx, ady;
long ax, ay;
long dist;
long dot;
int i;
int theta;

  t2 = &things[ballCount+6];
  if(t2->type < 0) return;
  /* determine the separation of the objects */
  dx = t2->Xpos.l - t1->Xpos.l;
  adx = abs(dx);
  dy = t2->Ypos.l - t1->Ypos.l;
  ady = abs(dy);
  dist = Distance(adx, ady) >> 16;
  if(dist <= BALL_RADIUS + 38){
    vx1 = t1->Xvel;
    vy1 = t1->Yvel;
    dot = (dx>>8) * (vx1>>8) + (dy>>8) * (vy1>>8);
    if(dot <= 0) return;
    if(dist == 0) dist = 1;
    ax = (dx>>8) * (dot>>8) / (dist * dist);
    ay = (dy>>8) * (dot>>8) / (dist * dist);
    t1->Xvel -= (ax<<1) - (ax>>4);
    t1->Yvel -= (ay<<1) - (ay>>4);
    if(!powerOff){
      theta = atan2(dx, dy);
      if((theta < 20) || (theta > 160)){
        t1->Xvel -= dx / 10 + (Random >> 8);
        t1->Yvel -= dy / 10 + (Random >> 8);
      } else {
        t1->Xvel -= dx / 20 + (Random >> 8);
        t1->Yvel -= dy / 20 + (Random >> 8);
      }
      StartSmallExplosion(t1->dl->screenX+128, t1->dl->screenY+128 - 7);
      if(t2->damage >= 47){
        t2->damage++;
        BeginAnimation(t2->dl, CrabDie);
        AddScore(JACKPOT);
        StartSound(120);
        StartMessage(4);
        for(i = 0; i < ballCount; i++){
          things[i].type = -1;
        }
        ballsRemaining = 0;
      } else {
        if(t1->behavior != 0){
          if(t2->damage < 45){
            t2->damage += 3;
            BeginAnimation(t2->dl, CrabHit);
          } else {
            t2->damage = 47;
            BeginAnimation(t2->dl, CrabHitNoshield);
            StartSound(106);
            StartSound(65);
          }
          StartMessage(15);
        } else {
          t2->damage++;
          if(t2->damage == 47){
            BeginAnimation(t2->dl, CrabFlinchNoshield);
            StartSound(106);
            StartSound(65);
          } else {
            BeginAnimation(t2->dl, CrabFlinch);
          }
        }
        StartSound(53);
        AddScore(HISCORE_1);
      }
    }
    t1->behavior = 0;       /* not from flipper */
  }
}

void CheckSaucerCollision(CreaturePtr t1){
register CreaturePtr t2;
long vxc, vyc, vx1, vx2, vy1, vy2;
long dx, dy;
long adx, ady;
long ax, ay;
long dist;
long dot;
int i;
short theta;

  t2 = &things[ballCount+6];
  if(t2->type < 0) return;
  /* determine the separation of the objects */
  dx = t2->Xpos.l - t1->Xpos.l;
  adx = abs(dx);
  dy = t2->Ypos.l - t1->Ypos.l;
  ady = abs(dy);
  dist = Distance(adx, ady) >> 16;
  if(dist <= BALL_RADIUS + 38){
    vx1 = t1->Xvel - t2->Xvel;
    vy1 = t1->Yvel - t2->Yvel;
    dot = (dx>>8) * (vx1>>8) + (dy>>8) * (vy1>>8);
    if(dot <= 0) return;
    if(dist == 0) dist = 1;
    ax = (dx>>8) * (dot>>8) / (dist * dist);
    ay = (dy>>8) * (dot>>8) / (dist * dist);
    t1->Xvel -= (ax<<1) - (ax>>4);
    t1->Yvel -= (ay<<1) - (ay>>4);
    StartSound(53);

    if(!powerOff){
      theta = atan2(dx, dy);
      if((theta < 20) || (theta > 160)){
        t1->Xvel -= dx / 10 + (Random >> 8);
        t1->Yvel -= dy / 10 + (Random >> 8);
      }
      if(t1->behavior == 7){
        t2->damage += 3;
        switch(t2->behavior){
          case 0:
          case 1:
          case 4:
          case 6:
            BeginAnimation(&roboParts[7], ShipSpinLeft);
            BeginAnimation(&roboParts[8], PilotSpin);
            t2->control_timer = 60;
            t2->behavior = 5;
            StartSound(44);
            break;
          case 2:
          case 3:
          case 7:
            BeginAnimation(&roboParts[7], ShipSpinRight);
            BeginAnimation(&roboParts[8], PilotSpin);
            t2->control_timer = 60;
            t2->behavior = 5;
            StartSound(118);
            break;
          case 5:
            break;
        }
      } else {
        t2->damage++;
      }
      AddScore(LOWSCORE_2);
    }
    t1->behavior = 0;       /* not from flipper */
  }
}

void CheckOtherCollisions(CreaturePtr t){
  switch(bonus_round){
    case 0:
      CheckRobotnikCollision(t);
      CheckDropCollision(t);
      break;
    case 1:
      CheckThumperCollision(t);
      CheckSlidingThumperCollision(t);
      break;
    case 2:
      CheckChamberCollision(t);
      CheckSaucerCollision(t);
      break;
    case 3:
      CheckEggCollision(t);
      CheckCluckCollisions(t);
      break;
  }
}

typedef struct {
  long x;
  long y;
}Point, *PointPtr;

Point pointList[] = {
    { 33<<16,  93<<16},
    { 61<<16,  47<<16},
    { 61<<16,  42<<16},
    { 28<<16,  57<<16},
    { 28<<16,  90<<16},
    { 22<<16, 137<<16},
    { 70<<16,  12<<16},
    {259<<16,  93<<16},
    {231<<16,  47<<16},
    {231<<16,  42<<16},
    {264<<16,  57<<16},
    {264<<16,  90<<16},

    {269<<16, 137<<16},
    {222<<16,  12<<16},
};

typedef struct {
  long x1;
  long y1;
  long x2;
  long y2;
  long lx;
  long ly;
  short active;     /* is it a live bumper? */
}Line, *LinePtr;

Line lineList[] = {
    { 70<<16,  12<<16,  70<<16,   0<<16, 0x00000000, 0xffff0000, 0},
    {  0<<16,  41<<16,  70<<16,  12<<16, 0x0000ec82, 0xffff9e05, 0},
    {  0<<16, 156<<16,  22<<16, 137<<16, 0x0000c1bf, 0xffff58ad, 0}, /**/
    { 22<<16, 137<<16,   0<<16, 112<<16, 0xffff56e2, 0xffff3fd2, 0},
    { 33<<16,  93<<16,  61<<16,  47<<16, 0x0000851b, 0xffff2554, 1},
    { 61<<16,  47<<16,  61<<16,  42<<16, 0x00000000, 0xffff0000, 0},
    { 61<<16,  42<<16,  28<<16,  57<<16, 0xffff16f3, 0x000069ef, 0},
    { 28<<16,  57<<16,  28<<16,  90<<16, 0x00000000, 0x00010000, 0},
    { 28<<16,  90<<16,  33<<16,  93<<16, 0x0000db84, 0x000083b5, 0},
    {269<<16, 137<<16, 292<<16, 156<<16, 0x0000c6af, 0x0000a16f, 0}, /**/
    {292<<16, 112<<16, 269<<16, 137<<16, 0xffff52ad, 0x0000ba6b, 0},
    {222<<16,   0<<16, 222<<16,  12<<16, 0x00000000, 0x00010000, 0},
    {222<<16,  12<<16, 292<<16,  41<<16, 0x0000ec4f, 0x00006276, 0},
    {259<<16,  93<<16, 264<<16,  90<<16, 0x0000db84, 0xffff7c4b, 0},
    {264<<16,  90<<16, 264<<16,  57<<16, 0x00000000, 0xffff0000, 0},
    {264<<16,  57<<16, 231<<16,  42<<16, 0xffff16f3, 0xffff9611, 0},
    {231<<16,  42<<16, 231<<16,  47<<16, 0x00000000, 0x00010000, 0},
    {231<<16,  47<<16, 259<<16,  93<<16, 0x0000851b, 0x0000daac, 1},
};

#if 1
Line flipperLineList[2][14] = {
 {
    { 79<<16,  10<<16, 132<<16,  -3<<16, 63576, -15892, 0},
    { 79<<16,  10<<16, 132<<16,  -3<<16, 63576, -15892, 0},
    { 77<<16,  10<<16, 135<<16,   4<<16, 65208, -6521, 1},
    { 77<<16,  10<<16, 135<<16,  10<<16, 65536, 0, 1},
    { 77<<16,  10<<16, 137<<16,  16<<16, 65208, 6521, 1},
    { 76<<16,  10<<16, 136<<16,  21<<16, 64501, 11613, 1},
    { 76<<16,  10<<16, 134<<16,  26<<16, 63255, 17131, 1},
    { 75<<16,  10<<16, 131<<16,  30<<16, 61630, 22289, 1},
    { 74<<16,  10<<16, 128<<16,  35<<16, 59389, 27715, 1},
    { 74<<16,  10<<16, 126<<16,  39<<16, 57226, 31942, 1},
    { 74<<16,   9<<16, 120<<16,  41<<16, 53687, 37585, 1},
    { 74<<16,   9<<16, 120<<16,  41<<16, 53687, 37585, 1},
    { 74<<16,   9<<16, 118<<16,  40<<16, 53687, 37585, 0},
    { 74<<16,   9<<16, 118<<16,  40<<16, 53687, 37585, 0},
 },
 {
    {160<<16,  -3<<16, 213<<16,  10<<16, 63576, 15892, 0},
    {160<<16,  -3<<16, 213<<16,  10<<16, 63576, 15892, 0},
    {158<<16,   4<<16, 215<<16,  10<<16, 65208, 6521, 1},
    {158<<16,  10<<16, 215<<16,  10<<16, 65536, 0, 1},
    {155<<16,  16<<16, 215<<16,  10<<16, 65208, -6521, 1},
    {156<<16,  21<<16, 215<<16,  10<<16, 64455, -11842, 1},
    {158<<16,  26<<16, 216<<16,  10<<16, 63255, -17131, 1},
    {161<<16,  30<<16, 217<<16,  10<<16, 61630, -22289, 1},
    {164<<16,  35<<16, 218<<16,  10<<16, 59389, -27715, 1},
    {166<<16,  39<<16, 218<<16,  10<<16, 57226, -31942, 1},
    {172<<16,  41<<16, 218<<16,   9<<16, 53687, -37585, 1},
    {172<<16,  41<<16, 218<<16,   9<<16, 53687, -37585, 1},
    {174<<16,  40<<16, 218<<16,   9<<16, 53687, -37585, 0},
    {174<<16,  40<<16, 218<<16,   9<<16, 53687, -37585, 0},
 }
};

Point flipperPointList[2][7] = {
  {
    {125<<16, -6<<16},
    {128<<16,  2<<16},
    {131<<16, 11<<16},
    {128<<16, 21<<16},
    {124<<16, 29<<16},
    {117<<16, 35<<16},
    {115<<16, 34<<16},
  },
  {
    {167<<16, -6<<16},
    {164<<16,  2<<16},
    {161<<16, 11<<16},
    {164<<16, 21<<16},
    {168<<16, 29<<16},
    {175<<16, 35<<16},
    {177<<16, 34<<16},
  }
};
#else
Line flipperLineList[2][14] = {
 {
    { 79<<16,  10<<16, 135<<16,  -4<<16, 63576, -15892, 0},
    { 79<<16,  10<<16, 135<<16,  -4<<16, 63576, -15892, 0},
    { 77<<16,  10<<16, 141<<16,   3<<16, 65208, -6521, 1},
    { 77<<16,  10<<16, 141<<16,  10<<16, 65536, 0, 1},
    { 77<<16,  10<<16, 143<<16,  17<<16, 65208, 6521, 1},
    { 76<<16,  10<<16, 142<<16,  22<<16, 64501, 11613, 1},
    { 76<<16,  10<<16, 140<<16,  28<<16, 63255, 17131, 1},
    { 75<<16,  10<<16, 137<<16,  32<<16, 61630, 22289, 1},
    { 74<<16,  10<<16, 133<<16,  38<<16, 59389, 27715, 1},
    { 74<<16,  10<<16, 131<<16,  42<<16, 57226, 31942, 1},
    { 74<<16,   9<<16, 125<<16,  44<<16, 53687, 37585, 1},
    { 74<<16,   9<<16, 125<<16,  44<<16, 53687, 37585, 1},
    { 74<<16,   9<<16, 122<<16,  43<<16, 53687, 37585, 0},
    { 74<<16,   9<<16, 122<<16,  43<<16, 53687, 37585, 0},
 },
 {
    {157<<16,  -4<<16, 213<<16,  10<<16, 63576, 15892, 0},
    {157<<16,  -4<<16, 213<<16,  10<<16, 63576, 15892, 0},
    {152<<16,   3<<16, 215<<16,  10<<16, 65208, 6521, 1},
    {152<<16,  10<<16, 215<<16,  10<<16, 65536, 0, 1},
    {149<<16,  17<<16, 215<<16,  10<<16, 65208, -6521, 1},
    {150<<16,  22<<16, 215<<16,  10<<16, 64455, -11842, 1},
    {152<<16,  28<<16, 216<<16,  10<<16, 63255, -17131, 1},
    {155<<16,  32<<16, 217<<16,  10<<16, 61630, -22289, 1},
    {159<<16,  38<<16, 218<<16,  10<<16, 59389, -27715, 1},
    {161<<16,  42<<16, 218<<16,  10<<16, 57226, -31942, 1},
    {167<<16,  44<<16, 218<<16,   9<<16, 53687, -37585, 1},
    {167<<16,  44<<16, 218<<16,   9<<16, 53687, -37585, 1},
    {170<<16,  43<<16, 218<<16,   9<<16, 53687, -37585, 0},
    {170<<16,  43<<16, 218<<16,   9<<16, 53687, -37585, 0},
 }
};

Point flipperPointList[2][7] = {
  {
    {128<<16, -7<<16},
    {134<<16,  1<<16},
    {137<<16, 11<<16},
    {134<<16, 23<<16},
    {129<<16, 32<<16},
    {122<<16, 38<<16},
    {119<<16, 37<<16},
  },
  {
    {167<<16, -7<<16},
    {164<<16,  1<<16},
    {161<<16, 11<<16},
    {164<<16, 23<<16},
    {168<<16, 32<<16},
    {175<<16, 38<<16},
    {177<<16, 37<<16},
  }
};

#endif

Point flipperBaseList[2] = {
    { 77<<16,  4<<16},
    {215<<16,  4<<16},
};

void CheckPlayfieldCollisions(CreaturePtr c){
int i, j, k;
long dx, dy;
long adx, ady;
long ax, ay;
long cross;
long dist;
long dot;
long d1, d2;
int lineCollision = 0;

  /* first, some preliminary screening */
  if(c->Ypos.w > 170) return;
  if(c->Ypos.w < 35){
    /* check for collision with flippers */
    /* first, do side */
    for(k = 0; k < 2; k++){
/*    for(i = 0; i < 2; i++){ */
        i = ( c->Xpos.w > 292/2);
        j = (i == 0) ? leftFlipperPhase : rightFlipperPhase;
        j *= 2;
        j += k;
        /* if the magnitude of the cross product is greater than the ball's radius, there is no collision */
        dx = flipperLineList[i][j].x1 - c->Xpos.l;
        dy = flipperLineList[i][j].y1 - c->Ypos.l;
        cross = (dx >> 8) * (flipperLineList[i][j].ly >> 8) - (flipperLineList[i][j].lx >> 8) * (dy >> 8);
        if(cross >= (BALL_RADIUS << 16)) continue;
        if(cross < -(BALL_RADIUS << 16)) continue;

        /* if the dot product is positive, there is no collision */
        d1 = (dx>>8) * (flipperLineList[i][j].lx>>8) + (dy>>8) * (flipperLineList[i][j].ly>>8);
        if(d1 >= 0) continue;

        /* if this dot product is negative, there is no collision */
        dx = flipperLineList[i][j].x2 - c->Xpos.l;
        dy = flipperLineList[i][j].y2 - c->Ypos.l;
        d2 = (dx>>8) * (flipperLineList[i][j].lx>>8) + (dy>>8) * (flipperLineList[i][j].ly>>8);
        if(d2 <= 0) continue;

        /* the velocity component perpendicular to the line is (n.v)n */
        dot = (c->Xvel>>8) * (flipperLineList[i][j].ly>>8) + (c->Yvel>>8) * (-flipperLineList[i][j].lx>>8);
        c->Xvel -= 7 * (dot >> 8) * (flipperLineList[i][j].ly>>8) / 4;
        c->Yvel -= 7 * (dot >> 8) * (-flipperLineList[i][j].lx>>8) / 4;

        /* if the line is active, add the bumper acceleration vector */
        if(((i == 0) && (leftFlipperCounter < 9)) || ((i != 0) && (rightFlipperCounter < 9))){
          if(flipperLineList[i][j].active){
            c->behavior = 7;       /* shot from flipper */
            if(i == 0){
              c->Xvel += (d1 >> 17) * flipperLineList[0][j].ly + (Random >> 4);
              c->Yvel -= (d1 >> 17) * flipperLineList[0][j].lx + (Random >> 4);
            } else {
              c->Xvel -= (d2 >> 17) * flipperLineList[1][j].ly + (Random >> 4);
              c->Yvel += (d2 >> 17) * flipperLineList[1][j].lx + (Random >> 4);
            }
          }
        }

        /* if we're leaking into the line, move it back out */
        if(cross < ((BALL_RADIUS - 1) << 16)){
          c->Xpos.l -= flipperLineList[i][j].ly;
          c->Ypos.l += flipperLineList[i][j].lx;
        }
/*    } */
      if(j == 0) break;
    }
    /* then, do big end */
/*  for(i = 0; i < 2; i++){ */
        i = ( c->Xpos.w < 292/2);
      /* determine the separation of the objects */
      dx = flipperBaseList[i].x - c->Xpos.l;
      dy = flipperBaseList[i].y - c->Ypos.l;
      adx = abs(dx);
      ady = abs(dy);
      dist = Distance(adx, ady) >> 16;
      if(dist <= BALL_RADIUS + 6){
        dot = (dx>>8) * (c->Xvel>>8) + (dy>>8) * (c->Yvel>>8);
        if(dot >= 0) {
          if(dist == 0) dist = 1;
          ax = (dx>>8) * (dot>>8) / (dist * dist);
          ay = (dy>>8) * (dot>>8) / (dist * dist);
          c->Xvel -= (ax<<1)-(ax>>4) + (Random >> 8);
          c->Yvel -= (ay<<1)-(ay>>4) + (Random >> 8);
          c->behavior = 0;       /* not from flipper */
        }
      }
/*  } */
    /* finally, do little end */
/*  for(i = 0; i < 2; i++){ */
        i = ( c->Xpos.w < 292/2);
      j = (i == 0) ? leftFlipperPhase : rightFlipperPhase;
      /* determine the separation of the objects */
      dx = flipperPointList[i][j].x - c->Xpos.l;
      dy = flipperPointList[i][j].y - c->Ypos.l;
      adx = abs(dx);
      ady = abs(dy);
      dist = Distance(adx, ady) >> 16;
      if(dist <= BALL_RADIUS + 4){
        dot = (dx>>8) * (c->Xvel>>8) + (dy>>8) * (c->Yvel>>8);
        if(dot >= 0) {
          if(dist == 0) dist = 1;
          ax = (dx>>8) * (dot>>8) / (dist * dist);
          ay = (dy>>8) * (dot>>8) / (dist * dist);
          c->Xvel -= (ax<<1)-(ax>>4) + (Random >> 6);
          c->Yvel -= (ay<<1)-(ay>>4) + (Random >> 6);
          c->behavior = 7;       /* not from flipper */
        }
      }
/*  } */
  }
  if((c->Xpos.w > 82) && (c->Xpos.w < 210)) return;

#if TIMING
  *(VLONG *)0xc00004 = 0xc0000000;
  *(VWORD *)0xc00000 = 0xff0;
#endif
  /* first check for collision with the straight parts */
        i = ( c->Xpos.w < 292/2) ? 0 : (sizeof(lineList)/sizeof(Line))/2;
        j = ( c->Xpos.w < 292/2) ? (sizeof(lineList)/sizeof(Line))/2 : sizeof(lineList)/sizeof(Line);

  for( /* i = 0 */ ;/* i < sizeof(lineList)/sizeof(Line) */ i < j; i++){
    /* if the magnitude of the cross product is greater than the ball's radius, there is no collision */
    dx = lineList[i].x1 - c->Xpos.l;
    dy = lineList[i].y1 - c->Ypos.l;
    cross = (dx>> 8) * (lineList[i].ly >> 8) - (lineList[i].lx >> 8) * (dy >> 8);
    if(cross >= (BALL_RADIUS << 16)) continue;
    if(cross < 0) continue;

    /* if the dot product is positive, there is no collision */
    dot = (dx>>8) * (lineList[i].lx>>8) + (dy>>8) * (lineList[i].ly>>8);
    if(dot >= 0) continue;

    /* if this dot product is negative, there is no collision */
    dx = lineList[i].x2 - c->Xpos.l;
    dy = lineList[i].y2 - c->Ypos.l;
    dot = (dx>>8) * (lineList[i].lx>>8) + (dy>>8) * (lineList[i].ly>>8);
    if(dot <= 0) continue;

    /* the velocity component perpendicular to the line is (n.v)n */
    dot = (c->Xvel>>8) * (lineList[i].ly>>8) + (c->Yvel>>8) * (-lineList[i].lx>>8);
    c->Xvel -= 7 * (dot >> 8) * (lineList[i].ly>>8) / 4;
    c->Yvel -= 7 * (dot >> 8) * (-lineList[i].lx>>8) / 4;
    c->behavior = 0;       /* not from flipper */

    /* if the line is active, add the bumper acceleration vector */
    if(!powerOff){
      if(lineList[i].active){
        c->Xvel -= 4 * lineList[i].ly;
        c->Yvel += 4 * lineList[i].lx;
        StartSound(41);
      }
    }

    /* if we're leaking into the line, move it back out */
    if(cross < ((BALL_RADIUS - 1) << 16)){
      c->Xpos.l -= lineList[i].ly;
      c->Ypos.l += lineList[i].lx;
    }

    /* since we can't collide with more than one at a time, we're done */
    lineCollision = 1;
    break;
  }

  /* then check for collision with the corners */
  if(!lineCollision){
    for(i = 0; i < sizeof(pointList)/sizeof(Point); i++){
      /* determine the separation of the objects */
      dx = pointList[i].x - c->Xpos.l;
      dy = pointList[i].y - c->Ypos.l;
      adx = abs(dx);
      ady = abs(dy);
      dist = Distance(adx, ady) >> 16;
      if(dist <= BALL_RADIUS){
        dot = (dx>>8) * (c->Xvel>>8) + (dy>>8) * (c->Yvel>>8);
        if(dot < 0) continue;
        if(dist == 0) dist = 1;
        ax = (dx>>8) * (dot>>8) / (dist * dist);
        ay = (dy>>8) * (dot>>8) / (dist * dist);
        c->Xvel -= (ax<<1)-(ax>>4);
        c->Yvel -= (ay<<1)-(ay>>4);
        c->behavior = 0;       /* not from flipper */
      }
    }
  }
}

void DoCreatureCollisions(){
short i, j;
register CreaturePtr t1, t2;
long vxc, vyc, vx1, vx2, vy1, vy2;
long dx, dy;
long adx, ady;
long ax, ay;
long dist;
long dot;

  for(i = 0; i < ballCount; i++){
    t1 = &things[i];
    if(t1->type < 0) continue;
    for(j = i + 1; j < ballCount; j++){
      t2 = &things[j];
      if(t2->type < 0) continue;
      /* determine the separation of the objects */
      dx = t2->Xpos.l - t1->Xpos.l;
      adx = abs(dx);
      dy = t2->Ypos.l - t1->Ypos.l;
      ady = abs(dy);
      dist = Distance(adx, ady) >> 16;
      if(dist <= 2 * BALL_RADIUS){

        vxc = (t1->Xvel + t2->Xvel)/2;
        vyc = (t1->Yvel + t2->Yvel)/2;

        vx1 = t1->Xvel - vxc;
        vy1 = t1->Yvel - vyc;
        vx2 = t2->Xvel - vxc;
        vy2 = t2->Yvel - vyc;

        dot = (dx>>8) * (vx1>>8) + (dy>>8) * (vy1>>8);
        if(dot < 0) continue;
        if(dist == 0) dist = 1;
        ax = (dx>>8) * (dot>>8) / (dist * dist);
        ay = (dy>>8) * (dot>>8) / (dist * dist);
        t1->Xvel -= (ax<<1)-(ax>>4);
        t1->Yvel -= (ay<<1)-(ay>>4);
        t1->behavior = 0;       /* not from flipper */

        dot = (-dx>>8) * (vx2>>8) + (-dy>>8) * (vy2>>8);
        if(dot < 0) continue;
        ax = (dx>>8) * (dot>>8) / (4 * BALL_RADIUS_SQUARED);
        ay = (dy>>8) * (dot>>8) / (4 * BALL_RADIUS_SQUARED);
        t2->Xvel += (ax<<1)-(ax>>4);
        t2->Yvel += (ay<<1)-(ay>>4);
        t2->behavior = 0;       /* not from flipper */
      }
    }
    CheckPlayfieldCollisions(t1);
    CheckOtherCollisions(t1);
  }
}
