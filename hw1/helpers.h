#pragma once

extern const char HOME_DIRECTORY_ALIAS;

char* lookup_path(char* cmd);
char **str_split(char* str, const char delim, int *size);

/** Returns a substring of the string, from 'start' till 'end', including both. */
char* str_substr(const char *str, size_t start, size_t end);

/** Replace HOME_DIRECTORY_ALIAS, if present as 1st character, with $HOME env var. */
char* replace_home(char *cmd);

/**
* Joints two paths, using '/'. The resulting string will not contain 2 consecutive
* occurences of '/' at the join point.
*/
char* join_paths(char *path1, char *path2);
