#include "../include/fitlog.h"

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

    // Check for optional --output file argument
    char output_path[100] = "fitlog_export.json";
    
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--output") == 0 && i + 1 < argc)
        {
            strncpy(output_path, argv[++i], sizeof(output_path) - 1);
            output_path[sizeof(output_path) - 1] = '\0';
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
    // Get the workouts array
    // Get the config object
    // Get the shortcuts object
    // Get the id_counter value

    fclose(fp);
    printf(ANSI_COLOR_GREEN "Exported data to %s\n" ANSI_COLOR_RESET, output_path);

    return 0;
}