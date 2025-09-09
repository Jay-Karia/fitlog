#include "../include/fitlog.h"

int cmd_init(int argc, char* argv[]) {
    printf("Init command - arguments: %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("  argv[%d]: %s\n", i, argv[i]);
    }
    
    printf("TODO: Init the CLI\n");
    return 0;
}