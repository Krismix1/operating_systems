#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "helpers.h"

const char* HOME_DIRECTORY_ALIAS = "~";


char** str_split(char* str, const char delim, int *size) {
  if (str == NULL) {
    *size = 0;
    return NULL;
  }
  size_t count = 0; // we need to know how much memory to allocate
  for (size_t i = 0; i < strlen(str); i++) {
    if (str[i] == delim) {
      ++count;
    }
  }
  ++count; // add 1 because of the fence-post problem
  char **tokens = malloc(count * sizeof(char*));
  if (tokens) {
    size_t index = 0;
    char *token = strtok(str, &delim);
    while (token != NULL) {
      tokens[sizeof(char*) * index++] = token;
      token = strtok(NULL, &delim);
    }
    *size = count;
  }
  return tokens;
}

char* lookup_path(char* cmd) {
  if (cmd == NULL) {
    return NULL;
  }
  // we should not modify the string of PATH
  // because that would change the environment of the process
  char *paths_env = strdup(getenv("PATH"));
  if (paths_env == NULL) {
    return cmd;
  }

  int length = 0;
  char **paths = str_split(paths_env, ':', &length);
  free(paths_env);
  if (paths == NULL) {
    return NULL;
  }

  unsigned long size = strlen(cmd);
  for (size_t i = 0; i < length; i++) {
    char *path = paths[i * sizeof(char*)];
    char buff[strlen(path) + size + 1]; // for extra '/'
    strcpy(buff, path);
    strcat(buff, "/");
    strcat(buff, (const char*)cmd);
    if (access(buff, F_OK) != -1) {
      free(paths);
      return strdup(buff);
    }
  }
  free(paths);
  return NULL;
}

char* replace_home(char* cmd) {
  // char *home = getenv("HOME");

  char *pos;

  if(!(pos = strstr(cmd, HOME_DIRECTORY_ALIAS))) {
    return cmd;
  }

  return cmd;
}
