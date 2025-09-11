#include <stdio.h>
#include "fitlog.h"

bool exercise_exists(const char *name)
{
    char full_path[256];
    sprintf(full_path, "%s/%s", FITLOG_DIR, EXERCISES_FILE);
    FILE *fp = fopen(full_path, "r");
    if (fp == NULL)
    {
        perror("Error opening exercises file");
        return false;
    }

    char line[256];
    // Skip header line
    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp))
    {
        char exercise_name[100];
        sscanf(line, "%*[^,],%99[^,]", exercise_name);
        if (strcmp(exercise_name, name) == 0)
        {
            fclose(fp);
            return true;
        }
    }

    fclose(fp);
    return false;
}

bool shortcut_exists(const char *shortcut)
{
    char full_path[256];
    sprintf(full_path, "%s/%s", FITLOG_DIR, EXERCISES_FILE);
    FILE *fp = fopen(full_path, "r");
    if (fp == NULL)
    {
        perror("Error opening exercises file");
        return false;
    }

    char line[256];
    // Skip header line
    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp))
    {
        char exercise_shortcut[100];
        sscanf(line, "%*[^,],%*[^,],%99[^,]", exercise_shortcut);
        if (strcmp(exercise_shortcut, shortcut) == 0 && strcmp(exercise_shortcut, "(null)") != 0)
        {
            fclose(fp);
            return true;
        }
    }

    fclose(fp);
    return false;
}

ExerciseType check_exercise_type(const char *name)
{
    char full_path[256];
    sprintf(full_path, "%s/%s", FITLOG_DIR, EXERCISES_FILE);
    FILE *fp = fopen(full_path, "r");
    if (fp == NULL)
    {
        perror("Error opening exercises file");
        return -1; // Indicate error
    }
    char line[256];
    // Skip header line
    fgets(line, sizeof(line), fp);
    while (fgets(line, sizeof(line), fp))
    {
        char exercise_name[100];
        char type_str[20];
        sscanf(line, "%*[^,],%99[^,],%*[^,],%*[^,],%19s", exercise_name, type_str);
        if (strcmp(exercise_name, name) == 0)
        {
            fclose(fp);
            if (strcmp(type_str, "sets") == 0)
            {
                return TYPE_SETS;
            }
            else if (strcmp(type_str, "time") == 0)
            {
                return TYPE_TIME;
            }
            else if (strcmp(type_str, "body") == 0)
            {
                return TYPE_BODY;
            }
        }
    }
    fclose(fp);
    return -1; // Exercise not found
}

char *get_exercise_name_from_shortcut(const char *shortcut)
{
    char full_path[256];
    sprintf(full_path, "%s/%s", FITLOG_DIR, EXERCISES_FILE);
    FILE *fp = fopen(full_path, "r");
    if (fp == NULL)
    {
        perror(ANSI_COLOR_RED "Error opening exercises file" ANSI_COLOR_RESET);
        return NULL;
    }

    static char exercise_name[100];
    char line[256];
    // Skip header line
    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp))
    {
        char current_shortcut[100];
        sscanf(line, "%*[^,],%99[^,],%99[^,]", exercise_name, current_shortcut);

        if (strcmp(current_shortcut, shortcut) == 0 && strcmp(current_shortcut, "(null)") != 0)
        {
            fclose(fp);
            return exercise_name;
        }
    }

    fclose(fp);
    return NULL; // Shortcut not found
}

bool print_exercise_details_from_id(const char *id)
{
    char full_path[256];
    sprintf(full_path, "%s/%s", FITLOG_DIR, EXERCISES_FILE);
    FILE *fp = fopen(full_path, "r");
    if (fp == NULL)
    {
        perror(ANSI_COLOR_RED "Error opening exercises file" ANSI_COLOR_RESET);
        return false;
    }

    char line[256];
    // Skip header line
    fgets(line, sizeof(line), fp);
    bool found = false;

    while (fgets(line, sizeof(line), fp))
    {
        char exercise_id[20], exercise_name[100], shortcut[100], description[200], type[20];
        sscanf(line, "%19[^,],%99[^,],%99[^,],%199[^,],%19s",
               exercise_id, exercise_name, shortcut, description, type);

        if (strcmp(exercise_id, id) == 0)
        {
            printf("+--------+----------------------+----------+----------------------+--------+\n");
            printf("| %-6s | %-20s | %-8s | %-20s | %-6s |\n",
                   "ID", "Name", "Shortcut", "Description", "Type");
            printf("+--------+----------------------+----------+----------------------+--------+\n");

            // Print with alternating gray colors for values
            printf("| " ANSI_COLOR_RESET "%-6s" ANSI_COLOR_RESET " | " DARK_GRAY_TEXT "%-20s" ANSI_COLOR_RESET " | " ANSI_COLOR_RESET "%-8s" ANSI_COLOR_RESET " | " DARK_GRAY_TEXT "%-20s" ANSI_COLOR_RESET " | " ANSI_COLOR_RESET "%-6s" ANSI_COLOR_RESET " |\n",
                   exercise_id,
                   exercise_name,
                   (strcmp(shortcut, "(null)") == 0) ? "none" : shortcut,
                   (strcmp(description, "(null)") == 0) ? "none" : description,
                   type);

            printf("+--------+----------------------+----------+----------------------+--------+\n");
            found = true;
            break;
        }
    }

    if (!found)
    {
        printf(ANSI_COLOR_RED "No exercise found with ID: %s\n" ANSI_COLOR_RESET, id);

        return false;
    }

    fclose(fp);

    return found;
}

