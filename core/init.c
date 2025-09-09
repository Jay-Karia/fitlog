#include "../include/fitlog.h"
#include <unistd.h>

#define FITLOG_DIR ".fitlog"
#define CONFIG_FILE "config.ini"
#define WORKOUTS_FILE "workouts.csv"
#define SHORTCUTS_FILE "shortcuts.map"

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

    printf("Initializing fitlog...\n");

    // Check for .fitlog directory
    if (access(FITLOG_DIR, F_OK) != -1)
    {
        fprintf(stderr, "fitlog is already initialized in this directory.\n\n");
        fprintf(stderr, "Delete the %s directory and re-run this command to re-initialize.\nTHIS WILL REMOVE ALL THE WORKOUT DATA, EXPORT IT FIRST!\n", FITLOG_DIR);
        return 1;
    }

    // Create .fitlog directory
    if (mkdir(FITLOG_DIR) == -1)
    {
        perror("Error creating .fitlog directory");
        return 1;
    }
    printf("Created .fitlog directory.\n");

    // Create config file (.ini)
    char config_path[256];
    sprintf(config_path, "%s/%s", FITLOG_DIR, CONFIG_FILE);

    FILE *config_fp = fopen(config_path, "w");
    if (config_fp == NULL)
    {
        perror("Error creating config file");
        return 1;
    }

    fprintf(config_fp, "# Date and time formatting\n");
    fprintf(config_fp, "date_format=YYYY-MM-DD\n");
    fprintf(config_fp, "time_format=24h\n");
    fprintf(config_fp, "show_timestamps=true\n\n");
    fprintf(config_fp, "# Units and measurements\n");
    fprintf(config_fp, "weight_unit=lbs\n");
    fprintf(config_fp, "decimal_places=1\n");

    fclose(config_fp);

    // Create workouts file (.csv)
    char workouts_path[256];
    sprintf(workouts_path, "%s/%s", FITLOG_DIR, WORKOUTS_FILE);

    FILE *workouts_fp = fopen(workouts_path, "w");
    if (workouts_fp == NULL) {
        perror("Error creating workouts data file");
        return 1;
    }

    fprintf(workouts_fp, "Date,Exercise,Sets,Reps,Weight,Notes\n");
    fclose(workouts_fp);

    // Create the shortcuts map file
    char shortcuts_path[256];
    sprintf(shortcuts_path, "%s/%s", FITLOG_DIR, SHORTCUTS_FILE);
    FILE *shortcuts_fp = fopen(shortcuts_path, "w");
    if (shortcuts_fp == NULL) {
        perror("Error creating shortcuts map file");
        return 1;
    }

    fclose(shortcuts_fp);

    printf("Initialized fitlog in ./%s/\n", FITLOG_DIR);

    return 0;
}