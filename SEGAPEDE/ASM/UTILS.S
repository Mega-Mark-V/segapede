	.opt	proc=68000
	.file	"c:\segapede\c\utils.c"
	.def	.0fake\	.scl	10\	.type	0x8\	.size	8\	.endef
	.def	vp\	.val	0\	.scl	8\	.type	0xd\	.endef
	.def	size\	.val	2\	.scl	8\	.type	0xc\	.endef
	.def	link\	.val	3\	.scl	8\	.type	0xc\	.endef
	.def	cell\	.val	4\	.scl	8\	.type	0xd\	.endef
	.def	hp\	.val	6\	.scl	8\	.type	0xd\	.endef
	.def	.eos\	.val	8\	.scl	102\	.size	8\	.tag	.0fake\	.endef
	.def	SpriteDef\	.scl	13\	.type	0x8\	.size	8\	.tag	.0fake\	.endef
	.def	SpriteDefPtr\	.scl	13\	.type	0x18\	.tag	.0fake\	.endef
	.def	.2fake\	.scl	12\	.type	0x9\	.size	4\	.endef
	.def	w\	.scl	11\	.type	0x3\	.endef
	.def	l\	.scl	11\	.type	0x5\	.endef
	.def	.eos\	.val	4\	.scl	102\	.size	4\	.tag	.2fake\	.endef
	.def	.3fake\	.scl	12\	.type	0x9\	.size	4\	.endef
	.def	w\	.scl	11\	.type	0x3\	.endef
	.def	l\	.scl	11\	.type	0x5\	.endef
	.def	.eos\	.val	4\	.scl	102\	.size	4\	.tag	.3fake\	.endef
	.def	.1fake\	.scl	10\	.type	0x8\	.size	24\	.endef
	.def	type\	.val	0\	.scl	8\	.type	0x3\	.endef
	.def	Xpos\	.val	2\	.scl	8\	.type	0x9\	.size	4\	.tag	.2fake\	.endef
	.def	Ypos\	.val	6\	.scl	8\	.type	0x9\	.size	4\	.tag	.3fake\	.endef
	.def	chBase\	.val	10\	.scl	8\	.type	0xd\	.endef
	.def	anim_time\	.val	12\	.scl	8\	.type	0x3\	.endef
	.def	Frame\	.val	14\	.scl	8\	.type	0x1d\	.endef
	.def	behavior\	.val	18\	.scl	8\	.type	0x3\	.endef
	.def	anim_next\	.val	20\	.scl	8\	.type	0x3\	.endef
	.def	control_timer\	.val	22\	.scl	8\	.type	0x3\	.endef
	.def	.eos\	.val	24\	.scl	102\	.size	24\	.tag	.1fake\	.endef
	.def	Creature\	.scl	13\	.type	0x8\	.size	24\	.tag	.1fake\	.endef
	.def	CreaturePtr\	.scl	13\	.type	0x18\	.tag	.1fake\	.endef
	.def	.4fake\	.scl	10\	.type	0x8\	.size	8\	.endef
	.def	frame\	.val	0\	.scl	8\	.type	0x1d\	.endef
	.def	duration\	.val	4\	.scl	8\	.type	0x3\	.endef
	.def	next\	.val	6\	.scl	8\	.type	0x3\	.endef
	.def	.eos\	.val	8\	.scl	102\	.size	8\	.tag	.4fake\	.endef
	.def	AnimState\	.scl	13\	.type	0x8\	.size	8\	.tag	.4fake\	.endef
	.def	AnimStatePtr\	.scl	13\	.type	0x18\	.tag	.4fake\	.endef
	.def	.6fake\	.scl	12\	.type	0x9\	.size	4\	.endef
	.def	w\	.scl	11\	.type	0x3\	.endef
	.def	l\	.scl	11\	.type	0x5\	.endef
	.def	.eos\	.val	4\	.scl	102\	.size	4\	.tag	.6fake\	.endef
	.def	.7fake\	.scl	12\	.type	0x9\	.size	4\	.endef
	.def	w\	.scl	11\	.type	0x3\	.endef
	.def	l\	.scl	11\	.type	0x5\	.endef
	.def	.eos\	.val	4\	.scl	102\	.size	4\	.tag	.7fake\	.endef
	.def	.5fake\	.scl	10\	.type	0x8\	.size	10\	.endef
	.def	data\	.val	0\	.scl	8\	.type	0x9\	.size	4\	.tag	.6fake\	.endef
	.def	control\	.val	4\	.scl	8\	.type	0x9\	.size	4\	.tag	.7fake\	.endef
	.def	h_count\	.val	8\	.scl	8\	.type	0xc\	.endef
	.def	v_count\	.val	9\	.scl	8\	.type	0xc\	.endef
	.def	.eos\	.val	10\	.scl	102\	.size	10\	.tag	.5fake\	.endef
			;#include "types.h"
			;#include "defs.h"
			;#include "vars.h"
			;#include "proto.h"
			;#include "io.h"
			;
			;
	.text
	.align	2
	.globl	_InterpolateColor
	.def	_InterpolateColor\	.val	_InterpolateColor\	.scl	2\	.type	0x10020\	.endef
