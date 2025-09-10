#include "../include/fitlog.h"

// TODO: update shortcuts file

int cmd_remove(int argc, char *argv[])
{
    // Check for help flag
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
        {
            print_remove_help();
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

    // Get the force option
    bool force = false;
    for (int i = 2; i < argc; i++)
    {
        if (strcmp(argv[i], "--force") == 0)
        {
            force = true;
            break;
        }
    }

    // Get the remove criteria
    char id[20] = "";
    char date[20] = "";

    for (int i = 2; i < argc; i++)
    {
        if (strcmp(argv[i], "--id") == 0 && i + 1 < argc)
        {
            strcpy(id, argv[++i]);
        }
        else if (strcmp(argv[i], "--date") == 0 && i + 1 < argc)
        {
            strcpy(date, argv[++i]);
        }
    }

    // Check if criteria is provided
    if (strcmp(type, "exercise") == 0 && strlen(id) == 0)
    {
        fprintf(stderr, ANSI_COLOR_RED "Error: Please provide --id for exercise removal.\n" ANSI_COLOR_RESET);
        return 1;
    }
    if (strcmp(type, "log") == 0 && strlen(id) == 0 && strlen(date) == 0)
    {
        fprintf(stderr, ANSI_COLOR_RED "Error: Please provide --id or --date for log removal.\n" ANSI_COLOR_RESET);
        return 1;
    }

    // Handle if both id and date are provided
    if (strlen(id) > 0 && strlen(date) > 0)
    {
        fprintf(stderr, ANSI_COLOR_RED "Error: Provide only one of --id or --date, not both.\n" ANSI_COLOR_RESET);
        return 1;
    }

    // Get date format from config
    enum DateFormat config_date_format = get_config_date_format();

    char *date_format_str;
    switch (config_date_format)
    {
    case DATE_DD_MM_YYYY:
        date_format_str = "DD-MM-YYYY";
        break;
    case DATE_MM_DD_YYYY:
        date_format_str = "MM-DD-YYYY";
        break;
    case DATE_YYYY_MM_DD:
        date_format_str = "YYYY-MM-DD";
        break;
    default:
        date_format_str = "Unknown";
        break;
    }

    // Check the date format
    if (!is_valid_date_format(date, config_date_format))
    {
        fprintf(stderr, ANSI_COLOR_RED "Invalid date format! Expected %s\n" ANSI_COLOR_RESET, date_format_str);
        return 1;
    }

    // Handle exercise removal
    if (strcmp(type, "exercise") == 0)
    {
        if (force)
        {
            printf(ANSI_COLOR_YELLOW "Warning: --force flag is used. No confirmation will be asked.\n" ANSI_COLOR_RESET);
            remove_exercise_by_id(id);
            printf(ANSI_COLOR_GREEN "Exercise successfully removed.\n" ANSI_COLOR_RESET);
        }
        else
        {

            // Print the exercise details for confirmation
            bool found = print_exercise_details_from_id(id);
            if (!found)
            {
                return 1; // Exercise not found
            }

            // Ask for confirmation
            char confirmation[10];
            printf(ANSI_COLOR_RED "Are you sure you want to remove this exercise? (y/N): " ANSI_COLOR_RESET);
            fgets(confirmation, sizeof(confirmation), stdin);

            // Check the response
            if (confirmation[0] == 'y' || confirmation[0] == 'Y')
            {
                remove_exercise_by_id(id);
            }
            else
            {
                printf(ANSI_COLOR_YELLOW "Exercise removal cancelled.\n" ANSI_COLOR_RESET);
                return 0;
            }
        }
    }

    // Handle log removal
    if (strcmp(type, "log") == 0)
    {
        // Remove by id
        if (strlen(id) > 0)
        {
            // Print the exercise details for confirmation
            bool found = print_workout_details_from_id(id);
            if (!found && strlen(date) == 0)
            {
                return 1; // Log not found
            }
        }
        // Remove by date
        else if (strlen(date) > 0)
        {
            // Remove by date
            bool found = print_workout_details_from_date(date);
            if (!found && strlen(id) == 0)
            {
                return 1; // No logs found for date
            }
        }

        // Ask for confirmation
        char confirmation[10];
        printf(ANSI_COLOR_RED "Are you sure you want to remove logs? (y/N): " ANSI_COLOR_RESET);
        fgets(confirmation, sizeof(confirmation), stdin);

        // Check the response
        if (confirmation[0] == 'y' || confirmation[0] == 'Y')
        {
            if (strlen(id) > 0)
            {
                remove_workout_by_id(id);
            }
            else if (strlen(date) > 0)
            {
                remove_workouts_by_date(date);
            }
        }
        else
        {
            printf(ANSI_COLOR_YELLOW "Workout removal cancelled.\n" ANSI_COLOR_RESET);
            return 0;
        }
    }

    return 0;
}