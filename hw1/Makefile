SRCS=shell.c tokenizer.c helpers.c application_runner.c
EXECUTABLES=shell

CC=gcc
CFLAGS=-g -Wall -std=gnu99 -O0
LDFLAGS=

OBJS=$(SRCS:.c=.o)

all: $(EXECUTABLES)

$(EXECUTABLES): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(EXECUTABLES) $(OBJS)
