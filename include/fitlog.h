#ifndef FITLOG_H
#define FITLOG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "fitlog/types.h"
#include "fitlog/constants.h"

// Core functions
int cmd_add(int argc, char *argv[]);
int cmd_show(int argc, char *argv[]);
int cmd_remove(int argc, char *argv[]);

#endif