/* TGALOAD.C -- A TGA file loader
 * Written by Paul Brannan
 * brannanp@edisto.cofc.edu
 * for 1995 science fair project
 */
#include "pictio.h"

int decode(int width) {
	unsigned int y=0;
	unsigned int currentbyte=0;

	unsigned char Buffer[768];

	while(y<200){
		Buffer[currentbyte++]=get_byte();
		if(currentbyte==width) {
			Putline(&Buffer,width);
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

bad_tga() {
	text_mode();
	fprintf(stderr,"Not a TGA file or bad TGA data\n");
	exit(1);
}

/* read_tga_header
 *
 * The read_tga_header function is used to read the TGA header.
 * If there is bad data, then the program will call bad_tga.
 */

int read_tga_header() {
	int counter;
	register char flags;
	int width;
	unsigned int c, separation, colormapstart, colormaplength;

	/* Get the number of separation bytes */
	separation=get_byte();

	/* Get the color map type */
	if((get_byte())!=1) bad_tga();

	/* Get the image type */
	if((get_byte())!=1) bad_tga();

	/* Get the color map information */
	c=get_byte();
	if((c+256*get_byte())!=0) bad_tga();
	c=get_byte();
	if((c+256*get_byte())!=256) bad_tga();
	/*if((get_byte())!=8) bad_tga();*/get_byte();

	/* Get the dimensions */
	for(counter=1;counter<=4;counter++) get_byte();

	/* Get the width */
	c=get_byte();
	width=c+256*get_byte();

	/* Get the height */
	get_byte(); get_byte();

	/* Get the bit information */
	if((get_byte())!=8) bad_tga();

	/* Get the descriptor byte */
	get_byte();

	/* Read the separation bytes */
	for(counter=1;counter<=separation;counter++) get_byte();

	/* Get the palette */
	Palette_load();

	return(width);
}

main(int argc, char *argv[]) {
	if(argc==0) exit(1);
	pictopen(argv[1]);
	graphic_mode();
	if(decode(read_tga_header())<0) bad_tga();
	if(argc>2) getch();
	text_mode();
}
