#ifndef FITLOG_TYPES_H
#define FITLOG_TYPES_H

#include <time.h>
#include <stdbool.h>

typedef struct Exercise Exercise;
typedef struct Config Config;
typedef enum WeightUnit WeightUnit;
typedef enum TimeFormat TimeFormat;
typedef enum DateFormat DateFormat;
typedef enum ExerciseType ExerciseType;
typedef enum TimeUnit TimeUnit;

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

enum WeightUnit
{
    UNIT_KG,
    UNIT_LBS
};

enum TimeFormat
{
    TIME_24H,
    TIME_12H
};

enum TimeUnit
{
    TIME_S,
    TIME_MIN,
    TIME_HR
};

enum DateFormat
{
    DATE_YYYY_MM_DD,
    DATE_DD_MM_YYYY,
    DATE_MM_DD_YYYY
};

enum ExerciseType
{
    TYPE_SETS,
    TYPE_TIME,
    TYPE_BODY,
};

#endif