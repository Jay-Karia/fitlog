#include "../include/fitlog.h"

int write_csv_from_array(const char *array_data, const char *output_path)
{
    if (array_data == NULL || strlen(array_data) < 2 || array_data[0] != '[' || array_data[strlen(array_data) - 1] != ']')
    {
        return 0; // Invalid JSON array
    }

    // Make a copy of the array data for processing
    char *data_copy = strdup(array_data);
    if (data_copy == NULL)
    {
        return 0; // Memory allocation failed
    }

    // Remove the surrounding brackets
    char *data = data_copy + 1;
    data[strlen(data) - 1] = '\0';

    FILE *fp = fopen(output_path, "w");
    if (fp == NULL)
    {
        free(data_copy);
        return 0;
    }

    // Determine the CSV type based on the first object's keys
    char *first_object = NULL;
    bool is_workout = false;

    // Find the first object by looking for a { character
    char *start = strchr(data, '{');
    if (start)
    {
        // Find the closing }
        char *end = strchr(start, '}');
        if (end)
        {
            size_t length = end - start + 1;
            first_object = (char *)malloc(length + 1);
            if (first_object)
            {
                strncpy(first_object, start, length);
                first_object[length] = '\0';

                // Check if it's a workout or exercise by looking for specific keys
                is_workout = strstr(first_object, "\"exercise\"") != NULL;
                free(first_object);
            }
        }
    }

    // Write the appropriate CSV header
    if (is_workout)
    {
        fprintf(fp, "Id,Exercise,Sets,Reps,Weight,Time,Date,Notes\n");
    }
    else
    {
        fprintf(fp, "Id,Name,Shortcut,Description,Type\n");
    }

    // Process each object in the array
    size_t count = 0;
    char *obj_start = data;

    while (obj_start != NULL && *obj_start != '\0')
    {
        // Find the start of the object
        obj_start = strchr(obj_start, '{');
        if (!obj_start)
            break;

        // Find the end of the object
        char *obj_end = NULL;
        int brace_count = 1;
        char *pos = obj_start + 1;

        while (*pos != '\0' && brace_count > 0)
        {
            if (*pos == '{')
                brace_count++;
            else if (*pos == '}')
                brace_count--;
            pos++;
        }

        if (brace_count == 0)
        {
            obj_end = pos - 1;
        }
        else
        {
            break; // Malformed JSON
        }

        // Extract the object
        size_t obj_len = obj_end - obj_start + 1;
        char *obj = (char *)malloc(obj_len + 1);
        if (!obj)
            break;

        strncpy(obj, obj_start, obj_len);
        obj[obj_len] = '\0';

        // Process this object to extract fields
        char *fields[8] = {NULL}; // Max 8 fields for either workouts or exercises

        if (is_workout)
        {
            // For workouts: id, exercise, sets, reps, weight, time, date, notes
            const char *keys[] = {"\"id\":", "\"exercise\":", "\"sets\":", "\"reps\":", "\"weight\":", "\"time\":", "\"date\":", "\"notes\":"};
            for (int i = 0; i < 8; i++)
            {
                char *key_pos = strstr(obj, keys[i]);
                if (key_pos)
                {
                    key_pos += strlen(keys[i]); // Move past the key

                    // Find the value (should be in quotes)
                    if (*key_pos == '\"')
                    {
                        key_pos++; // Skip opening quote
                        char *end_val = strchr(key_pos, '\"');
                        if (end_val)
                        {
                            size_t val_len = end_val - key_pos;
                            fields[i] = (char *)malloc(val_len + 1);
                            if (fields[i])
                            {
                                strncpy(fields[i], key_pos, val_len);
                                fields[i][val_len] = '\0';
                            }
                        }
                    }
                }

                // If field is NULL, use empty string
                if (fields[i] == NULL)
                {
                    fields[i] = strdup("");
                }
            }
        }
        else
        {
            // For exercises: id, name, shortcut, description, type
            const char *keys[] = {"\"id\":", "\"name\":", "\"shortcut\":", "\"description\":", "\"type\":"};
            for (int i = 0; i < 5; i++)
            {
                char *key_pos = strstr(obj, keys[i]);
                if (key_pos)
                {
                    key_pos += strlen(keys[i]); // Move past the key

                    // Find the value (should be in quotes)
                    if (*key_pos == '\"')
                    {
                        key_pos++; // Skip opening quote
                        char *end_val = strchr(key_pos, '\"');
                        if (end_val)
                        {
                            size_t val_len = end_val - key_pos;
                            fields[i] = (char *)malloc(val_len + 1);
                            if (fields[i])
                            {
                                strncpy(fields[i], key_pos, val_len);
                                fields[i][val_len] = '\0';
                            }
                        }
                    }
                }

                // If field is NULL, use empty string
                if (fields[i] == NULL)
                {
                    fields[i] = strdup("");
                }
            }
        }

        // Write CSV line
        if (is_workout)
        {
            fprintf(fp, "%s,%s,%s,%s,%s,%s,%s,%s\n",
                    fields[0], fields[1], fields[2], fields[3],
                    fields[4], fields[5], fields[6], fields[7]);
        }
        else
        {
            fprintf(fp, "%s,%s,%s,%s,%s\n",
                    fields[0], fields[1], fields[2], fields[3], fields[4]);
        }

        // Free fields
        for (int i = 0; i < (is_workout ? 8 : 5); i++)
        {
            free(fields[i]);
        }

        free(obj);
        count++;

        // Move to the next object
        obj_start = obj_end + 1;
    }

    fclose(fp);
    free(data_copy);
    return count; // Return the number of objects processed
}

