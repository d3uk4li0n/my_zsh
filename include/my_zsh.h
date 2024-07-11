#ifndef MY_ZSH_H
#define MY_ZSH_H

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_STR_LEN 1024
#define ENVV 'e'
#define VAR 'v'

typedef enum{
    SUCCESS, // 0
    QUIT, // 1
    ERROR, // 2
}status_t;

typedef enum{
    CD,
    ECHO,
    SETENV,
    UNSETENV,
    ENV,
    EXIT,
    PWD,
    WHICH,
    RUN,
    EXECUTE,
    UNKNOWN,
}builtins;

typedef struct arguments_s{
    int size;
    char **args;
    char **env;
    int env_length;
    int paths_size;
    char **paths;
    builtins command;
}arguments;

#endif

#ifndef STRING_UTILS_C
#define STRING_UTILS_C

int my_strlen(char *);
int my_strcmp(char *, char *);
int my_strncmp(char *, char *, int);
char *my_strcat(char *, char *);
int my_atoi(char *);
void rev_string(char *, char *);
int is_digit(char);
int is_char(char);
void write_string(char *);
void write_char(int, char *);
int is_exec(char *);
char *my_itoa(int, char *, int);
void *my_memset(void *, int, int);
char *my_strncpy(char *, char *, int n);
char *my_strcpy(char *, char *);

#endif

#ifndef READ_INPUT_C
#define READ_INPUT_C

char *read_input(void);

#endif
