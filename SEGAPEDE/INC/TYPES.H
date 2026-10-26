
typedef struct {
    unsigned short  vp;
    unsigned char   size;
    unsigned char   link;
    unsigned short  cell;
    unsigned short  hp;
}SpriteDef, *SpriteDefPtr;

typedef union {
    short   w;
    long    l;
}FixedPoint, *FixedPointPtr;

typedef struct {
    short           type;
    FixedPoint      Xpos;
    FixedPoint      Ypos;
    long            oldX;
    long            oldY;
    long            Xvel;
    long            Yvel;
    long            Xvel_old;
    long            Yvel_old;
    unsigned short  chBase;
    short           anim_time;
    unsigned short *Frame;
    short           behavior;
    short           state;
    short           oldBehavior;
    short           oldState;
    short           anim_next;
    short           control_timer;
    short           surface;
    short           follow;
    short           lead;
    short           newanim;
    short           angle;
    unsigned short *artset;
}Creature, *CreaturePtr;

typedef struct {
    unsigned short *frame;
    short           duration;
    short           next;
    unsigned short *artset;
}AnimState, *AnimStatePtr;