_InterpolateColor:
	movem.l	d3-d7/a2-a5,-(sp)
	lea 	-12(sp),sp
	move.w	52(sp),d3
	move.w	54(sp),d4
	move.w	56(sp),d5
	movea.l	58(sp),a2
	.ln 	1
	.def	.bf\	.val	.\	.scl	101\	.line	8\	.endef
	.def	target\	.val	3\	.scl	17\	.type	0x3\	.endef
	.def	time\	.val	4\	.scl	17\	.type	0x3\	.endef
	.def	duration\	.val	5\	.scl	17\	.type	0x3\	.endef
	.def	color\	.val	10\	.scl	17\	.type	0x13\	.endef
			;void InterpolateColor(short target, short time, short duration, short *color){
			;short dr, dg, db;
			;short tr, tg, tb;
			;short r, g, b;
			;int i;
			;
			;    r = *color & 0x00e;
	.ln 	7
	move.w	(a2),d6
	andi.b	#14,d6
	ext.w	d6
			;    g = (*color & 0x0e0) >> 4;
	.ln 	8
	move.w	(a2),d0
	andi.w	#224,d0
	asr.w	#4,d0
	move.w	d0,d7
			;    b = (*color & 0xe00) >> 8;
	.ln 	9
	move.w	(a2),d0
	andi.w	#3584,d0
	asr.w	#8,d0
	move.w	d0,10(sp)
			;    tr = target & 0x00e;
	.ln 	10
	move.b	d3,d0
	andi.b	#14,d0
	ext.w	d0
	movea.w	d0,a3
			;    tg = (target & 0x0e0) >> 4;
	.ln 	11
	move.b	d3,d0
	andi.w	#224,d0
	asr.w	#4,d0
	movea.w	d0,a4
			;    tb = (target & 0xe00) >> 8;
	.ln 	12
	andi.w	#3584,d3
	asr.w	#8,d3
	movea.w	d3,a5
			;    dr = r - tr + 1;
	.ln 	13
	sub.w	a3,d6
	addq.w	#1,d6
	move.w	d6,d3
			;    dg = g - tg + 1;
	.ln 	14
	sub.w	a4,d7
	addq.w	#1,d7
	move.w	d7,d6
			;    db = b - tb + 1;
	.ln 	15
	move.w	10(sp),d0
	sub.w	a5,d0
	addq.w	#1,d0
	move.w	d0,d7
			;    r = tr + (short)(dr * time) / duration;
	.ln 	16
	muls.w	d4,d3
	ext.l	d3
	divs.w	d5,d3
	adda.w	d3,a3
	move.w	a3,d3
			;    g = tg + (dg * time) / duration;
	.ln 	17
	muls.w	d4,d6
	move.l	d6,d1
	move.w	d5,d0
	ext.l	d0
	jsr 	__ds32s32
	adda.w	d1,a4
	move.w	a4,d6
			;    b = tb + (db * time) / duration;
	.ln 	18
	muls.w	d4,d7
	move.l	d7,d1
	move.w	d5,d0
	ext.l	d0
	jsr 	__ds32s32
	adda.w	d1,a5
	move.w	a5,d4
			;    *color = (r & 0x00e) | ((g & 0x00e) << 4) | ((b & 0x00e) << 8);
	.ln 	19
	andi.b	#14,d3
	andi.b	#14,d6
	ext.w	d6
	lsl.w	#4,d6
	ext.w	d3
	or.w	d6,d3
	andi.b	#14,d4
	ext.w	d4
	lsl.w	#8,d4
	or.w	d4,d3
	move.w	d3,(a2)
	.def	.ef\	.val	.\	.scl	101\	.line	27\	.endef
	lea 	12(sp),sp
			;}
	.ln 	20
	movem.l	(sp)+,d3-d7/a2-a5
	rts	
	.def	_InterpolateColor\	.val	.\	.scl	-1\	.endef
			;
	.align	2
	.globl	_InterpolatePalette
	.def	_InterpolatePalette\	.val	_InterpolatePalette\	.scl	2\	.type	0x10020\	.endef
