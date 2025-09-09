#include "../include/fitlog.h"

int get_next_exercise_id(void)
{
    // Path to the ID counter file
    char id_path[256];
    sprintf(id_path, "%s/%s", FITLOG_DIR, ID_COUNTER_FILE);

    // Open the file for reading
    FILE *fp = fopen(id_path, "r");
    if (fp == NULL)
    {
        // If the file doesn't exist, start from 1
        return 1;
    }

    int id = 1;
    if (fscanf(fp, "%d", &id) != 1)
    {
        // If reading fails, assume starting from 1
        id = 1;
    }
    fclose(fp);
    return id;
}

void increment_exercise_id(void)
{
    // Path to the ID counter file
    char id_path[256];
    sprintf(id_path, "%s/%s", FITLOG_DIR, ID_COUNTER_FILE);

    int current_id = get_next_exercise_id();
    int next_id = current_id + 1;

    // Open the file for writing (this will create or overwrite)
    FILE *fp = fopen(id_path, "w");
    if (fp == NULL)
    {
        fprintf(stderr, "Error: Unable to open ID counter file for writing.\n");
        return;
    }

    fprintf(fp, "%d\n", next_id);
    fclose(fp);
}