#define TIMING 0
#define FALSE   0
#define TRUE    (!FALSE)
#define NULL    ((void *)0L)

#define exchange(x, y)  (x ^= y ^= x ^= y)
#define max(a, b)       ((a > b) ? a : b)
#define min(a, b)       ((a > b) ? b : a)
#define offsetof(a, b)  (&(((a *)0)->b))
#define abs(x)          ((x < 0) ? -x : x)

/* VDP access codes */
#define VRAMWR          0x40000000L
#define VRAMRD          0x00000000L
#define VSRAMWR         0x40000010L
#define VSRAMRD         0x00000010L
#define CRAMWR          0xc0000000L
#define CRAMRD          0x00000020L

/* Codes for sprites and cells  */
#define PALETTE0        0x0000
#define PALETTE1        0x2000
#define PALETTE2        0x4000
#define PALETTE3        0x6000
#define PRIORITY_HI     0x8000
#define PRIORITY_LO     0
#define HFlip           0x0800
#define VFlip           0x1000

/* Joystick Bits */

#define JOYUP           0x01
#define JOYDOWN         0x02
#define JOYLEFT         0x04
#define JOYRIGHT        0x08
#define JOYBUTTONB      0x10
#define JOYBUTTONC      0x20
#define JOYBUTTONA      0x40
#define JOYSTART        0x80

#define VCTRLL VDPPort.control.l
#define VCTRLW VDPPort.control.w
#define VDATAW VDPPort.data.w

#define VDPDEST(x) VCTRLL = 0x40000000L + (((x) << 16) & 0x3fff0000L) + (((x) >> 14) & 3)

#if 0
#define DMA(src, dst, bytes) \
        VCTRLL = 0x93009400 + (((bytes) >> 9) & 0xff) + (((bytes) << 15) & 0xff0000); \
        VCTRLL = 0x95009600 + (((src) << 15) & 0xff0000) + (((src) >> 9) & 0xff); \
        VCTRLW = 0x9700 + (((src) >> 17) & 0x7f); \
        VCTRLL = 0x40000080 + (((dst) << 16) & 0x3fff0000) + (((dst) >> 14) & 3);
#else
#define DMA(src, dst, bytes) \
        VCTRLW = 0x9300 + (((bytes) >> 1) & 0xff); \
        VCTRLW = 0x9400 + (((bytes) >> 9) & 0xff); \
        VCTRLW = 0x9500 + (((src) >> 1) & 0xff); \
        VCTRLW = 0x9600 + (((src) >> 9) & 0xff); \
        VCTRLW = 0x9700 + (((src) >> 17) & 0x7f); \
        VCTRLL = 0x40000080 + (((dst) << 16) & 0x3fff0000) + (((dst) >> 14) & 3);
#endif

/* This is the elevation returned by GetElevation (segments.c) when
 * the point in question has no nearby surface. */
#define NO_SURFACE 0

/* A useful abbreviation */
#define Zip things[0]

/* These  are the definitions of the segment types available */
#define HEAD_SEGMENT       1
#define SPACER_SEGMENT     2
#define TAILCAP_SEGMENT    3
#define ENGINE_SEGMENT     4
#define SEGMENT_TYPE_COUNT 5

/* These are the definitions of the major behavior groups for Zip and the segments */
#define DEAD                0
#define H_SURFACE           1
#define V_SURFACE           2
#define INVERTED_SURFACE    3
#define IN_AIR              4
#define INVERTED_IN_AIR     5
#define BOUNCING_FREE       6
#define FREE_ON_SURFACE     7

/* These are the definitions of the activity states within the behavior groups */
#define STOPPED             1
#define MOVING_LEFT         2
#define MOVING_RIGHT        3
#define MOVING_UP           4
#define MOVING_DOWN         5
#define COASTING_LEFT       6
#define COASTING_RIGHT      7
#define BRAKING_LEFT        8
#define BRAKING_RIGHT       9
#define JUMPING_LEFT        10
#define JUMPING_RIGHT       11

/* These ate the definitions of the activity overrides */
#define NONE                0
#define SHOOT               1
#define FLINCH              2
#define BIGFLINCH           3
