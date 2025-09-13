#include <../include/fitlog.h>
#include <io.h>

int cmd_import(int argc, char *argv[])
{
    // Check for help flag
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
        {
            print_import_help();
            return 0;
        }
    }

    // Check for file name argument
    if (argc < 2)
    {
        fprintf(stderr, ANSI_COLOR_RED "Error: No import file specified.\n" ANSI_COLOR_RESET);
        return 1;
    }

    const char *import_file = argv[1];

    // Check if file exists
    FILE *fp = fopen(import_file, "r");
    if (fp == NULL)
    {
        perror(ANSI_COLOR_RED "Error: Could not open import file" ANSI_COLOR_RESET);
        return 1;
    }

    // Read the json file
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *json_data = (char *)malloc(file_size + 1);
    if (json_data == NULL)
    {
        perror(ANSI_COLOR_RED "Error: Memory allocation failed" ANSI_COLOR_RESET);
        fclose(fp);
        return 1;
    }

    fread(json_data, 1, file_size, fp);
    json_data[file_size] = '\0';
    fclose(fp);

    // Check the .fitlog directory
    if (access(FITLOG_DIR, F_OK) == -1)
    {
        fprintf(stderr, ANSI_COLOR_RED "Error: fitlog is not initialized. Please run 'fitlog init' first.\n" ANSI_COLOR_RESET);
        free(json_data);
        return 1;
    }

    // Get the exercises array
    char *exercise_array = read_exercise_array(json_data);
    if (exercise_array != NULL)
    {
        // Write the exercises array to the exercises file
        char exercises_path[256];
        sprintf(exercises_path, "%s/%s", FITLOG_DIR, EXERCISES_FILE);
        int exercises_written = write_csv_from_array(exercise_array, exercises_path);
        if (exercises_written <= 0)
        {
            fprintf(stderr, ANSI_COLOR_RED "Warning: Could not write exercises data to file.\n" ANSI_COLOR_RESET);
        }
        free(exercise_array);
    }
    else
    {
        printf("No exercises data found in the import file.\n");
    }

    // Get the workouts array
    char *workout_array = read_workout_array(json_data);
    if (workout_array != NULL)
    {
        // Write the workouts array to the workouts file
        char workouts_path[256];
        sprintf(workouts_path, "%s/%s", FITLOG_DIR, WORKOUTS_FILE);
        int workouts_written = write_csv_from_array(workout_array, workouts_path);
        if (workouts_written <= 0)
        {
            fprintf(stderr, ANSI_COLOR_RED "Warning: Could not write workouts data to file.\n" ANSI_COLOR_RESET);
        }
        free(workout_array);
    }
    else
    {
        printf("No workouts data found in the import file.\n");
    }

    // Get the config object
    char *config_object = read_config_object(json_data);
    if (config_object != NULL)
    {
        char *config_ini = object_to_ini(config_object);
        free(config_object);

        if (config_ini != NULL)
        {
            // Write the config ini to the config file
            char config_path[256];
            sprintf(config_path, "%s/%s", FITLOG_DIR, CONFIG_FILE);
            FILE *config_fp = fopen(config_path, "w");
            if (config_fp != NULL)
            {
                // Use fputs for direct writing without formatting
                fputs(config_ini, config_fp);
                fclose(config_fp);
            }
            else
            {
                fprintf(stderr, ANSI_COLOR_RED "Warning: Could not write config data to file.\n" ANSI_COLOR_RESET);
            }
            free(config_ini);
        }
        else
        {
            fprintf(stderr, ANSI_COLOR_RED "Warning: Could not convert config data to INI format.\n" ANSI_COLOR_RESET);
        }
    }
    else
    {
        printf(ANSI_COLOR_YELLOW "No config data found in the import file.\n" ANSI_COLOR_RESET);
    }

    // Get the shortcuts object
    char *shortcuts_object = read_shortcuts_object(json_data);
    if (shortcuts_object != NULL)
    {
        char *shortcuts_ini = object_to_ini(shortcuts_object);
        free(shortcuts_object);

        if (shortcuts_ini != NULL)
        {
            // Write the shortcuts ini to the shortcuts file
            char shortcuts_path[256];
            sprintf(shortcuts_path, "%s/%s", FITLOG_DIR, SHORTCUTS_FILE);
            FILE *shortcuts_fp = fopen(shortcuts_path, "w");
            if (shortcuts_fp != NULL)
            {
                // Use fputs for direct writing without formatting
                fputs(shortcuts_ini, shortcuts_fp);
                fclose(shortcuts_fp);
            }
            else
            {
                fprintf(stderr, ANSI_COLOR_RED "Warning: Could not write shortcuts data to file.\n" ANSI_COLOR_RESET);
            }
            free(shortcuts_ini);
        }
        else
        {
            fprintf(stderr, ANSI_COLOR_RED "Warning: Could not convert shortcuts data to INI format.\n" ANSI_COLOR_RESET);
        }
    }
    else
    {
        printf(ANSI_COLOR_YELLOW "No shortcuts data found in the import file.\n" ANSI_COLOR_RESET);
    }

    // Get the id counter value

    free(json_data);

    printf(ANSI_COLOR_GREEN "Import completed.\n" ANSI_COLOR_RESET);
    return 0;
}