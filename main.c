#include "functions.h"

int main(int argc, char **argv) {
	int c;
    int bflag = 0;

    int n = 0;
	float L = 0;
	int X = 0;
	int Y = 0;
    float d = 0;

    opterr = 0;

    while ((c = getopt(argc, argv, "bn:L:X:Y:d:")) != -1) {
        switch (c) {
            case 'b':
                bflag = 1;
                break;
            case 'n':
                sscanf(optarg, "%d", &n);
                break;
            case 'L':
                sscanf(optarg, "%f", &L);
                break;
            case 'X':
                sscanf(optarg, "%d", &X);
                break;
            case 'Y':
                sscanf(optarg, "%d", &Y);
                break;
            case 'd':
                sscanf(optarg, "%f", &d);
                break;
            case '?':
                if (optopt == 'h') {
                    fprintf(stderr, "Opcion -%c requiere un argumento.\n", optopt);
                }
                else if (isprint(optopt)) {
                    fprintf(stderr, "Opcion desconocida '-%c'.\n", optopt);
                }
                else {
                    fprintf(
                            stderr,
                            "Opción con caracter desconocido '\\x%x'.\n",
                            optopt
                        );
                }
                return 1;
            default:
                abort();
        }
    }

	init(bflag, n, L, X, Y, d);

    return 0;
}
