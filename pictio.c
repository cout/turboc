#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

#define ScreenMemory ((char far *)0xa0000000)

FILE *pic_fp;

char get_byte() {
	return (char)fgetc(pic_fp);
}

void pictopen(char *filename) {
	if((pic_fp = fopen(filename, "rb")) == NULL)
		exit(1);
}

void graphic_mode() {
	union REGS regs;
	regs.x.ax = 0x0013;
	int86(0x10, &regs, &regs);
}

void text_mode() {
	union REGS regs;
	regs.x.ax = 0;
	int86(0x10, &regs, &regs);
}

void Palette_load() {
	int j;

	for(j = 0; j < 256; j++) {
		outp(0x3c8, j);
		outp(0x3c9, get_byte() >> 2);
		outp(0x3c9, get_byte() >> 2);
		outp(0x3c9, get_byte() >> 2);
	}
}

void Putline(char *buffer, int width) {
	static int y = 0;
	int j;
	for(j = 0; j < width; j++)
		*(ScreenMemory + y*width + j) = buffer[j];
	y++;
}
