* Modified from Dark Empires kernel
*
* $Header$
*

*************************************************************************************************************************************
*************************************************************************************************************************************
*
********           *******                      ***
* **   **           **   **                      **
* **    **          **    **                     **
* **   **           **   **                      **
* ******    *****   ******    ****  *** **  **   *****
* **   **   *****   **   **  **  **  *** *** **  **  **
* **    **          **    ** **  **  **  **  **  **   **
* **   **           **   **  **  **  **  **  **  **  **
********           *******    ****  ***  **  **  *****
*
*
*
*  ****                                    **
* **  **
***        ****   *****    ****    *****  ***    *****
***  ***  **  **  **  **  **  **  **       **   **
***   **  ******  **  **  ******   ****    **    ****
* **  **  **      **  **  **          **   **       **
*  *****   ****   **  **   ****   *****   ****  *****
*
*
*
****  **                                  ***
* **  **                                   **
* ** **    ****  *** **   *****    ****    **
* ****    **  **  *** **  **  **  **  **   **
* ** **   ******  **  **  **  **  ******   **
* **  **  **      **      **  **  **       **
****  **   ****  ****     **  **   ****   ****


        xref _main
        xref _random
        xref _tickCounter
        xref _vblFlag
        xref _CurrentVBHandler
        xref _CurrentHBHandler
        xref _HBCount
        xref _HBSave
*       xref _colorTable
_colorTable     equ $00ff0000
*       xref _colorSave
_colorSave      equ $00ff0080
*       xref _StartGame
        xref _dStick1
        xref _dStick2
        xref _bStick1
        xref _bStick2
        xref _cStick1
        xref _cStick2
        xref _GameLoop
        xref _gemsdmastart
        xref _gemsdmaend
        xref _gemsholdz80
        xref _gemsreleasez80
        xref _gemsinit
        xref _patchbank
        xref _sampbank
        xref _seqbank
        xref _envbank

        OPT iopt
        OPT equ
        OPT case
        OPT fwdsize
        OPT mex

DEFINE_GLOBALS  equ 1
        INCLUDE "bbdefs.i"
        PAGE
*************************************************************************************************************************************
*************************************************************************************************************************************
*
*   ***    ****    ****    ****    ****
*  **     **  **  **  **  **  **  **  **
* **      **  **  ** ***  ** ***  ** ***
* *****    ****   ******  ******  ******
* **  **  **  **  *** **  *** **  *** **
* **  **  **  **  **  **  **  **  **  **
*  ****    ****    ****    ****    ****
*
***    **                    *
***    **                   **
* **  **   ****    ****    *****   ****  *** **    *****
* **  **  **  **  **  **    **    **  **  *** **  **
*  ****   ******  **        **    **  **  **  **   ****
*  ****   **      **  **    ** *  **  **  **          **
*   **     ****    ****      **    ****  ****     *****


                dc.l    STACKTOP                ; 0     initial SP
                dc.l    ColdStart               ; 1     initial PC
                dc.l    ErrorHandler2           ; 2     Bus Error
                dc.l    ErrorHandler3           ; 3     Address Error
                dc.l    ErrorHandler4           ; 4     Illegal Instruction
                dc.l    ErrorHandler5           ; 5     Zero Divide
                dc.l    ErrorHandler6           ; 6     CHK Instruction
                dc.l    ErrorHandler7           ; 7     TRAPV Instrction
                dc.l    ErrorHandler8           ; 8     Privilege Violation
                dc.l    ErrorHandler9           ; 9     Trace
                dc.l    ErrorHandler10          ; 10    Line 1010 Emulator
                dc.l    ErrorHandler11          ; 11    Line 1111 Emualtor
                dc.l    ErrorHandler12          ; 12    Unassigned Reserved
                dc.l    ErrorHandler13          ; 13    Unassigned Reserved
                dc.l    ErrorHandler14          ; 14    Unassigned Reserved
                dc.l    ErrorHandler15          ; 15    Unitialized interrupt Vector
* unassigned/reserved
                dc.l    ErrorHandler16,ErrorHandler17,ErrorHandler18,ErrorHandler19
                dc.l    ErrorHandler20,ErrorHandler21,ErrorHandler22,ErrorHandler23
* autovectors
                dc.l    ErrorHandler24          ; 24    Spurious Interrupts
                dc.l    ErrorHandler25          ; 25    Level 1 I Autovector
                dc.l    ErrorHandler26          ; 26    Level 2 I Autovector
                dc.l    ErrorHandler27          ; 27    Level 3 I Autovector
                dc.l    HBLHandler              ; 28    Level 4 I Autovector
                dc.l    ErrorHandler28          ; 29    Level 5 I Autovector
                dc.l    VBLHandler              ; 30    Level 6 I Autovector
                dc.l    ColdStart               ; 31    Level 7 I Autovector
* trap handlers
                dc.l    ErrorHandler30,ErrorHandler31,ErrorHandler32,ErrorHandler33
                dc.l    ErrorHandler34,ErrorHandler35,ErrorHandler36,ErrorHandler37
                dc.l    ErrorHandler38,ErrorHandler39,ErrorHandler40,ErrorHandler41
                dc.l    ErrorHandler42,ErrorHandler43,ErrorHandler44,ErrorHandler45
