#include "../include/my_zsh.h"

int my_strlen(char *string){
    int i=0;
    if(string == NULL) return -1;

    while(string[i] != '\0') i++;

    return i;
}

int my_strcmp(char *s1, char *s2){
    int i = 0;
    while((s1[i] != '\0') || (s2[i] != '\0')){
        if(s1[i] > s2[i]) return 1;
        if(s1[i] < s2[i]) return -1;
        i++;
    }
    return 0;
}

int my_strncmp(char *s1, char *s2, int len){

    int i = 0;
    while(i < len && ((s1[i] != '\0') || (s2[i] != '\0'))){
        if(s1[i] > s2[i]) return 1;
        if(s1[i] < s2[i]) return -1;
        i++;
    }

    if (i == len)
    {
        return 0;
    }else if (s1[i] != '\0')
    {
        return 1;
    }else{
        return -1;
    }
}

char *my_strncpy(char *dst, char *src, int n){
    int i;
    
    for(i=0; i<n && src[i] != '\0';i++)
        dst[i] = src[i];

    dst[i] = '\0';
    return dst;
}

char *my_strcpy(char *dst, char *src){       
    return my_strncpy(dst, src, my_strlen(src));
}

int is_digit(char c){
    if (c >= '0' && c <= '9') return 1;

    return 0;
}

int is_char(char c){
    if (c >= 'A' && c <= 'Z') return 1;

    return 0;
}

void write_string(char *str){
    write(0, str, my_strlen(str));
}


void write_char(int fd, char *str){
    int index = 0;

    while (str[index]){
        if (str[index] != '"'){
            write(fd, &str[index], 1);
        }
        index++;
    }
}

int is_exec(char *str){
    if(!str) return 0;

    if (str[0] == '.' && str[1] == '/') return 1;
    return 0;
}

char *my_strcat(char *src, char *dst){
    int i;
    int dst_len = my_strlen(dst);

    for (i = 0; src[i] != '\0'; i++){
        dst[dst_len] = src[i];
        dst_len += 1;
    }
    dst[dst_len] = '\0';
    return dst;
}

int my_atoi(char *str){
    int value = 0;
    char* temp = str;

    while (*temp != '\0')
    {
        value = value * 10 + (*temp - '0');
        temp++;
    }

    return value;
}


void rev_string(char *begin, char *end){
    char aux;

    while(end>begin)
        aux=*end, *end--=*begin, *begin++=aux;
}

char *my_itoa(int value, char *str, int base){
    static char chars[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    char *wstr = str;
    int sign;

    if((sign=value) < 0)
        value = -value;

    do{
        *wstr++ = chars[value % base];
    }while(value /= base);

    if(sign < 0) *wstr++ = '-';
    *wstr = '\0';
    rev_string(str, wstr-1);
    return wstr;
}

void *my_memset(void *string, int fill, int length){
    unsigned char *s = string;
    while(length--){
        *s++ = (unsigned char)fill;
    }
    return string;
}
