*
* Modulator Offset Table
*

	dc.b	$02,$00		; Offset to modulator #0: 2
*
* Modulator Data
*

* Modulator 0 "jump up": Pitch
	dc.b	$00,$00			; initial value = 0
	dc.b	$18,$2C,$01		; len = 24, slope = 300
	dc.b	$43,$50,$FB		; len = 67, slope = -1200
	dc.b	0
