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
    printf("  config      Manage configuration values\n");
    printf("  export      Export fitlog data to JSON\n");
    printf("  help        Show this help message\n\n");
    printf("Options:\n");
    printf("  --version     Show version information\n");
    printf("  --dir         Print the .fitlog directory path\n");
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
    else if (strcmp(command, "config") == 0)
    {
        print_config_help();
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
    printf("Usage: fitlog create <name> [shortcut] [options]\n\n");
    printf("Arguments:\n");
    printf("  <name>                   Name of the exercise to create (must be unique)\n");
    printf("  [shortcut]               Optional short alias for exercise\n\n");
    printf("Options:\n");
    printf("  --desc <text>            Description for the exercise\n");
    printf("  --type <sets|time|body>       Exercise type (default: sets)\n");
    printf("  --help                   Show this help message\n");
    printf("\nDescription:\n");
    printf("  Creates a new exercise definition that can be referenced when\n");
    printf("  logging workouts with the 'add' command. If a shortcut is provided,\n");
    printf("  it can be used in place of the full name.\n");
}

void print_add_help(void)
{
    printf("fitlog add - Log a new exercise\n\n");
    printf("Usage: fitlog add <exercise> [options]\n\n");
    printf("Arguments:\n");
    printf("  <exercise>          Name of the exercise or shortcut to log\n\n");
    printf("Options:\n");
    printf("  --sets <number>     Number of sets performed (default: 1)\n");
    printf("  --reps <number>     Number of repetitions per set\n");
    printf("  --weight <value>    Weight used for the exercise\n");
    printf("  --time <value>      Time taken to complete the exercise\n");
    printf("  --date <date>       Date of the workout (default: today)\n");
    printf("  --notes <text>      Additional notes about the workout\n");
    printf("  --help              Show this help message\n");
    printf("\nDescription:\n");
    printf("  This command logs a workout entry for the specified exercise,\n");
    printf("  including performance details like sets, reps, and weight.\n");
}

void print_show_help(void)
{
    printf("fitlog show - Display workouts and exercise\n\n");
    printf("Usage: fitlog show [type] [options]\n\n");
    printf("Types:\n");
    printf("  exercise            Show all the exercises created\n");
    printf("  log                 Show all workout logs\n\n");
    printf("Options:\n");
    printf("  --id <number>       Show entry for this id\n");
    printf("  --from <date>       Show entries from this date\n");
    printf("  --to <date>         Show entries until this date\n");
    printf("  --last <number>     Show only the last N entries\n");
    printf("  --all               Show all the entries in the database\n");
    printf("  --help              Show this help message\n");
    printf("\nDescription:\n");
    printf("  This command displays workout entries from your log,\n");
    printf("  with various filtering and display options.\n");
}

void print_remove_help(void)
{
    printf("fitlog remove - Remove exercises or logs\n\n");
    printf("Usage: fitlog remove [type] [options]\n\n");
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

void print_config_help(void)
{
    printf("fitlog config - Manage configuration values\n\n");
    printf("Usage: fitlog config [options]\n\n");
    printf("Options:\n");
    printf("  --get <key>            Get the value of a configuration key\n");
    printf("  --set <key> <value>    Set the value of a configuration key\n");
    printf("  --list                 Print all configuration key-value pairs\n");
    printf("  --reset                Reset configuration to default values\n");
    printf("  --help                 Show this help message\n");
    printf("\nDescription:\n");
    printf("  This command allows you to view and modify configuration\n");
    printf("  settings for the fitlog CLI application.\n");
}
