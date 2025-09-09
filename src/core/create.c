#include "../../include/fitlog.h"

int cmd_create(int argc, char* argv[]) {
    printf("CREATE command - arguments: %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("  argv[%d]: %s\n", i, argv[i]);
    }
    
    printf("TODO: create exercise to database\n");
    return 0;
}