_InterpolatePalette:
	movem.l	d3-d6/a2,-(sp)
	move.w	24(sp),d3
	move.w	26(sp),d4
	move.w	28(sp),d5
	.ln 	1
	.def	.bf\	.val	.\	.scl	101\	.line	29\	.endef
	.def	target\	.val	3\	.scl	17\	.type	0x3\	.endef
	.def	time\	.val	4\	.scl	17\	.type	0x3\	.endef
	.def	duration\	.val	5\	.scl	17\	.type	0x3\	.endef
	.def	palette\	.val	14\	.scl	9\	.type	0x13\	.endef
			;void InterpolatePalette(short target, short time, short duration, short *palette){
			;int i;
			;
			;    for(i = 0; i < 16; i++){
	.ln 	4
	moveq	#0,d6
	movea.l	30(sp),a2
L12:
			;        InterpolateColor(target, time, duration, &palette[i]);
	.ln 	5
	move.l	a2,-(sp)
	move.w	d5,-(sp)
	move.w	d4,-(sp)
	move.w	d3,-(sp)
	jsr 	_InterpolateColor
	lea 	10(sp),sp
	.ln 	6
	addq.l	#2,a2
	addq.l	#1,d6
	moveq	#16,d0
	cmp.l	d0,d6
	blt 	L12
	.def	.ef\	.val	.\	.scl	101\	.line	35\	.endef
			;    }
			;}
	.ln 	7
	movem.l	(sp)+,d3-d6/a2
	rts	
	.def	_InterpolatePalette\	.val	.\	.scl	-1\	.endef
			;
	.align	2
	.globl	_InterpolateFullPalette
	.def	_InterpolateFullPalette\	.val	_InterpolateFullPalette\	.scl	2\	.type	0x10020\	.endef
_InterpolateFullPalette:
	movem.l	d3-d6/a2,-(sp)
	move.w	24(sp),d3
	move.w	26(sp),d4
	move.w	28(sp),d5
	.ln 	1
	.def	.bf\	.val	.\	.scl	101\	.line	37\	.endef
	.def	target\	.val	3\	.scl	17\	.type	0x3\	.endef
	.def	time\	.val	4\	.scl	17\	.type	0x3\	.endef
	.def	duration\	.val	5\	.scl	17\	.type	0x3\	.endef
	.def	palette\	.val	14\	.scl	9\	.type	0x13\	.endef
			;void InterpolateFullPalette(short target, short time, short duration, short *palette){
			;int i;
			;
			;    for(i = 0; i < 64; i++){
	.ln 	4
	moveq	#0,d6
	movea.l	30(sp),a2
L26:
			;        InterpolateColor(target, time, duration, &palette[i]);
	.ln 	5
	move.l	a2,-(sp)
	move.w	d5,-(sp)
	move.w	d4,-(sp)
	move.w	d3,-(sp)
	jsr 	_InterpolateColor
	lea 	10(sp),sp
	.ln 	6
	addq.l	#2,a2
	addq.l	#1,d6
	moveq	#64,d0
	cmp.l	d0,d6
	blt 	L26
	.def	.ef\	.val	.\	.scl	101\	.line	43\	.endef
			;    }
			;}
	.ln 	7
	movem.l	(sp)+,d3-d6/a2
	rts	
	.def	_InterpolateFullPalette\	.val	.\	.scl	-1\	.endef
			;
	.align	2
	.globl	_Interpolate2Palettes
	.def	_Interpolate2Palettes\	.val	_Interpolate2Palettes\	.scl	2\	.type	0x10020\	.endef
