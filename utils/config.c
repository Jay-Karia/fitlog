#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "../include/fitlog.h"

int read_config_value(const char* key, const char* default_value, char* buffer, size_t buffer_size) {
    if (!key || !buffer || buffer_size == 0) {
        return -1; // Invalid parameters
    }
    
    // Set default value first
    strncpy(buffer, default_value, buffer_size - 1);
    buffer[buffer_size - 1] = '\0';
    
    // Build path to config file
    char config_path[256];
    sprintf(config_path, "%s/%s", FITLOG_DIR, CONFIG_FILE);
    
    // Open config file
    FILE* fp = fopen(config_path, "r");
    if (!fp) {
        // Config file doesn't exist, return default
        return 1;
    }
    
    // Read line by line
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        // Remove newline character
        size_t len = strlen(line);
        if (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) {
            line[len-1] = '\0';
        }
        
        // Skip comments and empty lines
        if (line[0] == '#' || line[0] == '\0') {
            continue;
        }
        
        // Split line into key and value
        char* value = strchr(line, '=');
        if (!value) {
            continue; // No equals sign, skip line
        }
        
        // Null-terminate the key part
        *value = '\0';
        value++; // Move to the value part
        
        // Check if this is our key
        if (strcmp(line, key) == 0) {
            strncpy(buffer, value, buffer_size - 1);
            buffer[buffer_size - 1] = '\0';
            fclose(fp);
            return 0; // Success
        }
    }
    
    fclose(fp);
    return 2; // Key not found
}
