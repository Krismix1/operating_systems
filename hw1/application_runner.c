#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "application_runner.h"
#include "helpers.h"

static char** get_argv(uint16_t argc, char** argv) {
  // The execv requires that the array of pointers is terminated by a null pointer
  char** result = malloc(argc + 1);
  if (result) {
    result[argc] = NULL;
    for (uint16_t i = 0; i < argc; i++) {
      result[i] = argv[i];
    }
  }
  return result;
}

int execute_cmd(uint16_t argc, char** argv) {
  pid_t pid = fork();
  if (pid == 0) {
    // TODO: Pre-handle cases where commands start with './' or let lookup_path deal with them?
    // So we don't search knowing we would fail?
    char *path = lookup_path(argv[0]);
    if (path) {
      // replace file name with file path
      argv[0] = path;
    }
    if (execv(argv[0], get_argv(argc, argv))) {
      perror("Command failed");
      exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
  } else {
    int status;
    waitpid(pid, &status, 0); // no option
    return 0;
  }
}
