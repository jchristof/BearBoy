rgbasm -o patch.o patch.asm
rgblink -o patch.gb patch.o
rgbfix -v -p 0 patch.gb

pause