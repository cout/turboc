#include <conio.h>
#include <dir.h>

int main(void)
{
   struct ffblk ffblk;
   int done;
   int ctr;
   cprintf("Directory listing of *.*\r\n");
   done = findfirst("*.*",&ffblk,0);
   while (!done)
   {
	 ctr++;
	 cprintf("  %s\r\n", ffblk.ff_name);
	 done = findnext(&ffblk);
	 if(ctr==22) {
		cprintf("--More--\r\n");
		getch();
	 }
   }

   return 0;
}

