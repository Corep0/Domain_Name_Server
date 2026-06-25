#include "dns.h"

/* 
    Creating a socket to make a connection 
    should be (UDP) -> Similar action as getaddrinfo(), used as reference
*/
int    build_socket(void){
    return (socket(AF_INET, SOCK_DGRAM, 0));
}