* unassigned error vectors
                dc.l    ErrorHandler46,ErrorHandler47,ErrorHandler48,ErrorHandler49
                dc.l    ErrorHandler50,ErrorHandler51,ErrorHandler52,ErrorHandler53
                dc.l    ErrorHandler54,ErrorHandler55,ErrorHandler56,ErrorHandler57
                dc.l    ErrorHandler58,ErrorHandler59,ErrorHandler60,ErrorHandler61
* standard page 1 Genesis image
*               dc.b    "SEGA MEGA DRIVE "      ;100
        dc.b 83,69,71,65,32,77,69,71,65,32,68,82,73,86,69,32
*               dc.b    "(C)SEGA 1992 JUL"      ;110
        dc.b 40,67,41,83,69,71,65,32,49,57,57,50,32,74,85,76
*               dc.b    "B-BOMB          "      ;120
        dc.b 66,45,66,79,77,66,32,32,32,32,32,32,32,32,32,32
*               dc.b    "                "      ;130
        dc.b 32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32
*               dc.b    "                "      ;140
        dc.b 32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32
*               dc.b    "B-BOMB          "      ;150
        dc.b 66,45,66,79,77,66,32,32,32,32,32,32,32,32,32,32
*               dc.b    "                "      ;160
        dc.b 32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32
*               dc.b    "                "      ;170
        dc.b 32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32
*               dc.b    "GM 00001014-00"        ;180
        dc.b 71,77,32,48,48,48,48,49,48,49,52,45,48,48
                dc.w    $0000                   ;18e checksum
*               dc.b    "J               "      ;190
        dc.b 74,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32
                dc.l    $000000,$07ffff         ;1a0
                dc.l    $ff0000,$ffffff         ;1a8
*               dc.b    "                "      ;1b0
        dc.b 32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32
*               dc.b    "                "      ;1c0
        dc.b 32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32
*               dc.b    "                "      ;1d0
        dc.b 32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32
*               dc.b    "                "      ;1e0
        dc.b 32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32
*               dc.b    "JUE             "      ;1f0
        dc.b 74,85,69,32,32,32,32,32,32,32,32,32,32,32,32,32

************************************************************************************************************************************

* Exception Handlers (for debugging purposes)

ErrorHandler:   FORBID
                move.l  2(sp),a0
                move.l  CUSTOM,a6
eh10:           move.l  #$c0000000,VCTRL(a6)
                move.w  d1,VDATA(a6)
                addq    #1,d1
                bra.s   eh10

ErrorHandler2:  move.w  #2,d0
                bra     ErrorHandler
ErrorHandler3:  move.w  #3,d0
                bra     ErrorHandler
ErrorHandler4:  move.w  #4,d0
                bra     ErrorHandler
ErrorHandler5:  move.w  #5,d0
                bra     ErrorHandler
ErrorHandler6:  move.w  #6,d0
                bra     ErrorHandler
ErrorHandler7:  move.w  #7,d0
                bra     ErrorHandler
ErrorHandler8:  move.w  #8,d0
                bra     ErrorHandler
ErrorHandler9:  move.w  #9,d0
                bra     ErrorHandler
ErrorHandler10: move.w  #10,d0
                bra     ErrorHandler
ErrorHandler11: move.w  #11,d0
                bra     ErrorHandler
ErrorHandler12: move.w  #12,d0
                bra     ErrorHandler
ErrorHandler13: move.w  #13,d0
                bra     ErrorHandler
ErrorHandler14: move.w  #14,d0
                bra     ErrorHandler
ErrorHandler15: move.w  #15,d0
                bra     ErrorHandler
ErrorHandler16: move.w  #16,d0
                bra     ErrorHandler
ErrorHandler17: move.w  #17,d0
                bra     ErrorHandler
ErrorHandler18: move.w  #18,d0
                bra     ErrorHandler
ErrorHandler19: move.w  #19,d0
                bra     ErrorHandler
ErrorHandler20: move.w  #20,d0
                bra     ErrorHandler
ErrorHandler21: move.w  #21,d0
                bra     ErrorHandler
ErrorHandler22: move.w  #22,d0
                bra     ErrorHandler
ErrorHandler23: move.w  #23,d0
                bra     ErrorHandler
ErrorHandler24: move.w  #24,d0
                bra     ErrorHandler
ErrorHandler25: move.w  #25,d0
                bra     ErrorHandler
ErrorHandler26: move.w  #26,d0
                bra     ErrorHandler
ErrorHandler27: move.w  #27,d0
                bra     ErrorHandler
ErrorHandler28: move.w  #28,d0
                bra     ErrorHandler
ErrorHandler29: move.w  #29,d0
                bra     ErrorHandler
ErrorHandler30: move.w  #30,d0
                bra     ErrorHandler
ErrorHandler31: move.w  #31,d0
                bra     ErrorHandler
ErrorHandler32: move.w  #32,d0
                bra     ErrorHandler
ErrorHandler33: move.w  #33,d0
                bra     ErrorHandler
ErrorHandler34: move.w  #34,d0
                bra     ErrorHandler
ErrorHandler35: move.w  #35,d0
                bra     ErrorHandler
ErrorHandler36: move.w  #36,d0
                bra     ErrorHandler
ErrorHandler37: move.w  #37,d0
                bra     ErrorHandler
ErrorHandler38: move.w  #38,d0
                bra     ErrorHandler
ErrorHandler39: move.w  #39,d0
                bra     ErrorHandler
ErrorHandler40: move.w  #40,d0
                bra     ErrorHandler
ErrorHandler41: move.w  #41,d0
                bra     ErrorHandler
ErrorHandler42: move.w  #42,d0
                bra     ErrorHandler
