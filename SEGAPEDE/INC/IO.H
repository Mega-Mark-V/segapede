/*****************************************************************************
*                                                                            *
*   IO.H - Defines interface for communication with i/o system               *
*                                                                            *
*****************************************************************************/

#ifndef _IO_H_
#define _IO_H_

#define IOR_READ_CONTROLLERS        1
#define IOR_SEND_SPRITE_CONTROL     2
#define IOR_SEND_PALETTES           3
#define IOR_SEND_BLOCK_16           4
#define IOR_SEND_BLOCK_32           5
#define IOR_SEND_SCROLLS            6
#define IOR_SEND_A_SCROLLS          7
#define IOR_SEND_B_SCROLLS          8
#define IOR_SEND_COLUMN             9

#define IOReqSize 8

#define ReadControllers() { \
    IORequestBuffer[IORequestPointer] = IOR_READ_CONTROLLERS; \
    IORequestPointer = (IORequestPointer + IOReqSize) % IOBufferLength; \
    }

#define SendSpriteControl(addr) { \
    IORequestBuffer[IORequestPointer] = IOR_SEND_SPRITE_CONTROL; \
    *((unsigned long **)(IORequestBuffer+IORequestPointer+1)) = (unsigned long *)(addr); \
    IORequestPointer = (IORequestPointer + IOReqSize) % IOBufferLength; \
    }

#define SendPalettes(addr) { \
    IORequestBuffer[IORequestPointer] = IOR_SEND_PALETTES; \
    *((unsigned long **)(IORequestBuffer+IORequestPointer+1)) = (unsigned long *)(addr); \
    IORequestPointer = (IORequestPointer + IOReqSize) % IOBufferLength; \
    }

#define SendBlock16(addr, target, size) { \
    IORequestBuffer[IORequestPointer] = IOR_SEND_BLOCK_16; \
    IORequestBuffer[IORequestPointer+1] = ((short)((long)addr & 0xffff)); \
    IORequestBuffer[IORequestPointer+2] = (target); \
    IORequestBuffer[IORequestPointer+3] = (size); \
    IORequestPointer = (IORequestPointer + IOReqSize) % IOBufferLength; \
    }

#define SendBlock32(addr, target, size) { \
    IORequestBuffer[IORequestPointer] = IOR_SEND_BLOCK_32; \
    *((unsigned long **)(IORequestBuffer+IORequestPointer+1)) = (unsigned long *)(addr); \
    IORequestBuffer[IORequestPointer+3] = (target); \
    IORequestBuffer[IORequestPointer+4] = (size); \
    IORequestPointer = (IORequestPointer + IOReqSize) % IOBufferLength; \
    }

#define SendScrolls(ay, by, ax, bx) { \
    IORequestBuffer[IORequestPointer] = IOR_SEND_SCROLLS; \
    IORequestBuffer[IORequestPointer+1] = (ay); \
    IORequestBuffer[IORequestPointer+2] = (by); \
    IORequestBuffer[IORequestPointer+3] = (ax); \
    IORequestBuffer[IORequestPointer+4] = (bx); \
    IORequestPointer = (IORequestPointer + IOReqSize) % IOBufferLength; \
    }

#define SendAScrolls(ay, ax) { \
    IORequestBuffer[IORequestPointer] = IOR_SEND_A_SCROLLS; \
    IORequestBuffer[IORequestPointer+1] = (ay); \
    IORequestBuffer[IORequestPointer+2] = (ax); \
    IORequestPointer = (IORequestPointer + IOReqSize) % IOBufferLength; \
    }

#define SendBScrolls(by, bx) { \
    IORequestBuffer[IORequestPointer] = IOR_SEND_B_SCROLLS; \
    IORequestBuffer[IORequestPointer+1] = (by); \
    IORequestBuffer[IORequestPointer+2] = (bx); \
    IORequestPointer = (IORequestPointer + IOReqSize) % IOBufferLength; \
    }

#define SendColumn(addr, target) { \
  IORequestBuffer[IORequestPointer] = IOR_SEND_COLUMN; \
  *((unsigned long **)(IORequestBuffer+IORequestPointer+1)) = (unsigned long *)(addr); \
  IORequestBuffer[IORequestPointer+3] = (target); \
  IORequestPointer = (IORequestPointer + IOReqSize) % IOBufferLength; \
  }

/* Prototype for IO.C */
    void DoIO(void);

#endif
