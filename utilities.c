/* utilities.c */

#include "search.h"

#include <errno.h>
#include <string.h>
#include <time.h>

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

/**
 * Test whether or not a directory is empty.
 * @param   path        Path to directory.
 * @return  Whether or not a directory is empty.
 */
bool        is_directory_empty(const char *path) {
    DIR *d = opendir(path);
    if (d == NULL) {    // Directory does not exist OR File is not a directory.
        perror("utilities.c");
        return false;
    }
    struct dirent *e;
    int numEntries = 0;
    while ((e = readdir(d)) != NULL) {
        numEntries++;
        if (numEntries > 2) { // There are more entries than '.' and '..'.
            return false;
        }
    }
    return true;
}

/**
 * Retrieve the modification time of the given file.
 * @param   path        Path to file of directory.
 * @return  The modification time of the given file.
 */
time_t      get_mtime(const char *path) {
    struct stat s;
    lstat(path, &s);
    if ((s.st_mode & F_OK) != F_OK) { // Directory/file does not exist.
        perror("utilities.c");
        return 0;
    }
    return s.st_mtim.tv_sec;

}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
