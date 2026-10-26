tr -d '\000' < SEGAPEDE.S > SEGAPEDE.S2 |
srec_cat SEGAPEDE.S2 -motorola -o SEGAPEDE.BIN -binary