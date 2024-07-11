#include "../include/my_zsh.h"

char *read_input(void){
    char *buffer = NULL;
    buffer = malloc(sizeof(char) * MAX_STR_LEN);
    my_memset(buffer, '\0', MAX_STR_LEN);

    int read_input = STDIN_FILENO; //= 0

    int last = read(read_input, buffer, MAX_STR_LEN);
    buffer[last - 1] = '\0';

    return buffer;
}
