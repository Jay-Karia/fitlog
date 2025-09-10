#include <stdio.h>
#include "fitlog.h"

// TODO: fix table dynamic border spacing

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

void safe_strncpy(char *dest, const char *src, size_t dest_size)
{
    if (dest_size == 0)
        return;
    strncpy(dest, src, dest_size - 1);
    dest[dest_size - 1] = '\0';
}

// Another helper for parsing a single field
const char *parse_csv_field(char *dest, size_t dest_size, const char *line)
{
    const char *comma = strchr(line, ',');
    if (comma)
    {
        size_t len = comma - line;
        if (len >= dest_size)
        {
            len = dest_size - 1;
        }
        strncpy(dest, line, len);
        dest[len] = '\0';
        return comma + 1;
    }
    else
    { // Last field
        safe_strncpy(dest, line, dest_size);
        return NULL;
    }
}

// This is the updated version of your function
bool print_workout_details_from_date(const char *input_date)
{
    char full_path[256];
    sprintf(full_path, "%s/%s", FITLOG_DIR, WORKOUTS_FILE);
    FILE *fp = fopen(full_path, "r");
    if (fp == NULL)
    {
        printf(ANSI_COLOR_RED "Error: Workouts file not found. Please log some workouts first.\n" ANSI_COLOR_RESET);
        return false;
    }

    char line[1024]; // Increased buffer size for safety
    // Skip header line
    fgets(line, sizeof(line), fp);

    bool found = false;
    int count = 0;

    // --- Loop through each workout entry in the file ---
    while (fgets(line, sizeof(line), fp))
    {
        // Your variables for storing the parsed data
        char workout_id[20] = "", exercise_name[100] = "";
        char sets[20] = "", reps[20] = "", weight[50] = "", date[20] = "", notes[200] = "", time[20] = "";

        // A more robust way to remove trailing newline characters (\n or \r\n)
        line[strcspn(line, "\r\n")] = 0;

        // --- Start of NEW, safer parsing logic ---
        const char *p = line;
        p = parse_csv_field(workout_id, sizeof(workout_id), p);
        if (p)
            p = parse_csv_field(exercise_name, sizeof(exercise_name), p);
        if (p)
            p = parse_csv_field(sets, sizeof(sets), p);
        if (p)
            p = parse_csv_field(reps, sizeof(reps), p);
        if (p)
            p = parse_csv_field(weight, sizeof(weight), p);
        if (p)
            p = parse_csv_field(time, sizeof(time), p);
        if (p)
            p = parse_csv_field(date, sizeof(date), p);
        if (p)
            parse_csv_field(notes, sizeof(notes), p);
        // --- End of parsing logic ---

        // Your existing logic for checking the date and printing remains the same
        if (strcmp(input_date, date) == 0)
        {
            // Print header for one time only
            if (count == 0)
            {
                printf("+--------+----------------------+--------+-------+-----------------+------------+------------+----------------------+\n");
                printf("| %-6s | %-20s | %-6s | %-5s | %-15s | %-10s | %-10s | %-20s |\n",
                       "ID", "Exercise", "Sets", "Reps", "Weight", "Time", "Date", "Notes");
                printf("+--------+----------------------+--------+-------+-----------------+------------+------------+----------------------+\n");
            }

            // Print with alternating gray colors for values
            printf("| " ANSI_COLOR_RESET "%-6s" ANSI_COLOR_RESET " | " DARK_GRAY_TEXT "%-20s" ANSI_COLOR_RESET " | " ANSI_COLOR_RESET "%-6s" ANSI_COLOR_RESET " | " DARK_GRAY_TEXT "%-5s" ANSI_COLOR_RESET " | " ANSI_COLOR_RESET "%-15s" ANSI_COLOR_RESET " | " DARK_GRAY_TEXT "%-10s" ANSI_COLOR_RESET " | " ANSI_COLOR_RESET "%-10s" ANSI_COLOR_RESET " | " DARK_GRAY_TEXT "%-20s" ANSI_COLOR_RESET " |\n",
                   workout_id,
                   exercise_name,
                   sets,
                   reps,
                   weight,
                   time,
                   date,
                   notes);
            found = true;
            count++;
        }
    }

    if (found)
    {
        printf("+--------+----------------------+--------+-------+-----------------+------------+------------+----------------------+\n");
        printf(ANSI_COLOR_GREEN "Total logs found for date %s: %d\n" ANSI_COLOR_RESET, input_date, count);
    }
    else
    {
        printf(ANSI_COLOR_RED "No workout logs found for date: %s\n" ANSI_COLOR_RESET, input_date);
    }

    fclose(fp);
    return found;
}

void remove_workouts_by_date(const char *date)
{
    printf(date);
}