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