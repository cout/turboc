/* PCXLOAD.C -- A PCX file loader
 * Written by Paul Brannan
 * brannanp@edisto.cofc.edu
 * for 1995 science fair project
 */

#include "pictio.h"

int decode(int width) {
	unsigned int y=0;
	unsigned int currentbyte=0;

	unsigned char data;
	unsigned char reps;
	unsigned char count;
	unsigned char Buffer[768];

	while(y<200){
		data=get_byte();
		if((data & 0xC0)==0xC0) {
			reps=data & 0x3F;
			data=get_byte();
			for(count=0;count<reps;count++) {
				Buffer[currentbyte++]=data;
				if(currentbyte==width) {
					Putline(&Buffer,width);
					currentbyte=0;
					y++;
				}
			}
		} else Buffer[currentbyte++]=data;
		if(currentbyte==width) {
			Putline(&Buffer,width);
			currentbyte=0;
			y++;
		}
	}
	return 0;
}

/* bad_pcx
 *
 * This function is called when there is an error in the program or PCX
 * file.  The program is aborted.
 */

bad_pcx() {
	text_mode();
	fprintf(stderr,"Not a PCX file or bad PCX data\n");
	exit(1);
}

/* read_pcx_header
 *
 * The read_pcx_header function is used to read the PCX header.
 * If there is bad data, then the program will call bad_pcx.
 */

int read_pcx_header() {
	int counter;
	register char flags;
	int xmin,xmax,ymin,ymax;
	unsigned int c;

	/* Check to see if it really is a PCX file */
	if((get_byte())!=10) bad_pcx();

	/* Read the version */
	get_byte();

	/* Check to make sure we are using a valid encoding method */
	if((get_byte())!=1) bad_pcx();

	/* Check to see if there are a valid number of colors (256) */
	if((get_byte())!=8) bad_pcx();

	/* Get the dimensions and make sure they are valid */
	c=(unsigned char) get_byte();
	xmin=get_byte()*256+c;
	c=(unsigned char) get_byte();
	ymin=get_byte()*256+c;
	c=(unsigned char) get_byte();
	xmax=get_byte()*256+c;
	c=(unsigned char) get_byte();
	ymax=get_byte()*256+c;

	if((ymax-ymin)!=199) bad_pcx();

	/* Get the dimensions again, but don't check them again */
	for (counter=1; counter<=4; counter++) get_byte();

	/* Ignore this palette -- there is a real one at the end */
	for (counter=1; counter<=48; counter++) get_byte();

	/* Get the reserved byte */
	get_byte();

	/* Make sure there are a valid number of color planes */
	if((c=get_byte())!=1) bad_pcx();

	/* Most 256-color PCX files will not have more bytes per line
	 * than the width.  So, we will ignore the color planes byte
	 */
	get_byte();get_byte();

	/* Make sure there is a valid palette type */
	if((get_byte())>10) bad_pcx();

	/* Get the rest of the header */
	for (counter=1; counter<=58; counter++) get_byte();

	return (xmax-xmin);
}

main(int argc, char *argv[]) {
	if (argc==0) exit(1);
	pictopen(argv[1]);
	graphic_mode();
	if(decode(read_pcx_header()+1)<0) bad_pcx();
	Palette_load();
	if(argc>2) getch();
	text_mode();
}
