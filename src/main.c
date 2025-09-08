#include <stdio.h>
#include "fitlog.h"

void print_usage(void)
{
    printf("fitlog %s - CLI Workout Logger\n\n", FITLOG_VERSION);
    printf("Usage: fitlog <command> [options]\n\n");
    printf("Commands:\n");
    printf("  add      Log a new exercise\n");
    printf("  show     Display workouts\n");
    printf("  config   Manage configuration\n");
    printf("  help     Show this help message\n");
    printf("\nFor command-specific help: fitlog <command> --help\n");
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        print_usage();
        return 1;
    }

    char *command = argv[1];

    if (strcmp(command, "add") == 0)
    {
        return cmd_add(argc - 1, argv + 1);
    }
    else if (strcmp(command, "show") == 0)
    {
        return cmd_show(argc - 1, argv + 1);
    }
    else if (strcmp(command, "remove") == 0)
    {
        return cmd_remove(argc - 1, argv + 1);
    }
    else if (strcmp(command, "help") == 0 || strcmp(command, "--help") == 0)
    {
        print_usage();
        return 0;
    }
    else
    {
        printf("Unknown command: %s\n", command);
        print_usage();
        return 1;
    }

    return 0;
}
