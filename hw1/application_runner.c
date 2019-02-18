#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h> // for open()
#include <fcntl.h> // for file descriptor flags

#include "application_runner.h"
#include "helpers.h"

#ifndef _IO_REDIRECT_SYMBOLS_
#define REDIRECT_OUTPUT ">"
#define REDIRECT_INPUT  "<"
#endif


static char** get_argv(uint16_t argc, char** argv) {
  // The execv requires that the array of pointers is terminated by a null pointer
  char** result = malloc(argc + 1);
  if (result) {
    result[argc] = NULL;
    for (uint16_t i = 0; i < argc; i++) {
      // TODO: Replace occurences of '~' in argv[i]
      result[i] = argv[i];
    }
  }
  return result;
}

static uint16_t restructure_after_io(uint16_t argc, char **argv, char*** out) {
  char** result = malloc(argc + 1);
  uint16_t count = 0;
  if (result) {
    for (uint16_t i = 0; i < argc - 1; i++) {
      // TODO: Replace occurences of '~' in argv[i]
      if (strcmp(argv[i], REDIRECT_OUTPUT) == 0 || strcmp(argv[i], REDIRECT_INPUT) == 0) {
        i++; // skip the next token, because that is the file descriptor
        continue;
      }
      count++;
      result[i] = argv[i];
    }
  }
  out = &result;
  return count;
}

static uint16_t array_find_item(uint16_t size, char **arr, char *item) {
  for (uint16_t i = 0; i < size; i++) {
    if (strcmp(arr[i], item) == 0) {
      return i;
    }
  }
  return -1;
}

int execute_cmd(uint16_t argc, char** argv) {
  pid_t pid = fork();
  if (pid == 0) {
    // Pre-handle cases where commands start with './'
    // So we don't search knowing we would fail
    if (strcmp(str_substr(argv[0], 0, 1), "./") != 0) {
      char *path = lookup_path(argv[0]);
      if (path) {
        // replace file name with file path
        argv[0] = path;
      }
    }
    // we should have at least 3 elements to be a valid I/O redirection syntax
    if (argc >= 3) {
      uint16_t index = -1;
      // the redirect token cannot be last element, it's invalid syntax
      if ((index = array_find_item(argc, argv, REDIRECT_OUTPUT)) != -1 && index < argc - 1) {
        // open() takes a third argument which is the set of permissions, i.e.
        // 0666 is an octal number, i.e. every one of the 6's corresponds to three permission bits
        // 6 = rw, 7 = rwx
        // https://stackoverflow.com/a/2245212
        int fd = open(argv[index + 1], O_CREAT|O_WRONLY, 0666);
        if (fd == -1) {
          printf("Failed to create file %s\n", argv[2]);
          printf("Error code: %d\n", errno);
          exit(errno);
        }
        dup2(fd, STDOUT_FILENO);
      } else if ((index = array_find_item(argc, argv, REDIRECT_INPUT)) != -1 && index < argc - 1) {
        // the redirect token cannot be last element, it's invalid syntax
        int fd = open(argv[index + 1], O_RDONLY);
        if (fd == -1) {
          printf("Failed to open file %s\n", argv[2]);
          printf("Error code: %d\n", errno);
          exit(errno);
        }
        dup2(fd, STDIN_FILENO);
      }
      argc = restructure_after_io(argc, argv, &argv);
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
