
#ifdef DEFINE_GLOBALS
#define global
#else
#define global extern
#endif

#pragma dsection .vdp

global volatile struct {
        union {
                short w;
                long l;
        } data;
        union {
                short w;
                long l;
        } control;
        unsigned char h_count;
        unsigned char v_count;
} VDPPort
#ifdef DEFINE_GLOBALS
 = 0
#endif
;

#pragma dsection .data

global volatile unsigned long   tickCounter;
global volatile unsigned char   vblFlag;

/* Event Manager */
global unsigned char    currentStick1;
global unsigned char    lastStick1;
global unsigned char    stickEdges1;
global unsigned char    currentStick2;
global unsigned char    lastStick2;
global unsigned char    stickEdges2;

#define IOBufferLength 256
global unsigned short IORequestBuffer[IOBufferLength];
global unsigned short IORequestPointer;

global unsigned int screenXPos;
global unsigned int screenYPos;
global unsigned short fieldARowUpdate[64];
global unsigned short fieldBRowUpdate[64];
global unsigned short fieldAColumnUpdate[32];
global unsigned short fieldBColumnUpdate[32];
global SpriteDef spriteAttributes[80];
global short sprite;
global int thingCount;
global Creature things[15];
global int segmentSpeedLimit;
