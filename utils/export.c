#include <../include/fitlog.h>

char *get_exercise_array(void)
{
    char full_path[256];
    sprintf(full_path, "%s/%s", FITLOG_DIR, EXERCISES_FILE);
    FILE *file = fopen(full_path, "r");
    if (file == NULL)
    {
        return NULL;
    }

    size_t count = 0;

    char buffer[1024];
    char *result = malloc(2);
    strcpy(result, "[");
    size_t total_length = 1;

    bool is_header = true;
    
    while (fgets(buffer, sizeof(buffer), file) && count < 256)
    {
        // Remove newline if present
        buffer[strcspn(buffer, "\n")] = '\0';

        // Skip empty lines
        if (strlen(buffer) == 0)
            continue;

        // Skip header row
        if (is_header)
        {
            is_header = false;
            continue;
        }

        // Parse CSV fields (assuming format: id,name,description)
        char *id = strtok(buffer, ",");
        char *name = strtok(NULL, ",");
        char *description = strtok(NULL, ",");

        if (id && name)
        {
            // Add comma if not first element
            if (count > 0)
            {
                result = realloc(result, total_length + 2);
                strcat(result, ",");
                total_length += 1;
            }

            // Parse CSV fields (assuming format: id,name,shortcut,description,type)
            char *shortcut = strtok(NULL, ",");
            char *type = strtok(NULL, ",");

            // Create JSON object for exercise
            char json_obj[512];
            snprintf(json_obj, sizeof(json_obj),
                     "{\"id\":\"%s\",\"name\":\"%s\",\"shortcut\":\"%s\",\"description\":\"%s\",\"type\":\"%s\"}",
                     id ? id : "", name ? name : "", shortcut ? shortcut : "", description ? description : "", type ? type : "");

            size_t obj_length = strlen(json_obj);
            result = realloc(result, total_length + obj_length + 1);
            strcat(result, json_obj);
            total_length += obj_length;
            count++;
        }
    }

    // Close the array
    result = realloc(result, total_length + 2);
    strcat(result, "]");

    fclose(file);
    return result;
}

char *get_workout_array(void)
{
    char full_path[256];
    sprintf(full_path, "%s/%s", FITLOG_DIR, WORKOUTS_FILE);
    FILE *file = fopen(full_path, "r");
    if (file == NULL)
    {
        return NULL;
    }

    size_t count = 0;
    bool is_header = true; // Flag to skip the header row

    char buffer[1024];
    char *result = malloc(2); // Start with "["
    strcpy(result, "[");
    size_t total_length = 1;

    while (fgets(buffer, sizeof(buffer), file) && count < 256)
    {
        // Remove newline if present
        buffer[strcspn(buffer, "\n")] = '\0';

        // Skip empty lines
        if (strlen(buffer) == 0)
            continue;

        // Skip header row
        if (is_header)
        {
            is_header = false;
            continue;
        }

        // Create a working copy of the buffer to parse
        char buffer_copy[1024];
        strncpy(buffer_copy, buffer, sizeof(buffer_copy));
        buffer_copy[sizeof(buffer_copy) - 1] = '\0';

        // Parse CSV fields properly preserving empty fields
        // Format: Id,Exercise,Sets,Reps,Weight,Time,Date,Notes
        char id[20] = "", exercise[100] = "", sets[20] = "", reps[20] = "";
        char weight[50] = "", time[20] = "", date[20] = "", notes[200] = "";

        // Manual CSV parsing to handle empty fields correctly
        char *ptr = buffer_copy;
        int field = 0;
        char *start = ptr;

        while (*ptr != '\0' && field < 8)
        {
            if (*ptr == ',')
            {
                // End of field
                *ptr = '\0'; // Terminate the field

                // Copy to appropriate field
                switch (field)
                {
                case 0:
                    strncpy(id, start, sizeof(id) - 1);
                    id[sizeof(id) - 1] = '\0';
                    break;
                case 1:
                    strncpy(exercise, start, sizeof(exercise) - 1);
                    exercise[sizeof(exercise) - 1] = '\0';
                    break;
                case 2:
                    strncpy(sets, start, sizeof(sets) - 1);
                    sets[sizeof(sets) - 1] = '\0';
                    break;
                case 3:
                    strncpy(reps, start, sizeof(reps) - 1);
                    reps[sizeof(reps) - 1] = '\0';
                    break;
                case 4:
                    strncpy(weight, start, sizeof(weight) - 1);
                    weight[sizeof(weight) - 1] = '\0';
                    break;
                case 5:
                    strncpy(time, start, sizeof(time) - 1);
                    time[sizeof(time) - 1] = '\0';
                    break;
                case 6:
                    strncpy(date, start, sizeof(date) - 1);
                    date[sizeof(date) - 1] = '\0';
                    break;
                }

                field++;
                ptr++; // Move past the comma
                start = ptr;
            }
            else
            {
                ptr++;
            }
        }

        // Handle the last field (notes) or any remaining text
        if (field == 7)
        {
            strncpy(notes, start, sizeof(notes) - 1);
            notes[sizeof(notes) - 1] = '\0';
        }

        // Only require id and exercise to be valid
        if (id[0] != '\0' && exercise[0] != '\0')
        {
            // Add comma if not first element
            if (count > 0)
            {
                result = realloc(result, total_length + 2);
                strcat(result, ",");
                total_length += 1;
            }

            // Create JSON object for workout
            // Increase buffer size to safely accommodate all fields (especially notes which can be up to 200 chars)
            char json_obj[1024];
            snprintf(json_obj, sizeof(json_obj),
                     "{\"id\":\"%s\",\"exercise\":\"%s\",\"sets\":\"%s\",\"reps\":\"%s\",\"weight\":\"%s\",\"time\":\"%s\",\"date\":\"%s\",\"notes\":\"%s\"}",
                     id, exercise, sets, reps, weight, time, date, notes);

            size_t obj_length = strlen(json_obj);
            result = realloc(result, total_length + obj_length + 1);
            strcat(result, json_obj);
            total_length += obj_length;
            count++;
        }
    }

    // Close the array
    result = realloc(result, total_length + 2);
    strcat(result, "]");

    fclose(file);
    return result;
}