ErrorHandler43: move.w  #43,d0
                bra     ErrorHandler
ErrorHandler44: move.w  #44,d0
                bra     ErrorHandler
ErrorHandler45: move.w  #45,d0
                bra     ErrorHandler
ErrorHandler46: move.w  #46,d0
                bra     ErrorHandler
ErrorHandler47: move.w  #47,d0
                bra     ErrorHandler
ErrorHandler48: move.w  #48,d0
                bra     ErrorHandler
ErrorHandler49: move.w  #49,d0
                bra     ErrorHandler
ErrorHandler50: move.w  #50,d0
                bra     ErrorHandler
ErrorHandler51: move.w  #51,d0
                bra     ErrorHandler
ErrorHandler52: move.w  #52,d0
                bra     ErrorHandler
ErrorHandler53: move.w  #53,d0
                bra     ErrorHandler
ErrorHandler54: move.w  #54,d0
                bra     ErrorHandler
ErrorHandler55: move.w  #55,d0
                bra     ErrorHandler
ErrorHandler56: move.w  #56,d0
                bra     ErrorHandler
ErrorHandler57: move.w  #57,d0
                bra     ErrorHandler
ErrorHandler58: move.w  #58,d0
                bra     ErrorHandler
ErrorHandler59: move.w  #59,d0
                bra     ErrorHandler
ErrorHandler60: move.w  #60,d0
                bra     ErrorHandler
ErrorHandler61: move.w  #61,d0
                bra     ErrorHandler

************************************************************************************************************************************

* Interrupt handlers

*
* VBLHandler
*
* Synopsis:
*       This is the vertical blank handler.  The VDP is NOT accessed
*       during interrupt context to prevent the interrupt routine from
*       interfering with the application's use of the VDP.
*
VBLHandler:     addq.l  #1,_tickCounter         * increment global time counter
                st      _vblFlag                * set flag to indicate VBL occured
                movem.l a0-a6/d0-d7,-(sp)
                move.l  _CurrentVBHandler,a0
                jsr     (a0)

                move.w  #0,_HBCount
                VDPREGI 10,0
                VDPREGI 0,$14                   * %00010100
                VDPREGI 1,$74
                movem.l (sp)+,a0-a6/d0-d7
                rte

*
* HBLHandler
*
* Synopsis:
*       This is the horizontal blank handler.  No HBL interrupts are used,
*       so this handler merely changes the interrupt mask to 5 to prevent
*       further horizontal blank interrupts from occuring.
*
*HBLHandler:     and.w   #$f0ff,(sp)
*                or.w    #$0500,(sp)
*
***
*  modified 15sept92 dbs
***

HBLHandler:
                move.l a0,-(sp)
                move.l d0,-(sp)
                move.l  _CurrentHBHandler,a0
                jsr     (a0)
                move.l (sp)+,d0
                move.l (sp)+,a0
                rte

*
*  turn back on the VDP
*
                XDEF _TopHBHandler

_TopHBHandler:
                addq.w  #1,_HBCount
                cmpi.w  #198,_HBCount
                bne.b   CheckLowHB
                VDPREGI 1,$34
                rts
CheckLowHB:
*               cmp.w   #40,_HBCount
*               bne.b   _ByeHBHandler
_ByeHBHandler:
                rts

_BottomHBHandler:
                VDPREGI 1,$34           * Turn off the VDP display
                VDPREGI 10,$ff
                move.l  #_TopHBHandler,_CurrentHBHandler
                rts

                PAGE
*************************************************************************************************************************************
*************************************************************************************************************************************
*
*  ****            ***       ***   ****      *                       *
* **  **            **        **  **  **    **                      **
***        ****     **     ** **  ***      *****   ****  *** **    *****
***       **  **    **    ** ***   ***      **        **  *** **    **
***       **  **    **    **  **     ***    **      ****  **  **    **
* **  **  **  **    **    **  **  **  **    ** *  **  **  **        ** *
*  ****    ****    ****    *** **  ****      **    *** ******        **
*
*
********************************************************************************
*       Routine:        InitRegisters
*       Parameters:
*       Returns:
*       Synopsis:
*
********************************************************************************

_InitRegisters:
        lea     irTable,a0
        moveq   #19-1,d7
        move.w  #$8000,d0               *set register write bit
ir10:
        move.b  (a0)+,d0                *get byte from table
        move.w  d0,VCTRL(a6)            *send to VCTRL
        add.w   #$100,d0                *select next register
        dbra    d7,ir10                 *do it several times

        rts

irTable:
        dc.b %00010100          *reg 0  graphic 5,h int on
        dc.b %01110100          *reg 1  display on, v int on, DMA on
        dc.b FG>>10             *reg 2  scroll_a base address = $00000
        dc.b SIDEBAR>>10        *reg 3  window base address = $00000
        dc.b BG>>13             *reg 4  scroll_b base address = $02000
        dc.b SP_ATTR>>9         *reg 5  sprite atri. base address = $03000
        dc.b %00000000          *reg 6  sprite gene. base address = $00000
        dc.b %00000000          *reg 7  back drop color = palette 0 color 0
        dc.b %00000000          *reg 8  (unused)
        dc.b %00000000          *reg 9  (unused)
        dc.b %00000000          *reg 10 h_int at every line
        dc.b %00000000          *reg 11 whole screen scroll, ext int off
        dc.b %10000001          *reg 12 40 cell mode, no shadow or interlace
        dc.b HSCROLL>>10        *reg 13 h_scroll data table base address = $03400
        dc.b %00000000          *reg 14 (unused)
        dc.b %00000010          *reg 15 vram address inc. = 2 (dc.w)
        dc.b %00000001          *reg 16 a,b_scroll (v_32,h_64)
        dc.b %00000000          *reg 17 window h position
        dc.b %00000000          *reg 18   "    v position
        dc.b 0

        XDEF _InitVDP
