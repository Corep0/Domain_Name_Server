#include "dns.h"

static void free_tab(char **tab)
{
    for (size_t i = 0; tab[i]; i++)
        free(tab[i]);
    free(tab);
}
/*
    Encode function -> www.google.com => 3www6google3com
                                      => \x03www\x06google\x03com\x00
*/

int encoding(char* s, uint8_t* buf){
    int pos = 0;
    char **tab = ft_split(s, '.');
    if (!tab)
        return -1;
    for (size_t j = 0; tab[j]; j++){
        // printf("tab[%zu] = %s\n", j, tab[j]);
        buf[pos] = (uint8_t)strlen(tab[j]);
        pos++;
        for (size_t k = 0; tab[j][k]; k++){
            buf[pos] = (uint8_t)tab[j][k];
            pos++;
        }
    }
    buf[pos] = '\x00';
    pos++;
    free_tab(tab);
    // printf("Result of encoding() \n");
    // for (size_t i = 0; buf[i]; i++){
    //     printf("%c ", (unsigned char)buf[i]);
    // }
    // printf("\n");
    return pos;
}
/* 
    Parse the input from the server. For now it's skipping most of the identification, and check up
    Later it will need to do not skip the header (+12bytes) and some other:
        - Compare question part (sent <--> received)
        - 
*/
void    parse_response(uint8_t* response){
    struct dns_header*  res;
    res = (struct dns_header*)response;
    uint16_t            acc = ntohs(res->ancount); //???
    int                 pos = 12;

    if (ntohs(res->id) != ID){
        printf("ID mismatch\n");
        return;
    }
    while (response[pos] != '\x00')
        pos++;                          // Skip the question part (similar to the one sent before) 
    pos++;                              // Skip the last char of the question \x00
    pos += 2;                           // Skip the QTYPE
    pos += 2;                           // Skip the QCLASS
    for (int i = 0; i < acc; i++){
        if ((response[pos] & 0xC0) == 0xC0)
            pos += 2;                   // Skip NAME / ID (always 2 bytes)
        else
            while (response[pos] != 0x00)
                pos++;
        uint16_t type = ntohs(*(uint16_t*) (response + pos));
        pos += 2;                       // Passed the type len
        pos += 6;                       // Skip CLASSES and TTL (2 and 4)
        uint16_t rdlen = ntohs(*(uint16_t*) (response + pos));
        pos += 2;                       // Passed the rdlen
        if (type == 01 && rdlen == 04){ // Rdlen == 4 -> IPV4 format such as : 8.8.8.8 -> Google DNS's
            uint8_t* ip = (response + pos);
            printf("%d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
        }
        pos += rdlen;
    }
}