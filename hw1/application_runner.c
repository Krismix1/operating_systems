#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "application_runner.h"

int execute_cmd(uint16_t argc, char** argv) {
  pid_t pid = fork();
  if (pid == 0) {
    int value = execv(argv[0], argv);
    if (value == -1) {
      perror("Command failed");
    }
    exit(value);
  } else {
    int status;
    waitpid(pid, &status, 0); // no option
    return 0;
  }
}
