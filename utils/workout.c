#include <stdio.h>
#include "fitlog.h"

bool print_workout_details_from_id(const char *id)
{
    char full_path[256];
    sprintf(full_path, "%s/%s", FITLOG_DIR, WORKOUTS_FILE);
    FILE *fp = fopen(full_path, "r");
    if (fp == NULL)
    {
        printf(ANSI_COLOR_RED "Error: Workouts file not found. Please log some workouts first.\n" ANSI_COLOR_RESET);
        return false;
    }

    char line[512];
    // Skip header line
    fgets(line, sizeof(line), fp);
    bool found = false;

    while (fgets(line, sizeof(line), fp))
    {
        // Remove newline character if present
        line[strcspn(line, "\n")] = 0;

        char workout_id[20], exercise_name[100], exercise_type[20], sets[20], reps[20], weight[50], date[20], notes[200];

        // Initialize all fields to empty strings
        strcpy(workout_id, "");
        strcpy(exercise_name, "");
        strcpy(exercise_type, "");
        strcpy(sets, "");
        strcpy(reps, "");
        strcpy(weight, "");
        strcpy(date, "");
        strcpy(notes, "");

        // Parse workout CSV: ID,Name,Type,Sets,Reps,Weight,Date,Notes
        // Use a more robust parsing approach
        char *token = strtok(line, ",");
        int field = 0;

        while (token != NULL && field < 8)
        {
            switch (field)
            {
            case 0:
                strncpy(workout_id, token, sizeof(workout_id) - 1);
                break;
            case 1:
                strncpy(exercise_name, token, sizeof(exercise_name) - 1);
                break;
            case 2:
                strncpy(exercise_type, token, sizeof(exercise_type) - 1);
                break;
            case 3:
                strncpy(sets, token, sizeof(sets) - 1);
                break;
            case 4:
                strncpy(reps, token, sizeof(reps) - 1);
                break;
            case 5:
                strncpy(weight, token, sizeof(weight) - 1);
                break;
            case 6:
                strncpy(date, token, sizeof(date) - 1);
                break;
            case 7:
                strncpy(notes, token, sizeof(notes) - 1);
                break;
            }
            token = strtok(NULL, ",");
            field++;
        }

        if (strcmp(workout_id, id) == 0)
        {
            printf("+--------+----------------------+--------+-------+-----------------+------------+------------+----------------------+\n");
            printf("| %-6s | %-20s | %-6s | %-5s | %-15s | %-10s | %-10s | %-20s |\n",
                   "ID", "Exercise", "Sets", "Reps", "Weight", "Time", "Date", "Notes");
            printf("+--------+----------------------+--------+-------+-----------------+------------+------------+----------------------+\n");

            // Print with alternating gray colors for values
            printf("| " ANSI_COLOR_RESET "%-6s" ANSI_COLOR_RESET " | " DARK_GRAY_TEXT "%-20s" ANSI_COLOR_RESET " | " ANSI_COLOR_RESET "%-6s" ANSI_COLOR_RESET " | " DARK_GRAY_TEXT "%-5s" ANSI_COLOR_RESET " | " ANSI_COLOR_RESET "%-15s" ANSI_COLOR_RESET " | " DARK_GRAY_TEXT "%-10s" ANSI_COLOR_RESET " | " ANSI_COLOR_RESET "%-10s" ANSI_COLOR_RESET " | " DARK_GRAY_TEXT "%-20s" ANSI_COLOR_RESET " |\n",
                   workout_id,
                   exercise_name,
                   exercise_type,
                   (strlen(sets) == 0) ? "none" : sets,
                   (strlen(reps) == 0) ? "none" : reps,
                   (strlen(weight) == 0) ? "none" : weight,
                   (strlen(date) == 0) ? "none" : date,
                   (strlen(notes) == 0) ? "none" : notes);

            printf("+--------+----------------------+--------+-------+-----------------+------------+------------+----------------------+\n");
            found = true;
            break;
        }
    }

    fclose(fp);

    if (!found)
    {
        printf(ANSI_COLOR_RED "No workout found with ID: %s\n" ANSI_COLOR_RESET, id);
        return false;
    }

    return true;
}

void remove_workout_by_id(const char *id)
{
    char full_path[256];
    sprintf(full_path, "%s/%s", FITLOG_DIR, WORKOUTS_FILE);

    // Check if workouts file exists
    FILE *fp = fopen(full_path, "r");
    if (fp == NULL)
    {
        printf(ANSI_COLOR_RED "Error: Workouts file not found. Please log some workouts first.\n" ANSI_COLOR_RESET);
        return;
    }

    // Create temporary file in the same directory as the original
    char temp_path[256];
    sprintf(temp_path, "%s/temp_workouts.csv", FITLOG_DIR);
    FILE *temp_fp = fopen(temp_path, "w");
    if (temp_fp == NULL)
    {
        printf(ANSI_COLOR_RED "Error: Could not create temporary file for removal operation.\n" ANSI_COLOR_RESET);
        fclose(fp);
        return;
    }

    char line[512];
    bool found = false;
    char exercise_name[100];
    char date[20];

    // Copy header line
    if (fgets(line, sizeof(line), fp))
    {
        fputs(line, temp_fp);
    }

    // Process each line
    while (fgets(line, sizeof(line), fp))
    {
        // Create a copy of the line for parsing (since strtok modifies the original)
        char line_copy[512];
        strcpy(line_copy, line);

        // Remove newline character if present
        line_copy[strcspn(line_copy, "\n")] = 0;

        char workout_id[20];
        strcpy(workout_id, "");
        strcpy(exercise_name, "");
        strcpy(date, "");

        // Parse to get workout ID, exercise name, and date for the success message
        char *token = strtok(line_copy, ",");
        int field = 0;

        while (token != NULL && field < 8)
        {
            switch (field)
            {
            case 0:
                strncpy(workout_id, token, sizeof(workout_id) - 1);
                break;
            case 1:
                strncpy(exercise_name, token, sizeof(exercise_name) - 1);
                break;
            case 6:
                strncpy(date, token, sizeof(date) - 1);
                break;
            }
            token = strtok(NULL, ",");
            field++;
        }

        if (strcmp(workout_id, id) == 0)
        {
            found = true;
            printf(ANSI_COLOR_GREEN "Workout log for '%s' on %s (ID: %s) has been removed successfully.\n" ANSI_COLOR_RESET,
                   exercise_name, date, id);
            continue; // Skip this line to remove it
        }
        fputs(line, temp_fp);
    }

    fclose(fp);
    fclose(temp_fp);

    if (found)
    {
        // Replace original file with temp file
        if (remove(full_path) != 0)
        {
            printf(ANSI_COLOR_RED "Error: Could not remove original workouts file.\n" ANSI_COLOR_RESET);
            remove(temp_path); // Clean up temp file
            return;
        }

        if (rename(temp_path, full_path) != 0)
        {
            printf(ANSI_COLOR_RED "Error: Could not update workouts file.\n" ANSI_COLOR_RESET);
            return;
        }
    }
    else
    {
        // No workout found, delete temp file and show error
        remove(temp_path);
        printf(ANSI_COLOR_RED "Error: No workout log found with ID '%s'.\n" ANSI_COLOR_RESET, id);
    }
}