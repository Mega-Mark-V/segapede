echo off
ma 0,0ffffff=us
pi all=en
icereset
clo 0
r reset
ems a=en
ex/w 0ff0000=060fe
r pc=0ff0000
r usp=0fff000
r ssp=0fff000
g
q