_Interpolate2Palettes:
	movem.l	d3-d5/a2-a3,-(sp)
	move.w	28(sp),d3
	move.w	30(sp),d4
	.ln 	1
	.def	.bf\	.val	.\	.scl	101\	.line	45\	.endef
	.def	target\	.val	8\	.scl	9\	.type	0x13\	.endef
	.def	time\	.val	3\	.scl	17\	.type	0x3\	.endef
	.def	duration\	.val	4\	.scl	17\	.type	0x3\	.endef
	.def	palette\	.val	16\	.scl	9\	.type	0x13\	.endef
			;void Interpolate2Palettes(short *target, short time, short duration, short *palette){
			;int i;
			;
			;    for(i = 0; i < 16; i++){
	.ln 	4
	moveq	#0,d5
	movea.l	32(sp),a2
	movea.l	24(sp),a3
L40:
			;        InterpolateColor(target[i], time, duration, &palette[i]);
	.ln 	5
	move.l	a2,-(sp)
	move.w	d4,-(sp)
	move.w	d3,-(sp)
	move.w	(a3),-(sp)
	jsr 	_InterpolateColor
	lea 	10(sp),sp
	.ln 	6
	addq.l	#2,a2
	addq.l	#2,a3
	addq.l	#1,d5
	moveq	#16,d0
	cmp.l	d0,d5
	blt 	L40
	.def	.ef\	.val	.\	.scl	101\	.line	51\	.endef
			;    }
			;}
	.ln 	7
	movem.l	(sp)+,d3-d5/a2-a3
	rts	
	.def	_Interpolate2Palettes\	.val	.\	.scl	-1\	.endef
			;
	.align	2
	.globl	_Interpolate2FullPalettes
	.def	_Interpolate2FullPalettes\	.val	_Interpolate2FullPalettes\	.scl	2\	.type	0x10020\	.endef
_Interpolate2FullPalettes:
	movem.l	d3-d5/a2-a3,-(sp)
	move.w	28(sp),d3
	move.w	30(sp),d4
	.ln 	1
	.def	.bf\	.val	.\	.scl	101\	.line	53\	.endef
	.def	target\	.val	8\	.scl	9\	.type	0x13\	.endef
	.def	time\	.val	3\	.scl	17\	.type	0x3\	.endef
	.def	duration\	.val	4\	.scl	17\	.type	0x3\	.endef
	.def	palette\	.val	16\	.scl	9\	.type	0x13\	.endef
			;void Interpolate2FullPalettes(short *target, short time, short duration, short *palette){
			;int i;
			;
			;    for(i = 0; i < 64; i++){
	.ln 	4
	moveq	#0,d5
	movea.l	32(sp),a2
	movea.l	24(sp),a3
L54:
			;        InterpolateColor(target[i], time, duration, &palette[i]);
	.ln 	5
	move.l	a2,-(sp)
	move.w	d4,-(sp)
	move.w	d3,-(sp)
	move.w	(a3),-(sp)
	jsr 	_InterpolateColor
	lea 	10(sp),sp
	.ln 	6
	addq.l	#2,a2
	addq.l	#2,a3
	addq.l	#1,d5
	moveq	#64,d0
	cmp.l	d0,d5
	blt 	L54
	.def	.ef\	.val	.\	.scl	101\	.line	59\	.endef
			;    }
			;}
	.ln 	7
	movem.l	(sp)+,d3-d5/a2-a3
	rts	
	.def	_Interpolate2FullPalettes\	.val	.\	.scl	-1\	.endef
			;
	.align	2
	.globl	_FadeIn
	.def	_FadeIn\	.val	_FadeIn\	.scl	2\	.type	0x10020\	.endef
