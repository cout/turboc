/* BMPLOAD.C -- A BMP file loader
 * Written by Paul Brannan
 * brannanp@edisto.cofc.edu
 * for 1995 science fair project
 */
#include "pictio.h"

int decode(int width) {
	unsigned int y=0;
	unsigned int currentbyte=0;
	unsigned int counter;

	unsigned char Buffer[768];

	ScreenMemory=(unsigned char far*)0xa000ff00;

	while(y<200){
		Buffer[currentbyte++]=get_byte();
		if(currentbyte==width) {
			/* The normal Putline() function cannot be used here
			 * as in the TGA loader, because the scan lines are
			 * stored from bottom to top.
			 */

			for(counter=0;counter<width;counter++)
				*ScreenMemory++=Buffer[counter];

			if(width<320)
				for(counter=0;counter<320-width;counter++)
					ScreenMemory++;

			ScreenMemory-=640;

			currentbyte=0;
			y++;
		}
	}

	return 0;
}

/* bad_tga
 *
 * This function is called when there is an error in the program or TGA
 * file.  The program is aborted.
 */

bad_bmp() {
	text_mode();
	fprintf(stderr,"Not a BMP file or bad BMP data\n");
	exit(1);
}

/* read_bmp_header
 *
 * The read_bmp_header function is used to read the BMP header.
 * If there is bad data, then the program will call bad_bmp.
 */

int read_bmp_header() {

	int counter, width;
	int blue, green, red;

	/* Check for BMP signature */

	if((get_byte())!=0x42) bad_bmp();
	if((get_byte())!=0x4D) bad_bmp();

	/* Get file size and reserved bytes */

	for(counter=1;counter<=8;counter++) get_byte();

	/* Check for correct number of colors */

	if((get_byte())!=0x36) bad_bmp();
	if((get_byte())!=0x04) bad_bmp();
	get_byte(); get_byte();

	/* Check for correct header size */

	if((get_byte())!=0x28) bad_bmp();
	get_byte(); get_byte(); get_byte();

	/* Get width and height */

	width=get_byte();
	width=width+256*get_byte();
	width=width+256*get_byte();
	width=width+256*get_byte();
	if((get_byte())!=200) bad_bmp();
	if((get_byte())!=0) bad_bmp();
	if((get_byte())!=0) bad_bmp();
	if((get_byte())!=0) bad_bmp();

	/* Get the number of planes */

	get_byte(); get_byte();

	/* Get the number of colors */

	if((get_byte())!=8) bad_bmp();
	if((get_byte())!=0) bad_bmp();

	/* Get compression scheme (should be none) */

	if((get_byte())!=0) bad_bmp();
	if((get_byte())!=0) bad_bmp();
	if((get_byte())!=0) bad_bmp();
	if((get_byte())!=0) bad_bmp();

	/* Get the rest of the header */

	for(counter=1;counter<=20;counter++) get_byte();

	/* Now, get the color table.  Note that the Palette_load
	 * function cannot be used here, as the palette in the BMP file is
	 * stored in reverse order, and has an excess byte.
	 */

	outp(0x3c8,0);
	for(counter=1;counter<=256;counter++) {
		blue=get_byte();
		green=get_byte();
		red=get_byte();
		outp(0x3c9,red>>2);
		outp(0x3c9,green>>2);
		outp(0x3c9,blue>>2);
		get_byte();
	}

	return width;
}

main(int argc, char *argv[]) {
	if(argc==0) exit(1);
	pictopen(argv[1]);
	graphic_mode();
	if(decode(read_bmp_header())!=0) bad_bmp();
	if(argc>2) getch();
	text_mode();
}
