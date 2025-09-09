#include <stdio.h>
#include "fitlog.h"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        print_usage();
        return 1;
    }

    char *command = argv[1];

    // Check for command-specific help flags
    if (argc > 2 && (strcmp(argv[2], "--help") == 0 || strcmp(argv[2], "-h") == 0)) {
        print_command_help(command);
        return 0;
    }

    if (strcmp(command, "add") == 0) {
        return cmd_add(argc - 1, argv + 1);
    } else if (strcmp(command, "init") == 0) {
        return cmd_init(argc - 1, argv + 1);
    } else if (strcmp(command, "create") == 0) {
        return cmd_create(argc - 1, argv + 1);
    } else if (strcmp(command, "show") == 0) {
        return cmd_show(argc - 1, argv + 1);
    } else if (strcmp(command, "remove") == 0) {
        return cmd_remove(argc - 1, argv + 1);
    } else if (strcmp(command, "help") == 0 || strcmp(command, "--help") == 0) {
        // If help command has an argument, show that command's help
        if (argc > 2) {
            print_command_help(argv[2]);
        } else {
            print_usage();
        }
        return 0;
    } else {
        printf("Unknown command: %s\n", command);
        print_usage();
        return 1;
    }

    return 0;
}