char *object_to_ini(const char *object_data)
{
    if (object_data == NULL || strlen(object_data) < 2 || object_data[0] != '{' || object_data[strlen(object_data) - 1] != '}')
    {
        return NULL; // Invalid JSON object
    }

    // Make a copy of the original JSON string for safer processing
    char *json_copy = strdup(object_data);
    if (json_copy == NULL) {
        return NULL; // Memory allocation failed
    }

    // Prepare result buffer
    char *result = malloc(strlen(json_copy) * 2); // Allocate plenty of space
    if (result == NULL) {
        free(json_copy);
        return NULL;
    }
    result[0] = '\0';

    // We'll parse the JSON manually to avoid issues with strtok modifying the string
    char *p = json_copy + 1; // Skip opening '{'
    char *end = json_copy + strlen(json_copy) - 1; // Points to closing '}'
    *end = '\0'; // Terminate at the closing brace
    
    char *lineStart = result;
    int resultLen = 0;
    
    while (p && *p) {
        // Skip whitespace
        while (*p && (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')) p++;
        if (!*p) break;
        
        // Check for key (should start with a quote)
        if (*p != '"') {
            p++;
            continue;
        }
        
        p++; // Skip opening quote of key
        char *keyStart = p;
        
        // Find end of key
        while (*p && *p != '"') p++;
        if (!*p) break;
        
        *p = '\0'; // Terminate key
        char *key = keyStart;
        
        p++; // Skip past the closing quote
        
        // Look for colon
        while (*p && *p != ':') p++;
        if (!*p) break;
        
        p++; // Skip colon
        
        // Skip whitespace before value
        while (*p && (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')) p++;
        if (!*p) break;
        
        char *valueStart;
        char *valueEnd;
        
        // Handle quoted value
        if (*p == '"') {
            p++; // Skip opening quote
            valueStart = p;
            
            // Find end of quoted value
            while (*p && *p != '"') p++;
            if (!*p) break;
            
            valueEnd = p;
            p++; // Skip closing quote
        } else {
            // Unquoted value - find end (comma or end of object)
            valueStart = p;
            while (*p && *p != ',' && *p != '}') p++;
            valueEnd = p;
            
            // Trim trailing whitespace from value
            char *vEnd = valueEnd - 1;
            while (vEnd > valueStart && (*vEnd == ' ' || *vEnd == '\t' || *vEnd == '\n' || *vEnd == '\r')) {
                vEnd--;
            }
            valueEnd = vEnd + 1;
        }
        
        // Temporarily terminate value
        char origChar = *valueEnd;
        *valueEnd = '\0';
        
        // Append to result: key=value\\n
        if (resultLen > 0) {
            strcat(result, "\n");
            resultLen += 1;
        }
        strcat(result, key);
        strcat(result, "=");
        strcat(result, valueStart);
        resultLen += strlen(key) + 1 + strlen(valueStart);
        
        // Restore value terminator
        *valueEnd = origChar;
        
        // Move to next key-value pair
        while (*p && *p != ',') p++;
        if (*p == ',') p++;
    }
    
    free(json_copy);
    return result;
}

char *read_workout_array(const char *json_data)
{
    if (json_data == NULL)
    {
        return NULL;
    }

    const char *workouts_key = "\"workouts\":";
    char *start = strstr(json_data, workouts_key);
    if (start == NULL)
    {
        return NULL; // "workouts" key not found
    }

    start += strlen(workouts_key);

    // Find the starting '[' of the array
    while (*start != '\0' && *start != '[')
    {
        start++;
    }
    if (*start != '[')
    {
        return NULL; // No array found
    }

    // Now find the matching closing ']'
    char *end = start;
    int bracket_count = 0;
    while (*end != '\0')
    {
        if (*end == '[')
        {
            bracket_count++;
        }
        else if (*end == ']')
        {
            bracket_count--;
            if (bracket_count == 0)
            {
                break; // Found the matching closing bracket
            }
        }
        end++;
    }

    if (*end != ']')
    {
        return NULL; // No matching closing bracket found
    }

    // Extract the array substring
    size_t array_length = end - start + 1;
    char *array_data = (char *)malloc(array_length + 1);
    if (array_data == NULL)
    {
        return NULL; // Memory allocation failed
    }
    strncpy(array_data, start, array_length);
    array_data[array_length] = '\0';

    return array_data;
}

char *read_exercise_array(const char *json_data)
{
    if (json_data == NULL)
    {
        return NULL;
    }

    const char *exercises_key = "\"exercises\":";
    char *start = strstr(json_data, exercises_key);
    if (start == NULL)
    {
        return NULL; // "exercises" key not found
    }

    start += strlen(exercises_key);

    // Find the starting '[' of the array
    while (*start != '\0' && *start != '[')
    {
        start++;
    }
    if (*start != '[')
    {
        return NULL; // No array found
    }

    // Now find the matching closing ']'
    char *end = start;
    int bracket_count = 0;
    while (*end != '\0')
    {
        if (*end == '[')
        {
            bracket_count++;
        }
        else if (*end == ']')
        {
            bracket_count--;
            if (bracket_count == 0)
            {
                break; // Found the matching closing bracket
            }
        }
        end++;
    }

    if (*end != ']')
    {
        return NULL; // No matching closing bracket found
    }

    // Extract the array substring
    size_t array_length = end - start + 1;
    char *array_data = (char *)malloc(array_length + 1);
    if (array_data == NULL)
    {
        return NULL; // Memory allocation failed
    }
    strncpy(array_data, start, array_length);
    array_data[array_length] = '\0';

    return array_data;
}

char *read_config_object(const char *json_data)
{
    if (json_data == NULL)
    {
        return NULL;
    }

    const char *config_key = "\"config\":";
    char *start = strstr(json_data, config_key);
    if (start == NULL)
    {
        return NULL; // "config" key not found
    }
    start += strlen(config_key);
    while (*start != '\0' && *start != '{')
    {
        start++;
    }
    if (*start != '{')
    {
        return NULL; // No object found
    }
    char *end = start;
    int brace_count = 0;
    while (*end != '\0')
    {
        if (*end == '{')
        {
            brace_count++;
        }
        else if (*end == '}')
        {
            brace_count--;
            if (brace_count == 0)
            {
                break; // Found the matching closing brace
            }
        }
        end++;
    }
    if (*end != '}')
    {
        return NULL; // No matching closing brace found
    }
    size_t object_length = end - start + 1;
    char *object_data = (char *)malloc(object_length + 1);
    if (object_data == NULL)
    {
        return NULL; // Memory allocation failed
    }
    strncpy(object_data, start, object_length);
    object_data[object_length] = '\0';
    return object_data;
}

char *read_shortcuts_object(const char *json_data)
{
    if (json_data == NULL)
    {
        return NULL;
    }

    const char *shortcuts_key = "\"shortcuts\":";
    char *start = strstr(json_data, shortcuts_key);
    if (start == NULL)
    {
        return NULL; // "shortcuts" key not found
    }
    start += strlen(shortcuts_key);
    while (*start != '\0' && *start != '{')
    {
        start++;
    }
    if (*start != '{')
    {
        return NULL; // No object found
    }
    char *end = start;
    int brace_count = 0;
    while (*end != '\0')
    {
        if (*end == '{')
        {
            brace_count++;
        }
        else if (*end == '}')
        {
            brace_count--;
            if (brace_count == 0)
            {
                break; // Found the matching closing brace
            }
        }
        end++;
    }
    if (*end != '}')
    {
        return NULL; // No matching closing brace found
    }
    size_t object_length = end - start + 1;
    char *object_data = (char *)malloc(object_length + 1);
    if (object_data == NULL)
    {
        return NULL; // Memory allocation failed
    }
    strncpy(object_data, start, object_length);
    object_data[object_length] = '\0';
    return object_data;
}