_InitVDP:
        lea     CUSTOM,a6

        jsr     _InitRegisters

* Clear vdp memory
        bsr     _ClearVDP

* Clear CRAM
        move.l  #CRAMWR,VCTRL(a6)
        moveq   #63,d0                  *128 bytes
        moveq   #0,d1
ivdp50:
        move.w  d1,VDATA(a6)
        dbra    d0,ivdp50

* vertical scroll
        move.l  #VSRAMWR,VCTRL(a6)
        moveq   #39,d0
ivdp60:
        move.w  d1,VDATA(a6)
        dbra    d0,ivdp60
        rts

        XDEF _ColdStart
_ColdStart:
ColdStart:
        move.w  #$2700,SR
        lea     STACKTOP,sp
        lea     CUSTOM,a6
        lea     _DoNothing,a5
        move.l  a5,_GameLoop
        move.l  a5,_CurrentVBHandler
        move.l  a5,_CurrentHBHandler            * 15sept92 dbs

* security
        move.b  VERSION,d0
        andi.b  #$f,d0
        beq.s   cs10
        move.l  #'SEGA',WATCHDOG

* ColdStart controllers
cs10:
        moveq   #$40,d0
        move.b  d0,JOY1DDR
        move.b  d0,JOY2DDR
        move.b  #$1f,COMDDR
        move.b  #$7f,COMPORT

* clear scratch ram (64K)
        lea     WORKRAM,a0
        move.w  #16384-1,d0             * # longdc.ws in 64K
        moveq   #0,d1
cs30:
        move.l  d1,(a0)+
        dbra    d0,cs30

* ColdStart VDP (load initial values from table)
        jsr     _InitVDP

        lea     _DoNothing,a0
        move.l  a0,_CurrentVBHandler
        move.l  a0,_GameLoop
        move.l  a0,_CurrentHBHandler
        move.l  #_sampbank,-(a7)
        move.l  #_seqbank,-(a7)
        move.l  #_envbank,-(a7)
        move.l  #_patchbank,-(a7)
        jsr     _gemsinit
        lea     STACKTOP,sp

        move.w  #$2300,SR

*        jsr     _SegaLogo                      *15sept92 dbs
*        jsr     _FadePalette                   *15sept92 dbs
        jmp     _main

        XDEF _DoNothing
_DoNothing:
        rts
        PAGE
*************************************************************************************************************************************
*************************************************************************************************************************************
*
*  ****                                  ****
* **  **                                  **
* ***      ****    *** **  ****           **       ****    *** **  ****
*  ***    **  **  **  **      **          **      **  **  **  **  **  **
*    ***  ******  **  **    ****          **   *  **  **  **  **  **  **
* **  **  **       ****   **  **          **  **  **  **   ****   **  **
*  ****    ****  **   **   *** **        *******   ****  **   **   ****
*                 *****                                   *****
*
LOGO1           equ   (FG+$61c+$0000)
LOGO2           equ   (FG+$61c+$0080)
LOGO3           equ   (FG+$61c+$0100)
LOGO4           equ   (FG+$61c+$0180)

        XDEF _SegaLogo
_SegaLogo:
        movem.l d0-d7/a0-a6,-(sp)
        lea     CUSTOM,a6
        DISABLE
        lea     logoData(pc),a0
        move.w  #100-1,d0

* load character set into VRAM
        VDPDEST FG_CELLS+32
sl10:
        move.l  (a0)+,VDATA(a6)
        move.l  (a0)+,VDATA(a6)
        move.l  (a0)+,VDATA(a6)
        move.l  (a0)+,VDATA(a6)
        dbra    d0,sl10

* load Color Palette into work ram
        lea     logoColors(pc),a2
        moveq   #0,d7                   *clear register
        move.b  (a2)+,d7                *read ram address
        lea     _colorTable,a3          *get address in work ram
        adda    d7,a3                   *add God-knows-what to address
        move.b  (a2)+,d7                *read counter
sl20:
        move.w  (a2)+,(a3)+             *move data into work ram
        dbra    d7,sl20

* load Map into VRAM
        lea     logoMap(pc),a0          *get address of table
        VDPDEST LOGO1                   *do VRAMWR and set address
        bsr.s   sl30
        VDPDEST LOGO2
        bsr.s   sl30
        VDPDEST LOGO3
        bsr.s   sl30
        VDPDEST LOGO4
        bsr.s   sl30
        bra.s   CycleLogo
sl30:
        moveq   #12-1,d0
sl40:
        move.w  (a0)+,d1
        addi.w  #(FG_CELLS>>5),d1
        move.w  d1,VDATA(a6)            *move the data there
        dbra    d0,sl40
        rts
CycleLogo:
        ENABLE
        bsr     _UnfadePalette
        clr.b   _dStick1
        move.w  #10,d2
cl10:
        bsr     _WaitTOF
