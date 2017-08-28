/* driver.c */

#include "search.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <unistd.h>

/* Globals */

char * PROGRAM_NAME = NULL;
char * PATH = NULL;

/* Functions */

void	    usage(const char *program_name, int status) {
    fprintf(stderr, "Usage: %s PATH [OPTIONS] [EXPRESSION]\n", program_name);
    fprintf(stderr, "\nOptions:\n");
    fprintf(stderr, "    -executable     File is executable or directory is searchable to user\n");
    fprintf(stderr, "    -readable       File readable to user\n");
    fprintf(stderr, "    -writable       File is writable to user\n\n");
    fprintf(stderr, "    -type [f|d]     File is of type f for regular file or d for directory\n\n");
    fprintf(stderr, "    -empty          File or directory is empty\n\n");
    fprintf(stderr, "    -name  pattern  Base of file name matches shell pattern\n");
    fprintf(stderr, "    -path  pattern  Path of file matches shell pattern\n\n");
    fprintf(stderr, "    -perm  mode     File's permission bits are exactly mode (octal)\n");
    fprintf(stderr, "    -newer file     File was modified more recently than file\n\n");
    fprintf(stderr, "    -uid   n        File's numeric user ID is n\n");
    fprintf(stderr, "    -gid   n        File's numeric group ID is n\n");
    fprintf(stderr, "\nExpressions:\n\n");
    fprintf(stderr, "    -print          Display file path (default)\n");
    fprintf(stderr, "    -exec cmd {} ;  Execute command on path\n");
    exit(status);
}

/* Main Execution */

int	    main(int argc, char *argv[]) {


    int argind = 2;
    PROGRAM_NAME = argv[0];

    if(argc > 1) {
        PATH = argv[1];
    }
    else {
        usage(PROGRAM_NAME, 1);
    }

    Settings settings = {
        .access = 0,
        .uid    = -1,
        .gid    = -1,
        .print = false,
        .exec_argc = -1,
    };

    bool execActive = false;

    while (argind < argc && strlen(argv[argind]) > 1 && argv[argind][0] == '-') {
        char *arg = argv[argind++];

        if (streq(arg, "-executable")) {
            settings.access |= X_OK;
        }
        else if (streq(arg, "-readable")) {
            settings.access |= R_OK;
        }
        else if (streq(arg, "-writable")) {
            settings.access |= W_OK;
        }
        else if (streq(arg, "-type")) {
            if(argind < argc) {
                arg = argv[argind++];
                if (streq(arg, "f")) {
                    settings.type |= S_IFREG;
                }
                else if (streq(arg, "d")) {
                    settings.type |= S_IFDIR;
                }
                else {
                    usage(PROGRAM_NAME, 1);
                }
            }
            else {
                usage(PROGRAM_NAME, 1);
            }
        }
        else if (streq(arg, "-empty")) {
            settings.empty = true;
        }
        else if (streq(arg, "-name")) {
            if(argind < argc) {
                arg = argv[argind++];
                settings.name = arg;
            }
            else {
                usage(PROGRAM_NAME, 1);
            }
        }
        else if (streq(arg, "-path")) {
            if(argind < argc) {
                arg = argv[argind++];
                settings.path = arg;
            }
            else {
                usage(PROGRAM_NAME, 1);
            }
        }
        else if (streq(arg, "-perm")) {
            if(argind < argc) {
                arg = argv[argind++];
                char * pEnd;
                settings.perm= strtol(arg, &pEnd, 8);
            }
            else {
                usage(PROGRAM_NAME, 1);
            }
        }
        else if (streq(arg, "-newer")) {
            if(argind < argc) {
                arg = argv[argind++];
                settings.newer = get_mtime(arg);
            }
            else {
                usage(PROGRAM_NAME, 1);
            }
        }
        else if (streq(arg, "-uid")) {
            if(argind < argc) {
                arg = argv[argind++];
                settings.uid = atoi(arg);
            }
            else {
                usage(PROGRAM_NAME, 1);
            }
        }
        else if (streq(arg, "-gid")) {
            if(argind < argc) {
                arg = argv[argind++];
                settings.gid = atoi(arg);
            }
            else {
                usage(PROGRAM_NAME, 1);
            }
        }
        else if (streq(arg, "-print")) {
            settings.print = true;
        }
        else if (streq(arg, "-exec")) {
            arg = argv[argind++];
            execActive = true;
            int count = 1;
            settings.exec_argv = &argv[argind - 1];
            while(!streq(arg, ";")) {
                arg = argv[argind++];
                count++;
            }
            settings.exec_argc = count - 1;
        }
        else if (streq(arg, "-help")) {
            usage(PROGRAM_NAME, 0);
        }
        else if (streq(arg, "-cowsay")) {
            system("echo \"What is a data structure?\" | cowsay");
        }
        else {
            usage(PROGRAM_NAME, 1);
        }
    }

    if(execActive == false) {
        settings.print = true;
    }

    if(filter(PATH, &settings)) {
        if(execute(PATH, &settings) != 0) {
            perror("main.c: execute");
        }
    }

    return search(PATH, &settings);
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
