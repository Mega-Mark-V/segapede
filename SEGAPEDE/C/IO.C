/*****************************************************************************
*                                                                            *
*                                                                            *
*                                                                            *
*                                                                            *
*                                                                            *
*                                                                            *
*****************************************************************************/

#include "types.h"
#include "defs.h"
#include "vars.h"
#include "proto.h"
#include "io.h"
#include "vdp.h"



static int ip;      /* i/o request buffer 'pointer' */

void DoIO(){
  short size;
  unsigned char *srcptr;
  unsigned char *dstptr;

  gemsdmastart();
  while(ip != IORequestPointer){
    switch(IORequestBuffer[ip]){
      case IOR_READ_CONTROLLERS:
        ReadHWControllers();
        break;

      case IOR_SEND_SPRITE_CONTROL:
        srcptr = *((unsigned char **)(IORequestBuffer+ip+1));
        DMA((long)srcptr, (long)SPRITE_ATTRIBUTES, 640L);
        break;

      case IOR_SEND_PALETTES:
        srcptr = (unsigned char *)((*((unsigned long *)(IORequestBuffer+ip+1))) >> 1);
        VDPPort.control.l = 0x93409400L;    /* R19, R20 (DMA length) */
        VDPPort.control.w = 0x9500 + ((long)srcptr & 0xff);
        VDPPort.control.w = 0x9600 + (((long)srcptr & 0xff00) >> 8);
        VDPPort.control.w = 0x9700 + ((short)((long)srcptr >> 16) & 0x7f);
        VDPPort.control.l = 0xc0000080L;
        break;

      case IOR_SEND_BLOCK_16:
        srcptr = (unsigned char *)(0x00ff0000 | (long)IORequestBuffer[ip+1]);
        dstptr = (unsigned char *)IORequestBuffer[ip+2];
        size = IORequestBuffer[ip+3];
        DMA((long)srcptr, (long)dstptr, (long)size);
        break;

      case IOR_SEND_BLOCK_32:
        srcptr = *((unsigned char **)(IORequestBuffer+ip+1));
        dstptr = (unsigned char *)IORequestBuffer[ip+3];
        size = IORequestBuffer[ip+4];
        DMA((long)srcptr, (long)dstptr, (long)size);
        break;

      case IOR_SEND_SCROLLS:
        /* do vertical */
        VDPPort.control.l = 0x40000010L;
        VDPPort.data.w = IORequestBuffer[ip+1];
        VDPPort.data.w = IORequestBuffer[ip+2];
        /* then do horizontal */
        VDPDEST(HSCROLL);
        VDPPort.data.w = IORequestBuffer[ip+3];
        VDPPort.data.w = IORequestBuffer[ip+4];
        break;

      case IOR_SEND_A_SCROLLS:
        /* do vertical */
        VDPPort.control.l = 0x40000010L;
        VDPPort.data.w = IORequestBuffer[ip+1];
        /* then do horizontal */
        VDPDEST(HSCROLL);
        VDPPort.data.w = IORequestBuffer[ip+2];
        break;

      case IOR_SEND_B_SCROLLS:
        /* do vertical */
        VDPPort.control.l = 0x40020010L;
        VDPPort.data.w = IORequestBuffer[ip+1];
        /* then do horizontal */
        VDPDEST(HSCROLL+2);
        VDPPort.data.w = IORequestBuffer[ip+2];
        break;

      case IOR_SEND_COLUMN:
        srcptr = *((unsigned char **)(IORequestBuffer+ip+1));
        dstptr = (unsigned char *)IORequestBuffer[ip+3];
        VDPPort.control.w = 0x8f80;
        DMA((long)srcptr, (long)dstptr, (long)64);
        VDPPort.control.w = 0x8f02;
        break;

      default:
        break;
    }
    ip = (ip + IOReqSize) % IOBufferLength;
  }
  gemsdmaend();
}