*       bsr     _EventManager
*       bclr    #JOYSTART,_dStick1
*       bne.s   cl99
        FORBID
        bsr     _PaletteDMA
        PERMIT
        dbra    d2,cl10

        moveq   #40,d3                  * mode_count
        move.w  #128,d2                 * time to display logo for
        moveq   #20,d1                  * clch_tmr
cl20:
*       move.l  d1,-(sp)
        bsr     _WaitTOF
*       bsr     _EventManager
*       move.l  (sp)+,d1
*       bclr    #JOYSTART,_dStick1
*       bne.s   cl99
        jsr     _gemsdmastart
        move.l  d1,-(sp)
        bsr     _PaletteDMA
        move.l  (sp)+,d1
        jsr     _gemsdmaend
        subq    #1,d1                   * color change timer
        bpl.b   cl40
        moveq   #4-1,d1                 * color change timer

        move.w  d3,d0                   * color change counter
        bmi.s   cl40
        subq.w  #2,d3                   * color change counter

        lea     logoCycle(pc,d0),a0
        lea     _colorTable+4,a1        * colorTable[2]
        moveq   #11-1,d0
cl30:
        move.w  (a0)+,(a1)+
        dbra    d0,cl30
cl40:
        dbra    d2,cl20
*       bra.s   cl98
*cl99:   move.w  #1,_StartGame
cl98:   movem.l (sp)+,d0-d7/a0-a6
        rts


logoColors:     dc.b    0,13-1
                dc.w    $0000,$0eee
logoCycle:      dc.w    $ec0,$ea0,$e80,$e60,$e40,$e20,$e00,$c00
                dc.w    $a00,$800,$600,$800,$a00,$c00,$e00,$e20
                dc.w    $e40,$e60,$e80,$ea0,$ec0,$ea0,$e80,$e60
                dc.w    $e40,$e20,$e00,$c00,$a00,$800,$600

logoMap:        dc.w    $8001,$8002,$8003,$8004,$8005,$8006,$8007,$8008,$8009,$800a,$800b,$800c
                dc.w    $800d,$800e,$800f,$8010,$8011,$8012,$8013,$8014,$8015,$8016,$8017,$8018
                dc.w    $8019,$801a,$801b,$801c,$801d,$801e,$801f,$8020,$8021,$8022,$8023,$8024
                dc.w    $8025,$8026,$8027,$8028,$8029,$802a,$802b,$802c,$802d,$802e,$802f,$8030

