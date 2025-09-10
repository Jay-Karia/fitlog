#include <stdio.h>
#include <time.h>
#include "fitlog.h"

bool is_valid_date_format(const char *input_date, const enum DateFormat required_format)
{
    if (required_format == DATE_DD_MM_YYYY)
    {
        int day, month, year;
        if (sscanf(input_date, "%d-%d-%d", &day, &month, &year) != 3)
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
        if (sscanf(input_date, "%d-%d-%d", &month, &day, &year) != 3)
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

char *convert_date_to_standard(const char *input_date, const enum DateFormat input_format)
{
    static char standard_date[11];
    int day, month, year;
    if (input_format == DATE_DD_MM_YYYY)
    {
        if (sscanf(input_date, "%d-%d-%d", &day, &month, &year) != 3)
            return NULL;
    }
    else if (input_format == DATE_MM_DD_YYYY)
    {
        if (sscanf(input_date, "%d-%d-%d", &month, &day, &year) != 3)
            return NULL;
    }
    else if (input_format == DATE_YYYY_MM_DD)
    {
        if (sscanf(input_date, "%d-%d-%d", &year, &month, &day) != 3)
            return NULL;
    }
    else
    {
        return NULL;
    }
    snprintf(standard_date, sizeof(standard_date), "%04d-%02d-%02d", year, month, day);
    return standard_date;
}