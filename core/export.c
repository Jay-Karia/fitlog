#include "../include/fitlog.h"
#include <io.h>

int cmd_export(int argc, char *argv[])
{
    // Check fro the --help flag
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
        {
            print_export_help();
            return 0;
        }
    }

    // Check for optional --file file argument
    char output_path[100] = "fitlog_export.json";
    
    for (int i = 1; i < argc; i++)
    {
        if ((strcmp(argv[i], "--file") == 0 || strcmp(argv[i], "-f") == 0) && i + 1 < argc)
        {
            strncpy(output_path, argv[++i], sizeof(output_path) - 1);
            output_path[sizeof(output_path) - 1] = '\0';
        } else {
            fprintf(stderr, ANSI_COLOR_RED "Unknown option: %s\n" ANSI_COLOR_RESET, argv[i]);
            return 1;
        }
    }

    // Get confirmation if file already exists
    if (access(output_path, F_OK) != -1)
    {
        char response[10];
        printf(ANSI_COLOR_YELLOW "Warning: File '%s' already exists. Overwrite? (y/N): " ANSI_COLOR_RESET, output_path);
        if (fgets(response, sizeof(response), stdin) != NULL)
        {
            if (response[0] != 'y' && response[0] != 'Y')
            {
                printf("Export cancelled.\n");
                return 0;
            }
        }
        else
        {
            printf("Export cancelled.\n");
            return 0;
        }
    }

    // Create a basic JSON structure
    FILE *fp = fopen(output_path, "w");
    if (fp == NULL)
    {
        perror(ANSI_COLOR_RED "Error: Could not open output file for writing" ANSI_COLOR_RESET);
        return 1;
    }

    fprintf(fp, "{\n");
    fprintf(fp, "  \"exercises\": [],\n");
    fprintf(fp, "  \"workouts\": [],\n");
    fprintf(fp, "  \"config\": {},\n");
    fprintf(fp, "  \"shortcuts\": {},\n");
    fprintf(fp, "  \"id_counter\": 0\n");
    fprintf(fp, "}\n");

    // Get the exercises array
    char *exercises_json = get_exercise_array();
    if (exercises_json == NULL)
    {
        fprintf(stderr, ANSI_COLOR_RED "Error: Could not retrieve exercises data\n" ANSI_COLOR_RESET);
        fclose(fp);
        return 1;
    }

    // Get the workouts array
    char *workouts_json = get_workout_array();
    if (workouts_json == NULL)
    {
        fprintf(stderr, ANSI_COLOR_RED "Error: Could not retrieve workouts data\n" ANSI_COLOR_RESET);
        free(exercises_json);
        fclose(fp);
        return 1;
    }
    
    // Get the config object
    char *config_json = get_config_object();
    if (config_json == NULL)
    {
        fprintf(stderr, ANSI_COLOR_RED "Error: Could not retrieve config data\n" ANSI_COLOR_RESET);
        free(exercises_json);
        free(workouts_json);
        fclose(fp);
        return 1;
    }
    
    // Get the shortcuts object
    char *shortcuts_json = get_shortcuts_object();
    if (shortcuts_json == NULL)
    {
        fprintf(stderr, ANSI_COLOR_RED "Error: Could not retrieve shortcuts data\n" ANSI_COLOR_RESET);
        free(exercises_json);
        free(workouts_json);
        free(config_json);
        fclose(fp);
        return 1;
    }
    
    // Get the id_counter value
    int id_counter = get_id_counter_value();
    if (id_counter < 0)
    {
        fprintf(stderr, ANSI_COLOR_RED "Error: Could not retrieve ID counter value\n" ANSI_COLOR_RESET);
        free(exercises_json);
        free(workouts_json);
        free(config_json);
        free(shortcuts_json);
        fclose(fp);
        return 1;
    }

    // Write the final JSON structure
    fseek(fp, 0, SEEK_SET);
    fprintf(fp, "{\n");
    fprintf(fp, "  \"exercises\": %s,\n", exercises_json);
    fprintf(fp, "  \"workouts\": %s,\n", workouts_json);
    fprintf(fp, "  \"config\": %s,\n", config_json);
    fprintf(fp, "  \"shortcuts\": %s,\n", shortcuts_json);
    fprintf(fp, "  \"id_counter\": %d\n", id_counter);
    fprintf(fp, "}\n");

    free(exercises_json);
    free(workouts_json);
    free(config_json);
    free(shortcuts_json);

    fclose(fp);
    char absolute_path[_MAX_PATH];
    if (_fullpath(absolute_path, output_path, _MAX_PATH) != NULL) {
        printf(ANSI_COLOR_GREEN "Data successfully exported to %s\n" ANSI_COLOR_RESET, absolute_path);
    } else {
        printf(ANSI_COLOR_GREEN "Data successfully exported to %s\n" ANSI_COLOR_RESET, output_path);
    }

    return 0;
}