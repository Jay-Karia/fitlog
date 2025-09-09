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
        fprintf(stderr, "Error: Exercise name is required.\n");
        return 1;
    }

    // Get the name of the exercise
    const char *exercise_name = argv[1];

    // Parse command-line options first to check for shortcuts later
    const char *description = NULL;
    const char *shortcut = NULL;
    enum ExerciseType type = TYPE_SETS;

    for (int i = 2; i < argc; i++)
    {
        if (strcmp(argv[i], "--desc") == 0 && i + 1 < argc)
        {
            description = argv[++i];
        }
        else if (strcmp(argv[i], "--shortcut") == 0 && i + 1 < argc)
        {
            shortcut = argv[++i];
        }
        else if (strcmp(argv[i], "--type") == 0 && i + 1 < argc)
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
            else
            {
                fprintf(stderr, "Error: Invalid exercise type. Use 'sets' or 'time'.\n");
                return 1;
            }
        }
        else
        {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            print_create_help();
            return 1;
        }
    }

    // Make sure fitlog is initialized
    struct stat st = {0};
    if (stat(FITLOG_DIR, &st) == -1)
    {
        fprintf(stderr, "Error: fitlog is not initialized. Run 'fitlog init' first.\n");
        return 1;
    }

    // Check the database for existing exercise or shortcut
    char db_path[256];
    sprintf(db_path, "%s/%s", FITLOG_DIR, EXERCISES_FILE);

    FILE *rfp = fopen(db_path, "r");
    if (rfp == NULL)
    {
        // Directory exists but file doesn't
        printf("Exercise database not found\n");
        return 1;
    }
    else
    {
        // Check for existing exercise name or shortcut
        char line[1024];
        while (fgets(line, sizeof(line), rfp))
        {
            // Make a copy of the line for strtok operations
            char tmp[1024];
            strncpy(tmp, line, sizeof(tmp) - 1);
            tmp[sizeof(tmp) - 1] = '\0';

            // Extract and check exercise name
            char *name = strtok(tmp, ",\n\r");
            if (name && strcmp(name, argv[1]) == 0)
            {
                fprintf(stderr, "Error: Exercise '%s' already exists.\n", argv[1]);
                fclose(rfp);
                return 1;
            }

            // Extract and check shortcut if one is provided in the command
            char *shortcut_check = strtok(NULL, ",\n\r");

            // Only check shortcuts if we're providing one in this command
            if (shortcut != NULL && shortcut_check != NULL && shortcut_check[0] != '\0')
            {
                if (strcmp(shortcut_check, shortcut) == 0)
                {
                    fprintf(stderr, "Error: Shortcut '%s' already exists.\n", shortcut);
                    fclose(rfp);
                    return 1;
                }
            }
        }
        fclose(rfp);
    }
    // Print the message
    printf("Creating exercise: %s\n", exercise_name);
    if (description)
    {
        printf("  Description: %s\n", description);
    }
    if (shortcut)
    {
        printf("  Shortcut: %s\n", shortcut);
    }
    printf("  Type: %s\n", type == TYPE_SETS ? "sets" : "time");

    // Access the database file - path already defined above
    // Directory existence already checked at the beginning

    FILE *fp = fopen(db_path, "a");
    if (fp == NULL)
    {
        perror("Error: Could not open exercises database");
        return 1;
    }

    // Write the new exercise to the file as a new line in CSV format
    fprintf(fp, "%s,%s,%s,%s\n", exercise_name, shortcut, description, type == TYPE_SETS ? "sets" : "time");
    fclose(fp);

    // Print the final message
    printf("Exercise '%s' created successfully.\n", exercise_name);

    return 0;
}