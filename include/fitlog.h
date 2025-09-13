#ifndef FITLOG_H
#define FITLOG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "fitlog/types.h"
#include "fitlog/constants.h"
#include "fitlog/utils.h"
#include "fitlog/colors.h"

// Core functions
int cmd_init(int argc, char *argv[]);
int cmd_create(int argc, char *argv[]);
int cmd_add(int argc, char *argv[]);
int cmd_show(int argc, char *argv[]);
int cmd_remove(int argc, char *argv[]);
int cmd_config(int argc, char *argv[]);
int cmd_export(int argc, char *argv[]);
int cmd_import(int argc, char *argv[]);

// Utility functions
void print_usage(void);

#endif