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

char *get_today_date()
{
    static char date_str[11];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(date_str, sizeof(date_str), "%Y-%d-%m", t);

    return date_str;
}

char *convert_date_to_standard(const char *input_date, const enum DateFormat input_format)
{
    // Use thread-local storage with two buffers to allow for two calls without conflict
    static __thread int buffer_index = 0;
    static __thread char standard_date_buffers[2][11];

    // Alternate between the two buffers
    buffer_index = (buffer_index + 1) % 2;
    char *standard_date = standard_date_buffers[buffer_index];

    int day, month, year;

    // Check for null input
    if (input_date == NULL)
    {
        return NULL;
    }

    // Initialize variables to avoid garbage values
    day = month = year = 0;

    if (input_format == DATE_DD_MM_YYYY)
    {
        // Try both '-' and '/' separators for DD-MM-YYYY or DD/MM/YYYY
        if (sscanf(input_date, "%d-%d-%d", &day, &month, &year) != 3 &&
            sscanf(input_date, "%d/%d/%d", &day, &month, &year) != 3)
        {
            return NULL;
        }
    }
    else if (input_format == DATE_MM_DD_YYYY)
    {
        // Try both '-' and '/' separators for MM-DD-YYYY or MM/DD/YYYY
        if (sscanf(input_date, "%d-%d-%d", &month, &day, &year) != 3 &&
            sscanf(input_date, "%d/%d/%d", &month, &day, &year) != 3)
        {
            return NULL;
        }
    }
    else if (input_format == DATE_YYYY_MM_DD)
    {
        // Try both '-' and '/' separators for YYYY-MM-DD or YYYY/MM/DD
        if (sscanf(input_date, "%d-%d-%d", &year, &month, &day) != 3 &&
            sscanf(input_date, "%d/%d/%d", &year, &month, &day) != 3)
        {
            return NULL;
        }
    }
    else
    {
        return NULL;
    }

    // Validate parsed values
    if (year < 1900 || year > 2100 ||
        month < 1 || month > 12 ||
        day < 1 || day > 31)
    {
        return NULL;
    }

    // Additional validation for days in month
    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Check for leap year
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
    {
        days_in_month[1] = 29;
    }

    if (day > days_in_month[month - 1])
    {
        return NULL;
    }

    // Use snprintf with proper bounds checking
    int result = snprintf(standard_date, 11, "%04d-%02d-%02d", year, month, day);

    // Check if snprintf succeeded
    if (result < 0 || result >= 11)
    {
        return NULL;
    }

    return standard_date;
}

char *get_date_in_format(const char *standard_date, const enum DateFormat required_format)
{
    static char date_str[11];
    int day, month, year;

    // Check for null input
    if (standard_date == NULL)
    {
        return NULL;
    }

    // Parse the standard date (YYYY-MM-DD)
    if (sscanf(standard_date, "%d-%d-%d", &year, &month, &day) != 3)
    {
        return NULL;
    }

    // Validate parsed values
    if (year < 1900 || year > 2100 ||
        month < 1 || month > 12 ||
        day < 1 || day > 31)
    {
        return NULL;
    }

    // Additional validation for days in month
    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Check for leap year
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
    {
        days_in_month[1] = 29;
    }

    if (day > days_in_month[month - 1])
    {
        return NULL;
    }

    // Format the date based on the required format
    if (required_format == DATE_DD_MM_YYYY)
    {
        snprintf(date_str, sizeof(date_str), "%02d-%02d-%04d", day, month, year);
    }
    else if (required_format == DATE_MM_DD_YYYY)
    {
        snprintf(date_str, sizeof(date_str), "%02d-%02d-%04d", month, day, year);
    }
    else if (required_format == DATE_YYYY_MM_DD)
    {
        snprintf(date_str, sizeof(date_str), "%04d-%02d-%02d", year, month, day);
    }
    else
    {
        return NULL;
    }

    return date_str;
}