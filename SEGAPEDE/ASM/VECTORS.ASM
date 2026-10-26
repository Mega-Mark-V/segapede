

    xref _start
    xref _ErrorHandler
    xref _HBLHandler
    xref _VBLHandler

    TSECTION .vectors,0

    dc.l $ffff8000
    dc.l _start
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _HBLHandler
    dc.l _ErrorHandler
    dc.l _VBLHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler
    dc.l _ErrorHandler

  dc.b 'SEGA MEGA DRIVE '
  dc.b '(C)SEGA 1994 JUL'
  dc.b 'SEGAPEDE        '
  dc.b '                '
  dc.b '                '
  dc.b 'SEGAPEDE        '
  dc.b '                '
  dc.b '                '
  dc.b 'GM 00001014-00'
  dc.w 0
  dc.b 'J               '
  dc.l $00000000
  dc.l $000fffff
  dc.l $00ff0000
  dc.l $00ffffff
  dc.b '                '
  dc.b '                '
  dc.b '                '
  dc.b '                '
  dc.b 'JUE             '

