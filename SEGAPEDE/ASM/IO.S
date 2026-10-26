	.opt	proc=68000
	.file	"c:\segapede\c\io.c"
			;/*****************************************************************************
			;*                                                                            *
			;*                                                                            *
			;*                                                                            *
			;*                                                                            *
			;*                                                                            *
			;*                                                                            *
			;*****************************************************************************/
			;
			;#include "io.h"
			;#include "vdp.h"
			;
			;extern unsigned char CurrentStick1;
			;extern unsigned char LastStick1;
			;extern unsigned char StickEdges1;
			;extern unsigned char CurrentStick2;
			;extern unsigned char LastStick2;
			;extern unsigned char StickEdges2;
	.def	.1fake\	.scl	12\	.type	0x9\	.size	4\	.endef
	.def	w\	.scl	11\	.type	0x3\	.endef
	.def	l\	.scl	11\	.type	0x5\	.endef
	.def	.eos\	.val	4\	.scl	102\	.size	4\	.tag	.1fake\	.endef
	.def	.2fake\	.scl	12\	.type	0x9\	.size	4\	.endef
	.def	w\	.scl	11\	.type	0x3\	.endef
	.def	l\	.scl	11\	.type	0x5\	.endef
	.def	.eos\	.val	4\	.scl	102\	.size	4\	.tag	.2fake\	.endef
	.def	.0fake\	.scl	10\	.type	0x8\	.size	10\	.endef
	.def	data\	.val	0\	.scl	8\	.type	0x9\	.size	4\	.tag	.1fake\	.endef
	.def	control\	.val	4\	.scl	8\	.type	0x9\	.size	4\	.tag	.2fake\	.endef
	.def	h_count\	.val	8\	.scl	8\	.type	0xc\	.endef
	.def	v_count\	.val	9\	.scl	8\	.type	0xc\	.endef
	.def	.eos\	.val	10\	.scl	102\	.size	10\	.tag	.0fake\	.endef
			;
			;#pragma dsection .vdp
			;
			;extern volatile struct {
			;  union {
			;    short w;
			;    long l;
			;  } data;
			;  union {
			;    short w;
			;    long l;
			;  } control;
			;  unsigned char h_count;
			;  unsigned char v_count;
			;} VDPPort;
	.def	_ip\	.val	_ip\	.scl	3\	.type	0x4\	.endef
	.lcomm	_ip,4,2
			;
			;#pragma dsection .data
			;
			;#define VDPDEST(x) \
			;  VDPPort.control.l = 0x40000000L + (((x) << 16) & 0x3fff0000L) + (((x) >> 14) & 3)
			;
			;#if 0
			;#define DMA(src, dst, bytes) \
			;  VDPPort.control.l = 0x93009400 + (((bytes) >> 9) & 0xff) + (((bytes) << 15) & 0xff0000); \
			;  VDPPort.control.l = 0x95009600 + (((src) << 15) & 0xff0000) + (((src) >> 9) & 0xff); \
			;  VDPPort.control.w = 0x9700 + (((src) >> 17) & 0x7f); \
			;  VDPPort.control.l = 0x40000080 + (((dst) << 16) & 0x3fff0000) + (((dst) >> 14) & 3);
			;#else
			;#define DMA(src, dst, bytes) \
			;  VDPPort.control.w = 0x9300 + (((bytes) >> 1) & 0xff); \
			;  VDPPort.control.w = 0x9400 + (((bytes) >> 9) & 0xff); \
			;  VDPPort.control.w = 0x9500 + (((src) >> 1) & 0xff); \
			;  VDPPort.control.w = 0x9600 + (((src) >> 9) & 0xff); \
			;  VDPPort.control.w = 0x9700 + (((src) >> 17) & 0x7f); \
			;  VDPPort.control.l = 0x40000080 + (((dst) << 16) & 0x3fff0000) + (((dst) >> 14) & 3);
			;#endif
			;
			;
			;static int ip;      /* i/o request buffer 'pointer' */
			;
	.text
	.align	2
	.globl	_DoIO
	.def	_DoIO\	.val	_DoIO\	.scl	2\	.type	0x20\	.endef
