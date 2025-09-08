#ifndef FITLOG_TYPES_H
#define FITLOG_TYPES_H

#include <time.h>
#include <stdbool.h>

typedef struct Exercise Exercise;
typedef struct Config Config;

struct Exercise
{
    char name[64];
    int sets;
    int reps;
    float weight;
    char notes[128];
    char date[11];
};

struct Config
{
    char date_format[16];
    char time_format[16];
    char weight_unit[8];
    int decimal_places;
    int recent_count;
};

#endif