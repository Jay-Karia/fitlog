#include <../include/fitlog.h>
#include <stdio.h>

int cmd_nuke(int argc, char *argv[])
{
    // Check for help flag
    if (argc > 1 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0))
    {
        print_nuke_help();
        return 0;
    }

    // Check for force flag
    bool force = false;
    if (argc > 1 && (strcmp(argv[1], "--force") == 0 || strcmp(argv[1], "-f") == 0))
    {
        force = true;
    }
    // If force flag is set, skip confirmation and delete immediately
    if (force)
    {
        if (remove_directory(FITLOG_DIR) != 0)
        {
            fprintf(stderr, ANSI_COLOR_RED "Error: Failed to delete the .fitlog directory.\n" ANSI_COLOR_RESET);
            return 1;
        }
        printf(ANSI_COLOR_GREEN ".fitlog directory deleted successfully.\n" ANSI_COLOR_RESET);
        return 0;
    }
    // Confirm with the user before proceeding
    char confirmation[4];
    printf(ANSI_COLOR_YELLOW "Are you sure you want to delete the entire .fitlog directory? This action cannot be undone. (y/N): " ANSI_COLOR_RESET);
    scanf("%3s", confirmation);

    if (strcmp(confirmation, "y") == 0)
    {
        // Proceed to delete the .fitlog directory
        if (remove_directory(FITLOG_DIR) != 0)
        {
            fprintf(stderr, ANSI_COLOR_RED "Error: Failed to delete the .fitlog directory.\n" ANSI_COLOR_RESET);
            return 1;
        }
        printf(ANSI_COLOR_GREEN ".fitlog directory deleted successfully.\n" ANSI_COLOR_RESET);

        return 0;
    }

    printf("Nuke operation cancelled.\n");
    return 0;
}