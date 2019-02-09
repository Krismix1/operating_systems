#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>

#include "helpers.h"

const char HOME_DIRECTORY_ALIAS = '~';


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
      tokens[index++] = token;
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

  for (size_t i = 0; i < length; i++) {
    char *path = paths[i];
    char *buff = join_paths(path, cmd);
    if (access(buff, F_OK) != -1) {
      free(paths);
      // TODO: Free all over elements?
      return buff;
    }
  }
  free(paths);
  return NULL;
}

char* str_substr(const char* str, size_t start, size_t end) {
  size_t length = end - start;
  assert(length >= 0);
  assert(start >= 0);
  if (str == NULL) {
    return NULL;
  }
  assert(length <= strlen(str));
  char* result = malloc(length + 1); // 1 for the null terminator
  if (result) {
    result[length] = '\0';
    size_t pos = 0;
    for (size_t i = start; i <= end; i++) {
      result[pos++] = str[i];
    }
  }
  return result;
}

char* join_paths(char* path1, char* path2) {
  if (path1 == NULL || path2 == NULL) {
    return NULL;
  }

  size_t length1 = strlen(path1);
  size_t length2 = strlen(path2);
  bool hasSlash = true;
  // Allows to handle cases of pairs: ('~/', 'dir'), ('~', '/user')
  if (path1[length1 - 1] != '/' || path2[0] != '/') {
    length1++; // in order to allocate 1 more byte for '/'
    hasSlash = false;
  }

  char* result = malloc(length1 + length2 + 1);
  if (result) {
    strcat(result, path1);
    if (!hasSlash) {
      strcat(result, "/");
    }
    strcat(result, path2);
    result[length1 + length2] = '\0';
  }
  return result;
}

char* replace_home(char* cmd) {
  // TODO: Maybe make the function replace any occurence of HOME_DIRECTORY_ALIAS
  // unless escaped by '\'? rather than only if found as 1st character
  char* result = NULL;

  if (cmd != NULL && cmd[0] == HOME_DIRECTORY_ALIAS) {
    char *home = getenv("HOME");
    if (home == NULL) {
      printf("No environment var 'HOME' found\n");
      return NULL;
    }
    home = strdup(home);
    cmd = str_substr(cmd, 1, strlen(cmd));
    result = join_paths(home, cmd);
    free(home);
    free(cmd);
  }

  return result;
}
