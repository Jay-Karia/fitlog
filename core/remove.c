#include "../include/fitlog.h"

int cmd_remove(int argc, char* argv[]) {
    // Check for help flag
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_remove_help();
            return 0;
        }
    }

    // Get the type
    char type[20] = "";
    if (argc > 1 && strcmp(argv[1], "exercise") == 0) {
        strcpy(type, "exercise");
    } else if (argc > 1 && strcmp(argv[1], "log") == 0) {
        strcpy(type, "log");
    } else {
        fprintf(stderr, ANSI_COLOR_RED "Error: Missing or invalid type. Use 'exercise' or 'log'.\n" ANSI_COLOR_RESET);
        return 1;
    }

    // Get the remove criteria
    char id[20] = "";
    char date[20] = "";

    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "--id") == 0 && i + 1 < argc) {
            strcpy(id, argv[++i]);
        } else if (strcmp(argv[i], "--date") == 0 && i + 1 < argc) {
            strcpy(date, argv[++i]);
        }
    }

    // Check if criteria is provided
    if (strcmp(type, "exercise") == 0 && strlen(id) == 0) {
        fprintf(stderr, ANSI_COLOR_RED "Error: Please provide --id for exercise removal.\n" ANSI_COLOR_RESET);
        return 1;
    }
    if (strcmp(type, "log") == 0 && strlen(id) == 0 && strlen(date) == 0) {
        fprintf(stderr, ANSI_COLOR_RED "Error: Please provide --id or --date for log removal.\n" ANSI_COLOR_RESET);
        return 1;
    }

    // Handle if both id and date are provided
    if (strlen(id) > 0 && strlen(date) > 0) {
        fprintf(stderr, ANSI_COLOR_RED "Error: Provide only one of --id or --date, not both.\n" ANSI_COLOR_RESET);
        return 1;
    }

    // Handle exercise removal
    if (strcmp(type, "exercise") == 0) {
        // Print the exercise details for confirmation
        print_exercise_details_from_id(id);
        // Ask for confirmation
        char confirmation[10];
        printf(ANSI_COLOR_RED "Are you sure you want to remove this exercise? (y/N): " ANSI_COLOR_RESET);
        fgets(confirmation, sizeof(confirmation), stdin);


        // Check the response
        if (confirmation[0] == 'y' || confirmation[0] == 'Y') {
            printf(ANSI_COLOR_GREEN "Exercise successfully removed.\n" ANSI_COLOR_RESET);
        } else {
            printf(ANSI_COLOR_YELLOW "Exercise removal cancelled.\n" ANSI_COLOR_RESET);
            return 0;
        }
    }
    
    return 0;
}