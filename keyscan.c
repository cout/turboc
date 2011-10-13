#include <conio.h>

void main(void) {
	char c='\0';
	while(c!=27){
		do {} while(!kbhit);
		c=getch();
		if(c!=0) cprintf("  ");
		cprintf("%d ",c);
		if(c==0) {
			c=getch();
			cprintf("%d ",c);
		}
		cprintf("\r\n");
	}
}