void remove_exercise_by_id(const char *id)
{
    char full_path[256];
    sprintf(full_path, "%s/%s", FITLOG_DIR, EXERCISES_FILE);

    // Check if exercises file exists
    FILE *fp = fopen(full_path, "r");
    if (fp == NULL)
    {
        printf(ANSI_COLOR_RED "Error: Exercises file not found. Please run 'fitlog init' first.\n" ANSI_COLOR_RESET);
        return;
    }

    // Create temporary file in the same directory as the original
    char temp_path[256];
    sprintf(temp_path, "%s/temp_exercises.csv", FITLOG_DIR);
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

        char exercise_id[20];
        strcpy(exercise_id, "");
        strcpy(exercise_name, "");

        // Parse using strtok for better CSV handling
        char *token = strtok(line_copy, ",");
        int field = 0;

        while (token != NULL && field < 5)
        {
            switch (field)
            {
            case 0:
                strncpy(exercise_id, token, sizeof(exercise_id) - 1);
                break;
            case 1:
                strncpy(exercise_name, token, sizeof(exercise_name) - 1);
                break;
            }
            token = strtok(NULL, ",");
            field++;
        }

        if (strcmp(exercise_id, id) == 0)
        {
            found = true;
            continue;
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
            printf(ANSI_COLOR_RED "Error: Could not remove original exercises file.\n" ANSI_COLOR_RESET);
            remove(temp_path); // Clean up temp file
            return;
        }

        if (rename(temp_path, full_path) != 0)
        {
            printf(ANSI_COLOR_RED "Error: Could not update exercises file.\n" ANSI_COLOR_RESET);
            return;
        }

        // Update shortcuts file
        char shortcuts_path[256];
        sprintf(shortcuts_path, "%s/%s", FITLOG_DIR, SHORTCUTS_FILE);
        FILE *sfp = fopen(shortcuts_path, "r");
        if (sfp != NULL)
        {
            char temp_shortcuts_path[256];
            sprintf(temp_shortcuts_path, "%s/temp_shortcuts.ini", FITLOG_DIR);
            FILE *temp_sfp = fopen(temp_shortcuts_path, "w");
            if (temp_sfp != NULL)
            {
                char sline[256];
                while (fgets(sline, sizeof(sline), sfp))
                {
                    char sline_copy[256];
                    strcpy(sline_copy, sline);
                    sline_copy[strcspn(sline_copy, "\n")] = 0;

                    // Skip lines that have the exercise name as the value (shortcut=exercise_name format)
                    char *equals_pos = strchr(sline_copy, '=');
                    if (equals_pos != NULL)
                    {
                        // Get the value part (after the equals sign)
                        char *value = equals_pos + 1;

                        // Trim whitespace from value
                        while (*value == ' ' || *value == '\t')
                            value++;
                        char *value_end = value + strlen(value) - 1;
                        while (value_end > value && (*value_end == ' ' || *value_end == '\t'))
                        {
                            *value_end = '\0';
                            value_end--;
                        }

                        // If the value matches the exercise name, skip this line
                        if (strcmp(value, exercise_name) != 0)
                        {
                            fputs(sline, temp_sfp);
                        }
                    }
                    else
                    {
                        // Keep non-key=value lines (comments, empty lines, etc.)
                        fputs(sline, temp_sfp);
                    }
                }
                fclose(temp_sfp);
                fclose(sfp);

                // Replace original shortcuts file
                if (remove(shortcuts_path) != 0)
                {
                    printf(ANSI_COLOR_RED "Error: Could not remove original shortcuts file.\n" ANSI_COLOR_RESET);
                    remove(temp_shortcuts_path); // Clean up temp file
                    return;
                }

                if (rename(temp_shortcuts_path, shortcuts_path) != 0)
                {
                    printf(ANSI_COLOR_RED "Error: Could not update shortcuts file.\n" ANSI_COLOR_RESET);
                    return;
                }
            }
            else
            {
                fclose(sfp);
                printf(ANSI_COLOR_RED "Error: Could not create temporary shortcuts file.\n" ANSI_COLOR_RESET);
            }
        }
    }
    else
    {
        // No exercise found, delete temp file and show error
        remove(temp_path);
        printf(ANSI_COLOR_RED "Error: No exercise found with ID '%s'.\n" ANSI_COLOR_RESET, id);
    }
}

int show_exercise_by_id(const char *id)
{
    if (print_exercise_details_from_id(id))
    {
        return 0; // Success
    }
    else
    {
        return 1; // Failure
    }
}

