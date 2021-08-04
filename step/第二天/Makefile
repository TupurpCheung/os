
default :
	../../dependency/tools/make.exe img
	
ipl.bin : ipl.nas Makefile
	../../dependency/tools/nask.exe ipl.nas ipl.bin ipl.lst

helloos.img : ipl.bin Makefile
	../../dependency/tools/edimg.exe   imgin:../../dependency/tools/fdimg0at.tek \
		wbinimg src:ipl.bin len:512 from:0 to:0   imgout:helloos.img


asm :
	../../dependency/tools/make.exe -r ipl.bin

img :
	../../dependency/tools/make.exe -r helloos.img

