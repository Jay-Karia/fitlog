#include "../include/fitlog.h"

int cmd_config(int argc, char *argv[])
{
    // Check for help flag
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
        {
            print_add_help();
            return 0;
        }
    }

    // Check for --get options
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--get") == 0 && i + 1 < argc)
        {
            const char *key = argv[++i];
            char value[256];
            if (read_config_value(key, "", value, sizeof(value)) == 0)
            {
                printf("%s\n", value);
            }
            else
            {
                fprintf(stderr, ANSI_COLOR_RED "Error: Could not read config key '%s'\n" ANSI_COLOR_RESET, key);
                printf("Check whether the key is valid.");
                return 1;
            }
            return 0;
        }
    }

    // Check for --set options
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--set") == 0 && i + 2 < argc)
        {
            const char *key = argv[++i];
            const char *value = argv[++i];

            // Read existing config
            char config_path[256];
            sprintf(config_path, "%s/%s", FITLOG_DIR, CONFIG_FILE);
            FILE *fp = fopen(config_path, "r");
            if (fp == NULL)
            {
                perror(ANSI_COLOR_RED "Error: Could not open config file" ANSI_COLOR_RESET);
                return 1;
            }

            char lines[100][256];
            int line_count = 0;
            bool key_found = false;

            while (fgets(lines[line_count], sizeof(lines[line_count]), fp) != NULL)
            {
                // Check if this line contains the key
                if (strncmp(lines[line_count], key, strlen(key)) == 0 && lines[line_count][strlen(key)] == '=')
                {
                    snprintf(lines[line_count], sizeof(lines[line_count]), "%s=%s\n", key, value);
                    key_found = true;
                }
                line_count++;
            }
            fclose(fp);

            if (!key_found)
            {
                fprintf(stderr, ANSI_COLOR_RED "Error: Key '%s' not found in config.\n" ANSI_COLOR_RESET, key);
                return 1;
            }

            // Validate config value
            bool is_valid_value = is_valid_config_value(key, value);
            
            if (!is_valid_value)
            {
                fprintf(stderr, ANSI_COLOR_RED "Error: Invalid config value '%s'\n" ANSI_COLOR_RESET, value);
                return 1;
            }

            // Write back the updated config
            fp = fopen(config_path, "w");
            if (fp == NULL)
            {
                perror(ANSI_COLOR_RED "Error: Could not open config file for writing" ANSI_COLOR_RESET);
                return 1;
            }
            for (int j = 0; j < line_count; j++)
            {
                fputs(lines[j], fp);
            }
            fclose(fp);

            printf(ANSI_COLOR_GREEN "Config key '%s' set to '%s'\n" ANSI_COLOR_RESET, key, value);
            return 0;
        }
    }

    // Check for --list option
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--list") == 0)
        {
            char config_path[256];
            sprintf(config_path, "%s/%s", FITLOG_DIR, CONFIG_FILE);
            FILE *fp = fopen(config_path, "r");
            if (fp == NULL)
            {
                perror(ANSI_COLOR_RED "Error: Could not open config file" ANSI_COLOR_RESET);
                return 1;
            }

            char line[256];
            printf(ANSI_COLOR_BLUE "Current Configuration:\n" ANSI_COLOR_RESET);
            while (fgets(line, sizeof(line), fp) != NULL)
            {
                if (line[0] != '#' && line[0] != '\n')
                {
                    printf("%s", line);
                }
            }
            fclose(fp);
            return 0;
        }
    }

    // Check for --reset option
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--reset") == 0)
        {
            return reset_config_to_defaults();
        }
    }

    printf("Config command is not yet implemented.\n");
    return 0;
}