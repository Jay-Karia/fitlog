#include <../include/fitlog.h>

int cmd_import(int argc, char *argv[])
{
    // Check for help flag
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
        {
            print_import_help();
            return 0;
        }
    }

    return 0;
}