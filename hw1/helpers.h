#pragma once

#ifndef HELPERS
#define HELPERS

const char* HOME_DIRECTORY_ALIAS = "~";

int lookup_path(char* cmd);
char** str_split(char* str, const char delim);

#endif
