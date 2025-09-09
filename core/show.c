#include "../include/fitlog.h"

int cmd_show(int argc, char* argv[]) {
    // Check for help flag
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_show_help();
            return 0;
        }
    }

    printf("SHOW command - arguments: %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("  argv[%d]: %s\n", i, argv[i]);
    }
    
    printf("TODO: Display workout history\n");
    return 0;
}