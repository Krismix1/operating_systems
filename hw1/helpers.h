#pragma once

extern const char* HOME_DIRECTORY_ALIAS;

char* lookup_path(char* cmd);
char **str_split(char* str, const char delim, int *size);
