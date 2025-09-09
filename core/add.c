#include "../../include/fitlog.h"

int cmd_add(int argc, char* argv[]) {
    printf("ADD command - arguments: %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("  argv[%d]: %s\n", i, argv[i]);
    }
    
    printf("TODO: Add exercise to workout log\n");
    return 0;
}