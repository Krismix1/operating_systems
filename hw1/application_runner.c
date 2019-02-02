#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "application_runner.h"
#include "helpers.h"

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
    int value = execv(argv[0], argv);
    if (value == -1) {
      perror("Command failed");
    }
    if (path) {
      free(path);
    }
    exit(value);
  } else {
    int status;
    waitpid(pid, &status, 0); // no option
    return 0;
  }
}
