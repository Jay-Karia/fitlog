#include <stdio.h>
#include <time.h>
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
            else
            {
                return -1; // Unknown type
            }
        }
    }
    fclose(fp);
    return -1; // Exercise not found
}

bool is_valid_date_format(const char *input_date, const enum DateFormat required_format)
{
    if (required_format == DATE_DD_MM_YYYY)
    {
        int day, month, year;
        if (sscanf(input_date, "%d/%d/%d", &day, &month, &year) != 3)
            return false;
        if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900 || year > 2100)
            return false;
        // Basic month-day validation (e.g., Feb has 28/29 days, etc.)
        int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
            days_in_month[1] = 29;
        if (day > days_in_month[month - 1])
            return false;
        return true;
    }
    else if (required_format == DATE_MM_DD_YYYY)
    {
        int month, day, year;
        if (sscanf(input_date, "%d/%d/%d", &month, &day, &year) != 3)
            return false;
        if (month < 1 || month > 12 || day < 1 || day > 31 || year < 1900 || year > 2100)
            return false;
        int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
            days_in_month[1] = 29;
        if (day > days_in_month[month - 1])
            return false;
        return true;
    }
    else if (required_format == DATE_YYYY_MM_DD)
    {
        int year, month, day;
        if (sscanf(input_date, "%d-%d-%d", &year, &month, &day) != 3)
            return false;
        if (year < 1900 || year > 2100 || month < 1 || month > 12 || day < 1 || day > 31)
            return false;
        int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
            days_in_month[1] = 29;
        if (day > days_in_month[month - 1])
            return false;
        return true;
    }
    return false;
}

char *get_today_date(const enum DateFormat required_format)
{
    static char date_str[11];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    if (required_format == DATE_DD_MM_YYYY)
    {
        strftime(date_str, sizeof(date_str), "%d/%m/%Y", t);
    }
    else if (required_format == DATE_MM_DD_YYYY)
    {
        strftime(date_str, sizeof(date_str), "%m/%d/%Y", t);
    }
    else if (required_format == DATE_YYYY_MM_DD)
    {
        strftime(date_str, sizeof(date_str), "%Y-%m-%d", t);
    }
    else
    {
        strcpy(date_str, "Invalid");
    }
    return date_str;
}