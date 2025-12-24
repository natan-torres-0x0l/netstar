#ifndef _NETSTAR_H
#define _NETSTAR_H

#ifdef __cplusplus
extern "C" {
#endif

#define NETSTAR_VERSION "1.0.0"

#include <network/netpacket/netpacket.h>
#include <network/netlink/netlink.h>

#include <network/network.h>

#include <netstar-terminate.h>
#include <netstar-threads.h>

#include <netstar-format.h>
#include <netstar-log.h>

#if defined __cplusplus && __cplusplus < 201103L
  #define thread_local __thread
#endif

#ifndef __cplusplus
  #define thread_local _Thread_local
#endif

#ifdef __cplusplus
  #define restrict __restrict
#endif

typedef struct {
  network_ipaddr4_t begin, end;
} netstar_network_ipaddr4_range_t;

typedef struct {
  network_ipaddr6_t begin, end;
} netstar_network_ipaddr6_range_t;

struct netstar_linkwave_mutex {
  netstar_thread_mutex_t send, recv;
};

typedef struct netstar_linkwave {
  struct netstar_linkwave_mutex mutex;

  struct netlink_interface *iface;
  netpacket_live_t *live;
} netstar_linkwave_t;


extern ssize_t netstar_recv(netstar_linkwave_t *, void *, size_t, struct netpacket_timeout *);
extern ssize_t netstar_send(netstar_linkwave_t *, void *, size_t);

static inline void
netstar_linkwave_free(netstar_linkwave_t *linkwave) {
  if (linkwave) {
    netlink_interface_free(linkwave->iface);
    linkwave->iface = NULL;
 
    netpacket_live_close(linkwave->live);
    linkwave->live = NULL;
  }
}

typedef struct netstar {
#if defined __linux__ || defined __linux
  netstar_linkwave_t loopback4;
  netstar_linkwave_t loopback6;
#endif
// @ loopback interface
  netstar_linkwave_t loopback;

// @ managed interface
  netstar_linkwave_t managed;

// @ interface forward mode
// forward mode ensures that all intercepted traffic is forwarded to its intended destinations.
// if disabled, captured remote traffic might be dropped, potentially leading to network congestion.
  bool forward;

// netstar_threads_t threads;

  bool interrupted;
} netstar_t;


extern void netstar_managed_interface_set(netstar_t *, const char *);
extern void netstar_loopback_interface_set(netstar_t *, const char *);

extern void netstar_forward_set(netstar_t *netstar, bool);

extern netstar_t *netstar_context(void);
extern void netstar_close(netstar_t *);


struct netstar_ethernet_type {
  uint16_t u16;
  const char *name;
};


static struct netstar_ethernet_type netstar_ethernet_types[] = {
  { 0x0200, "XEROX PUP"                      },
  { 0x0201, "PUP Addr. Trans."               },
  { 0x0600, "XEROX NS IDP"                   },
  { 0x0800, "IP Version 4"                   },
  { 0x0801, "X.75 Internet"                  },
  { 0x0802, "NBS Internet"                   },
  { 0x0803, "ECMA Internet"                  },
  { 0x0804, "Chaosnet"                       },
  { 0x0805, "X.25 Level 3"                   },
  { 0x0806, "ARP"                            },
  { 0x0807, "XNS Compatability"              },
  { 0x081C, "Symbolics Private"              },
  { 0x1000, "Berkeley Trailer"               },
  { 0x1600, "Valid"                          },
  { 0x5208, "BBN Simnet"                     },
  { 0x6001, "DEC MOP Dump/Load"              },
  { 0x6002, "DEC MOP Remote Console"         },
  { 0x6003, "DEC DECNET Phase IV"            },
  { 0x6004, "DEC LAT"                        },
  { 0x6005, "DEC"                            },
  { 0x6006, "DEC"                            },
  { 0x8003, "Cronus VLN"                     },
  { 0x8004, "Cronus Direct"                  },
  { 0x8005, "HP Probe"                       },
  { 0x8006, "Nestar"                         },
  { 0x8010, "Excelan"                        },
  { 0x8035, "Reverse ARP"                    },
  { 0x8038, "DEC LANBridge"                  },
  { 0x805B, "Stanford V Kernel experimental" },
  { 0x805C, "Stanford V Kernel production"   },
  { 0x807C, "Merit Internodal"               },
  { 0x809B, "Appletalk"                      },
  { 0x86DD, "IP version 6"                   },
  { 0x9000, "Loopback"                       },
  { 0x0000, NULL                             }
};

static inline struct netstar_ethernet_type *
netstar_ethernet_type(uint16_t type) {
  struct netstar_ethernet_type *types = NULL;

  for (types = netstar_ethernet_types; types->name; types++)
    if (types->u16 == type && types->name)
      return types;

  return NULL;
}

typedef enum {
// NETSTAR_ETHERNET_PROTOCOL_IP   = 0x0800, NETSTAR_ETHERNET_PROTOCOL_IPV4 = NETSTAR_ETHERNET_PROTOCOL_IP,
  NETSTAR_ETHERNET_PROTOCOL_IPV4 = 0x0800,
  NETSTAR_ETHERNET_PROTOCOL_IPV6 = 0x86DD,
  NETSTAR_ETHERNET_PROTOCOL_ARP  = 0x0806,
} netstar_ethernet_protocol_t;

struct netstar_ethernethdr {
// network_macaddr_t dhardware;
  uint8_t      dhardware[NETWORK_MACADDR_SIZE];
// network_macaddr_t shardware;
  uint8_t      shardware[NETWORK_MACADDR_SIZE];
  uint16_t     type;
};

#define NETSTAR_ETHERNET_SIZE sizeof(struct netstar_ethernethdr)


extern ssize_t netstar_sendethernet(netstar_linkwave_t *, const network_macaddr_t *, const network_macaddr_t *, uint16_t, uint8_t *, uint16_t);


typedef enum {
  NETSTAR_ARP_HARDWARE_ETHERNET = 0x0001,
} netstar_arp_hardware_t;

typedef enum {
// NETSTAR_ARP_PROTOCOL_IP = 0x0800, NETSTAR_ARP_PROTOCOL_IPV4 = NETSTAR_ARP_PROTOCOL_IP,
  NETSTAR_ARP_PROTOCOL_IPV4 = 0x0800,
} netstar_arp_protocol_t;

typedef enum {
  NETSTAR_ARP_OPCODE_REQUEST = 0x0001,
  NETSTAR_ARP_OPCODE_REPLY   = 0x002,
} netstar_arp_opcode_t;

struct netstar_arphdr {
  uint16_t     hardware,
               protocol;
  uint8_t      hardware_size,
               protocol_size;

  uint16_t     opcode;

// network_macaddr_t shardware;
  uint8_t      shardware[NETWORK_MACADDR_SIZE];
// network_ipaddr4_t     saddr;
  uint8_t      saddr[NETWORK_IPADDR4_SIZE];
// network_macaddr_t dhardware;
  uint8_t      dhardware[NETWORK_MACADDR_SIZE];
// network_ipaddr4_t     daddr;
  uint8_t      daddr[NETWORK_IPADDR4_SIZE];
};

#define NETSTAR_ARP_SIZE sizeof(struct netstar_arphdr)


extern ssize_t netstar_sendarp(netstar_linkwave_t *, const network_macaddr_t *, const network_macaddr_t *, uint16_t, const network_macaddr_t *, const network_ipaddr4_t *, const network_macaddr_t *, const network_ipaddr4_t *);


typedef enum {
// NETSTAR_IP_PROTOCOL_IP     = 0x0000,
// NETSTAR_IP_PROTOCOL_IPV4   = NETSTAR_IP_PROTOCOL_IP,
  NETSTAR_IP_PROTOCOL_IPV4   = 0x0000,
  NETSTAR_IP_PROTOCOL_IPV6   = 0x0029,
  NETSTAR_IP_PROTOCOL_GRE    = 0x002F,
  NETSTAR_IP_PROTOCOL_TCP    = 0x0006,
  NETSTAR_IP_PROTOCOL_UDP    = 0x0011,
// NETSTAR_IP_PROTOCOL_ICMP   = 0x0001,
// NETSTAR_IP_PROTOCOL_ICMPV4 = NETSTAR_IP_PROTOCOL_ICMP,
  NETSTAR_IP_PROTOCOL_ICMPV4 = 0x0001,
  NETSTAR_IP_PROTOCOL_ICMPV6 = 0x003A,
  NETSTAR_IP_PROTOCOL_IGMP   = 0x0002,
} netstar_ip_protocol_t;

struct netstar_ip_protocol {
  uint8_t u8;
  const char *name;
};


static struct netstar_ip_protocol netstar_ip_protocols[] = {
  { 0x0001, "ICMPV4"      }, // Internet Control Message Version 4
  { 0x0002, "IGMP"        }, // Internet Group Management
  { 0x0003, "GGP"         }, // Gateway-to-Gateway
// { 0x0004, "*"           }, // Unassigned
  { 0x0005, "ST"          }, // Stream
  { 0x0006, "TCP"         }, // Transmission Control
  { 0x0007, "UCL"         }, // UCL
  { 0x0008, "EGP"         }, // Exterior Gateway Protocol
  { 0x0009, "IGP"         }, // Any private interior gateway
  { 0x000A, "BBN-RCC-MON" }, // BBN RCC Monitoring
  { 0x000B, "NVP-II"      }, // Network Voice Protocol
  { 0x000C, "PUP"         }, // PUP
  { 0x000D, "ARGUS"       }, // ARGUS
  { 0x000E, "EMCON"       }, // EMCON
  { 0x000F, "XNET"        }, // Cross Net Debugger
  { 0x0010, "CHAOS"       }, // Chaos
  { 0x0011, "UDP"         }, // User Datagram
  { 0x0012, "MUX"         }, // Multiplexing
  { 0x0013, "DCN-MEAS"    }, // DCN Measurement Subsystems
  { 0x0014, "HMP"         }, // Host Monitoring
  { 0x0015, "PRM"         }, // Packet Radio Measurement
  { 0x0016, "XNS-IDP"     }, // XEROX NS IDP
  { 0x0017, "TRUNK-1"     }, // Trunk-1
  { 0x0018, "TRUNK-2"     }, // Trunk-2
  { 0x0019, "LEAF-1"      }, // Leaf-1
  { 0x001A, "LEAF-2"      }, // Leaf-2
  { 0x001B, "RDP"         }, // Reliable Data Protocol
  { 0x001C, "IRTP"        }, // Internet Reliable Transaction
  { 0x001D, "ISO-TP4"     }, // ISO Transport Protocol Class 4
  { 0x001E, "NETBLT"      }, // Bulk Data Transfer Protocol
  { 0x001F, "MFE-NSP"     }, // MFE Network Services Protocol
  { 0x0020, "MERIT-INP"   }, // MERIT Internodal Protocol
  { 0x0021, "SEP"         }, // Sequential Exchange Protocol
// { 0x0022-0x003C 34-60, "*" }, // Unassigned
  { 0x002F, "GRE"         }, // Generic Routing Encapsulation
  { 0x003A, "ICMPV6"      }, // Internet Control Message Version 6
// { 0x003D, "*"           }, // any host internal protocol
  { 0x003E, "CFTP"        }, // CFTP
// { 0x003F, "*"           }, // any local network
  { 0x0040, "SAT-EXPAK"   }, // SATNET and Backroom EXPAK
  { 0x0041, "MIT-SUBNET"  }, // MIT Subnet Support
  { 0x0042, "RVD"         }, // MIT Remote Virtual Disk Protocol
  { 0x0043, "IPPC"        }, // Internet Pluribus Packet Core
// { 0x0044, "*"           }, any distributed file system
  { 0x0045, "SAT-MON"     },  // SATNET Monitoring
// { 0x0046, "*"           }, // Unassigned
  { 0x0047, "IPCV"        }, // Internet Packet Core Utility
// { 0x0048-0x004B 72-75, "*" }, // Unassigned
  { 0x004C, "BR-SAT-MON"  }, // Backroom SATNET Monitoring
  { 0x004D, "*"           }, // Unassigned
  { 0x004E, "WB-MON"      }, // WIDEBAND Monitoring
  { 0x004F, "WB-EXPAK"    }, // WIDEBAND EXPAK
// { 0x0050-0x00FE 80-254, "*" }, // Unassigned
// { 0x00FF, "*"           }, // Reserved
  { 0x0000, NULL          }
};

static inline struct netstar_ip_protocol *
netstar_ip_protocol(uint8_t protocol) {
  struct netstar_ip_protocol *protocols = NULL;

  for (protocols = netstar_ip_protocols; protocols->name; protocols++)
    if (protocols->u8 == protocol)
      return protocols;

  return NULL;
}

struct netstar_iphdr {
#if __BYTE_ORDER == __LITTLE_ENDIAN || defined __LITTLE_ENDIAN || defined __LITTLE_ENDIAN_BITFIELD
  uint8_t  ihl:4,
           version:4;
#elif __BYTE_ORDER == __BIG_ENDIAN || defined __BIG_ENDIAN || defined __BIG_ENDIAN_BITFIELD
  uint8_t  version:4,
           ihl:4;
#else
  #error "Adjust your <asm/byteorder.h> defines"

#endif
  uint8_t  typeof_service;
  uint16_t length,
           id,
           fragment_offset;
  uint8_t  ttl,
           protocol;
  uint16_t checksum;
// network_ipaddr4_t saddr;
  uint8_t  saddr[NETWORK_IPADDR4_SIZE];
// network_ipaddr4_t daddr;
  uint8_t  daddr[NETWORK_IPADDR4_SIZE];
};

#define NETSTAR_IP_SIZE sizeof(struct netstar_iphdr)


struct netstar_ipv6hdr {
#if __BYTE_ORDER == __LITTLE_ENDIAN || defined __LITTLE_ENDIAN || defined __LITTLE_ENDIAN_BITFIELD
  uint8_t  priority:4,
           version:4;
#elif __BYTE_ORDER == __BIG_ENDIAN || defined __BIG_ENDIAN || defined __BIG_ENDIAN_BITFIELD
  uint8_t  version:4,
           priority:4;
#else
  #error "Adjust your <asm/byteorder.h> defines"

#endif
  uint8_t  flow_label[3];
  uint16_t length;
  uint8_t  nexthdr,
           hop_limit;
// network_ipaddr6_t saddr;
  uint8_t  saddr[NETWORK_IPADDR6_SIZE];
// network_ipaddr6_t daddr;
  uint8_t  daddr[NETWORK_IPADDR6_SIZE];
};

#define NETSTAR_IPV6_SIZE sizeof(struct netstar_ipv6hdr)


struct netstar_grehdr {
#if __BYTE_ORDER == __LITTLE_ENDIAN || defined __LITTLE_ENDIAN || defined __LITTLE_ENDIAN_BITFIELD
  uint16_t version:3,
           flags:13;
#elif __BYTE_ORDER == __BIG_ENDIAN || defined __BIG_ENDIAN || defined __BIG_ENDIAN_BITFIELD
  uint16_t flags:13,
           version:3;
#else
  #error "Adjust your <asm/byteorder.h> defines"

#endif
  uint16_t protocol;
};

#define NETSTAR_GRE_FLAGS_CHECKSUM 0x8000  /* C */
#define NETSTAR_GRE_FLAGS_ROUTING  0x4000  /* R */
#define NETSTAR_GRE_FLAGS_KEY      0x2000  /* K */
#define NETSTAR_GRE_FLAGS_SEQUENCE 0x1000  /* S */
#define NETSTAR_GRE_FLAGS_STRICT   0x0800  /* s */


typedef enum {
  NETSTAR_IGMP_TYPE_MEMBERSHIP_QUERY     = 0x11,
  NETSTAR_IGMP_TYPE_MEMBERSHIP_REPORT_V1 = 0x12,
  NETSTAR_IGMP_TYPE_MEMBERSHIP_REPORT_V2 = 0x16,
  NETSTAR_IGMP_TYPE_LEAVE_GROUP          = 0x17,
  NETSTAR_IGMP_TYPE_MEMBERSHIP_REPORT_V3 = 0x22,
} netstar_igmp_type_t;

struct netstar_igmphdr {
  uint8_t  type,
           code;
  uint16_t checksum;
  uint8_t  group[NETWORK_IPADDR4_SIZE];
};

struct netstar_igmpv3hdr {
  uint8_t  type,
           response_time;
  uint16_t checksum;
  uint16_t groups_count;
};


typedef enum {
  NETSTAR_ICMP_TYPE_ECHO_REPLY              = 0x00,
  NETSTAR_ICMP_TYPE_ECHO                    = 0x08,
  NETSTAR_ICMP_TYPE_DESTINATION_UNREACHABLE = 0x03,
  NETSTAR_ICMP_TYPE_TIME_EXCEEDED           = 0x0B,
  NETSTAR_ICMP_TYPE_PARAMETER_PROBLEM       = 0x0C,
  NETSTAR_ICMP_TYPE_REDIRECT                = 0x05,
  NETSTAR_ICMP_TYPE_ROUTER_ADVERT           = 0x09,
  NETSTAR_ICMP_TYPE_ROUTER_SOLICIT          = 0x0A,
  NETSTAR_ICMP_TYPE_TIMESTAMP_REQUEST       = 0x01,
  NETSTAR_ICMP_TYPE_TIMESTAMP_REPLY         = 0x02,
  NETSTAR_ICMP_TYPE_UNFORMATION_REQUEST     = 0x04,
  NETSTAR_ICMP_TYPE_UNFORMATION_REPLY       = 0x06,
  NETSTAR_ICMP_TYPE_NETMASK_REQUEST         = 0x11,
  NETSTAR_ICMP_TYPE_NETMASK_REPLY           = 0x12,
} netstar_icmp_type_t;

typedef enum {
  NETSTAR_ICMP_CODE_REDIRECT_NETWORK = 0x0000,
  NETSTAR_ICMP_CODE_REDIRECT_HOST    = 0x0001,
  NETSTAR_ICMP_CODE_REDIRECT_NETTOS  = 0x0002,
  NETSTAR_ICMP_CODE_REDIRECT_HOSTTOS = 0x0003,
} netstar_icmp_code_t;

struct netstar_icmphdr {
  uint8_t  type,
           code;
  uint16_t checksum;

/*
  union {
    struct {
      uint16_t id,
               sequence;
    }; // echo

 // network_ipaddr4_t gateway;
    uint8_t gateway[NETWORK_IPADDR4_SIZE];

    struct {
      uint16_t unused,
               mtu;
    }; // fragmentation needed
  };
*/
};

#define NETSTAR_ICMP_SIZE sizeof(struct netstar_icmphdr)

struct netstar_icmp_echo {
  uint16_t id,
           sequence;
};

#define NETSTAR_ICMP_ECHO_SIZE sizeof(struct netstar_icmp_echo)


extern ssize_t netstar_sendicmpecho(netstar_linkwave_t *, const network_macaddr_t *, const network_macaddr_t *, uint8_t, const network_ipaddr4_t *, const network_ipaddr4_t *);


struct netstar_icmp_redirect {
  uint8_t gateway[NETWORK_IPADDR4_SIZE];
};

#define NETSTAR_ICMP_REDIRECT_SIZE sizeof(struct netstar_icmp_redirect)


extern ssize_t netstar_sendicmpredirect(netstar_linkwave_t *, const network_macaddr_t *, const network_macaddr_t *, uint8_t, const network_ipaddr4_t *, const network_ipaddr4_t *, const network_ipaddr4_t *, struct netstar_iphdr *);


typedef enum {
  NETSTAR_ICMPV6_TYPE_ECHO_REPLY              = 0x81,
  NETSTAR_ICMPV6_TYPE_ECHO                    = 0x80,
  NETSTAR_ICMPV6_TYPE_DESTINATION_UNREACHABLE = 0x01,
  NETSTAR_ICMPV6_TYPE_PACKET_TOO_BIG          = 0x02,
  NETSTAR_ICMPV6_TYPE_TIME_EXCEEDED           = 0x03,
  NETSTAR_ICMPV6_TYPE_PARAMETER_PROBLEM       = 0x04,
} netstar_icmpv6_type_t;

struct netstar_icmpv6hdr {
  uint8_t  type,
           code;
  uint16_t checksum;
/*
  union {
    uint32_t parameterptr,
             mtu;

    struct {
      uint16_t id,
               sequence;
    }; // echo;

    uint16_t delay;
  };
*/
};

#define NETSTAR_ICMPV6_SIZE sizeof(struct netstar_icmpv6hdr)

struct netstar_icmpv6_echo {
  uint16_t id,
           sequence;
};

#define NETSTAR_ICMPV6_ECHO_SIZE sizeof(struct netstar_icmpv6_echo)


extern ssize_t netstar_sendicmpv6echo(netstar_linkwave_t *, const network_macaddr_t *, const network_macaddr_t *, uint8_t, const network_ipaddr6_t *, const network_ipaddr6_t *);


typedef enum {
  NETSTAR_NDP_TYPE_ROUTER_SOLICIT   = 0x85,
  NETSTAR_NDP_TYPE_ROUTER_ADVERT    = 0x86,
  NETSTAR_NDP_TYPE_NEIGHBOR_SOLICIT = 0x87,
  NETSTAR_NDP_TYPE_NEIGHBOR_ADVERT  = 0x88,
  NETSTAR_NDP_TYPE_REDIRECT         = 0x89,
} netstar_ndp_type_t;

struct netstar_ndphdr {
  uint8_t  type,
           code;
  uint16_t checksum;
};

#define NETSTAR_NDP_SIZE NETSTAR_ICMPV6_SIZE


struct netstar_ndp_solicit {
  uint32_t reserved;
  uint8_t  daddr[NETWORK_IPADDR6_SIZE];
};

#define NETSTAR_NDP_SOLICIT_SIZE sizeof(struct netstar_ndp_solicit)


extern ssize_t netstar_sendndpsolicit(netstar_linkwave_t *, const network_macaddr_t *, const network_macaddr_t *, uint8_t, const network_ipaddr6_t *, const network_ipaddr6_t *, const network_macaddr_t *);


typedef enum {
  NETSTAR_NDP_ADVERT_FLAG_ROUTER    = 0x80000000,
  NETSTAR_NDP_ADVERT_FLAG_SOLICITED = 0x40000000,
  NETSTAR_NDP_ADVERT_FLAG_OVERRIDE  = 0x20000000,
} netstar_ndp_advert_flags;

struct netstar_ndp_advert {
  uint32_t flags;
  uint8_t  saddr[NETWORK_IPADDR6_SIZE];
};

#define NETSTAR_NDP_ADVERT_SIZE sizeof(struct netstar_ndp_advert)


extern ssize_t netstar_sendndpadvert(netstar_linkwave_t *, const network_macaddr_t *, const network_macaddr_t *, uint8_t, uint32_t, const network_ipaddr6_t *, const network_ipaddr6_t *, const network_macaddr_t *);


typedef enum {
  NETSTAR_NDP_OPTION_SOURCE_LINKADDR     = 0x01,
  NETSTAR_NDP_OPTION_TARGET_LINKADDR     = 0x02,
  NETSTAR_NDP_OPTION_PREFIX_INFORMATION  = 0x03,
  NETSTAR_NDP_OPTION_REDIRECTED_HEADER   = 0x04,
  NETSTAR_NDP_OPTION_MTU                 = 0x05,
  NETSTAR_NDP_OPTION_RTR_ADVERT_INTERVAL = 0x07,
  NETSTAR_NDP_OPTION_HOME_AGENT_INFO     = 0x08,
} netstar_ndp_option_type_t;

struct netstar_ndp_option {
  uint8_t type,
          length;
};

#define NETSTAR_NDP_OPTION_SIZE sizeof(struct netstar_ndp_option)

#define NETSTAR_NDP_OPTIONS_SIZE (0xFF * 8)

struct netstar_tcphdr {
  uint16_t sport,
           dport;
  uint32_t sequence,
           acknowledgment;

  union {
    struct {
#if __BYTE_ORDER == __LITTLE_ENDIAN || defined __LITTLE_ENDIAN || defined __LITTLE_ENDIAN_BITFIELD
      uint16_t reserved:4,
               data_offset:4,
               fin:1,
               syn:1,
               rst:1,
               psh:1,
               ack:1,
               urg:1,
               res:2;
            // ece:1,
            // cwr:1;
#elif __BYTE_ORDER == __BIG_ENDIAN || defined __BIG_ENDIAN || defined __BIG_ENDIAN_BITFIELD
      uint16_t data_offset:4,
               reserved:4,
               res:2,
            // cwr:1,
            // ece:1,
               urg:1,
               ack:1,
               psh:1,
               rst:1,
               syn:1,
               fin:1;
#else
  #error "Adjust your <asm/byteorder.h> defines"

#endif
    };

    struct {
#if __BYTE_ORDER == __LITTLE_ENDIAN || defined __LITTLE_ENDIAN || defined __LITTLE_ENDIAN_BITFIELD
      uint8_t unused:4,
              header_length:4;
#elif __BYTE_ORDER == __BIG_ENDIAN || defined __BIG_ENDIAN || defined __BIG_ENDIAN_BITFIELD
      uint8_t header_length:4,
              unused:4;
#else
  #error "Adjust your <asm/byteorder.h> defines"

#endif
      uint8_t flags;

#define NETSTAR_TCP_FLAGS_FIN 0x01
#define NETSTAR_TCP_FLAGS_SYN 0x02
#define NETSTAR_TCP_FLAGS_RST 0x04
#define NETSTAR_TCP_FLAGS_PSH 0x08
#define NETSTAR_TCP_FLAGS_ACK 0x10
#define NETSTAR_TCP_FLAGS_URG 0x20
    };
  };

  uint16_t window,
           checksum,
           urgentptr;
};

#define NETSTAR_TCP_SIZE         sizeof(struct netstar_tcphdr)
#define NETSTAR_TCP_OPTIONS_SIZE (0x0F * 5)

#define NETSTAR_TCP_OPTION_EOL        0x00
#define NETSTAR_TCP_OPTION_NOP        0x01
#define NETSTAR_TCP_OPTION_MSS        0x02
#define NETSTAR_TCP_OPTION_WS         0x03
#define NETSTAR_TCP_OPTION_SACK_PERM  0x04
#define NETSTAR_TCP_OPTION_SACK       0x05
#define NETSTAR_TCP_OPTION_TS         0x08

extern ssize_t netstar_sendtcp(netstar_linkwave_t *, const network_macaddr_t *, const network_macaddr_t *, const network_ipaddr4_t *, const network_ipaddr4_t *, uint16_t, uint16_t, uint32_t, uint32_t, uint8_t, uint8_t *, uint16_t);
extern ssize_t netstar_sendtcp6(netstar_linkwave_t *, const network_macaddr_t *, const network_macaddr_t *, const network_ipaddr6_t *, const network_ipaddr6_t *, uint16_t, uint16_t, uint32_t, uint32_t, uint8_t, uint8_t *, uint16_t);


struct netstar_udphdr {
  uint16_t sport,
           dport,
           length,
           checksum;
};

#define NETSTAR_UDP_SIZE sizeof(struct netstar_udphdr)


extern ssize_t netstar_sendudp(netstar_linkwave_t *, const network_macaddr_t *, const network_macaddr_t *, const network_ipaddr4_t *, const network_ipaddr4_t *, uint16_t, uint16_t, uint8_t *, uint16_t);
extern ssize_t netstar_sendudp6(netstar_linkwave_t *, const network_macaddr_t *, const network_macaddr_t *, const network_ipaddr6_t *, const network_ipaddr6_t *, uint16_t, uint16_t, uint8_t *, uint16_t);


struct netstar_dhcphdr {
  uint8_t  opcode,
           hardware,
           hardware_size,
           hops;
  uint32_t transition_id;
  uint16_t secs,
           flags;
  uint8_t  caddr[NETWORK_IPADDR4_SIZE],
           yaddr[NETWORK_IPADDR4_SIZE],
           saddr[NETWORK_IPADDR4_SIZE],
           gaddr[NETWORK_IPADDR4_SIZE];
  uint8_t  cmac[16],
           server_name[64],
           bootfile[128],
           options[312];
};


#define NETSTAR_FTP_DATA_PROTOCOL_PORT    20
#define NETSTAR_FTP_CONTROL_PROTOCOL_PORT 21

#define NETSTAR_TFTP_PROTOCOL_PORT        69

#define NETSTAR_TELNET_PROTOCOL_PORT      23

#define NETSTAR_HTTPS_PROTOCOL_PORT       443
#define NETSTAR_HTTP_PROTOCOL_PORT        80

#define NETSTAR_MDNS_PROTOCOL_PORT        5353
#define NETSTAR_DNS_PROTOCOL_PORT         53

#define NETSTAR_NTP_PROTOCOL_PORT         123

#define NETSTAR_SMTP_PROTOCOL_PORT        25
#define NETSTAR_POP3_PROTOCOL_PORT        110
#define NETSTAR_IMAP_PROTOCOL_PORT        143

#ifdef __cplusplus
}
#endif

#endif
