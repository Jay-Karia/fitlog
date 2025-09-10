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
        // Check for shortcut match as well
        char exercise_shortcut[100];
        sscanf(line, "%*[^,],%*[^,],%99[^,],%*[^,],%19s", exercise_shortcut, type_str);
        if (strcmp(exercise_shortcut, name) == 0 && strcmp(exercise_shortcut, "(null)") != 0)
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
            else
            {
                return -1; // Unknown type
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
