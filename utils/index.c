#include <stdio.h>
#include "fitlog.h"

void print_usage(void)
{
    printf("fitlog %s - CLI Workout Logger\n\n", FITLOG_VERSION);
    printf("Usage: fitlog <command> [options]\n\n");
    printf("Commands:\n");
    printf("  init        Initialze the CLI\n");
    printf("  create      Create a new exercise\n");
    printf("  add         Log a new exercise\n");
    printf("  show        Display workouts\n");
    printf("  config      Manage configuration\n");
    printf("  help        Show this help message\n");
    printf("\nFor command-specific help: fitlog <command> --help\n");
}