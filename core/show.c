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
    bool all = false;

    for (int i = 2; i < argc; i++)
    {
        if ((strcmp(argv[i], "--id") == 0 || strcmp(argv[i], "-i") == 0) && i + 1 < argc)
        {
            strcpy(id, argv[++i]);
        }
        else if ((strcmp(argv[i], "--from") == 0 || strcmp(argv[i], "-f") == 0) && i + 1 < argc)
        {
            strcpy(from_date, argv[++i]);
        }
        else if ((strcmp(argv[i], "--to") == 0 || strcmp(argv[i], "-t") == 0) && i + 1 < argc)
        {
            strcpy(to_date, argv[++i]);
        }
        else if ((strcmp(argv[i], "--last") == 0 || strcmp(argv[i], "-l") == 0) && i + 1 < argc)
        {
            last_n = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "--all") == 0 || strcmp(argv[i], "-a") == 0)
        {
            all = true;
        }
        else
        {
            fprintf(stderr, ANSI_COLOR_RED "Error: Unknown or incomplete argument '%s'.\n" ANSI_COLOR_RESET, argv[i]);
            return 1;
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

        // Check for alteast one of --id or --last or --all
        if (strlen(id) == 0 && last_n == -1 && !all)
        {
            fprintf(stderr, ANSI_COLOR_RED "Error: Please provide --id or --last for exercise display.\n" ANSI_COLOR_RESET);
            return 1;
        }

        // Check if multiple arguments are given
        int criteria_count = 0;
        if (strlen(id) > 0)
            criteria_count++;
        if (last_n != -1)
            criteria_count++;
        if (all)
            criteria_count++;
        if (criteria_count > 1)
        {
            fprintf(stderr, ANSI_COLOR_RED "Error: Please provide only one of --id or --last or --all for exercise display.\n" ANSI_COLOR_RESET);
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
        else if (all)
        {
            return show_all_exercises();
        }
    }
    if (strcmp(type, "log") == 0)
    {
        // If --from or --to is given, --last, --id and --all is not allowed
        if ((strlen(from_date) > 0 || strlen(to_date) > 0) && (last_n != -1 || strlen(id) > 0 || all))
        {
            fprintf(stderr, ANSI_COLOR_RED "Error: Arguments --from and --to cannot be used with --last or --id or --all.\n" ANSI_COLOR_RESET);
            return 1;
        }

        // Check for alteast one of --id or --last or --from or --to or --add
        if (strlen(id) == 0 && last_n == -1 && strlen(from_date) == 0 && strlen(to_date) == 0 && !all)
        {
            fprintf(stderr, ANSI_COLOR_RED "Error: Please provide --id or --last or --from or --to or --all for log display.\n" ANSI_COLOR_RESET);
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
        if (all)
            if (criteria_count > 1)
            {
                fprintf(stderr, ANSI_COLOR_RED "Error: Please provide only one of --id or --last or --from --to or --all for log display.\n" ANSI_COLOR_RESET);
                return 1;
            }

        // Display from id
        if (strlen(id) > 0)
        {
            return show_workout_by_id(id);
        }
        // Display last n workouts
        else if (last_n != -1)
        {
            return show_last_n_workouts(last_n);
        }
        // Display all workouts
        else if (all)
        {
            return show_all_workouts();
        }
        // Display workouts --from and --to
        if (strlen(from_date) > 0 || strlen(to_date) > 0)
        {
            // If only --from is given, set --to as today
            if (strlen(from_date) > 0 && strlen(to_date) == 0)
            {
                char *today = get_today_date();
                strcpy(to_date, today);
            }
            // If only --to is given, set --from as a very old date
            else if (strlen(from_date) == 0 && strlen(to_date) > 0)
            {
                DateFormat config_date_format = get_config_date_format();
                char *old_date = get_date_in_format("1900-01-01", config_date_format);
                strcpy(from_date, old_date);
            }

            printf("Showing workouts from " ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET " to " ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET ":\n", from_date, to_date);

            // Validate date formats
            DateFormat config_date_format = get_config_date_format();

            if (!is_valid_date_format(from_date, config_date_format))
            {
                fprintf(stderr, ANSI_COLOR_RED "Error: Invalid date format for --from. Please use the configured date format: %s\n" ANSI_COLOR_RESET, config_date_format == DATE_DD_MM_YYYY ? "DD-MM-YYYY" : config_date_format == DATE_MM_DD_YYYY ? "MM-DD-YYYY"
                                                                                                                                                                                                                                                   : "YYYY-MM-DD");
                return 1;
            }
            if (!is_valid_date_format(to_date, config_date_format))
            {
                fprintf(stderr, ANSI_COLOR_RED "Error: Invalid date format for --to. Please use the configured date format: %s\n" ANSI_COLOR_RESET, config_date_format == DATE_DD_MM_YYYY ? "DD-MM-YYYY" : config_date_format == DATE_MM_DD_YYYY ? "MM-DD-YYYY"
                                                                                                                                                                                                                                                 : "YYYY-MM-DD");
                return 1;
            }

            // Convert dates to standard format for comparison
            char std_from_date[11];
            char std_to_date[11];

            // Get standardized dates and store them in local variables
            char *temp_from_date = convert_date_to_standard(from_date, config_date_format);
            char *temp_to_date = convert_date_to_standard(to_date, config_date_format);

            if (temp_from_date == NULL || temp_to_date == NULL)
            {
                fprintf(stderr, ANSI_COLOR_RED "Error: Failed to convert date formats.\n" ANSI_COLOR_RESET);
                return 1;
            }

            // Copy the results to our local buffers to ensure we have our own copies
            strcpy(std_from_date, temp_from_date);
            strcpy(std_to_date, temp_to_date);

            return show_workouts_in_date_range(std_from_date, std_to_date);
        }
    }

    return 0;
}