logoData:       dc.w    $0000,$0000,$0000,$0111,$0001,$1667,$0015,$6666
                dc.w    $0155,$6666,$0155,$5666,$1555,$5661,$1455,$5516
                dc.w    $0000,$0000,$1111,$1111,$7778,$8889,$7777,$8888
                dc.w    $7777,$8888,$6777,$7888,$1111,$1111,$6677,$7788
                dc.w    $0000,$0000,$1111,$1100,$999a,$a100,$9999,$a101
                dc.w    $9999,$a115,$8999,$9115,$1111,$1155,$8899,$9155
                dc.w    $0000,$0000,$0011,$1111,$1166,$7777,$6666,$7777
                dc.w    $5666,$6777,$5666,$6777,$5566,$6111,$5566,$1677
                dc.w    $0000,$0000,$1111,$1111,$8888,$9999,$8888,$9999
                dc.w    $7888,$8999,$7888,$8999,$1111,$1111,$7788,$8899
                dc.w    $0000,$0000,$1111,$0000,$aaa1,$0011,$aaa1,$0166
                dc.w    $9aa1,$1566,$9aa1,$1566,$1111,$5556,$99a1,$5556
                dc.w    $0000,$0000,$1111,$1111,$6777,$7888,$6777,$7888
                dc.w    $6677,$7788,$6677,$7788,$6661,$1111,$6117,$7778
                dc.w    $0000,$0000,$1111,$1111,$8999,$9aaa,$8999,$9aaa
                dc.w    $8899,$99aa,$8899,$99aa,$1111,$1111,$8889,$999a
                dc.w    $0000,$0000,$1100,$0000,$a100,$0000,$a100,$0000
                dc.w    $a100,$0000,$a100,$0000,$1100,$0000,$a100,$0001
                dc.w    $0000,$0000,$0000,$0111,$0001,$1888,$0017,$7888
                dc.w    $0177,$7888,$1677,$7788,$1677,$7788,$6667,$7711
                dc.w    $0000,$0111,$1000,$0001,$9110,$0001,$8991,$0001
                dc.w    $8999,$1001,$8899,$9101,$8899,$9100,$8889,$9910
                dc.w    $1101,$0001,$0001,$1011,$0001,$0101,$0001,$0001 *
                dc.w    $0001,$0001,$0001,$0001,$0000,$0000,$0000,$0000 *
                dc.w    $1455,$5166,$1445,$5156,$1445,$5156,$1444,$5155
                dc.w    $1444,$5155,$1444,$4155,$1444,$4155,$1344,$4415
                dc.w    $6677,$7788,$6667,$7778,$6667,$7778,$6611,$1111
                dc.w    $6666,$7777,$5666,$6777,$5666,$6777,$5566,$6677
                dc.w    $8899,$9145,$8889,$9145,$8889,$9144,$1111,$1144
                dc.w    $1111,$1144,$7811,$1144,$7881,$1144,$7788,$1144
                dc.w    $5551,$6667,$5551,$6667,$5551,$6666,$5551,$6661
                dc.w    $4551,$5666,$4551,$5666,$4451,$5566,$4451,$5566
                dc.w    $7778,$8889,$7778,$8889,$7777,$8888,$1111,$1111
                dc.w    $6777,$7888,$6777,$7888,$6677,$7788,$6677,$7788
                dc.w    $9991,$5555,$9991,$4555,$9991,$4555,$1111,$4455
                dc.w    $8111,$4455,$8111,$4445,$8111,$4445,$8111,$4444
                dc.w    $1666,$7777,$1666,$6777,$1666,$6777,$1566,$6111
                dc.w    $1566,$6177,$1556,$6167,$1556,$6167,$1555,$6166
                dc.w    $8888,$9999,$7888,$9999,$7888,$8999,$1111,$1111
                dc.w    $7788,$8899,$7778,$8899,$7778,$8889,$7777,$8889
                dc.w    $a100,$0001,$a100,$0001,$9100,$0015,$1100,$0015
                dc.w    $9100,$0015,$9100,$0155,$9100,$0155,$9100,$0145
                dc.w    $6667,$7711,$6666,$7177,$6666,$7177,$5666,$6177
                dc.w    $5666,$1777,$5566,$1677,$5566,$1677,$5551,$6667
                dc.w    $8889,$9910,$1888,$9910,$1888,$9991,$1888,$8991
                dc.w    $7188,$8991,$7188,$8899,$7188,$8899,$7718,$8889
                dc.w    $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                dc.w    $0000,$0000,$1000,$0000,$1000,$0000,$1000,$0000
                dc.w    $0144,$4451,$0134,$4445,$0014,$4445,$0013,$4444
                dc.w    $0001,$1144,$1111,$1111,$1333,$3444,$1333,$3344
                dc.w    $1111,$1117,$5556,$6661,$5556,$6667,$5555,$6666
                dc.w    $5555,$6666,$1111,$5666,$4555,$5666,$4455,$5566
                dc.w    $7788,$1134,$7778,$8134,$1778,$8133,$1777,$8133
                dc.w    $1777,$8133,$1777,$7133,$1777,$7133,$1677,$7133
                dc.w    $4441,$1111,$4441,$5556,$4441,$5555,$4441,$5555
                dc.w    $3441,$4555,$3441,$4551,$3341,$4455,$3341,$4455
                dc.w    $1111,$1111,$6667,$7778,$6666,$7777,$6666,$7777
                dc.w    $5666,$6777,$1111,$1111,$5566,$6677,$5566,$6677
                dc.w    $1111,$4444,$8111,$3444,$8111,$3444,$8111,$3344
                dc.w    $7111,$3344,$1111,$3334,$7781,$3334,$7781,$3333
                dc.w    $1555,$6111,$1555,$5166,$1555,$5166,$1455,$5166
                dc.w    $1455,$5166,$1445,$5111,$1445,$5556,$1444,$5555
                dc.w    $1111,$1888,$6777,$1888,$6777,$1888,$6677,$1788
                dc.w    $6677,$1788,$6667,$1778,$6667,$1778,$6666,$1777
                dc.w    $9100,$1445,$9100,$1444,$8100,$1444,$8101,$3444
                dc.w    $8101,$3444,$8101,$3344,$8113,$3344,$8113,$3334
                dc.w    $5551,$6667,$5551,$6666,$5515,$6666,$4515,$5666
                dc.w    $4515,$5611,$4155,$5511,$4155,$5566,$4145,$5556
                dc.w    $7718,$8889,$7717,$8888,$7771,$8888,$6771,$7888
                dc.w    $6771,$7888,$6677,$1788,$6677,$1788,$6667,$1778
                dc.w    $9100,$0000,$9100,$0000,$9100,$0000,$8910,$0000
                dc.w    $8910,$0000,$8810,$0000,$8891,$0000,$8881,$0000
                dc.w    $1233,$3344,$1223,$3334,$1111,$1111,$1222,$3333
                dc.w    $1222,$3333,$1222,$2333,$1222,$2333,$1111,$1111
                dc.w    $4455,$5566,$4445,$5551,$1111,$1116,$4444,$5555
                dc.w    $4444,$5555,$3444,$4555,$3444,$4555,$1111,$1111
                dc.w    $1677,$7123,$6667,$7123,$6667,$7122,$6666,$1012
                dc.w    $6666,$1012,$5661,$0001,$5110,$0000,$1000,$0000
                dc.w    $3331,$4445,$3334,$1445,$3333,$4111,$3333,$4444
                dc.w    $2333,$3444,$2333,$3444,$1133,$3344,$0011,$1111
                dc.w    $5556,$6667,$5556,$6667,$1111,$1111,$5555,$6666
                dc.w    $4555,$5666,$4555,$5666,$4455,$5566,$1111,$1111
                dc.w    $7771,$2333,$7771,$2333,$1111,$1233,$7771,$1233
                dc.w    $6771,$1223,$6771,$0123,$6671,$0011,$1111,$0000
                dc.w    $1444,$5555,$3114,$4555,$3341,$1111,$3344,$4455
                dc.w    $3334,$4445,$3334,$4445,$3333,$4444,$1111,$1111
                dc.w    $6666,$1777,$5666,$1777,$1111,$1677,$5566,$6677
                dc.w    $5556,$6667,$5556,$6667,$5555,$6666,$1111,$1111
                dc.w    $8123,$3334,$7122,$3333,$7722,$3333,$7722,$2331
                dc.w    $7722,$2331,$7722,$2231,$7722,$2231,$1111,$1111
                dc.w    $1445,$5556,$1444,$5555,$1444,$1111,$3444,$1555
                dc.w    $3441,$1555,$3341,$1455,$3311,$1455,$1111,$1111
                dc.w    $6667,$7178,$6666,$7177,$1111,$1177,$5666,$6777
                dc.w    $5666,$6777,$5566,$6677,$5566,$6677,$1111,$1111
                dc.w    $8881,$0000,$8888,$1000,$8888,$1000,$7888,$1000
                dc.w    $7888,$8100,$7788,$8100,$7788,$8100,$1111,$1100
                dc.w    $0000,$1111,$1000,$0010,$9110,$0010,$8991,$0010
                dc.w    $8999,$1010,$8899,$9100,$8899,$9100,$8889,$9910
                dc.w    $1010,$0001,$0011,$0011,$0010,$1101,$0010,$0001
                dc.w    $0010,$0001,$0000,$0000,$0000,$0000,$0000,$0000

        PAGE