int show_last_n_exercises(int n)
{
    if (n <= 0)
    {
        printf(ANSI_COLOR_RED "Error: Please provide a positive integer for --last.\n" ANSI_COLOR_RESET);
        return 1;
    }

    char full_path[256];
    sprintf(full_path, "%s/%s", FITLOG_DIR, EXERCISES_FILE);
    FILE *fp = fopen(full_path, "r");
    if (fp == NULL)
    {
        printf(ANSI_COLOR_RED "Error: Exercises file not found. Please run 'fitlog init' first.\n" ANSI_COLOR_RESET);
        return 1;
    }

    // First, count the total number of exercises
    int total_exercises = 0;
    char line[256];
    // Skip header line
    fgets(line, sizeof(line), fp);
    while (fgets(line, sizeof(line), fp))
    {
        total_exercises++;
    }

    if (total_exercises == 0)
    {
        printf(ANSI_COLOR_YELLOW "No exercises found to display.\n" ANSI_COLOR_RESET);
        fclose(fp);
        return 0;
    }

    // Calculate how many exercises to skip
    int to_skip = total_exercises - n;
    if (to_skip < 0)
    {
        to_skip = 0; // If n is greater than total, show all
    }

    // Reset file pointer to beginning and skip the required lines
    fseek(fp, 0, SEEK_SET);
    fgets(line, sizeof(line), fp); // Skip header

    for (int i = 0; i < to_skip; i++)
    {
        fgets(line, sizeof(line), fp);
    }

    // Print header for display
    printf("+--------+----------------------+----------+----------------------+--------+\n");
    printf("| %-6s | %-20s | %-8s | %-20s | %-6s |\n",
           "ID", "Name", "Shortcut", "Description", "Type");
    printf("+--------+----------------------+----------+----------------------+--------+\n");

    // Now print the remaining exercises
    while (fgets(line, sizeof(line), fp))
    {
        char exercise_id[20], exercise_name[100], shortcut[100], description[200], type[20];
        sscanf(line, "%19[^,],%99[^,],%99[^,],%199[^,],%19s",
               exercise_id, exercise_name, shortcut, description, type);
        // Print with alternating gray colors for values
        printf("| " ANSI_COLOR_RESET "%-6s" ANSI_COLOR_RESET " | " DARK_GRAY_TEXT "%-20s" ANSI_COLOR_RESET " | " ANSI_COLOR_RESET "%-8s" ANSI_COLOR_RESET " | " DARK_GRAY_TEXT "%-20s" ANSI_COLOR_RESET " | " ANSI_COLOR_RESET "%-6s" ANSI_COLOR_RESET " |\n",
               exercise_id,
               exercise_name,
               (strcmp(shortcut, "(null)") == 0) ? "none" : shortcut,
               (strcmp(description, "(null)") == 0) ? "none" : description,
               type);
        printf("+--------+----------------------+----------+----------------------+--------+\n");
    }
    fclose(fp);
    return 0;
}

int show_all_exercises()
{
    char full_path[256];
    sprintf(full_path, "%s/%s", FITLOG_DIR, EXERCISES_FILE);
    FILE *fp = fopen(full_path, "r");
    if (fp == NULL)
    {
        printf(ANSI_COLOR_RED "Error: Exercises file not found. Please run 'fitlog init' first.\n" ANSI_COLOR_RESET);
        return 1;
    }

    char line[256];
    // Skip header line
    fgets(line, sizeof(line), fp);

    // Print header for display
    printf("+--------+----------------------+----------+----------------------+--------+\n");
    printf("| %-6s | %-20s | %-8s | %-20s | %-6s |\n",
           "ID", "Name", "Shortcut", "Description", "Type");
    printf("+--------+----------------------+----------+----------------------+--------+\n");

    bool found = false;
    while (fgets(line, sizeof(line), fp))
    {
        found = true;
        char exercise_id[20], exercise_name[100], shortcut[100], description[200], type[20];
        sscanf(line, "%19[^,],%99[^,],%99[^,],%199[^,],%19s",
               exercise_id, exercise_name, shortcut, description, type);
        // Print with alternating gray colors for values
        printf("| " ANSI_COLOR_RESET "%-6s" ANSI_COLOR_RESET " | " DARK_GRAY_TEXT "%-20s" ANSI_COLOR_RESET " | " ANSI_COLOR_RESET "%-8s" ANSI_COLOR_RESET " | " DARK_GRAY_TEXT "%-20s" ANSI_COLOR_RESET " | " ANSI_COLOR_RESET "%-6s" ANSI_COLOR_RESET " |\n",
               exercise_id,
               exercise_name,
               (strcmp(shortcut, "(null)") == 0) ? "none" : shortcut,
               (strcmp(description, "(null)") == 0) ? "none" : description,
               type);
        printf("+--------+----------------------+----------+----------------------+--------+\n");
    }

    if (!found)
    {
        printf(ANSI_COLOR_YELLOW "No exercises found to display.\n" ANSI_COLOR_RESET);
    }

    fclose(fp);
    return 0;
}