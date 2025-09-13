#include <stdio.h>
#include "fitlog.h"

// TODO: modify directory while exporting and importing data

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        print_usage();
        return 1;
    }

    char *command = argv[1];

    // Check for command-specific help flags
    if (argc > 2 && (strcmp(argv[2], "--help") == 0 || strcmp(argv[2], "-h") == 0))
    {
        print_command_help(command);
        return 0;
    }

    // Check for version flag
    if (strcmp(command, "--version") == 0 || strcmp(command, "-v") == 0)
    {
        printf("fitlog version %s\n", FITLOG_VERSION);
        return 0;
    }

    // Check for the --dir flag
    if (strcmp(command, "--dir") == 0)
    {
        char absolute_path[256];
        if (_fullpath(absolute_path, FITLOG_DIR, sizeof(absolute_path)) != NULL)
        {
            printf("%s\n", absolute_path);
        }
        else
        {
            printf("%s\n", FITLOG_DIR);
        }
        return 0;
    }

    if (strcmp(command, "add") == 0)
    {
        return cmd_add(argc - 1, argv + 1);
    }
    else if (strcmp(command, "init") == 0)
    {
        return cmd_init(argc - 1, argv + 1);
    }
    else if (strcmp(command, "create") == 0)
    {
        return cmd_create(argc - 1, argv + 1);
    }
    else if (strcmp(command, "show") == 0)
    {
        return cmd_show(argc - 1, argv + 1);
    }
    else if (strcmp(command, "remove") == 0)
    {
        return cmd_remove(argc - 1, argv + 1);
    }
    else if (strcmp(command, "config") == 0)
    {
        return cmd_config(argc - 1, argv + 1);
    }
    else if (strcmp(command, "export") == 0)
    {
        return cmd_export(argc - 1, argv + 1);
    }
    else if (strcmp(command, "import") == 0)
    {
        return cmd_import(argc - 1, argv + 1);
    }
    else if (strcmp(command, "help") == 0 || strcmp(command, "--help") == 0)
    {
        // If help command has an argument, show that command's help
        if (argc > 2)
        {
            print_command_help(argv[2]);
        }
        else
        {
            print_usage();
        }
        return 0;
    }
    else
    {
        printf(ANSI_COLOR_RED "Unknown command: %s\n" ANSI_COLOR_RESET, command);
        return 1;
    }

    return 0;
}
