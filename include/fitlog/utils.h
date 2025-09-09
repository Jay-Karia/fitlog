#ifndef FITLOG_UTILS_H
#define FITLOG_UTILS_H

// General help functions
void print_usage(void);
void print_command_help(const char* command);

// Command-specific help functions
void print_init_help(void);
void print_create_help(void);
void print_add_help(void);
void print_show_help(void);
void print_remove_help(void);

// Configuration handling
int read_config_value(const char* key, const char* default_value, char* buffer, size_t buffer_size);

// Id counter handling
int get_next_exercise_id(void);
void increment_exercise_id(void);

#endif