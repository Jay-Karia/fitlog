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
            if (strcmp(type_str, "weight") == 0)
            {
                return TYPE_WEIGHT;
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

    // Define minimum column widths
    int id_width = 6;
    int name_width = 10;
    int shortcut_width = 8;
    int description_width = 15;
    int type_width = 6;

    // Column headers
    const char *headers[] = {"ID", "Name", "Shortcut", "Description", "Type"};

    // Update with header lengths
    id_width = id_width < (int) strlen(headers[0]) ? (int) strlen(headers[0]) : id_width;
    name_width = name_width < (int) strlen(headers[1]) ? (int) strlen(headers[1]) : name_width;
    shortcut_width = shortcut_width < (int) strlen(headers[2]) ? (int) strlen(headers[2]) : shortcut_width;
    description_width = description_width < (int) strlen(headers[3]) ? (int) strlen(headers[3]) : description_width;
    type_width = type_width < (int) strlen(headers[4]) ? (int) strlen(headers[4]) : type_width;

    // Variables to store the found exercise data
    char exercise_id[20] = "", exercise_name[100] = "", shortcut[100] = "", description[200] = "", type[20] = "";

    while (fgets(line, sizeof(line), fp))
    {
        sscanf(line, "%19[^,],%99[^,],%99[^,],%199[^,],%19s",
               exercise_id, exercise_name, shortcut, description, type);

        if (strcmp(exercise_id, id) == 0)
        {
            found = true;
            break;
        }
    }

    fclose(fp);

    if (!found)
    {
        printf(ANSI_COLOR_RED "No exercise found with ID: %s\n" ANSI_COLOR_RESET, id);
        return false;
    }

    // Calculate widths based on content
    id_width = id_width < (int) strlen(exercise_id) ? (int) strlen(exercise_id) : id_width;
    name_width = name_width < (int) strlen(exercise_name) ? (int) strlen(exercise_name) : name_width;
    
    // Handle shortcut display
    char display_shortcut[100];
    if (strcmp(shortcut, "(null)") == 0) {
        strcpy(display_shortcut, "none");
    } else {
        strcpy(display_shortcut, shortcut);
    }
    shortcut_width = shortcut_width < (int) strlen(display_shortcut) ? (int) strlen(display_shortcut) : shortcut_width;
    
    // Handle description display
    char display_desc[200];
    if (strcmp(description, "(null)") == 0) {
        strcpy(display_desc, "none");
    } else {
        strcpy(display_desc, description);
    }
    description_width = description_width < (int) strlen(display_desc) ? (int) strlen(display_desc) : description_width;
    
    type_width = type_width < (int) strlen(type) ? (int) strlen(type) : type_width;

    // Add padding for better readability
    id_width += 2;
    name_width += 2;
    shortcut_width += 2;
    description_width += 2;
    type_width += 2;

    // Limit maximum width for readability
    if (name_width > 30) name_width = 30;
    if (description_width > 40) description_width = 40;

    // Truncate if needed
    char truncated_name[31] = {0};
    strncpy(truncated_name, exercise_name, 30);
    if ((int) strlen(exercise_name) > 30) {
        truncated_name[27] = '.';
        truncated_name[28] = '.';
        truncated_name[29] = '.';
    }
    
    char truncated_desc[41] = {0};
    strncpy(truncated_desc, display_desc, 40);
    if ((int) strlen(display_desc) > 40) {
        truncated_desc[37] = '.';
        truncated_desc[38] = '.';
        truncated_desc[39] = '.';
    }

    // Print table with dynamic widths
    // Top border
    printf("+");
    for (int i = 0; i < id_width + 2; i++) printf("-");
    printf("+");
    for (int i = 0; i < name_width + 2; i++) printf("-");
    printf("+");
    for (int i = 0; i < shortcut_width + 2; i++) printf("-");
    printf("+");
    for (int i = 0; i < description_width + 2; i++) printf("-");
    printf("+");
    for (int i = 0; i < type_width + 2; i++) printf("-");
    printf("+\n");

    // Header
    printf("| %-*s | %-*s | %-*s | %-*s | %-*s |\n",
           id_width, headers[0],
           name_width, headers[1],
           shortcut_width, headers[2],
           description_width, headers[3],
           type_width, headers[4]);

    // Header separator
    printf("+");
    for (int i = 0; i < id_width + 2; i++) printf("-");
    printf("+");
    for (int i = 0; i < name_width + 2; i++) printf("-");
    printf("+");
    for (int i = 0; i < shortcut_width + 2; i++) printf("-");
    printf("+");
    for (int i = 0; i < description_width + 2; i++) printf("-");
    printf("+");
    for (int i = 0; i < type_width + 2; i++) printf("-");
    printf("+\n");

    // Data row with color alternation
    printf("| " ANSI_COLOR_RESET "%-*s" ANSI_COLOR_RESET " | " DARK_GRAY_TEXT "%-*s" ANSI_COLOR_RESET " | " ANSI_COLOR_RESET "%-*s" ANSI_COLOR_RESET " | " DARK_GRAY_TEXT "%-*s" ANSI_COLOR_RESET " | " ANSI_COLOR_RESET "%-*s" ANSI_COLOR_RESET " |\n",
           id_width, exercise_id,
           name_width, truncated_name,
           shortcut_width, display_shortcut,
           description_width, truncated_desc,
           type_width, type);

    // Bottom border
    printf("+");
    for (int i = 0; i < id_width + 2; i++) printf("-");
    printf("+");
    for (int i = 0; i < name_width + 2; i++) printf("-");
    printf("+");
    for (int i = 0; i < shortcut_width + 2; i++) printf("-");
    printf("+");
    for (int i = 0; i < description_width + 2; i++) printf("-");
    printf("+");
    for (int i = 0; i < type_width + 2; i++) printf("-");
    printf("+\n");

    return true;
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

    // Store the exercises to display for dynamic width calculation
    struct ExerciseData {
        char id[20];
        char name[100];
        char shortcut[100];
        char description[200];
        char type[20];
    };

    struct ExerciseData exercises[100] = {0};
    int count = 0;
    
    // Define minimum column widths
    int id_width = 6;
    int name_width = 10;
    int shortcut_width = 8;
    int description_width = 15;
    int type_width = 6;

    // Column headers
    const char *headers[] = {"ID", "Name", "Shortcut", "Description", "Type"};

    // Update with header lengths
    id_width = id_width < (int) strlen(headers[0]) ? (int) strlen(headers[0]) : id_width;
    name_width = name_width < (int) strlen(headers[1]) ? (int) strlen(headers[1]) : name_width;
    shortcut_width = shortcut_width < (int) strlen(headers[2]) ? (int) strlen(headers[2]) : shortcut_width;
    description_width = description_width < (int) strlen(headers[3]) ? (int) strlen(headers[3]) : description_width;
    type_width = type_width < (int) strlen(headers[4]) ? (int) strlen(headers[4]) : type_width;

    // Read and store the remaining exercises
    while (fgets(line, sizeof(line), fp) && count < n)
    {
        sscanf(line, "%19[^,],%99[^,],%99[^,],%199[^,],%19s",
               exercises[count].id, exercises[count].name, exercises[count].shortcut, 
               exercises[count].description, exercises[count].type);
               
        // Update max column widths
        id_width = id_width < (int) strlen(exercises[count].id) ? (int) strlen(exercises[count].id) : id_width;
        name_width = name_width < (int) strlen(exercises[count].name) ? (int) strlen(exercises[count].name) : name_width;
        
        // Handle shortcut display
        if (strcmp(exercises[count].shortcut, "(null)") == 0) {
            strcpy(exercises[count].shortcut, "none");
        }
        shortcut_width = shortcut_width < (int) strlen(exercises[count].shortcut) ? 
                         (int) strlen(exercises[count].shortcut) : shortcut_width;
        
        // Handle description display
        if (strcmp(exercises[count].description, "(null)") == 0) {
            strcpy(exercises[count].description, "none");
        }
        description_width = description_width < (int) strlen(exercises[count].description) ? 
                            (int) strlen(exercises[count].description) : description_width;
        
        type_width = type_width < (int) strlen(exercises[count].type) ? 
                     (int) strlen(exercises[count].type) : type_width;
        
        count++;
    }
    fclose(fp);

    // Add padding for better readability
    id_width += 2;
    name_width += 2;
    shortcut_width += 2;
    description_width += 2;
    type_width += 2;

    // Limit maximum width for readability
    if (name_width > 30) name_width = 30;
    if (description_width > 40) description_width = 40;

    // Print table with dynamic widths
    // Top border
    printf("+");
    for (int i = 0; i < id_width + 2; i++) printf("-");
    printf("+");
    for (int i = 0; i < name_width + 2; i++) printf("-");
    printf("+");
    for (int i = 0; i < shortcut_width + 2; i++) printf("-");
    printf("+");
    for (int i = 0; i < description_width + 2; i++) printf("-");
    printf("+");
    for (int i = 0; i < type_width + 2; i++) printf("-");
    printf("+\n");

    // Header
    printf("| %-*s | %-*s | %-*s | %-*s | %-*s |\n",
           id_width, headers[0],
           name_width, headers[1],
           shortcut_width, headers[2],
           description_width, headers[3],
           type_width, headers[4]);

    // Header separator
    printf("+");
    for (int i = 0; i < id_width + 2; i++) printf("-");
    printf("+");
    for (int i = 0; i < name_width + 2; i++) printf("-");
    printf("+");
    for (int i = 0; i < shortcut_width + 2; i++) printf("-");
    printf("+");
    for (int i = 0; i < description_width + 2; i++) printf("-");
    printf("+");
    for (int i = 0; i < type_width + 2; i++) printf("-");
    printf("+\n");

    // Print each exercise
    for (int i = 0; i < count; i++)
    {
        // Truncate if needed
        char truncated_name[31] = {0};
        strncpy(truncated_name, exercises[i].name, 30);
        if ((int) strlen(exercises[i].name) > 30) {
            truncated_name[27] = '.';
            truncated_name[28] = '.';
            truncated_name[29] = '.';
        }
        
        char truncated_desc[41] = {0};
        strncpy(truncated_desc, exercises[i].description, 40);
        if ((int) strlen(exercises[i].description) > 40) {
            truncated_desc[37] = '.';
            truncated_desc[38] = '.';
            truncated_desc[39] = '.';
        }

        // Data row with color alternation
        printf("| " ANSI_COLOR_RESET "%-*s" ANSI_COLOR_RESET " | " DARK_GRAY_TEXT "%-*s" ANSI_COLOR_RESET " | " ANSI_COLOR_RESET "%-*s" ANSI_COLOR_RESET " | " DARK_GRAY_TEXT "%-*s" ANSI_COLOR_RESET " | " ANSI_COLOR_RESET "%-*s" ANSI_COLOR_RESET " |\n",
               id_width, exercises[i].id,
               name_width, truncated_name,
               shortcut_width, exercises[i].shortcut,
               description_width, truncated_desc,
               type_width, exercises[i].type);

        // Row separator
        printf("+");
        for (int j = 0; j < id_width + 2; j++) printf("-");
        printf("+");
        for (int j = 0; j < name_width + 2; j++) printf("-");
        printf("+");
        for (int j = 0; j < shortcut_width + 2; j++) printf("-");
        printf("+");
        for (int j = 0; j < description_width + 2; j++) printf("-");
        printf("+");
        for (int j = 0; j < type_width + 2; j++) printf("-");
        printf("+\n");
    }

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

    // First pass: collect all exercises and calculate max column widths
    struct ExerciseData {
        char id[20];
        char name[100];
        char shortcut[100];
        char description[200];
        char type[20];
    };

    struct ExerciseData exercises[100] = {0};
    int count = 0;
    
    // Define minimum column widths
    int id_width = 6;
    int name_width = 10;
    int shortcut_width = 8;
    int description_width = 15;
    int type_width = 6;

    // Column headers
    const char *headers[] = {"ID", "Name", "Shortcut", "Description", "Type"};

    // Update with header lengths
    id_width = id_width < (int) strlen(headers[0]) ? (int) strlen(headers[0]) : id_width;
    name_width = name_width < (int) strlen(headers[1]) ? (int) strlen(headers[1]) : name_width;
    shortcut_width = shortcut_width < (int) strlen(headers[2]) ? (int) strlen(headers[2]) : shortcut_width;
    description_width = description_width < (int) strlen(headers[3]) ? (int) strlen(headers[3]) : description_width;
    type_width = type_width < (int) strlen(headers[4]) ? (int) strlen(headers[4]) : type_width;

    // Read and store all exercises
    bool found = false;
    while (fgets(line, sizeof(line), fp) && count < 100)
    {
        found = true;
        sscanf(line, "%19[^,],%99[^,],%99[^,],%199[^,],%19s",
               exercises[count].id, exercises[count].name, exercises[count].shortcut, 
               exercises[count].description, exercises[count].type);
               
        // Update max column widths
        id_width = id_width < (int) strlen(exercises[count].id) ? (int) strlen(exercises[count].id) : id_width;
        name_width = name_width < (int) strlen(exercises[count].name) ? (int) strlen(exercises[count].name) : name_width;
        
        // Handle shortcut display
        if (strcmp(exercises[count].shortcut, "(null)") == 0) {
            strcpy(exercises[count].shortcut, "none");
        }
        shortcut_width = shortcut_width < (int) strlen(exercises[count].shortcut) ? 
                         (int) strlen(exercises[count].shortcut) : shortcut_width;
        
        // Handle description display
        if (strcmp(exercises[count].description, "(null)") == 0) {
            strcpy(exercises[count].description, "none");
        }
        description_width = description_width < (int) strlen(exercises[count].description) ? 
                            (int) strlen(exercises[count].description) : description_width;
        
        type_width = type_width < (int) strlen(exercises[count].type) ? 
                     (int) strlen(exercises[count].type) : type_width;
        
        count++;
    }
    fclose(fp);

    if (!found)
    {
        printf(ANSI_COLOR_YELLOW "No exercises found to display.\n" ANSI_COLOR_RESET);
        return 0;
    }

    // Add padding for better readability
    id_width += 2;
    name_width += 2;
    shortcut_width += 2;
    description_width += 2;
    type_width += 2;

    // Limit maximum width for readability
    if (name_width > 30) name_width = 30;
    if (description_width > 40) description_width = 40;

    // Print table with dynamic widths
    // Top border
    printf("+");
    for (int i = 0; i < id_width + 2; i++) printf("-");
    printf("+");
    for (int i = 0; i < name_width + 2; i++) printf("-");
    printf("+");
    for (int i = 0; i < shortcut_width + 2; i++) printf("-");
    printf("+");
    for (int i = 0; i < description_width + 2; i++) printf("-");
    printf("+");
    for (int i = 0; i < type_width + 2; i++) printf("-");
    printf("+\n");

    // Header
    printf("| %-*s | %-*s | %-*s | %-*s | %-*s |\n",
           id_width, headers[0],
           name_width, headers[1],
           shortcut_width, headers[2],
           description_width, headers[3],
           type_width, headers[4]);

    // Header separator
    printf("+");
    for (int i = 0; i < id_width + 2; i++) printf("-");
    printf("+");
    for (int i = 0; i < name_width + 2; i++) printf("-");
    printf("+");
    for (int i = 0; i < shortcut_width + 2; i++) printf("-");
    printf("+");
    for (int i = 0; i < description_width + 2; i++) printf("-");
    printf("+");
    for (int i = 0; i < type_width + 2; i++) printf("-");
    printf("+\n");

    // Print each exercise
    for (int i = 0; i < count; i++)
    {
        // Truncate if needed
        char truncated_name[31] = {0};
        strncpy(truncated_name, exercises[i].name, 30);
        if ((int) strlen(exercises[i].name) > 30) {
            truncated_name[27] = '.';
            truncated_name[28] = '.';
            truncated_name[29] = '.';
        }
        
        char truncated_desc[41] = {0};
        strncpy(truncated_desc, exercises[i].description, 40);
        if ((int) strlen(exercises[i].description) > 40) {
            truncated_desc[37] = '.';
            truncated_desc[38] = '.';
            truncated_desc[39] = '.';
        }

        // Data row with color alternation
        printf("| " ANSI_COLOR_RESET "%-*s" ANSI_COLOR_RESET " | " DARK_GRAY_TEXT "%-*s" ANSI_COLOR_RESET " | " ANSI_COLOR_RESET "%-*s" ANSI_COLOR_RESET " | " DARK_GRAY_TEXT "%-*s" ANSI_COLOR_RESET " | " ANSI_COLOR_RESET "%-*s" ANSI_COLOR_RESET " |\n",
               id_width, exercises[i].id,
               name_width, truncated_name,
               shortcut_width, exercises[i].shortcut,
               description_width, truncated_desc,
               type_width, exercises[i].type);

        // Row separator
        printf("+");
        for (int j = 0; j < id_width + 2; j++) printf("-");
        printf("+");
        for (int j = 0; j < name_width + 2; j++) printf("-");
        printf("+");
        for (int j = 0; j < shortcut_width + 2; j++) printf("-");
        printf("+");
        for (int j = 0; j < description_width + 2; j++) printf("-");
        printf("+");
        for (int j = 0; j < type_width + 2; j++) printf("-");
        printf("+\n");
    }

    return 0;
}