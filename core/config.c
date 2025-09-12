#include "../include/fitlog.h"

int cmd_config(int argc, char *argv[]) {
    // Check for help flag
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
        {
            print_add_help();
            return 0;
        }
    }

    // Check for --get options
    for(int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--get") == 0 && i + 1 < argc) {
            const char *key = argv[++i];
            char value[256];
            if (read_config_value(key, "", value, sizeof(value)) == 0) {
                printf("%s\n", value);
            } else {
                fprintf(stderr, ANSI_COLOR_RED "Error: Could not read config key '%s'\n" ANSI_COLOR_RESET, key);
                printf("Check whether the key is valid.");
                return 1;
            }
            return 0;
        }
    }

    printf("Config command is not yet implemented.\n");
    return 0;
}