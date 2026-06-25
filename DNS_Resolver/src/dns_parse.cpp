#include "dns.h"
# include <string.h>
// Parsing validiy of the returned header and answer section
void    parse_header();
void    parse_answer();

static char* encode_len(size_t n){
    char *s = calloc(sizeof(char) * (4) + 1);
    unsigned int b = n;
    if (!s)
        return NULL;
    s = '\0';
    char *hex = strdup("\\x");
    ft_strlcat(s,hex, strlen(hex));
    free(hex);
    if (n == 0){
        char *h = strdup("00");
        ft_strlcat(s, h, 2);
        free(h);
        return s;
    }
    else if (n >= 100)
        return NULL;
    else{

        if (n < 10)
            s += '0';
        char *d = ft_itoa(b);
        if (!d)
            return NULL;
        ft_strlcat(s, d, strlen(s) + 1);
    }
    if (!s)
        return NULL;
    return s;
}
// Encode the domain name as labels ex: google.com -> \0x6google\0x3com\0x0
// Bytes length, chars, bytes end (0x00) from every dot '.'
char*    encoding(char* s){
    char *tok, *dest;
    //  \x00 -> end
    dest = malloc(sizeof(char) * (strlen(s) + (count_char(s, '.') * 4)) + 1);
    if (!dest)
        return NULL;
    tok = strtok(s, ".");
    while (tok != NULL){
        strcat(dest, encode_len(strlen(tok)));
        strcat(dest, tok);
    }
//    dest += "\x00";
    dest += '\0';
    return dest;
}

// int main(void){
//     char *s = strdup("Salut.com");
//     char *s1 = strdup("salut");
//     char *s2 = strdup("test...");
//     char *s3 = strdup("...");
//     char *s4 = strdup("a.aa.aaa");
//     char *s5 = strdup("");

//     printf("S : %s => E : %s\n", s, encoding(s));
//     printf("S1 : %s => E : %s\n", s1, encoding(s1));
//     printf("S2 : %s => E : %s\n", s2, encoding(s2));
//     printf("S3 : %s => E : %s\n", s3, encoding(s3));
//     printf("S4 : %s => E : %s\n", s4, encoding(s4));
//     printf("S5 : %s => E : %s\n", s5, encoding(s5));

//     return 0;

// }
