/* search.c */

#include "search.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

/**
 * Recursively search through the provided root directory
 * @param   root        Path to directory
 * @param   settings    Settings structure
 * @return  Whether or not the search was successful.
 */
int	    search(const char *root, const Settings *settings) {
    DIR *dirp = opendir(root);

    if(dirp == NULL) {
        fprintf(stderr, "search.c: opendir %s\n", strerror(errno));
        return 1;
    }

    for(struct dirent* entry = readdir(dirp); entry != 0; entry = readdir(dirp)) {
        if((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..")) == 0) {
            continue;
        }

        if((entry->d_type & DT_DIR) == DT_DIR) {
            char path[BUFSIZ];
            sprintf(path, "%s/%s", root, entry->d_name);
            search(path, settings);
        }
        char path[BUFSIZ];
        sprintf(path, "%s/%s", root, entry->d_name);
        if(filter(path, settings)) {
            if(execute(path, settings) != 0) {
                perror("search.c: execute");
            }
        }
    }

    closedir(dirp);
    return EXIT_SUCCESS;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
