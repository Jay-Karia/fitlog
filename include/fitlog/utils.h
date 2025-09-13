#include <stdbool.h>
#include <stddef.h>

#ifndef FITLOG_UTILS_H
#define FITLOG_UTILS_H

// General help functions
void print_usage(void);
void print_command_help(const char *command);

// Command-specific help functions
void print_init_help(void);
void print_create_help(void);
void print_add_help(void);
void print_show_help(void);
void print_remove_help(void);
void print_config_help(void);
void print_export_help(void);
void print_import_help(void);

// Configuration handling
int read_config_value(const char *key, const char *default_value, char *buffer, size_t buffer_size);
WeightUnit get_config_weight_unit();
TimeUnit get_config_time_unit();
DateFormat get_config_date_format();

// Id counter handling
int get_next_exercise_id(void);
void increment_exercise_id(void);

// Exercise utils
bool exercise_exists(const char *name);
ExerciseType check_exercise_type(const char *name);
bool shortcut_exists(const char *shortcut);
char *get_exercise_name_from_shortcut(const char *shortcut);
bool print_exercise_details_from_id(const char *id);
void remove_exercise_by_id(const char *id);
int show_exercise_by_id(const char *id);
int show_last_n_exercises(int n);
int show_all_exercises();

// Workout utils
void get_workouts_by_id(const char *id, WorkoutLog *workouts);
void get_workouts_by_date(const char *date, WorkoutLog *workouts);
void remove_workouts(const WorkoutLog *workouts);
void print_workouts(const WorkoutLog *workouts);
int show_workout_by_id(const char *id);
int show_last_n_workouts(int n);
int show_workouts_in_date_range(const char *from_date, const char *to_date);
int show_all_workouts(void);

// Date utils
bool is_valid_date_format(const char *input_date, const enum DateFormat required_format);
char *get_today_date(const enum DateFormat required_format);
char *convert_date_to_standard(const char *input_date, const enum DateFormat input_format);
char *get_date_in_format(const char *standard_date, const enum DateFormat required_format);

// Config utils
bool is_valid_config_value(const char *config_key, const char *value);
int reset_config_to_defaults(void);

// Export Utils
char *get_exercise_array();
char *get_workout_array();
char *get_config_object();
char *get_shortcuts_object();
int get_id_counter_value();

#endif