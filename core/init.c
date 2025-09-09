#include "../include/fitlog.h"

int cmd_init(int argc, char* argv[]) {
    // Check for help flag
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_init_help();
            return 0;
        }
    }

    printf("Init command - arguments: %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("  argv[%d]: %s\n", i, argv[i]);
    }
    
    printf("TODO: Init the CLI\n");
    return 0;
}