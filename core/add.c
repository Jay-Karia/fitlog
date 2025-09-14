#include "../include/fitlog.h"

int cmd_add(int argc, char *argv[])
{
    // Check for help flag
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
        {
            print_add_help();
            return 0;
        }
    }

    // Get the name of the exercise
    char exercise_name[100] = "";
    if (argc < 2)
    {
        fprintf(stderr, ANSI_COLOR_RED "No exercise name provided.\n" ANSI_COLOR_RESET);
        return 1;
    }
    else
    {
        strncpy(exercise_name, argv[1], sizeof(exercise_name) - 1);
        exercise_name[sizeof(exercise_name) - 1] = '\0';
    }

    // Check if exercise exists
    if (!exercise_exists(exercise_name))
    {
        if (shortcut_exists(exercise_name))
        {
            strcpy(exercise_name, get_exercise_name_from_shortcut(exercise_name));
        }
        else
        {
            fprintf(stderr, ANSI_COLOR_RED "Exercise '%s' does not exist. Please add it first using 'fitlog add-exercise'.\n" ANSI_COLOR_RESET, exercise_name);
            return 1;
        }
    }

    // Get the date format
    enum DateFormat date_format = get_config_date_format();

    // Get the date argument
    char date_str[20] = "";
    char standard_date_str[20];

    for (int i = 2; i < argc; i++)
    {
        if ((strcmp(argv[i], "--date") == 0 || strcmp(argv[i], "-d") == 0) && i + 1 < argc)
        {
            strncpy(date_str, argv[++i], sizeof(date_str) - 1);
            date_str[sizeof(date_str) - 1] = '\0';
        }
    }

    // Use today date if not given date
    if (strlen(date_str) == 0)
    {
        strcpy(date_str, get_today_date(date_format));
    }
    else
    {

        char *date_format_str;
        switch (date_format)
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
        if (!is_valid_date_format(date_str, date_format))
        {
            fprintf(stderr, ANSI_COLOR_RED "Invalid date format! Expected %s\n" ANSI_COLOR_RESET, date_format_str);
            return 1;
        }
    }
    // Check the earguments based on exercise type
    enum ExerciseType type = check_exercise_type(exercise_name);

    // Update the date string to standard format (YYYY-MM-DD)
    strcpy(standard_date_str, convert_date_to_standard(date_str, date_format));

    // Get notes
    char notes[256] = "";
    for (int i = 2; i < argc; i++)
    {
        if ((strcmp(argv[i], "--notes") == 0 || strcmp(argv[i], "-n") == 0) && i + 1 < argc)
        {
            strncpy(notes, argv[++i], sizeof(notes) - 1);
            notes[sizeof(notes) - 1] = '\0';
        }
    }

    if (type == TYPE_WEIGHT)
    {
        // Check for reps, sets and weight options
        int reps = -1;
        int sets = -1;
        float weight = -1.0;

        for (int i = 2; i < argc; i++)
        {
            if (strcmp(argv[i], "--reps") == 0 || strcmp(argv[i], "-r") == 0)
            {
                if (i + 1 < argc)
                {
                    reps = atoi(argv[++i]);
                }
            }
            else if (strcmp(argv[i], "--weight") == 0 || strcmp(argv[i], "-w") == 0)
            {
                if (i + 1 < argc)
                {
                    weight = atof(argv[++i]);
                }
            }
            else if (strcmp(argv[i], "--sets") == 0 || strcmp(argv[i], "-s") == 0)
            {
                if (i + 1 < argc)
                {
                    sets = atoi(argv[++i]);
                }
            }
            else if (strcmp(argv[i], "--date") == 0 || strcmp(argv[i], "-d") == 0)
            {
                if (i + 1 < argc)
                {
                    i++;
                    continue;
                }
            }
            else if (strcmp(argv[i], "--notes") == 0 || strcmp(argv[i], "-n") == 0)
            {
                if (i + 1 < argc)
                {
                    i++;
                    continue;
                }
            }
            else
            {
                fprintf(stderr, ANSI_COLOR_RED "Unknown option: %s\n" ANSI_COLOR_RESET, argv[i]);
                return 1;
            }
        }

        // Check if reps, sets and weight are provided
        if (reps == -1 || weight == -1.0 || sets == -1)
        {
            fprintf(stderr, ANSI_COLOR_RED "--reps, --weight, and --sets must be provided for weight-based exercises.\n" ANSI_COLOR_RESET);
            return 1;
        }

        // Get the unit of weight
        char *weight_unit_str = get_config_weight_unit() == UNIT_KG ? "kg" : "lbs";
        char weight_str[50];
        snprintf(weight_str, sizeof(weight_str), "%.2f %s", weight, weight_unit_str);

        // Get the id
        int next_id = get_next_exercise_id();
        increment_exercise_id();

        // Update the database
        char db_path[256];
        sprintf(db_path, "%s/%s", FITLOG_DIR, WORKOUTS_FILE);

        FILE *fp = fopen(db_path, "a");
        if (fp == NULL)
        {
            perror(ANSI_COLOR_RED "Error: Could not open workouts database" ANSI_COLOR_RESET);
            return 1;
        }

        fprintf(fp, "%d,%s,%d,%d,%s,%s,%s,%s,%s\n",
                next_id,
                exercise_name,
                sets,
                reps,
                weight_str,
                "",
                "",
                standard_date_str,
                notes);

        fclose(fp);

        // Print the exercise details
        printf("+------------------------------+\n");
        printf(BOLD_TEXT "Exercise Logged :\n" ANSI_COLOR_RESET);
        printf(DARK_GRAY_TEXT "  ID: %d\n" ANSI_COLOR_RESET, next_id);
        printf("  Name: %s\n", exercise_name);
        printf(DARK_GRAY_TEXT "  Sets: %d\n" ANSI_COLOR_RESET, sets);
        printf("  Reps: %d\n", reps);
        printf(DARK_GRAY_TEXT "  Weight: %.2f %s\n" ANSI_COLOR_RESET, weight, weight_unit_str);
        printf("  Date: %s\n", standard_date_str);
        printf(DARK_GRAY_TEXT "  Notes: %s\n" ANSI_COLOR_RESET, strlen(notes) == 0 ? "(null)" : notes);
        printf("+------------------------------+\n");
    }
    else if (type == TYPE_TIME)
    {
        // Check for time, distance and sets option
        int duration = -1;
        int sets = -1;
        int distance = -1;

        for (int i = 2; i < argc; i++)
        {
            if ((strcmp(argv[i], "--time") == 0 || strcmp(argv[i], "-t") == 0) && i + 1 < argc)
            {
                duration = atoi(argv[++i]);
            }
            else if ((strcmp(argv[i], "--sets") == 0 || strcmp(argv[i], "-s") == 0) && i + 1 < argc)
            {
                sets = atoi(argv[++i]);
            }
            else if ((strcmp(argv[i], "--distance") == 0 || strcmp(argv[i], "-dist") == 0) && i + 1 < argc)
            {
                distance = atoi(argv[++i]);
            }
            else if ((strcmp(argv[i], "--date") == 0 || strcmp(argv[i], "-d") == 0) && i + 1 < argc)
            {
                i++;
                continue;
            }
            else if ((strcmp(argv[i], "--notes") == 0 || strcmp(argv[i], "-n") == 0) && i + 1 < argc)
            {
                i++;
                continue;
            }
            else
            {
                fprintf(stderr, ANSI_COLOR_RED "Unknown option: %s\n" ANSI_COLOR_RESET, argv[i]);
                return 1;
            }
        }

        if (duration == -1)
        {
            fprintf(stderr, ANSI_COLOR_RED "--time must be provided for time-based exercises.\n" ANSI_COLOR_RESET);
            return 1;
        }

        // Get time unit
        char *time_unit_str;
        switch (get_config_time_unit())
        {
        case TIME_S:
            time_unit_str = "s";
            break;
        case TIME_HR:
            time_unit_str = "hr";
            break;
        default:
            time_unit_str = "min";
            break;
        }
        char time_str[50];
        snprintf(time_str, sizeof(time_str), "%.2d %s", duration, time_unit_str);

        // Get the id
        int next_id = get_next_exercise_id();
        increment_exercise_id();

        // Update the database
        char db_path[256];
        sprintf(db_path, "%s/%s", FITLOG_DIR, WORKOUTS_FILE);

        FILE *fp = fopen(db_path, "a");
        if (fp == NULL)
        {
            perror(ANSI_COLOR_RED "Error: Could not open workouts database" ANSI_COLOR_RESET);
            return 1;
        }

        char sets_str[20];
        if (sets == -1)
        {
            strcpy(sets_str, "");
        }
        else
        {
            snprintf(sets_str, sizeof(sets_str), "%d", sets);
        }

        char distance_str[20];
        if (distance == -1)
        {
            strcpy(distance_str, "");
        }
        else
        {
            snprintf(distance_str, sizeof(distance_str), "%d", distance);
        }

        // Get the distance unit
        char *distance_unit_str;
        switch (get_config_distance_unit())
        {
        case DISTANCE_KM:
            distance_unit_str = "km";
            break;
        case DISTANCE_MI:
            distance_unit_str = "miles";
            break;
        default:
            distance_unit_str = "m";
            break;
        }
        if (distance != -1)
        {
            strcat(distance_str, " ");
            strcat(distance_str, distance_unit_str);
        }

        fprintf(fp, "%d,%s,%s,%s,%s,%s,%s,%s,%s\n",
                next_id,
                exercise_name,
                sets_str,
                "",
                "",
                time_str,
                distance_str,
                standard_date_str,
                notes);

        fclose(fp);

        // Print the exercise details
        printf("+------------------------------+\n");
        printf(BOLD_TEXT "Exercise Logged :\n" ANSI_COLOR_RESET);
        printf(DARK_GRAY_TEXT "  ID: %d\n" ANSI_COLOR_RESET, next_id);
        printf("  Name: %s\n", exercise_name);
        printf(DARK_GRAY_TEXT "  Sets: %s\n" ANSI_COLOR_RESET, sets_str);
        printf("  Distance: %s\n", distance_str);
        printf(DARK_GRAY_TEXT "  Duration: %s\n" ANSI_COLOR_RESET, time_str);
        printf("  Date: %s\n", standard_date_str);
        printf(DARK_GRAY_TEXT "  Notes: %s\n" ANSI_COLOR_RESET, strlen(notes) == 0 ? "(null)" : notes);
        printf("+------------------------------+\n");
    }
    else if (type == TYPE_BODY)
    {
        int sets = -1;
        int reps = -1;

        // Check for --sets and --reps required options
        for (int i = 2; i < argc; i++)
        {
            if ((strcmp(argv[i], "--sets") == 0 || strcmp(argv[i], "-s") == 0) && i + 1 < argc)
            {
                sets = atoi(argv[++i]);
            }
            else if ((strcmp(argv[i], "--reps") == 0 || strcmp(argv[i], "-r") == 0) && i + 1 < argc)
            {
                reps = atoi(argv[++i]);
            }
            else if ((strcmp(argv[i], "--date") == 0 || strcmp(argv[i], "-d") == 0) && i + 1 < argc)
            {
                i++;
                continue;
            }
            else if ((strcmp(argv[i], "--notes") == 0 || strcmp(argv[i], "-n") == 0) && i + 1 < argc)
            {
                i++;
                continue;
            }
            else
            {
                fprintf(stderr, ANSI_COLOR_RED "Unknown option: %s\n" ANSI_COLOR_RESET, argv[i]);
                return 1;
            }
        }

        if (sets == -1 || reps == -1)
        {
            fprintf(stderr, ANSI_COLOR_RED "--sets and --reps must be provided for bodyweight exercises.\n" ANSI_COLOR_RESET);
            return 1;
        }

        // Get the id
        int next_id = get_next_exercise_id();
        increment_exercise_id();

        // Update the database
        char db_path[256];
        sprintf(db_path, "%s/%s", FITLOG_DIR, WORKOUTS_FILE);

        FILE *fp = fopen(db_path, "a");
        if (fp == NULL)
        {
            perror(ANSI_COLOR_RED "Error: Could not open workouts database" ANSI_COLOR_RESET);
            return 1;
        }

        fprintf(fp, "%d,%s,%d,%d,%s,%s,%s,%s,%s\n",
                next_id,
                exercise_name,
                sets,
                reps,
                "",
                "",
                "",
                standard_date_str,
                notes);

        fclose(fp);

        // Print the exercise details
        printf("+------------------------------+\n");
        printf(BOLD_TEXT "Exercise Logged :\n" ANSI_COLOR_RESET);
        printf(DARK_GRAY_TEXT "  ID: %d\n" ANSI_COLOR_RESET, next_id);
        printf("  Name: %s\n", exercise_name);
        printf(DARK_GRAY_TEXT "  Sets: %d\n" ANSI_COLOR_RESET, sets);
        printf("  Reps: %d\n", reps);
        printf("  Date: %s\n", standard_date_str);
        printf(DARK_GRAY_TEXT "  Notes: %s\n" ANSI_COLOR_RESET, strlen(notes) == 0 ? "(null)" : notes);
        printf("+------------------------------+\n");
    }
    else
    {
        fprintf(stderr, ANSI_COLOR_RED "Unknown exercise type for '%s'.\n" ANSI_COLOR_RESET, exercise_name);
        return 1;
    }

    return 0;
}