_DoIO:
	movem.l	d3/a2-a5,-(sp)
	subq.l	#8,sp
	.ln 	1
	.def	.bf\	.val	.\	.scl	101\	.line	59\	.endef
			;void DoIO(){
			;  short size;
			;  unsigned char *srcptr;
			;  unsigned char *dstptr;
			;
			;  gemsdmastart();
	.ln 	6
	jsr 	_gemsdmastart
	lea 	_IORequestBuffer,a4
	lea 	_VDPPort+4,a5
	move.l	#_VDPPort,4(sp)
			;  while(ip != IORequestPointer){
	.ln 	7
	bra.w	L2
L12:
			;    switch(IORequestBuffer[ip]){
	.ln 	8
	move.l	_ip,d0
	add.l	d0,d0
	move.w	0(a4,d0.l),d0
	ext.l	d0
	subq.l	#1,d0
	bmi.w	L18
	cmpi.l	#8,d0
	bgt.w	L18
	add.l	d0,d0
	move.w	L38(pc,d0.l),d0
	jmp 	L38(pc,d0.w)
L38:
	.opt 	rngchk 3
	.word	L16-L38
	.word	L20-L38
	.word	L22-L38
	.word	L24-L38
	.word	L26-L38
	.word	L28-L38
	.word	L30-L38
	.word	L32-L38
	.word	L34-L38
	.opt 	rngchk 1
L16:
			;      case IOR_READ_CONTROLLERS:
			;        EventManager();
	.ln 	10
	jsr 	_EventManager
			;        break;
	.ln 	11
	bra.w	L18
L20:
			;
			;      case IOR_SEND_SPRITE_CONTROL:
			;        srcptr = *((unsigned char **)(IORequestBuffer+ip+1));
	.ln 	14
	move.l	_ip,d0
	add.l	d0,d0
	movea.l	2(a4,d0.l),a2
			;        DMA((long)srcptr, (long)SPRITE_ATTRIBUTE_VDP_ADDRESS, 640L);
	.ln 	15
	move.w	#-27840,(a5)
	move.w	#-27647,(a5)
	move.l	a2,d0
	asr.l	#1,d0
	andi.w	#255,d0
	addi.w	#38144,d0
	move.w	d0,(a5)
	move.l	a2,d0
	moveq	#9,d2
	asr.l	d2,d0
	andi.w	#255,d0
	addi.w	#38400,d0
	move.w	d0,(a5)
	move.l	a2,d0
	swap	d0
	ext.l	d0
	asr.l	#1,d0
	andi.b	#127,d0
	ext.w	d0
	addi.w	#-26880,d0
	move.w	d0,(a5)
	move.l	#1342177411,(a5)
			;        break;
	.ln 	16
	bra.w	L18
L22:
			;
			;      case IOR_SEND_PALETTES:
			;        srcptr = (unsigned char *)((*((unsigned long *)(IORequestBuffer+ip+1))) >> 1);
	.ln 	19
	move.l	_ip,d0
	add.l	d0,d0
	move.l	2(a4,d0.l),d0
	lsr.l	#1,d0
	movea.l	d0,a2
			;        VDPPort.control.l = 0x93409400L;    /* R19, R20 (DMA length) */
	.ln 	20
	move.l	#-1824484352,(a5)
			;        VDPPort.control.w = 0x9500 + ((long)srcptr & 0xff);
	.ln 	21
	move.l	a2,d0
	andi.w	#255,d0
	addi.w	#38144,d0
	move.w	d0,(a5)
			;        VDPPort.control.w = 0x9600 + (((long)srcptr & 0xff00) >> 8);
	.ln 	22
	move.l	a2,d0
	andi.l	#65280,d0
	asr.l	#8,d0
	addi.w	#38400,d0
	move.w	d0,(a5)
			;        VDPPort.control.w = 0x9700 + ((short)((long)srcptr >> 16) & 0x7f);
	.ln 	23
	move.l	a2,d0
	swap	d0
	ext.l	d0
	andi.b	#127,d0
	ext.w	d0
	addi.w	#-26880,d0
	move.w	d0,(a5)
			;        VDPPort.control.l = 0xc0000080L;
	.ln 	24
	move.l	#-1073741696,(a5)
			;        break;
	.ln 	25
	bra.w	L18
L24:
			;
			;      case IOR_SEND_BLOCK_16:
			;        srcptr = 0x00ff0000 | IORequestBuffer[ip+1];
	.ln 	28
	move.l	_ip,d0
	add.l	d0,d0
	move.w	2(a4,d0.l),d0
	ext.l	d0
	ori.l	#16711680,d0
	movea.l	d0,a2
			;        dstptr = IORequestBuffer[ip+2];
	.ln 	29
	move.l	_ip,d0
	add.l	d0,d0
	movea.w	4(a4,d0.l),a3
			;        size = IORequestBuffer[ip+3];
	.ln 	30
	move.l	_ip,d3
	add.l	d3,d3
	move.w	6(a4,d3.l),d3
			;        DMA((long)srcptr, (long)dstptr, (long)size);
	.ln 	31
	move.w	d3,d0
	ext.l	d0
	asr.l	#1,d0
	andi.w	#255,d0
	addi.w	#37632,d0
	move.w	d0,(a5)
	ext.l	d3
	moveq	#9,d0
	asr.l	d0,d3
	andi.w	#255,d3
	addi.w	#37888,d3
	move.w	d3,(a5)
	move.l	a2,d0
	asr.l	#1,d0
	andi.w	#255,d0
	addi.w	#38144,d0
	move.w	d0,(a5)
	move.l	a2,d0
	moveq	#9,d2
	asr.l	d2,d0
	andi.w	#255,d0
	addi.w	#38400,d0
	move.w	d0,(a5)
	move.l	a2,d0
	swap	d0
	ext.l	d0
	asr.l	#1,d0
	andi.b	#127,d0
	ext.w	d0
	addi.w	#-26880,d0
	move.w	d0,(a5)
	move.l	a3,d0
	swap	d0
	clr.w	d0
	andi.l	#1073676288,d0
	move.l	a3,d2
	moveq	#14,d3
	asr.l	d3,d2
	andi.b	#3,d2
	ext.w	d2
	ext.l	d2
	add.l	d2,d0
	addi.l	#1073741952,d0
	move.l	d0,(a5)
			;        break;
	.ln 	32
	bra.w	L18
L26:
			;
			;      case IOR_SEND_BLOCK_32:
			;        srcptr = *((unsigned char **)(IORequestBuffer+ip+1));
	.ln 	35
	move.l	_ip,d0
	add.l	d0,d0
	movea.l	2(a4,d0.l),a2
			;        dstptr = IORequestBuffer[ip+3];
	.ln 	36
	move.l	_ip,d0
	add.l	d0,d0
	movea.w	6(a4,d0.l),a3
			;        size = IORequestBuffer[ip+4];
	.ln 	37
	move.l	_ip,d3
	add.l	d3,d3
	move.w	8(a4,d3.l),d3
			;        DMA((long)srcptr, (long)dstptr, (long)size);
	.ln 	38
	move.w	d3,d0
	ext.l	d0
	asr.l	#1,d0
	andi.w	#255,d0
	addi.w	#37632,d0
	move.w	d0,(a5)
	ext.l	d3
	moveq	#9,d0
	asr.l	d0,d3
	andi.w	#255,d3
	addi.w	#37888,d3
	move.w	d3,(a5)
	move.l	a2,d0
	asr.l	#1,d0
	andi.w	#255,d0
	addi.w	#38144,d0
	move.w	d0,(a5)
	move.l	a2,d0
	moveq	#9,d2
	asr.l	d2,d0
	andi.w	#255,d0
	addi.w	#38400,d0
	move.w	d0,(a5)
	move.l	a2,d0
	swap	d0
	ext.l	d0
	asr.l	#1,d0
	andi.b	#127,d0
	ext.w	d0
	addi.w	#-26880,d0
	move.w	d0,(a5)
	move.l	a3,d0
	swap	d0
	clr.w	d0
	andi.l	#1073676288,d0
	move.l	a3,d2
	moveq	#14,d3
	asr.l	d3,d2
	andi.b	#3,d2
	ext.w	d2
	ext.l	d2
	add.l	d2,d0
	addi.l	#1073741952,d0
	move.l	d0,(a5)
			;        break;
	.ln 	39
	bra.w	L18
L28:
			;
			;      case IOR_SEND_SCROLLS:
			;        /* do vertical */
			;        VDPPort.control.l = 0x40000010L;
	.ln 	43
	move.l	#1073741840,(a5)
			;        VDPPort.data.w = IORequestBuffer[ip+1];
	.ln 	44
	move.l	_ip,d0
	add.l	d0,d0
	movea.l	4(sp),a1
	move.w	2(a4,d0.l),(a1)
			;        VDPPort.data.w = IORequestBuffer[ip+2];
	.ln 	45
	move.l	_ip,d0
	add.l	d0,d0
	move.w	4(a4,d0.l),(a1)
			;        /* then do horizontal */
			;        VDPDEST(H_SCROLL_VDP_ADDRESS);
	.ln 	47
	move.l	#1409286147,(a5)
			;        VDPPort.data.w = IORequestBuffer[ip+3];
	.ln 	48
	move.l	_ip,d0
	add.l	d0,d0
	movea.l	4(sp),a1
	move.w	6(a4,d0.l),(a1)
			;        VDPPort.data.w = IORequestBuffer[ip+4];
	.ln 	49
	move.l	_ip,d0
	add.l	d0,d0
	move.w	8(a4,d0.l),(a1)
			;        break;
	.ln 	50
	bra.w	L18
L30:
			;
			;      case IOR_SEND_A_SCROLLS:
			;        /* do vertical */
			;        VDPPort.control.l = 0x40000010L;
	.ln 	54
	move.l	#1073741840,(a5)
			;        VDPPort.data.w = IORequestBuffer[ip+1];
	.ln 	55
	move.l	_ip,d0
	add.l	d0,d0
	movea.l	4(sp),a1
	move.w	2(a4,d0.l),(a1)
			;        /* then do horizontal */
			;        VDPDEST(H_SCROLL_VDP_ADDRESS);
	.ln 	57
	move.l	#1409286147,(a5)
			;        VDPPort.data.w = IORequestBuffer[ip+2];
	.ln 	58
	move.l	_ip,d0
	add.l	d0,d0
	move.w	4(a4,d0.l),(a1)
			;        break;
	.ln 	59
	bra.s	L18
L32:
			;
			;      case IOR_SEND_B_SCROLLS:
			;        /* do vertical */
			;        VDPPort.control.l = 0x40020010L;
	.ln 	63
	move.l	#1073872912,(a5)
			;        VDPPort.data.w = IORequestBuffer[ip+1];
	.ln 	64
	move.l	_ip,d0
	add.l	d0,d0
	movea.l	4(sp),a1
	move.w	2(a4,d0.l),(a1)
			;        /* then do horizontal */
			;        VDPDEST(H_SCROLL_VDP_ADDRESS+2);
	.ln 	66
	move.l	#1409417219,(a5)
			;        VDPPort.data.w = IORequestBuffer[ip+2];
	.ln 	67
	move.l	_ip,d0
	add.l	d0,d0
	move.w	4(a4,d0.l),(a1)
			;        break;
	.ln 	68
	bra.s	L18
L34:
			;
			;      case IOR_SEND_COLUMN:
			;        srcptr = *((unsigned char **)(IORequestBuffer+ip+1));
	.ln 	71
	move.l	_ip,d0
	add.l	d0,d0
	movea.l	2(a4,d0.l),a2
			;        dstptr = (unsigned char *)IORequestBuffer[ip+3];
	.ln 	72
	move.l	_ip,d0
	add.l	d0,d0
	movea.w	6(a4,d0.l),a3
			;        VDPPort.control.w = 0x8f80;
	.ln 	73
	move.w	#-28800,(a5)
			;        InitDMA((long)srcptr, (long)dstptr, (long)64);
	.ln 	74
	moveq	#64,d0
	move.l	d0,(sp)
	move.l	a3,-(sp)
	move.l	a2,-(sp)
	jsr 	_InitDMA
	addq.l	#8,sp
			;        VDPPort.control.w = 0x8f02;
	.ln 	75
	move.w	#-28926,(a5)
L18:
			;        break;
			;
			;      default:
			;        break;
			;    }
			;    ip = (ip + IOReqSize) & 0xff;
	.ln 	81
	move.l	_ip,d0
	addq.w	#8,d0
	andi.w	#255,d0
	ext.l	d0
	move.l	d0,_ip
L2:
			;  }
	.ln 	82
	move.w	_IORequestPointer,d0
	ext.l	d0
	cmp.l	_ip,d0
	bne 	L12
			;  gemsdmaend();
	.ln 	83
	jsr 	_gemsdmaend
	.def	.ef\	.val	.\	.scl	101\	.line	142\	.endef
	addq.l	#8,sp
			;}
	.ln 	84
	movem.l	(sp)+,d3/a2-a5
	rts	
	.def	_DoIO\	.val	.\	.scl	-1\	.endef
