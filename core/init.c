#include "../include/fitlog.h"
#include <unistd.h>

int cmd_init(int argc, char *argv[])
{
    // Check for help flag
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
        {
            print_init_help();
            return 0;
        }
    }

    // Check for .fitlog directory
    if (access(FITLOG_DIR, F_OK) != -1)
    {
        fprintf(stderr, ANSI_COLOR_RED "fitlog is already initialized.\n\n" ANSI_COLOR_RESET  "Delete the %s directory and re-run this command to re-initialize.\nTHIS WILL REMOVE ALL THE WORKOUT DATA, EXPORT IT FIRST!\n", FITLOG_DIR);
        return 1;
    }

    printf("Initializing fitlog...\n");

    // Create .fitlog directory
    if (mkdir(FITLOG_DIR) == -1)
    {
        perror(ANSI_COLOR_RED "Error creating .fitlog directory" ANSI_COLOR_RESET);
        return 1;
    }
    printf("Created .fitlog directory.\n");

    // Create config file (.ini)
    char config_path[256];
    sprintf(config_path, "%s/%s", FITLOG_DIR, CONFIG_FILE);

    FILE *config_fp = fopen(config_path, "w");
    if (config_fp == NULL)
    {
        perror(ANSI_COLOR_RED "Error creating config file" ANSI_COLOR_RESET);
        return 1;
    }

    fprintf(config_fp, "# Date and time formatting\n");
    fprintf(config_fp, "date_format=YYYY-MM-DD\n");
    fprintf(config_fp, "time_format=24h\n");
    fprintf(config_fp, "show_timestamps=true\n\n");
    fprintf(config_fp, "# Units and measurements\n");
    fprintf(config_fp, "weight_unit=lbs\n");
    fprintf(config_fp, "decimal_places=1\n");
    fprintf(config_fp, "time_unit=s\n\n");
    fprintf(config_fp, "# Behaviour\n");
    fprintf(config_fp, "default_exercise_type=sets\n");

    fclose(config_fp);

    // Create workouts file (.csv)
    char workouts_path[256];
    sprintf(workouts_path, "%s/%s", FITLOG_DIR, WORKOUTS_FILE);

    FILE *workouts_fp = fopen(workouts_path, "w");
    if (workouts_fp == NULL)
    {
        perror(ANSI_COLOR_RED "Error creating workouts data file" ANSI_COLOR_RESET);
        return 1;
    }

    fprintf(workouts_fp, "Id,Exercise,Sets,Reps,Weight,Time,Date,Notes\n");
    fclose(workouts_fp);

    // Create the shortcuts map file
    char shortcuts_path[256];
    sprintf(shortcuts_path, "%s/%s", FITLOG_DIR, SHORTCUTS_FILE);
    FILE *shortcuts_fp = fopen(shortcuts_path, "w");
    if (shortcuts_fp == NULL)
    {
        perror(ANSI_COLOR_RED "Error creating shortcuts map file" ANSI_COLOR_RESET);
        return 1;
    }

    // Write empty JSON object
    fprintf(shortcuts_fp, "{}\n");

    fclose(shortcuts_fp);

    // Create the exercises database file
    char exercises_path[256];
    sprintf(exercises_path, "%s/%s", FITLOG_DIR, EXERCISES_FILE);
    FILE *exercises_fp = fopen(exercises_path, "w");

    if (exercises_fp == NULL)
    {
        perror(ANSI_COLOR_RED "Error creating exercises database file" ANSI_COLOR_RESET);
        return 1;
    }

    fprintf(exercises_fp, "Id,Name,Shortcut,Description,Type\n");
    fclose(exercises_fp);

    // Create the ID counter file
    char id_path[256];
    sprintf(id_path, "%s/%s", FITLOG_DIR, ID_COUNTER_FILE);
    FILE *id_fp = fopen(id_path, "w");

    if (id_fp == NULL)
    {
        perror(ANSI_COLOR_RED "Error creating ID counter file" ANSI_COLOR_RESET);
        return 1;
    }

    fprintf(id_fp, "1\n");
    fclose(id_fp);

    printf(ANSI_COLOR_GREEN "\nInitialized fitlog\n" ANSI_COLOR_RESET);

    return 0;
}