#include "inet.h"


int
inet_strisaddr(const char *string) {
  return ((!string || !*string) ? -1 : ((inetv4_parse(NULL, string) != -1) ? INETV4_ADDRESS : ((inetv6_parse(NULL, string) != -1) ? INETV6_ADDRESS : -1)));
}
