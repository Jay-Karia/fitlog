#include <stdio.h>
#include "fitlog.h"

void print_usage(void)
{
    printf("fitlog %s - CLI Workout Logger\n\n", FITLOG_VERSION);
    printf("Usage: fitlog <command> [options]\n\n");
    printf("Commands:\n");
    printf("  init        Initialize the CLI\n");
    printf("  create      Create a new exercise\n");
    printf("  add         Log a new exercise\n");
    printf("  show        Display workouts\n");
    printf("  remove      Remove exercises or logs\n");
    printf("  help        Show this help message\n");
    printf("\nFor command-specific help: fitlog <command> --help\n");
}

void print_command_help(const char *command)
{
    if (strcmp(command, "init") == 0)
    {
        print_init_help();
    }
    else if (strcmp(command, "create") == 0)
    {
        print_create_help();
    }
    else if (strcmp(command, "add") == 0)
    {
        print_add_help();
    }
    else if (strcmp(command, "show") == 0)
    {
        print_show_help();
    }
    else if (strcmp(command, "remove") == 0)
    {
        print_remove_help();
    }
    else
    {
        printf("Unknown command: %s\n", command);
        print_usage();
    }
}

void print_init_help(void)
{
    printf("fitlog init - Initialize the CLI\n\n");
    printf("Usage: fitlog init [options]\n\n");
    printf("Options:\n");
    printf("  --help              Show this help message\n");
    printf("\nDescription:\n");
    printf("  This command initializes the fitlog CLI, creating necessary\n");
    printf("  directories and configuration files. It should be run before\n");
    printf("  using any other commands.\n");
}

void print_create_help(void)
{
    printf("fitlog create - Create a new exercise\n\n");
    printf("Usage: fitlog create <name> [options]\n\n");
    printf("Arguments:\n");
    printf("  <name>                Name of the exercise to create\n\n");
    printf("Options:\n");
    printf("  --desc <text>         Add a description for the exercise\n");
    printf("  --shortcut <text>     Register a shortcut for the exercisr\n");
    printf("  --help                Show this help message\n");
    printf("\nDescription:\n");
    printf("  This command creates a new exercise definition that can be\n");
    printf("  referenced when logging workouts with the 'add' command.\n");
}

void print_add_help(void)
{
    printf("fitlog add - Log a new exercise\n\n");
    printf("Usage: fitlog add <exercise> [options]\n\n");
    printf("Arguments:\n");
    printf("  <exercise>          Name of the exercise to log or the shortcut registered\n\n");
    printf("Options:\n");
    printf("  --sets <number>     Number of sets performed (default: 1)\n");
    printf("  --reps <number>     Number of repetitions per set\n");
    printf("  --weight <value>    Weight used for the exercise\n");
    printf("  --date <date>       Date of the workout (default: today)\n");
    printf("  --notes <text>      Additional notes about the workout\n");
    printf("  --help              Show this help message\n");
    printf("\nDescription:\n");
    printf("  This command logs a workout entry for the specified exercise,\n");
    printf("  including performance details like sets, reps, and weight.\n");
}

void print_show_help(void)
{
    printf("fitlog show - Display workouts\n\n");
    printf("Usage: fitlog show [options]\n\n");
    printf("Options:\n");
    printf("  --exercise <name>   Filter by exercise name or shortcut\n");
    printf("  --from <date>       Show entries from this date\n");
    printf("  --to <date>         Show entries until this date\n");
    printf("  --last <number>     Show only the last N entries\n");
    printf("  --summary           Show summary statistics instead of raw data\n");
    printf("  --help              Show this help message\n");
    printf("\nDescription:\n");
    printf("  This command displays workout entries from your log,\n");
    printf("  with various filtering and display options.\n");
}

void print_remove_help(void)
{
    printf("fitlog remove - Remove exercises or logs\n\n");
    printf("Usage: fitlog remove [type] [identifier] [options]\n\n");
    printf("Types:\n");
    printf("  exercise            Remove an exercise definition\n");
    printf("  log                 Remove a logged workout entry\n\n");
    printf("Options:\n");
    printf("  --id <id>           Remove by specific ID\n");
    printf("  --date <date>       Remove entries from specific date\n");
    printf("  --force             Skip confirmation prompt\n");
    printf("  --help              Show this help message\n");
    printf("\nDescription:\n");
    printf("  This command removes exercises or workout log entries\n");
    printf("  from your fitlog database.\n");
}