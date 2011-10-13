#include <stdio.h>

main(int argc, char *argv[]) {
        FILE *in, *out;

        if(argc<3) {
                fprintf(stderr, "Usage: %s <input file> <output file>\n",
                        argv[0]);
                exit(1);
        }

        if((in = fopen(argv[1], "rb")) == NULL) {
                fprintf(stderr, "Error: unable to open file %s for reading",
                        argv[1]);
                exit(1);
        }

        if((out = fopen(argv[2], "wb")) == NULL) {
                fprintf(stderr, "Error: unable to open file %s for writing",
                        argv[2]);
                exit(1);
        }

        while(!feof(in)) {
                char c;
                c = fgetc(in);
                if(c == '\n') fputc('\r', out);
                fputc(c, out);;
        }

        return 0;
}

