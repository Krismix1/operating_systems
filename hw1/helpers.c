#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "helpers.h"


// char** str_split(char* str, const char delim) {
//   return strtok(str, &delim);
// }

char** str_split(char* a_str, const char a_delim) {
  char** result    = 0;
  size_t count     = 0;
  char* tmp        = a_str;
  char* last_delim = 0;
  char delim[2];
  delim[0] = a_delim;
  delim[1] = 0;
  /* Count how many elements will be extracted. */
  while (*tmp) {
    if (a_delim == *tmp) {
      ++count;
      last_delim = tmp;
    }
    ++tmp;
  }
  /* Add space for trailing token. */
  count += last_delim < (a_str + strlen(a_str) - 1);
  /* Add space for terminating null string so caller
     knows where the list of returned strings ends. */
  ++count;

  result = malloc(sizeof(char*) * count);
  if (result) {
    size_t idx  = 0;
    char* token = strtok(a_str, delim);

    while (token) {
      assert(idx < count);
      *(result + idx++) = strdup(token);
      token = strtok(0, delim);
    }
    assert(idx == count - 1);
    *(result + idx) = 0;
  }

  return result;
}

int lookup_path(char* cmd) {
  char *paths_src = getenv("PATH");
  char paths_cp[strlen(paths_src)];
  strcpy(paths_cp, paths_src);
  char **paths = str_split(paths_cp, ':');

  for (size_t i = 0; i < 6; i++) {
    printf("%s\n", paths[i]);
  }
  return 1;
}

char* replace_home(char* cmd) {
  // char *home = getenv("HOME");

  char *pos;

  if(!(pos = strstr(cmd, HOME_DIRECTORY_ALIAS))) {
    return cmd;
  }

  return cmd;
}
