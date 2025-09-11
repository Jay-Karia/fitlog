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

// Date utils
bool is_valid_date_format(const char *input_date, const enum DateFormat required_format);
char *get_today_date(const enum DateFormat required_format);
char *convert_date_to_standard(const char *input_date, const enum DateFormat input_format);

// Display utils

#endif