
# Building

* Mount this folder into DOSBOX onto `C:`, then run `setup.bat`. This'll put you into a FreeDOS environment with the variables set up.
* Go into the `SEGAPEDE` directory and run `make rom`. The ROM will output as a M68K COFF file (`SEGAPEDE.OUT`), and a Motorolla S-Record file (`SEGAPEDE.S`).

# Modifications made:

* Commented out `.MISER_MODE disk` in `MAKEFILE`. This was causing issues in DOSBOX.
* Changed some directories in `MAKEFILE` to all use the same drive
* Created `SEGAPEDE\OBJ` since the builder does not generate one.
* Created dummy files (in `SPART`) to make the compiler not regenerate the .H files.
The original source code is included in SEGAPEDE.ZIP.
