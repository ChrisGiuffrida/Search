/* expression.c */

#include "search.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <dirent.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * Executes the -print or -exec expressions (or both) on the specified path.
 * @param   path        Path to a file or directory
 * @param   settings    Settings structure.
 * @return  Whether or not the execution was successful.
 */
int	    execute(const char *path, const Settings *settings) {

    // Print the file/directory path
    if(settings->print == true) {
        printf("%s\n", path);
    }

    if(settings->exec_argc != -1) {
        pid_t pid = fork();

        if(pid < 0) {
                fprintf(stderr, "Unable to fork: %s\n", strerror(errno));
                return EXIT_FAILURE;
            }

        if(pid == 0) {
            char * exec_argv_local[settings->exec_argc + 1];
            for(int i = 0; i < settings->exec_argc; i++) {
                if(streq(settings->exec_argv[i], "{}")) {
                    exec_argv_local[i] = (char *)path;
                }
                else {
                    exec_argv_local[i] = settings->exec_argv[i];
                }
            }
            exec_argv_local[settings->exec_argc] = NULL;
            printf("test");
            if(execvp(exec_argv_local[0], exec_argv_local)) {
                fprintf(stderr, "Unable to exec: %s\n", strerror(errno));
                return EXIT_FAILURE;
            }
            _exit(0);
        }
        else {
            int status;
            while ((pid = wait(&status)) < 0);
        }
    }

    return 0;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
