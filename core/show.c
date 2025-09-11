#include "../include/fitlog.h"

int cmd_show(int argc, char *argv[])
{
    // Check for help flag
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
        {
            print_show_help();
            return 0;
        }
    }

    // Get the type
    char type[20] = "";
    if (argc > 1 && strcmp(argv[1], "exercise") == 0)
    {
        strcpy(type, "exercise");
    }
    else if (argc > 1 && strcmp(argv[1], "log") == 0)
    {
        strcpy(type, "log");
    }
    else
    {
        fprintf(stderr, ANSI_COLOR_RED "Error: Missing or invalid type. Use 'exercise' or 'log'.\n" ANSI_COLOR_RESET);
        return 1;
    }

    // Get the show criteria
    char id[20] = "";
    char from_date[20] = "";
    char to_date[20] = "";
    int last_n = -1;

    for (int i = 2; i < argc; i++)
    {
        if (strcmp(argv[i], "--id") == 0 && i + 1 < argc)
        {
            strcpy(id, argv[++i]);
        }
        else if (strcmp(argv[i], "--from") == 0 && i + 1 < argc)
        {
            strcpy(from_date, argv[++i]);
        }
        else if (strcmp(argv[i], "--to") == 0 && i + 1 < argc)
        {
            strcpy(to_date, argv[++i]);
        }
        else if (strcmp(argv[i], "--last") == 0 && i + 1 < argc)
        {
            last_n = atoi(argv[++i]);
        }
    }

    // Check if criteria is provided
    if (strcmp(type, "exercise") == 0)
    {
        // --from and --to is not allowed
        if (strlen(from_date) > 0 || strlen(to_date) > 0)
        {

            fprintf(stderr, ANSI_COLOR_RED "Error: Arguments --from and --to is not allowed while displaying exercise.\n" ANSI_COLOR_RESET);
            return 1;
        }

        // Check for alteast one of --id or --last
        if (strlen(id) == 0 && last_n == -1)
        {
            fprintf(stderr, ANSI_COLOR_RED "Error: Please provide --id or --last for exercise display.\n" ANSI_COLOR_RESET);
            return 1;
        }

        // Check if both --id and --last are given
        if (strlen(id) > 0 && last_n != -1)
        {
            fprintf(stderr, ANSI_COLOR_RED "Error: Arguments --id and --last cannot be used together.\n" ANSI_COLOR_RESET);
            return 1;
        }

        // Get exercise details
        if (strlen(id) > 0)
        {
            return show_exercise_by_id(id);
        }
        else if (last_n != -1)
        {
            return show_last_n_exercises(last_n);
        }
    }
    if (strcmp(type, "log") == 0)
    {
        // If --from or --to is given, --last and --id is not allowed
        if ((strlen(from_date) > 0 || strlen(to_date) > 0) && (last_n != -1 || strlen(id) > 0))
        {
            fprintf(stderr, ANSI_COLOR_RED "Error: Arguments --from and --to cannot be used with --last or --id.\n" ANSI_COLOR_RESET);
            return 1;
        }

        // Check for alteast one of --id or --last or --from or --to
        if (strlen(id) == 0 && last_n == -1 && strlen(from_date) == 0 && strlen(to_date) == 0)
        {
            fprintf(stderr, ANSI_COLOR_RED "Error: Please provide --id or --last or --from or --to for log display.\n" ANSI_COLOR_RESET);
            return 1;
        }

        // Check for multiple arguments
        int criteria_count = 0;
        if (strlen(id) > 0)
            criteria_count++;
        if (last_n != -1)
            criteria_count++;
        if (strlen(from_date) > 0 || strlen(to_date) > 0)
            criteria_count++;
        if (criteria_count > 1)
        {
            fprintf(stderr, ANSI_COLOR_RED "Error: Please provide only one of --id or --last or --from --to for log display.\n" ANSI_COLOR_RESET);
            return 1;
        }
    }

    return 0;
}