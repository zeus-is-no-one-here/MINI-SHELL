#ifndef SHEEL_H
#define SHEEL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
// Color definitions
#define RESET   "\x1b[0m"
#define BLACK   "\x1b[30m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define WHITE   "\x1b[37m"
#define BUFFERSIZE 1024
#define SHEEL_0 0
// Print macro
#define p(...) printf(__VA_ARGS__)
struct s_bullitin
{
    const char *name;
    int (*func)(char **p);
}s_bullitin;

// Optional function declarations
// void print_prompt();
// void print_error(const char *msg);
#endif /* SHEEL_H */
