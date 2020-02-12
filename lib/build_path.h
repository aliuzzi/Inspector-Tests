#ifndef _BUILD_PATH_H_
#define _BUILD_PATH_H_

char *build_path(char *proc_dir, char *suffix)
{
    size_t len = strlen(proc_dir) + strlen(suffix);
    char *str = malloc(sizeof(char) * len + 1);
    strcpy(str, proc_dir);
    strcat(str, suffix);
    return str;
}

#endif
