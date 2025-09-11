#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "fitlog.h"

// TODO: fix table dynamic border spacing

// Returns the number of valid entries in a WorkoutLog array
int get_workout_array_length(const WorkoutLog *workouts, int max_workouts)
{
    int count = 0;
    for (int i = 0; i < max_workouts; i++)
    {
        if (workouts[i].id[0] == '\0')
            break;
        count++;
    }
    return count;
}

void safe_strncpy(char *dest, const char *src, size_t dest_size)
{
    if (dest_size == 0)
        return;
    strncpy(dest, src, dest_size - 1);
    dest[dest_size - 1] = '\0';
}

const char *parse_csv_field(char *dest, size_t dest_size, const char *line)
{
    const char *comma = strchr(line, ',');
    if (comma)
    {
        size_t len = comma - line;
        if (len >= dest_size)
            len = dest_size - 1;
        strncpy(dest, line, len);
        dest[len] = '\0';
        return comma + 1;
    }
    else
    {
        size_t len = strlen(line);
        // Trim trailing newline if present
        if (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
            len--;
        if (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
            len--;

        if (len >= dest_size)
            len = dest_size - 1;

        strncpy(dest, line, len);
        dest[len] = '\0';
        return NULL;
    }
}

void get_workouts_by_id(const char *id, WorkoutLog *workouts)
{
    char full_path[256];
    sprintf(full_path, "%s/%s", FITLOG_DIR, WORKOUTS_FILE);
    FILE *fp = fopen(full_path, "r");
    if (!fp)
        return;
    char line[1024];
    int count = 0;
    int row = 0;
    while (fgets(line, sizeof(line), fp))
    {
        if (row == 0)
        {
            row++;
            continue;
        } // skip header
        char workout_id[20] = "", exercise[100] = "", sets[20] = "", reps[20] = "", weight[50] = "", time[20] = "", date[20] = "", notes[200] = "";
        const char *p = line;
        p = parse_csv_field(workout_id, sizeof(workout_id), p);
        if (p)
            p = parse_csv_field(exercise, sizeof(exercise), p);
        if (p)
            p = parse_csv_field(sets, sizeof(sets), p);
        if (p)
            p = parse_csv_field(reps, sizeof(reps), p);
        if (p)
            p = parse_csv_field(weight, sizeof(weight), p);
        if (p)
            p = parse_csv_field(time, sizeof(time), p);
        if (p)
            p = parse_csv_field(date, sizeof(date), p);
        if (p)
            parse_csv_field(notes, sizeof(notes), p);
        if (strcmp(workout_id, id) == 0)
        {
            safe_strncpy(workouts[count].id, workout_id, sizeof(workouts[count].id));
            safe_strncpy(workouts[count].exercise, exercise, sizeof(workouts[count].exercise));
            safe_strncpy(workouts[count].sets, sets, sizeof(workouts[count].sets));
            safe_strncpy(workouts[count].reps, reps, sizeof(workouts[count].reps));
            safe_strncpy(workouts[count].weight, weight, sizeof(workouts[count].weight));
            safe_strncpy(workouts[count].time, time, sizeof(workouts[count].time));
            safe_strncpy(workouts[count].date, date, sizeof(workouts[count].date));
            safe_strncpy(workouts[count].notes, notes, sizeof(workouts[count].notes));
            count++;
        }
        row++;
    }
    fclose(fp);
}

void get_workouts_by_date(const char *date, WorkoutLog *workouts)
{
    char full_path[256];
    sprintf(full_path, "%s/%s", FITLOG_DIR, WORKOUTS_FILE);
    FILE *fp = fopen(full_path, "r");
    if (!fp)
        return;
    char line[1024];
    int count = 0;
    int row = 0;
    while (fgets(line, sizeof(line), fp))
    {
        if (row == 0)
        {
            row++;
            continue;
        } // skip header
        char workout_id[20] = "", exercise[100] = "", sets[20] = "", reps[20] = "", weight[50] = "", time[20] = "", date_field[20] = "", notes[200] = "";
        const char *p = line;
        p = parse_csv_field(workout_id, sizeof(workout_id), p);
        if (p)
            p = parse_csv_field(exercise, sizeof(exercise), p);
        if (p)
            p = parse_csv_field(sets, sizeof(sets), p);
        if (p)
            p = parse_csv_field(reps, sizeof(reps), p);
        if (p)
            p = parse_csv_field(weight, sizeof(weight), p);
        if (p)
            p = parse_csv_field(time, sizeof(time), p);
        if (p)
            p = parse_csv_field(date_field, sizeof(date_field), p);
        if (p)
            parse_csv_field(notes, sizeof(notes), p);
        if (strcmp(date_field, date) == 0)
        {
            safe_strncpy(workouts[count].id, workout_id, sizeof(workouts[count].id));
            safe_strncpy(workouts[count].exercise, exercise, sizeof(workouts[count].exercise));
            safe_strncpy(workouts[count].sets, sets, sizeof(workouts[count].sets));
            safe_strncpy(workouts[count].reps, reps, sizeof(workouts[count].reps));
            safe_strncpy(workouts[count].weight, weight, sizeof(workouts[count].weight));
            safe_strncpy(workouts[count].time, time, sizeof(workouts[count].time));
            safe_strncpy(workouts[count].date, date_field, sizeof(workouts[count].date));
            safe_strncpy(workouts[count].notes, notes, sizeof(workouts[count].notes));
            count++;
        }
        row++;
    }
    fclose(fp);
}

void remove_workouts(const WorkoutLog *workouts)
{
    char full_path[256];
    sprintf(full_path, "%s/%s", FITLOG_DIR, WORKOUTS_FILE);

    // Create a temporary file path
    char temp_path[270];
    sprintf(temp_path, "%s.tmp", full_path);

    // Open the original file for reading
    FILE *original_file = fopen(full_path, "r");
    if (!original_file)
    {
        printf(ANSI_COLOR_RED "Error: Could not open workouts file.\n" ANSI_COLOR_RESET);
        return;
    }

    // Open a temporary file for writing
    FILE *temp_file = fopen(temp_path, "w");
    if (!temp_file)
    {
        printf(ANSI_COLOR_RED "Error: Could not create temporary file.\n" ANSI_COLOR_RESET);
        fclose(original_file);
        return;
    }

    // Get the number of workouts to remove
    int workouts_to_remove_count = get_workout_array_length(workouts, 100);
    if (workouts_to_remove_count == 0)
    {
        printf(ANSI_COLOR_YELLOW "No workouts to remove.\n" ANSI_COLOR_RESET);
        fclose(original_file);
        fclose(temp_file);
        remove(temp_path);
        return;
    }

    // Read line by line from the original file
    char line[1024];
    int row = 0;
    int removed_count = 0;

    while (fgets(line, sizeof(line), original_file))
    {
        // Copy the header as-is
        if (row == 0)
        {
            fputs(line, temp_file);
            row++;
            continue;
        }

        // Parse this workout's data
        char workout_id[20] = "", exercise[100] = "", sets[20] = "", reps[20] = "",
             weight[50] = "", time[20] = "", date[20] = "", notes[200] = "";

        const char *p = line;
        p = parse_csv_field(workout_id, sizeof(workout_id), p);
        if (p)
            p = parse_csv_field(exercise, sizeof(exercise), p);
        if (p)
            p = parse_csv_field(sets, sizeof(sets), p);
        if (p)
            p = parse_csv_field(reps, sizeof(reps), p);
        if (p)
            p = parse_csv_field(weight, sizeof(weight), p);
        if (p)
            p = parse_csv_field(time, sizeof(time), p);
        if (p)
            p = parse_csv_field(date, sizeof(date), p);
        if (p)
            parse_csv_field(notes, sizeof(notes), p);

        // Check if this workout should be removed
        bool should_remove = false;
        for (int i = 0; i < workouts_to_remove_count; i++)
        {
            // If both ID and date match, mark for removal
            if (strcmp(workout_id, workouts[i].id) == 0 &&
                strcmp(date, workouts[i].date) == 0)
            {
                should_remove = true;
                removed_count++;
                break;
            }
        }

        // If not marked for removal, copy to the new file
        if (!should_remove)
        {
            fputs(line, temp_file);
        }

        row++;
    }

    // Close both files
    fclose(original_file);
    fclose(temp_file);

    // Replace the original file with the temporary one
    if (removed_count > 0)
    {
        if (remove(full_path) != 0)
        {
            printf(ANSI_COLOR_RED "Error: Could not remove the original file.\n" ANSI_COLOR_RESET);
            remove(temp_path);
            return;
        }

        if (rename(temp_path, full_path) != 0)
        {
            printf(ANSI_COLOR_RED "Error: Could not rename the temporary file.\n" ANSI_COLOR_RESET);
            return;
        }

        printf(ANSI_COLOR_GREEN "Successfully removed %d workout(s).\n" ANSI_COLOR_RESET, removed_count);
    }
    else
    {
        printf(ANSI_COLOR_YELLOW "No matching workouts found to remove.\n" ANSI_COLOR_RESET);
        remove(temp_path);
    }
}

void print_workouts(const WorkoutLog *workouts)
{
    // Get the length of the array
    int length = get_workout_array_length(workouts, 100);
    if (length == 0)
    {
        printf(ANSI_COLOR_YELLOW "No workouts found.\n" ANSI_COLOR_RESET);
        return;
    }

    printf("+----+----------------------+-----+-----+--------+--------+------------+----------------------+\n");
    printf("| %-2s | %-20s | %-3s | %-3s | %-6s | %-6s | %-10s | %-20s |\n",
           "ID", "Exercise", "Set", "Rep", "Weight", "Time", "Date", "Notes");
    printf("+----+----------------------+-----+-----+--------+--------+------------+----------------------+\n");

    for (int i = 0; i < length; i++)
    {
        printf("| %-2s | %-20s | %-3s | %-3s | %-6s | %-6s | %-10s | %-20s |",
               workouts[i].id,
               workouts[i].exercise,
               workouts[i].sets,
               workouts[i].reps,
               workouts[i].weight,
               workouts[i].time,
               workouts[i].date,
               workouts[i].notes);
        printf("\n");
    }
    printf("+----+----------------------+-----+-----+--------+--------+------------+----------------------+\n");
}

int show_workout_by_id(const char *id)
{
    WorkoutLog workouts[100] = {0};
    get_workouts_by_id(id, workouts);
    int length = get_workout_array_length(workouts, 100);
    if (length == 0)
    {
        printf(ANSI_COLOR_YELLOW "No workouts found with ID '%s'.\n" ANSI_COLOR_RESET, id);
        return 0;
    }
    print_workouts(workouts);
    return 0;
}

int show_last_n_workouts(int n)
{
    if (n <= 0)
    {
        printf(ANSI_COLOR_RED "Error: Please provide a positive integer for --last.\n" ANSI_COLOR_RESET);
        return 1;
    }

    char full_path[256];
    sprintf(full_path, "%s/%s", FITLOG_DIR, WORKOUTS_FILE);
    FILE *fp = fopen(full_path, "r");
    if (fp == NULL)
    {
        printf(ANSI_COLOR_RED "Error: Workouts file not found. Please run 'fitlog init' first.\n" ANSI_COLOR_RESET);
        return 1;
    }

    // First, count the total number of workouts
    int total_workouts = 0;
    char line[1024];
    // Skip header line
    fgets(line, sizeof(line), fp);
    while (fgets(line, sizeof(line), fp))
    {
        total_workouts++;
    }

    if (total_workouts == 0)
    {
        printf(ANSI_COLOR_YELLOW "No workouts found to display.\n" ANSI_COLOR_RESET);
        fclose(fp);
        return 0;
    }

    // Calculate how many workouts to skip
    int to_skip = total_workouts - n;
    if (to_skip < 0)
    {
        to_skip = 0; // If n is greater than total, show all
    }

    // Reset file pointer to beginning and skip the required lines
    fseek(fp, 0, SEEK_SET);
    fgets(line, sizeof(line), fp); // Skip header

    for (int i = 0; i < to_skip; i++)
    {
        fgets(line, sizeof(line), fp);
    }

    // Prepare an array to hold the workouts
    WorkoutLog workouts[100] = {0};
    int count = 0;

    // Now read the remaining workouts into the array
    while (fgets(line, sizeof(line), fp) && count < 100)
    {
        char workout_id[20] = "", exercise[100] = "", sets[20] = "", reps[20] = "", weight[50] = "", time[20] = "", date[20] = "", notes[200] = "";
        const char *p = line;
        p = parse_csv_field(workout_id, sizeof(workout_id), p);
        if (p)
            p = parse_csv_field(exercise, sizeof(exercise), p);
        if (p)
            p = parse_csv_field(sets, sizeof(sets), p);
        if (p)
            p = parse_csv_field(reps, sizeof(reps), p);
        if (p)
            p = parse_csv_field(weight, sizeof(weight), p);
        if (p)
            p = parse_csv_field(time, sizeof(time), p);
        if (p)
            p = parse_csv_field(date, sizeof(date), p);
        if (p)
            parse_csv_field(notes, sizeof(notes), p);
        safe_strncpy(workouts[count].id, workout_id, sizeof(workouts[count].id));
        safe_strncpy(workouts[count].exercise, exercise, sizeof(workouts[count].exercise));
        safe_strncpy(workouts[count].sets, sets, sizeof(workouts[count].sets));
        safe_strncpy(workouts[count].reps, reps, sizeof(workouts[count].reps));
        safe_strncpy(workouts[count].weight, weight, sizeof(workouts[count].weight));
        safe_strncpy(workouts[count].time, time, sizeof(workouts[count].time));
        safe_strncpy(workouts[count].date, date, sizeof(workouts[count].date));
        safe_strncpy(workouts[count].notes, notes, sizeof(workouts[count].notes));
        count++;
    }
    fclose(fp);
    print_workouts(workouts);
    return 0;
}

int show_all_workouts()
{
    char full_path[256];
    sprintf(full_path, "%s/%s", FITLOG_DIR, WORKOUTS_FILE);
    FILE *fp = fopen(full_path, "r");
    if (fp == NULL)
    {
        printf(ANSI_COLOR_RED "Error: Workouts file not found. Please run 'fitlog init' first.\n" ANSI_COLOR_RESET);
        return 1;
    }

    // Prepare an array to hold the workouts
    WorkoutLog workouts[100] = {0};
    int count = 0;
    char line[1024];

    // Skip header line
    fgets(line, sizeof(line), fp);

    // Read all workouts into the array
    while (fgets(line, sizeof(line), fp) && count < 100)
    {
        char workout_id[20] = "", exercise[100] = "", sets[20] = "", reps[20] = "", weight[50] = "", time[20] = "", date[20] = "", notes[200] = "";
        const char *p = line;
        p = parse_csv_field(workout_id, sizeof(workout_id), p);
        if (p)
            p = parse_csv_field(exercise, sizeof(exercise), p);
        if (p)
            p = parse_csv_field(sets, sizeof(sets), p);
        if (p)
            p = parse_csv_field(reps, sizeof(reps), p);
        if (p)
            p = parse_csv_field(weight, sizeof(weight), p);
        if (p)
            p = parse_csv_field(time, sizeof(time), p);
        if (p)
            p = parse_csv_field(date, sizeof(date), p);
        if (p)
            parse_csv_field(notes, sizeof(notes), p);
        safe_strncpy(workouts[count].id, workout_id, sizeof(workouts[count].id));
        safe_strncpy(workouts[count].exercise, exercise, sizeof(workouts[count].exercise));
        safe_strncpy(workouts[count].sets, sets, sizeof(workouts[count].sets));
        safe_strncpy(workouts[count].reps, reps, sizeof(workouts[count].reps));
        safe_strncpy(workouts[count].weight, weight, sizeof(workouts[count].weight));
        safe_strncpy(workouts[count].time, time, sizeof(workouts[count].time));
        safe_strncpy(workouts[count].date, date, sizeof(workouts[count].date));
        safe_strncpy(workouts[count].notes, notes, sizeof(workouts[count].notes));
        count++;
    }
    fclose(fp);
    print_workouts(workouts);
    return 0;
}

int show_workouts_in_date_range(const char *from_date, const char *to_date)
{
    // The dates are already in YYYY-MM-DD format from the calling function
    
    if (strcmp(from_date, to_date) > 0)
    {
        printf(ANSI_COLOR_RED "Error: 'from' date cannot be later than 'to' date.\n" ANSI_COLOR_RESET);
        return 1;
    }

    char full_path[256];
    sprintf(full_path, "%s/%s", FITLOG_DIR, WORKOUTS_FILE);
    FILE *fp = fopen(full_path, "r");
    if (fp == NULL)
    {
        printf(ANSI_COLOR_RED "Error: Workouts file not found. Please run 'fitlog init' first.\n" ANSI_COLOR_RESET);
        return 1;
    }

    // Prepare an array to hold the workouts
    WorkoutLog workouts[100] = {0};
    int count = 0;
    char line[1024];

    // Skip header line
    fgets(line, sizeof(line), fp);

    // Read all workouts into the array
    while (fgets(line, sizeof(line), fp) && count < 100)
    {
        char workout_id[20] = "", exercise[100] = "", sets[20] = "", reps[20] = "", weight[50] = "", time[20] = "", date[20] = "", notes[200] = "";
        const char *p = line;
        p = parse_csv_field(workout_id, sizeof(workout_id), p);
        if (p)
            p = parse_csv_field(exercise, sizeof(exercise), p);
        if (p)
            p = parse_csv_field(sets, sizeof(sets), p);
        if (p)
            p = parse_csv_field(reps, sizeof(reps), p);
        if (p)
            p = parse_csv_field(weight, sizeof(weight), p);
        if (p)
            p = parse_csv_field(time, sizeof(time), p);
        if (p)
            p = parse_csv_field(date, sizeof(date), p);
        if (p)
            parse_csv_field(notes, sizeof(notes), p);
            
        // Skip entries with invalid dates
        if (strlen(date) == 0) {
            continue;
        }
        
        // Check if the workout date is within the range
        if (strcmp(date, from_date) >= 0 && strcmp(date, to_date) <= 0)
        {
            safe_strncpy(workouts[count].id, workout_id, sizeof(workouts[count].id));
            safe_strncpy(workouts[count].exercise, exercise, sizeof(workouts[count].exercise));
            safe_strncpy(workouts[count].sets, sets, sizeof(workouts[count].sets));
            safe_strncpy(workouts[count].reps, reps, sizeof(workouts[count].reps));
            safe_strncpy(workouts[count].weight, weight, sizeof(workouts[count].weight));
            safe_strncpy(workouts[count].time, time, sizeof(workouts[count].time));
            safe_strncpy(workouts[count].date, date, sizeof(workouts[count].date));
            safe_strncpy(workouts[count].notes, notes, sizeof(workouts[count].notes));
            count++;
        }
    }
    fclose(fp);
    if (count == 0)
    {
        printf(ANSI_COLOR_YELLOW "No workouts found in the date range %s to %s.\n" ANSI_COLOR_RESET, from_date, to_date);
        return 0;
    }
    print_workouts(workouts);
    return 0;
}