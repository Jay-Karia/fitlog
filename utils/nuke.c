#include <../include/fitlog.h>
#include <stdio.h>

int remove_directory(const char *path)
{
    char command[512];
    snprintf(command, sizeof(command), "rmdir /s /q \"%s\"", path);
    return system(command);
}