_FadeIn:
	movem.l	d3-d5/a2-a5,-(sp)
	lea 	-132(sp),sp
	movea.l	164(sp),a4
	move.w	168(sp),d5
	.ln 	1
	.def	.bf\	.val	.\	.scl	101\	.line	61\	.endef
	.def	palette\	.val	12\	.scl	17\	.type	0x13\	.endef
	.def	time\	.val	5\	.scl	17\	.type	0x3\	.endef
			;void FadeIn(short *palette, short time){
			;int i, j;
			;short workpal[64];
			;
			;    for(i = 1; i <= time; i++){
	.ln 	5
	moveq	#1,d3
	lea 	_IORequestBuffer,a5
	bra.s	L58
L68:
			;        for(j = 0; j < 64; j++){
	.ln 	6
	moveq	#0,d4
	movea.l	a4,a3
	lea 	4(sp),a2
L82:
	moveq	#64,d0
L86:
			;            workpal[j] = palette[j];
	.ln 	7
	move.w	(a3)+,(a2)+
			;        }
	.ln 	8
	addq.l	#1,d4
	cmp.l	d0,d4
	blt 	L86
			;        InterpolateFullPalette(0, i, time, workpal);
	.ln 	9
	lea 	4(sp),a0
	move.l	a0,(sp)
	move.w	d5,-(sp)
	move.w	d3,-(sp)
	move.w	#0,-(sp)
	jsr 	_InterpolateFullPalette
	addq.l	#6,sp
			;        SendPalettes(workpal);
	.ln 	10
	move.w	_IORequestPointer,d0
	add.w	d0,d0
	move.w	#3,0(a5,d0.w)
	lea 	4(sp),a0
	move.w	_IORequestPointer,d0
	add.w	d0,d0
	move.l	a0,2(a5,d0.w)
	move.w	_IORequestPointer,d0
	addq.w	#8,d0
	andi.w	#255,d0
	move.w	d0,_IORequestPointer
			;        WaitTOF();
	.ln 	11
	jsr 	_WaitTOF
			;    }
	.ln 	12
	addq.l	#1,d3
L58:
	ext.l	d5
	cmp.l	d5,d3
	ble 	L68
	.def	.ef\	.val	.\	.scl	101\	.line	73\	.endef
	lea 	132(sp),sp
			;}
	.ln 	13
	movem.l	(sp)+,d3-d5/a2-a5
	rts	
	.def	_FadeIn\	.val	.\	.scl	-1\	.endef
			;
	.align	2
	.globl	_FadeOut
	.def	_FadeOut\	.val	_FadeOut\	.scl	2\	.type	0x10020\	.endef
_FadeOut:
	movem.l	d3-d5/a2-a5,-(sp)
	lea 	-132(sp),sp
	movea.l	164(sp),a4
	move.w	168(sp),d5
	.ln 	1
	.def	.bf\	.val	.\	.scl	101\	.line	75\	.endef
	.def	palette\	.val	12\	.scl	17\	.type	0x13\	.endef
	.def	time\	.val	5\	.scl	17\	.type	0x3\	.endef
			;void FadeOut(short *palette, short time){
			;int i, j;
			;short workpal[64];
			;
			;    for(i = time-1; i >= 0; i--){
	.ln 	5
	move.w	d5,d3
	ext.l	d3
	subq.l	#1,d3
	lea 	_IORequestBuffer,a5
	bra.s	L88
L98:
			;        for(j = 0; j < 64; j++){
	.ln 	6
	moveq	#0,d4
	movea.l	a4,a3
	lea 	4(sp),a2
L112:
	moveq	#64,d0
L116:
			;            workpal[j] = palette[j];
	.ln 	7
	move.w	(a3)+,(a2)+
			;        }
	.ln 	8
	addq.l	#1,d4
	cmp.l	d0,d4
	blt 	L116
			;        InterpolateFullPalette(0, i, time, &workpal);
	.ln 	9
	lea 	4(sp),a0
	move.l	a0,(sp)
	move.w	d5,-(sp)
	move.w	d3,-(sp)
	move.w	#0,-(sp)
	jsr 	_InterpolateFullPalette
	addq.l	#6,sp
			;        SendPalettes(workpal);
	.ln 	10
	move.w	_IORequestPointer,d0
	add.w	d0,d0
	move.w	#3,0(a5,d0.w)
	lea 	4(sp),a0
	move.w	_IORequestPointer,d0
	add.w	d0,d0
	move.l	a0,2(a5,d0.w)
	move.w	_IORequestPointer,d0
	addq.w	#8,d0
	andi.w	#255,d0
	move.w	d0,_IORequestPointer
			;        WaitTOF();
	.ln 	11
	jsr 	_WaitTOF
			;    }
	.ln 	12
	subq.l	#1,d3
L88:
	tst.l	d3
	bpl 	L98
	.def	.ef\	.val	.\	.scl	101\	.line	87\	.endef
	lea 	132(sp),sp
			;}
	.ln 	13
	movem.l	(sp)+,d3-d5/a2-a5
	rts	
	.def	_FadeOut\	.val	.\	.scl	-1\	.endef