*************************************************************************************************************************************
*
* ******                   ***   ***
* * ** *                    **    **
*   **     ****    ****     **    ** **    ****   **   **
*   **    **  **  **  **    **    *** **  **  **   ** **
*   **    **  **  **  **    **    **  **  **  **    ***
*   **    **  **  **  **    **    **  **  **  **   ** **
*  ****    ****    ****    ****    ****    ****   **   **
*
*
*
* Forbid
*
* Synopsis:
*       This subroutine is called to disable interrupts.  While interrupts
*       are disabled, the Z80 is also disabled to allow free access to
*       the entire Genesis hardware system in this mode.  This routine
*       nests, which means that a subroutine can use Forbid/Permit without
*       affecting the state of the calling routine.
*
*       This subroutine is called by the FORBID macro, which should be
*       used rather than directly calling this routine.
*
        XDEF _Forbid
_Forbid:
        move.w  #$2700,SR
        jsr     _gemsholdz80
        rts

*.............................................................................
* Permit
*
* Synopsis:
*       This subroutine is called to enable interrupts.  If there is only
*       one level of FORBID requested, then interrupts are actually enabled.
*       Otherwise, the nest counter is decremented and the interrupts are
*       left off, allowing for nested FORBID/PERMIT usage.  When interrupts
*       are enabled, the Z80 is allowed to resume its processing.
*
        XDEF _Permit
_Permit:
        move.w  #$2300,SR                       * 16sept92 dbs
        jsr     _gemsreleasez80
        rts

*.............................................................................
* WaitTOF
*
* Synopsis:
*       Waits until system is in VBlank.
*
        XDEF _WaitTOF
_WaitTOF:
        sf      _vblFlag
Label:
        stop #$2300                             * 16sept92 dbs
        tst.b   _vblFlag
        beq     Label
        rts

*.............................................................................
* WaitDMA
*
* Synopsis:
*       Waits until any current DMA operation is complete.
*
        XDEF _WaitDMA
_WaitDMA:
        push.l  a6
        lea     CUSTOM,a6
wdma10:
        move.w  VCTRL(a6),d6
        btst    #1,d6
        bne.s   wdma10
        pop.l   a6
        rts

*.............................................................................
* PaletteDMA
*
* Synopsis:
*       This routine DMA's the 64 words of color palette data from
*       zeropage RAM to CRAM (VDP).
*
        XDEF _PaletteDMA
_PaletteDMA:
        push.l  a6
        lea     CUSTOM,a6
        move.l  #$93409400,VCTRL(a6)                    *set R19,R20 (DMA length counter)

cdmasrc       equ     _colorTable/2
cdmalo        equ     (cdmasrc&$ff)<<16
cdmamid       equ     (cdmasrc&$ff00)>>8
        move.l  #$95009600+cdmalo+cdmamid,VCTRL(a6)     *set R21,R22 (source address [lo & mid])
        move.w  #$977f,VCTRL(a6)                        *set R23 (source address [hi] & function)
        move.l  #(CRAMWR+$00000080),VCTRL(a6)           *start DMA by setting destination
        pop.l   a6
        rts

*
*       Routine:        ClearVDP
*       Parameters:     None
*       Synopsis:
        XDEF _ClearVDP
_ClearVDP:
        push.l  a6
        lea     CUSTOM,a6
        move.l  #VRAMWR,VCTRL(a6)       *initiate write sequence to VRAM
        move.w  #32767,d0               *32768-1 (size of VRAM -- 64K)
        move.w   #$0,d1                  *set up d1
cvdp10:
        move.w  d1,VDATA(a6)            *write to VRAM
        dbra    d0,cvdp10
        pop.l   a6
        rts
        PAGE
*************************************************************************************************************************************
*************************************************************************************************************************************
*
********                             *
* **  **                            **
* **      **  **   ****   *****    *****
* ****    **  **  **  **  **  **    **
* **      **  **  ******  **  **    **
* **  **   ****   **      **  **    ** *
********    **     ****   **  **     **
*
**     *
***   **
**** ***   ****   *****    ****    *** **  ****  *** **
********      **  **  **      **  **  **  **  **  *** **
*** * **    ****  **  **    ****  **  **  ******  **  **
***   **  **  **  **  **  **  **   ****   **      **
***   **   *** ** **  **   *** **     **   ****  ****
*                                 *****
*
*.............................................................................
* EventManager
*
* Synopsis:
*       This routine reads the controller hardware and puts the bits into
*       bStick.  The values for cStick and dStick are also updated.
*
        XDEF _EventManager
