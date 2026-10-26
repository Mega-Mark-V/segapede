/*****************************************************************************
*                                                                            *
*   VDP.H - Defines VDP allocation                                           *
*                                                                            *
*****************************************************************************/

#ifndef _VDP_H_
#define _VDP_H_

#define FG                 0xc000
#define SPRITE_ATTRIBUTES  0xd000
#define SIDEBAR            0xd000
#define HSCROLL            0xd400
#define BG                 0xe000

#define ZIP_SWAP_AREA 0xd420
#define SEGMENT_1_AREA (0xc000 - 1 * 32 * 36)
#define SEGMENT_2_AREA (0xc000 - 2 * 32 * 36)
#define SEGMENT_3_AREA (0xc000 - 3 * 32 * 36)
#define SEGMENT_4_AREA (0xc000 - 4 * 32 * 36)
#define SEGMENT_5_AREA (0xc000 - 5 * 32 * 36)
#define SEGMENT_6_AREA (0xc000 - 6 * 32 * 36)
#define SEGMENT_7_AREA (0xc000 - 7 * 32 * 36)
#define SEGMENT_8_AREA (0xc000 - 8 * 32 * 36)
#define SEGMENT_9_AREA (0xc000 - 9 * 32 * 36)

#define ZIP_CELL_BASE  (ZIP_SWAP_AREA / 32)
#define SEGMENT_1_BASE (SEGMENT_1_AREA / 32)
#define SEGMENT_2_BASE (SEGMENT_2_AREA / 32)
#define SEGMENT_3_BASE (SEGMENT_3_AREA / 32)
#define SEGMENT_4_BASE (SEGMENT_4_AREA / 32)
#define SEGMENT_5_BASE (SEGMENT_5_AREA / 32)
#define SEGMENT_6_BASE (SEGMENT_6_AREA / 32)
#define SEGMENT_7_BASE (SEGMENT_7_AREA / 32)
#define SEGMENT_8_BASE (SEGMENT_8_AREA / 32)
#define SEGMENT_9_BASE (SEGMENT_9_AREA / 32)

#define VERSION 0xa10001
#define WATCHDOG 0xa14000
#endif
