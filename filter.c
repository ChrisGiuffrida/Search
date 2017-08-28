/* filter.c */

#include "search.h"

#include <errno.h>
#include <fcntl.h>
#include <string.h>

#include <dirent.h>
#include <fnmatch.h>
#include <sys/stat.h>
#include <unistd.h>

#include <libgen.h>

/**
 * Test whether or not the given path should be filtered out of the output.
 * @param   path        Path to file or directory.
 * @param   settings    Settings structure.
 * @return  Whether or not the path should be excluded from the output (true to
 * exclude, false to include).
 */
bool        filter(const char *path, const Settings *settings) {

    struct stat s;
    lstat(path, &s);


    // Executable Check
    if ((settings->access & X_OK) == X_OK) {
        if (access(path, X_OK) != 0) {
            return false;
        }
    }

    // Readable Check
    if ((settings->access & R_OK) == R_OK) {
        if (access(path, R_OK) != 0) {
            return false;
        }
    }

    // Writeable Check
    if ((settings->access & W_OK) == W_OK) {
        if (access(path, W_OK) != 0) {
            return false;
        }
    }

    // Type Check
        // is File
    if ((settings->type & S_IFMT) == S_IFREG) {
        if ((s.st_mode & S_IFMT) != S_IFREG) {
            return false;
        }
    }
        // is Directory
    if ((settings->type & S_IFMT) == S_IFDIR) {
        if ((s.st_mode & S_IFMT) != S_IFDIR) {
            return false;
        }
    }

    // Empty Check
    if (settings->empty == true) {
        if ((s.st_mode & S_IFDIR) == S_IFDIR) {
            if (is_directory_empty(path) == false) {
                return false;
            }
        }
        else {
            if (s.st_size != 0) {
                return false;
            }
        }
    }

    // Name Check
    if (settings->name != NULL) {
        char base[BUFSIZ];
        strcpy(base, path);
        if (fnmatch(settings->name, basename((char*) base), 0) != 0) {
            return false;
        }
    }

    // Path Check
    if (settings->path != NULL) {
        if (fnmatch(settings->path, path, 0) != 0) {
            return false;
        }
    }

    // Perm Check
    if (settings->perm != 0) {
        if (settings->perm != (s.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO))) {
            return false;
        }
    }

    // Newer Check
    if (settings->newer != 0) {
        if (get_mtime(path) <= settings->newer) {
            return false;
        }
    }

    // User ID Check
    if (settings->uid != -1) {
        if (settings->uid != s.st_uid) {
            return false;
        }
    }

    // Group ID Check
    if (settings->gid != -1) {
        if (settings->gid != s.st_gid) {
            return false;
        }
    }

    return true;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
