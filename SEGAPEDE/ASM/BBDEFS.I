
* $Header$

* Memory Map

ROM             equ     $0

* Z80 equates
Z80RAM          equ     $a00000
YAMAHA          equ     $a04000
Z80BANK         equ     $a06000
Z80BUSREQ       equ     $a11100
Z80RESET        equ     $a11200

* CIA equates
VERSION         equ     $a10001
JOY1PORT        equ     $a10003
JOY2PORT        equ     $a10005
COMPORT         equ     $a10007         ; Sega calls this EXP
JOY1DDR         equ     $a10009         ; data direction register
JOY2DDR         equ     $a1000b         ; data direction register
COMDDR          equ     $a1000d         ; data direction register

* Watchdog circuit (protection)
WATCHDOG        equ     $a14000

CUSTOM          equ     $c00000
VDATA           equ     0
VCTRL           equ     4

TANDY           equ     $c00011

STACKTOP        equ     $fff800
STACKRAM        equ     $ff0000
ZEROPAGE        equ     $fff800


*
* System Memory Map
*

************************************************************************
* VDP access codes
VRAMWR          equ     $40000000
VRAMRD          equ     $00000000
VSRAMWR         equ     $40000010
VSRAMRD         equ     $00000010
CRAMWR          equ     $c0000000
CRAMRD          equ     $00000020

* VDP allocation (temporary)
FG              equ     $c000           ; foreground    128 cells
FOREGROUND      equ     FG
SIDEBAR         equ     $a000           ; window map
BG              equ     $e000           ; background    128 cells
BACKGROUND      equ     BG
SP_ATTR         equ     $d000           ; sprite parameters
HSCROLL         equ     $d400           ; scroll parameters
W_CELLS         equ     $3440           ; window cells
FG_CELLS        equ     $0000           ; the foreground cells
BG_CELLS        equ     $0000           ; the background cells
SP_CELLS        equ     $D880           ; sprite cells

* Palette codes for sprites and cells
PALETTE0        equ     $0000
PALETTE1        equ     $2000
PALETTE2        equ     $4000
PALETTE3        equ     $6000

* Priority definitions
A_PRI_HI        equ     $8000
A_PRI_LO        equ     0
B_PRI_HI        equ     $4000
B_PRI_LO        equ     0
PRIORITY_HI     equ     $8000
PRIORITY_LO     equ     0

* Various vectors
WORKRAM         equ     $ffff0000
BUFFERRAM       equ     $ffff0000
STACKBOTTOM     equ     $fffff000

HFlip           equ     $0800
VFlip           equ     $1000

* Joystick Bits

JOYUP           equ     0
JOYDOWN         equ     1
JOYLEFT         equ     2
JOYRIGHT        equ     3
JOYBUTTONB      equ     4
JOYBUTTONC      equ     5
JOYBUTTONA      equ     6
JOYSTART        equ     7

********************************************************************************
************************ D A T A   S T R U C T U R E S *************************
********************************************************************************


*******************************************************************************
********************************** MACROS *************************************
*******************************************************************************

push            MACRO
                move.\0 \1,-(sp)
                ENDM

pop             MACRO
                move.\0 (sp)+,\1
                ENDM

FORBID          MACRO
                jsr     _Forbid
                ENDM

PERMIT          MACRO
                jsr     _Permit
                ENDM

DISABLE         MACRO
                move.w  #$2700,SR
*                move.w  #$8134,VCTRL(a6)
                ENDM

ENABLE          MACRO
*                move.w  #$8174,VCTRL(a6)
                move.w  #$2300,SR		* 16sept92 dbs
                ENDM

* VDPDEST vdpaddr
VDPDEST         MACRO
                move.l  #$40000000+(((\1)<<16)&$3fff0000)+(((\1)/16384)&3),VCTRL(a6)
                ENDM

* DMA src,dst,bytes
DMA             MACRO
_dmasrc         set     (\1/2)
_dmalo          set     (_dmasrc&$ff)<<16
_dmamid         set     (_dmasrc&$ff00)>>8
_dmahi          set     (_dmasrc>>16)&$7f
_dmasize        set     \3/2
                jsr     GetZ80Bus         ;_ENTRY
                move.l  #$93009400+((_dmasize&$ff00)>>8)+((_dmasize&$ff)<<16),VCTRL(a6)
                move.l  #$95009600+_dmalo+_dmamid,VCTRL(a6)
                move.w  #$9700+_dmahi,VCTRL(a6)
                move.l  #$40000080+((\2<<16)&$3fff0000)+((\2/16384)&3),DMARAM
                move.w  DMARAM,VCTRL(a6)
                move.w  DMARAM+2,VCTRL(a6)
                jsr     RetZ80Bus       ;_ENTRY
                ENDM

* VDPREGI  vdp_reg,immediate_data
*
*
*	Writes immediate data (1 byte) to vdp_reg number
*
*	Assumes A6 -> CUSTOM
*
* History:
* --------
* 8 may 1992	dave sanner	created.
*

VDPREGI		MACRO
		move.w	#$8000+(((\1)<<8)+(\2)),$c00004
		ENDM


