#include "../include/fitlog.h"
#include <sys/stat.h>

int cmd_create(int argc, char *argv[])
{
    // Check for help flag
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
        {
            print_create_help();
            return 0;
        }
    }

    if (argc < 2)
    {
        fprintf(stderr, ANSI_COLOR_RED "Error: Exercise name is required.\n" ANSI_COLOR_RESET);
        return 1;
    }

    // Get the name of the exercise
    const char *exercise_name = argv[1];

    // Parse command-line options first to check for shortcuts later
    const char *description = NULL;
    const char *shortcut = NULL;

    // Optional shortcut
    if (argc >= 3 && argv[2][0] != '-')
    {
        shortcut = argv[2];
        // Shift remaining args left so the options loop works unchanged
        for (int j = 3; j < argc; j++)
        {
            argv[j - 1] = argv[j];
        }
        argc--;
    }

    // Get the default exercise type from config (sets or time)
    char default_type_str[32];
    read_config_value("default_exercise_type", "time", default_type_str, sizeof(default_type_str));
    enum ExerciseType type;
    switch (default_type_str[0])
    {
    case 't':
        if (strcmp(default_type_str, "time") == 0)
        {
            type = TYPE_TIME;
        }
        else
        {
            type = TYPE_TIME; // default
        }
        break;
    case 's':
        if (strcmp(default_type_str, "sets") == 0)
        {
            type = TYPE_SETS;
        }
        else
        {
            type = TYPE_TIME; // default
        }
        break;
    case 'b':
        if (strcmp(default_type_str, "body") == 0)
        {
            type = TYPE_BODY;
        }
        else
        {
            type = TYPE_TIME; // default
        }
        break;
    default:
        type = TYPE_TIME; // default
        break;
    }

    for (int i = 2; i < argc; i++)
    {
        if ((strcmp(argv[i], "--desc") == 0 || strcmp(argv[i], "-d") == 0) && i + 1 < argc)
        {
            description = argv[++i];
        }
        else if ((strcmp(argv[i], "--type") == 0 || strcmp(argv[i], "-t") == 0) && i + 1 < argc)
        {
            i++; // Move to the type value
            if (strcmp(argv[i], "sets") == 0)
            {
                type = TYPE_SETS;
            }
            else if (strcmp(argv[i], "time") == 0)
            {
                type = TYPE_TIME;
            }
            else if (strcmp(argv[i], "body") == 0)
            {
                type = TYPE_BODY;
            }
            else
            {
                fprintf(stderr, ANSI_COLOR_RED "Error: Invalid exercise type. Use 'sets' or 'time' or 'body'.\n" ANSI_COLOR_RESET);
                return 1;
            }
        }
        else
        {
            fprintf(stderr, ANSI_COLOR_RED "Unknown option: %s\n" ANSI_COLOR_RESET, argv[i]);
            return 1;
        }
    }

    // Make sure fitlog is initialized
    struct stat st = {0};
    if (stat(FITLOG_DIR, &st) == -1)
    {
        fprintf(stderr, ANSI_COLOR_RED "Error: fitlog is not initialized. Run 'fitlog init' first.\n" ANSI_COLOR_RESET);
        return 1;
    }

    // Check the database for existing exercise or shortcut
    char db_path[256];
    sprintf(db_path, "%s/%s", FITLOG_DIR, EXERCISES_FILE);

    FILE *rfp = fopen(db_path, "r");
    if (rfp == NULL)
    {
        // Directory exists but file doesn't
        printf(ANSI_COLOR_RED "Error: Exercise database not found\n" ANSI_COLOR_RESET);
        return 1;
    }
    else
    {
        // Check for existing exercise name (2nd column) or shortcut (3rd column)
        char line[1024];
        while (fgets(line, sizeof(line), rfp))
        {
            if (line[0] == '\n' || line[0] == '\r' || line[0] == '#')
                continue;

            char tmp[1024];
            strncpy(tmp, line, sizeof(tmp) - 1);
            tmp[sizeof(tmp) - 1] = '\0';

            // Column order: id, name, shortcut, description, type
            strtok(tmp, ",\n\r");
            char *name = strtok(NULL, ",\n\r");
            char *shortcut_check = strtok(NULL, ",\n\r");

            if (name && strcmp(name, exercise_name) == 0)
            {
                fprintf(stderr, ANSI_COLOR_RED "Error: Exercise '%s' already exists.\n" ANSI_COLOR_RESET, exercise_name);
                fclose(rfp);
                return 1;
            }

            if (shortcut && shortcut[0] != '\0' && shortcut_check && shortcut_check[0] != '\0')
            {
                if (strcmp(shortcut_check, shortcut) == 0)
                {
                    fprintf(stderr, ANSI_COLOR_RED "Error: Shortcut '%s' already exists.\n" ANSI_COLOR_RESET, shortcut);
                    fclose(rfp);
                    return 1;
                }
            }
        }
        fclose(rfp);
    }

    // Access the database file - path already defined above
    FILE *fp = fopen(db_path, "a");
    if (fp == NULL)
    {
        perror(ANSI_COLOR_RED "Error: Could not open exercises database" ANSI_COLOR_RESET);
        return 1;
    }

    int next_id = get_next_exercise_id();
    increment_exercise_id();

    // Write the new exercise to the file as a new line in CSV format
    fprintf(fp, "%d,%s,%s,%s,%s\n", next_id, exercise_name, shortcut, description, type == TYPE_SETS ? "sets" : (type == TYPE_TIME ? "time" : "body"));
    fclose(fp);

    // Update the shortcuts map file if a shortcut is provided
    if (shortcut && shortcut[0] != '\0')
    {
        char shortcuts_path[256];
        sprintf(shortcuts_path, "%s/shortcuts.ini", FITLOG_DIR);

        // Read the existing shortcuts file
        FILE *sfp = fopen(shortcuts_path, "r");
        if (sfp == NULL)
        {
            // If file doesn't exist, create it
            sfp = fopen(shortcuts_path, "w");
            if (sfp == NULL)
            {
                perror(ANSI_COLOR_RED "Error: Could not create shortcuts file" ANSI_COLOR_RESET);
                return 1;
            }
            fprintf(sfp, "%s=%s\n", shortcut, exercise_name);
            fclose(sfp);
            return 0;
        }

        // Append the new shortcut
        sfp = fopen(shortcuts_path, "a");
        if (sfp == NULL)
        {
            perror(ANSI_COLOR_RED "Error: Could not open shortcuts file for append" ANSI_COLOR_RESET);
            return 1;
        }
        fprintf(sfp, "%s=%s\n", shortcut, exercise_name);
        fclose(sfp);
    }

    // Print the success message
    printf(ANSI_COLOR_GREEN "Exercise '%s' created successfully.\n" ANSI_COLOR_RESET, exercise_name);

    // Print the details in a box
    printf("+------------------------------+\n");
    printf(BOLD_TEXT "Exercise Created:\n" ANSI_COLOR_RESET);
    printf(DARK_GRAY_TEXT "  ID: %d\n" ANSI_COLOR_RESET, next_id);
    printf("  Name: %s\n", exercise_name);
    printf(DARK_GRAY_TEXT "  Shortcut: %s\n" ANSI_COLOR_RESET, shortcut);
    printf("  Description: %s\n", description);
    printf(DARK_GRAY_TEXT "  Type: %s\n" ANSI_COLOR_RESET, type == TYPE_SETS ? "sets" : (type == TYPE_TIME ? "time" : "body"));
    printf("+------------------------------+\n");

    return 0;
}