char *get_config_object(void)
{
    char full_path[256];
    sprintf(full_path, "%s/%s", FITLOG_DIR, CONFIG_FILE);
    FILE *file = fopen(full_path, "r");
    if (file == NULL)
    {
        return NULL;
    }

    char *result = malloc(2); // Start with "{"
    strcpy(result, "{");
    size_t total_length = 1;
    char buffer[1024];
    bool first_entry = true;

    while (fgets(buffer, sizeof(buffer), file))
    {
        // Remove newline if present
        buffer[strcspn(buffer, "\n")] = '\0';

        // Skip empty lines and comments
        if (strlen(buffer) == 0 || buffer[0] == '#' || buffer[0] == ';')
            continue;

        // Look for key=value pairs
        char *equals = strchr(buffer, '=');
        if (equals != NULL)
        {
            *equals = '\0'; // Split at equals sign
            char *key = buffer;
            char *value = equals + 1;

            // Trim whitespace from key and value
            while (*key == ' ' || *key == '\t')
                key++;
            while (*value == ' ' || *value == '\t')
                value++;

            char *key_end = key + strlen(key) - 1;
            while (key_end > key && (*key_end == ' ' || *key_end == '\t'))
            {
                *key_end = '\0';
                key_end--;
            }

            char *value_end = value + strlen(value) - 1;
            while (value_end > value && (*value_end == ' ' || *value_end == '\t'))
            {
                *value_end = '\0';
                value_end--;
            }

            if (strlen(key) > 0)
            {
                // Add comma if not first entry
                if (!first_entry)
                {
                    result = realloc(result, total_length + 2);
                    strcat(result, ",");
                    total_length += 1;
                }

                // Create key-value pair in JSON format
                char json_pair[1044];
                snprintf(json_pair, sizeof(json_pair), "\"%s\":\"%s\"", key, value);

                size_t pair_length = strlen(json_pair);
                result = realloc(result, total_length + pair_length + 1);
                strcat(result, json_pair);
                total_length += pair_length;
                first_entry = false;
            }
        }
    }

    // Close the object
    result = realloc(result, total_length + 2);
    strcat(result, "}");

    fclose(file);
    return result;
}

char *get_shortcuts_object(void)
{
    char full_path[256];
    sprintf(full_path, "%s/%s", FITLOG_DIR, SHORTCUTS_FILE);
    FILE *file = fopen(full_path, "r");
    if (file == NULL)
    {
        return NULL;
    }

    char *result = malloc(2); // Start with "{"
    strcpy(result, "{");
    size_t total_length = 1;
    char buffer[1024];
    bool first_entry = true;

    while (fgets(buffer, sizeof(buffer), file))
    {
        // Remove newline if present
        buffer[strcspn(buffer, "\n")] = '\0';

        // Skip empty lines and comments
        if (strlen(buffer) == 0 || buffer[0] == '#' || buffer[0] == ';')
            continue;

        // Look for key=value pairs
        char *equals = strchr(buffer, '=');
        if (equals != NULL)
        {
            *equals = '\0'; // Split at equals sign
            char *key = buffer;
            char *value = equals + 1;

            // Trim whitespace from key and value
            while (*key == ' ' || *key == '\t')
                key++;
            while (*value == ' ' || *value == '\t')
                value++;

            char *key_end = key + strlen(key) - 1;
            while (key_end > key && (*key_end == ' ' || *key_end == '\t'))
            {
                *key_end = '\0';
                key_end--;
            }

            char *value_end = value + strlen(value) - 1;
            while (value_end > value && (*value_end == ' ' || *value_end == '\t'))
            {
                *value_end = '\0';
                value_end--;
            }

            if (strlen(key) > 0)
            {
                // Add comma if not first entry
                if (!first_entry)
                {
                    result = realloc(result, total_length + 2);
                    strcat(result, ",");
                    total_length += 1;
                }

                // Create key-value pair in JSON format
                char json_pair[1044];
                snprintf(json_pair, sizeof(json_pair), "\"%s\":\"%s\"", key, value);

                size_t pair_length = strlen(json_pair);
                result = realloc(result, total_length + pair_length + 1);
                strcat(result, json_pair);
                total_length += pair_length;
                first_entry = false;
            }
        }
    }

    // Close the object
    result = realloc(result, total_length + 2);
    strcat(result, "}");

    fclose(file);
    return result;
}

int get_id_counter_value(void)
{
    char full_path[256];
    sprintf(full_path, "%s/%s", FITLOG_DIR, ID_COUNTER_FILE);
    FILE *file = fopen(full_path, "r");
    if (file == NULL)
    {
        return -1;
    }

    int counter_value = 0;
    if (fscanf(file, "%d", &counter_value) != 1)
    {
        fclose(file);
        return -1;
    }

    fclose(file);
    return counter_value;
}