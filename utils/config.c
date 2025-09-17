#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "../include/fitlog.h"

int read_config_value(const char *key, const char *default_value, char *buffer, size_t buffer_size)
{
    if (!key || !buffer || buffer_size == 0)
    {
        return -1; // Invalid parameters
    }

    // Set default value first
    strncpy(buffer, default_value, buffer_size - 1);
    buffer[buffer_size - 1] = '\0';

    // Build path to config file
    char config_path[256];
    sprintf(config_path, "%s/%s", FITLOG_DIR, CONFIG_FILE);

    // Open config file
    FILE *fp = fopen(config_path, "r");
    if (!fp)
    {
        // Config file doesn't exist, return default
        return 1;
    }

    // Read line by line
    char line[256];
    while (fgets(line, sizeof(line), fp))
    {
        // Remove newline character
        size_t len = strlen(line);
        if (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
        {
            line[len - 1] = '\0';
        }

        // Skip comments and empty lines
        if (line[0] == '#' || line[0] == '\0')
        {
            continue;
        }

        // Split line into key and value
        char *value = strchr(line, '=');
        if (!value)
        {
            continue; // No equals sign, skip line
        }

        // Null-terminate the key part
        *value = '\0';
        value++; // Move to the value part

        // Check if this is our key
        if (strcmp(line, key) == 0)
        {
            strncpy(buffer, value, buffer_size - 1);
            buffer[buffer_size - 1] = '\0';
            fclose(fp);
            return 0; // Success
        }
    }

    fclose(fp);
    return 2; // Key not found
}

WeightUnit get_config_weight_unit()
{
    char buffer[32];
    int result = read_config_value("weight_unit", "kg", buffer, sizeof(buffer));
    if (result == 0 || result == 1)
    {
        if (strcmp(buffer, "lbs") == 0)
        {
            return UNIT_LBS;
        }
        else
        {
            return UNIT_KG;
        }
    }
    else
    {
        return DEFAULT_WEIGHT_UNIT; // Default if error
    }
}

DateFormat get_config_date_format()
{
    char buffer[32];
    int result = read_config_value("date_format", "YYYY-DD-MM", buffer, sizeof(buffer));
    if (result == 0 || result == 1)
    {
        if (strcmp(buffer, "MM-DD-YYYY") == 0)
        {
            return DATE_MM_DD_YYYY;
        }
        else if (strcmp(buffer, "DD-MM-YYYY") == 0)
        {
            return DATE_DD_MM_YYYY;
        }
        else
        {
            return DATE_YYYY_MM_DD;
        }
    }
    else
    {
        return DEFAULT_DATE_FORMAT; // Default if error
    }
}

TimeUnit get_config_time_unit()
{
    char buffer[32];
    int result = read_config_value("time_unit", "s", buffer, sizeof(buffer));
    if (result == 0 || result == 1)
    {
        if (strcmp(buffer, "min") == 0)
        {
            return TIME_MIN;
        }
        else if (strcmp(buffer, "hr") == 0)
        {
            return TIME_HR;
        }
        else
        {
            return TIME_S;
        }
    }
    else
    {
        return DEFAULT_TIME_UNIT; // Default if error
    }
}

bool is_valid_config_value(const char *config_key, const char *value)
{
    if (strcmp(config_key, "date_format") == 0)
    {
        return (strcmp(value, "YYYY-DD-MM") == 0 ||
                strcmp(value, "DD-MM-YYYY") == 0 ||
                strcmp(value, "MM-DD-YYYY") == 0);
    }
    else if (strcmp(config_key, "weight_unit") == 0)
    {
        return (strcmp(value, "kg") == 0 || strcmp(value, "lbs") == 0);
    }
    else if (strcmp(config_key, "time_unit") == 0)
    {
        return (strcmp(value, "s") == 0 ||
                strcmp(value, "min") == 0 ||
                strcmp(value, "hr") == 0);
    }
    else if (strcmp(config_key, "default_exercise_type") == 0)
    {
        return (strcmp(value, "weight") == 0 ||
                strcmp(value, "time") == 0 ||
                strcmp(value, "body") == 0);
    }
    else if (strcmp(config_key, "distance_unit") == 0)
    {
        return (strcmp(value, "m") == 0 ||
                strcmp(value, "km") == 0 ||
                strcmp(value, "miles") == 0);
    }
    return false; // Unknown key
}

int reset_config_to_defaults(void)
{
    // Build path to config file
    char config_path[256];
    sprintf(config_path, "%s/%s", FITLOG_DIR, CONFIG_FILE);

    FILE *fp = fopen(config_path, "w");
    if (fp == NULL)
    {
        perror(ANSI_COLOR_RED "Error resetting config to defaults" ANSI_COLOR_RESET);
        return 1;
    }

    fprintf(fp, "# Date and time\n");
    fprintf(fp, "date_format=YYYY-DD-MM\n\n");
    fprintf(fp, "# Units and measurements\n");
    fprintf(fp, "weight_unit=lbs\n");
    fprintf(fp, "time_unit=s\n");
    fprintf(fp, "distance_unit=m\n\n");
    fprintf(fp, "# Behaviour\n");
    fprintf(fp, "default_exercise_type=weight\n");

    fclose(fp);

    printf(ANSI_COLOR_GREEN "Configuration reset to default values.\n" ANSI_COLOR_RESET);

    return 0;
}

DistanceUnit get_config_distance_unit()
{
    char buffer[32];
    int result = read_config_value("distance_unit", "m", buffer, sizeof(buffer));
    if (result == 0 || result == 1)
    {
        if (strcmp(buffer, "km") == 0)
        {
            return DISTANCE_KM;
        }
        else if (strcmp(buffer, "miles") == 0)
        {
            return DISTANCE_MI;
        }
        else
        {
            return DISTANCE_M;
        }
    }
    else
    {
        return DEFAULT_DISTANCE_UNIT; // Default if error
    }
}