_EventManager:
        move.b #0,_dStick1
        move.b #0,_dStick2
        FORBID
em20:
        lea     JOY1PORT,a0
        bsr.s   ReadJoy
        move.b  d0,_bStick1
        lea     JOY2PORT,a0
        bsr.s   ReadJoy
        move.b  d0,_bStick2
        PERMIT

        move.b  _bStick1,d0             * current
        move.b  _cStick1,d1             * last
        eor.b   d1,d0                   * current xor last
        and.b   _bStick1,d0             * and current
        or.b    d0,_dStick1             * dStick values are now latched
        move.b  _bStick1,_cStick1

        move.b  _bStick2,d0             * current
        move.b  _cStick2,d1             * last
        eor.b   d1,d0                   * current xor last
        and.b   _bStick2,d0             * and current
        or.b    d0,_dStick2             * dStick values are now latched
        move.b  _bStick2,_cStick2

*       jsr _random
        rts

*.............................................................................
* ReadJoy
*
* Synopsis:
*       This routine reads from the controller hardware pointed to by A0.
*
ReadJoy:
        move.b  #0,(a0)
        nop
        nop
        move.b  (a0),d0
        lsl.b   #2,d0
        andi.b  #$c0,d0

        move.b  #$40,(a0)
        nop
        nop
        move.b  (a0),d1
        andi.b  #$3f,d1
        or.b    d1,d0
        not.b   d0
        rts


*
* FadePalette
*
* Synopsis:
*       This routine fades the color palette to black.  The current color palette is
*       in colorTable.  Upon exit, the screen will be black, and the colors in the
*       colorTable will be zeroed.
*
        XDEF _FadePalette
_FadePalette:
* copy colorTable to temp buffer
                movem.l d0-d7/a0-a6,-(sp)
                lea     CUSTOM,a6
                lea     _colorTable,a0
                lea     _colorSave,a1
                moveq   #64-1,d0
fp10:           move.w  (a0)+,(a1)+
                dbra    d0,fp10

                moveq   #100/4,d7
                moveq   #100,d0
fp20:           lea     _colorSave,a0
                lea     _colorTable,a1
                moveq   #63,d1
fp30:           moveq   #0,d2
                move.l  d2,d3
                move.l  d3,d4
                move.l  d4,d5
                move.w  (a0)+,d2                * bgr
                move.w  d2,d3
                lsr.w   #4,d3
                and.w   #15,d3                  * d3 = g
                move.w  d2,d4
                lsr.w   #8,d4
                and.w   #15,d4                  * d4 = b
                and.w   #15,d2                  * d2 = r

                mulu    d0,d2
                divu    #100,d2
                and.w   #$f,d2

                mulu    d0,d3
                divu    #100,d3
                and.w   #$f,d3

                mulu    d0,d4
                divu    #100,d4
                and.w   #$f,d4

                lsl.w   #8,d4
                or.w    d4,d2
                lsl.w   #4,d3
                or.w    d3,d2
                move.w  d2,(a1)+
                dbra    d1,fp30
                move.l  d0,-(sp)
                jsr     _WaitTOF
                jsr     _gemsdmastart
                jsr     _PaletteDMA
                jsr     _gemsdmaend
                move.l  (sp)+,d0
                subq    #4,d0
                dbra    d7,fp20
                movem.l (sp)+,d0-d7/a0-a6
                rts

*
* UnfadePalette
*
* Synopsis:
*       This routine unfades the screen to the palette in colorTable.  The screen
*       is assumed to be black upon entry, and colorTable will not be modified
*       upon exit.
*
        XDEF _UnfadePalette
_UnfadePalette:
                movem.l d0-d7/a0-a6,-(sp)
                lea     CUSTOM,a6
                moveq   #9,d0
ufp5:           move.l  d0,-(sp)
                jsr     _WaitTOF
                move.l  _GameLoop,a0
                jsr     (a0)
                move.l  (sp)+,d0
                dbra    d0,ufp5

                lea     _colorTable,a0
                lea     _colorSave,a1
                moveq   #64-1,d0
ufp10:          move.w  (a0)+,(a1)+
                dbra    d0,ufp10

                moveq   #100/4,d7
                moveq   #0,d0
ufp20:          lea     _colorSave,a0
                lea     _colorTable,a1
                moveq   #63,d1
ufp30:          moveq   #0,d2
                move.l  d2,d3
                move.l  d3,d4
                move.l  d4,d5

                move.w  (a0)+,d2                * bgr
                move.w  d2,d3
                lsr.w   #4,d3
                and.w   #15,d3                  * d3 = g

                move.w  d2,d4
                lsr.w   #8,d4
                and.w   #15,d4                  * d4 = b

                and.w   #15,d2                  * d2 = r

                mulu    d0,d2
                divu    #100,d2
                and.w   #$f,d2

                mulu    d0,d3
                divu    #100,d3
                and.w   #$f,d3

                mulu    d0,d4
                divu    #100,d4
                and.w   #$f,d4

                lsl.w   #8,d4
                or.w    d4,d2
                lsl.w   #4,d3
                or.w    d3,d2
                move.w  d2,(a1)+
                dbra    d1,ufp30
                move.l  d0,-(sp)
                jsr     _WaitTOF
                jsr     _gemsdmastart
                jsr     _PaletteDMA
                jsr     _gemsdmaend
                move.l  (sp)+,d0
                addq    #4,d0
                dbra    d7,ufp20
                movem.l (sp)+,d0-d7/a0-a6
                rts
