#include "network.h"


int
network_macaddr_compare(const network_macaddr_t *mac1, const network_macaddr_t *mac2) {
  return macieee802_compare(mac1, mac2);
}

char *
network_macaddr_format(const network_macaddr_t *mac, char *string, size_t length) {
  return macieee802_format(mac, string, length);
}

int
network_macaddr_parse(network_macaddr_t *mac, const char *string) {
  return macieee802_parse(mac, string);
}


int
network_ipaddr4_compare(const network_ipaddr4_t *addr1, const network_ipaddr4_t *addr2) {
  return inetv4_compare(addr1, addr2);
}

int
network_ipaddr4_cidr(const char *cidr, uint8_t *prefix, network_ipaddr4_t *network, network_ipaddr4_t *netmask) {
  return inetv4_cidr(cidr, prefix, network, netmask);
}

bool
network_ipaddr4_ismulticast(const network_ipaddr4_t *addr) {
  return inetv4_ismulticast(addr);
}

bool
network_ipaddr4_isloopback(const network_ipaddr4_t *addr) {
  return inetv4_isloopback(addr);
}

bool
network_ipaddr4_isclassa(const network_ipaddr4_t *addr) {
  return inetv4_isclassa(addr);
}

bool
network_ipaddr4_isclassb(const network_ipaddr4_t *addr) {
  return inetv4_isclassb(addr);
}

bool
network_ipaddr4_isclassc(const network_ipaddr4_t *addr) {
  return inetv4_isclassc(addr);
}

char *
network_ipaddr4_format(const network_ipaddr4_t *addr, char *string, size_t length) {
  return inetv4_format(addr, string, length);
}

int
network_ipaddr4_parse(network_ipaddr4_t *addr, const char *string) {
  return inetv4_parse(addr, string);
}


int
network_ipaddr6_compare(const network_ipaddr6_t *addr1, const network_ipaddr6_t *addr2) {
  return inetv6_compare(addr1, addr2);
}

int
network_ipaddr6_cidr(const char *cidr, uint8_t *prefix, network_ipaddr6_t *network, network_ipaddr6_t *netmask) {
  return inetv6_cidr(cidr, prefix, network, netmask);
}

bool
network_ipaddr6_isunspecified(const network_ipaddr6_t *addr) {
  return inetv6_isunspecified(addr);
}

bool
network_ipaddr6_isloopback(const network_ipaddr6_t *addr) {
  return inetv6_isloopback(addr);
}

bool
network_ipaddr6_ismulticast(const network_ipaddr6_t *addr) {
  return inetv6_ismulticast(addr);
}

bool
network_ipaddr6_islinklocal(const network_ipaddr6_t *addr) {
  return inetv6_islinklocal(addr);
}

bool
network_ipaddr6_issitelocal(const network_ipaddr6_t *addr) {
  return inetv6_issitelocal(addr);
}

bool
network_ipaddr6_isglobal(const network_ipaddr6_t *addr) {
  return inetv6_isglobal(addr);
}

bool
network_ipaddr6_isv4mapped(const network_ipaddr6_t *addr) {
  return inetv6_isv4mapped(addr);
}

char *
network_ipaddr6_format(const network_ipaddr6_t *addr, char *string, size_t length) {
  return inetv6_format(addr, string, length);
}

int
network_ipaddr6_parse(network_ipaddr6_t *addr, const char *string) {
  return inetv6_parse(addr, string);
}


int
network_ipaddr(const char *string) {
  return inet_strisaddr(string);
}


char *
network_ipaddr_format(network_ipaddr_t *addr, char *string, size_t length) {
  switch (addr->size) {
    case NETWORK_IPADDR4_SIZE:
      network_ipaddr4_format(&addr->v4, string, length);
      break;

    case NETWORK_IPADDR6_SIZE:
      network_ipaddr6_format(&addr->v6, string, length);
      break;
  }

  return string;
}

int
network_ipaddr_parse(network_ipaddr_t *addr, const char *string) {
  switch (network_ipaddr(string)) {
    case NETWORK_IPADDR4:
      network_ipaddr4_parse(&addr->v4, string);
      return NETWORK_IPADDR4;

    case NETWORK_IPADDR6:
      network_ipaddr6_parse(&addr->v6, string);
      return NETWORK_IPADDR6;
  }

  return -1;
}

uint16_t
network_u16tohost(uint16_t number) {
  return network_u16tonet(number);
}

uint16_t
network_u16tonet(uint16_t number) {
#if __BYTE_ORDER == __LITTLE_ENDIAN || defined __LITTLE_ENDIAN || defined __LITTLE_ENDIAN_BITFIELD
  return (number << 8)|(number >> 8);
#else // #elif __BYTE_ORDER == __BIG_ENDIAN || defined __BIG_ENDIAN || defined __BIG_ENDIAN_BITFIELD
  return number;
#endif
}

uint32_t
network_u32tohost(uint32_t number) {
  return network_u32tonet(number);
}

uint32_t
network_u32tonet(uint32_t number) {
#if __BYTE_ORDER == __LITTLE_ENDIAN || defined __LITTLE_ENDIAN || defined __LITTLE_ENDIAN_BITFIELD
  return ((number & 0x000000FFU) << 24)|((number & 0x0000FF00U) << 8)|((number & 0x00FF0000U) >> 8)|((number & 0xFF000000U) >> 24);
#else // #elif __BYTE_ORDER == __BIG_ENDIAN || defined __BIG_ENDIAN || defined __BIG_ENDIAN_BITFIELD
  return number;
#endif
}
