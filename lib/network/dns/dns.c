#include "dns.h"

#include <network/network.h>

#include <strings/strings.h>
#include <string.h>

#include <stdlib.h>
#include <stdio.h>


/*
static inline uint16_t
network_dns_u16_host_to_network(uint16_t bytes) {
// #if __BYTE_ORDER == __LITTLE_ENDIAN || defined __LITTLE_ENDIAN || defined __LITTLE_ENDIAN_BITFIELD
//   return (uint16_t)((uint16 >> 8) | (uint16 << 8));
// #elif __BYTE_ORDER == __BIG_ENDIAN || defined __BIG_ENDIAN || defined __BIG_ENDIAN_BITFIELD
//   return uint16;
// #endif

  return htons(bytes);
}
*/

static inline uint16_t
network_dns_u16_network_to_host(uint16_t bytes) {
// #if __BYTE_ORDER == __LITTLE_ENDIAN || defined __LITTLE_ENDIAN || defined __LITTLE_ENDIAN_BITFIELD
//   return (uint16_t)((uint16 >> 8) | (uint16 << 8));
// #elif __BYTE_ORDER == __BIG_ENDIAN || defined __BIG_ENDIAN || defined __BIG_ENDIAN_BITFIELD
//   return uint16;
// #endif

  return ntohs(bytes);
}

/*
static inline uint32_t
network_dns_u32_host_to_network(uint32_t u32) {
// #if __BYTE_ORDER == __LITTLE_ENDIAN || defined __LITTLE_ENDIAN || defined __LITTLE_ENDIAN_BITFIELD
//   return (uint32_t)(network_dns_uint16((uint16_t)(uint32 >> 16)) | (network_dns_uint16((uint16_t)uint32) << uint32));
// #elif __BYTE_ORDER == __BIG_ENDIAN || defined __BIG_ENDIAN || defined __BIG_ENDIAN_BITFIELD
//   return uint32;
// #endif

  return htonl(u32);
}
*/

static inline uint32_t
network_dns_u32_network_to_host(uint32_t u32) {
// #if __BYTE_ORDER == __LITTLE_ENDIAN || defined __LITTLE_ENDIAN || defined __LITTLE_ENDIAN_BITFIELD
//   return (uint32_t)(network_dns_to_little_endian_uint16((uint16_t)(uint32 >> 16)) | (network_dns_to_little_endian_uint16((uint16_t)uint32) << 16));
// #elif __BYTE_ORDER == __BIG_ENDIAN || defined __BIG_ENDIAN || defined __BIG_ENDIAN_BITFIELD
//   return uint32;
// #endif

  return ntohl(u32);
}

const char *
network_dns_opcode_format(uint8_t opcode) {
  switch (opcode) {
    case NETWORK_DNS_OPCODE_QUERY:
      return "Standart query";

    case NETWORK_DNS_OPCODE_IQUERY:
      return "Inverse query";

    case NETWORK_DNS_OPCODE_STATUS:
      return "Server status";

    case NETWORK_DNS_OPCODE_NOTIFY:
      return "Notify";

    case NETWORK_DNS_OPCODE_UPDATE:
      return "Update";

    case NETWORK_DNS_OPCODE_DSO:
      return "DSO";
  }

  return "Unknown";
}

const char *
network_dns_rcode_reason(uint8_t rcode) {
  switch (rcode) {
    case NETWORK_DNS_RCODE_FORMER:
      return "Resource format error";

    case NETWORK_DNS_RCODE_SERVFAIL:
      return "Server failed to complete the request";

    case NETWORK_DNS_RCODE_NXDOMAIN:
      return "Non-existent requested domain";

    case NETWORK_DNS_RCODE_NOTIMP:
      return "Server does not implement the type of query performed";

    case NETWORK_DNS_RCODE_REFUSED:
      return "Search query refused";

    case NETWORK_DNS_RCODE_YXDOMAIN:
      return "Name exists when it should not";

    case NETWORK_DNS_RCODE_YXRRSET:
      return "RR set exists when it should not";

    case NETWORK_DNS_RCODE_NXRRSET:
      return "RR set that should exist does not";

    case NETWORK_DNS_RCODE_NOTAUTH:
      return "Server not authoritative for zone";

    case NETWORK_DNS_RCODE_NOTZONE:
      return "Name not contained in zone";
  }

  return "Unknown";
}


struct network_dns_type {
  uint16_t u16;
  const char *string;
};

static struct network_dns_type network_dns_types[] = {
  { NETWORK_DNS_TYPE_A         , "A"          },
  { NETWORK_DNS_TYPE_NS        , "NS"         },
  { NETWORK_DNS_TYPE_MD        , "MD"         },
  { NETWORK_DNS_TYPE_MF        , "MF"         },
  { NETWORK_DNS_TYPE_CNAME     , "CNAME"      },
  { NETWORK_DNS_TYPE_SOA       , "SOA"        },
  { NETWORK_DNS_TYPE_MB        , "MB"         },
  { NETWORK_DNS_TYPE_MG        , "MG"         },
  { NETWORK_DNS_TYPE_MR        , "MR"         },
  { NETWORK_DNS_TYPE_NULL      , "NULL"       },
  { NETWORK_DNS_TYPE_WKS       , "WKS"        },
  { NETWORK_DNS_TYPE_PTR       , "PTR"        },
  { NETWORK_DNS_TYPE_HINFO     , "HINFO"      },
  { NETWORK_DNS_TYPE_MINFO     , "MINFO"      },
  { NETWORK_DNS_TYPE_MX        , "MX"         },
  { NETWORK_DNS_TYPE_TXT       , "TXT"        },
  { NETWORK_DNS_TYPE_RP        , "RP"         },
  { NETWORK_DNS_TYPE_AFSDB     , "AFSDB"      },
  { NETWORK_DNS_TYPE_X25       , "X25"        },
  { NETWORK_DNS_TYPE_ISDN      , "ISDN"       },
  { NETWORK_DNS_TYPE_RT        , "RT"         },
  { NETWORK_DNS_TYPE_NSAP      , "NSAP"       },
  { NETWORK_DNS_TYPE_NSAPPTR   , "NSAPPTR"    },
  { NETWORK_DNS_TYPE_SIG       , "SIG"        },
  { NETWORK_DNS_TYPE_KEY       , "KEY"        },
  { NETWORK_DNS_TYPE_PX        , "PX"         },
  { NETWORK_DNS_TYPE_GPOS      , "GPOS"       },
  { NETWORK_DNS_TYPE_AAAA      , "AAAA"       },
  { NETWORK_DNS_TYPE_LOC       , "LOC"        },
  { NETWORK_DNS_TYPE_NXT       , "NXT"        },
  { NETWORK_DNS_TYPE_EID       , "EID"        },
  { NETWORK_DNS_TYPE_NIMLOC    , "NIMLOC"     },
  { NETWORK_DNS_TYPE_SRV       , "SRV"        },
  { NETWORK_DNS_TYPE_ATMA      , "ATMA"       },
  { NETWORK_DNS_TYPE_NAPTR     , "NAPTR"      },
  { NETWORK_DNS_TYPE_KX        , "KX"         },
  { NETWORK_DNS_TYPE_CERT      , "CERT"       },
  { NETWORK_DNS_TYPE_A6        , "A6"         },
  { NETWORK_DNS_TYPE_DNAME     , "DNAME"      },
  { NETWORK_DNS_TYPE_SINK      , "SINK"       },
  { NETWORK_DNS_TYPE_OPT       , "OPT"        },
  { NETWORK_DNS_TYPE_APL       , "APL"        },
  { NETWORK_DNS_TYPE_DS        , "DS"         },
  { NETWORK_DNS_TYPE_SSHFP     , "SSHFP"      },
  { NETWORK_DNS_TYPE_IPSECKEY  , "IPSECKEY"   },
  { NETWORK_DNS_TYPE_RRSIG     , "RRSIG"      },
  { NETWORK_DNS_TYPE_NSEC      , "NSEC"       },
  { NETWORK_DNS_TYPE_DNSKEY    , "DNSKEY"     },
  { NETWORK_DNS_TYPE_DHCID     , "DHCID"      },
  { NETWORK_DNS_TYPE_NSEC3     , "NSEC3"      },
  { NETWORK_DNS_TYPE_NSEC3PARAM, "NSEC3PARAM" },
  { NETWORK_DNS_TYPE_TLSA      , "TLSA"       },
  { NETWORK_DNS_TYPE_SMIMEA    , "SMIMEA"     },
  { NETWORK_DNS_TYPE_HIP       , "HIP"        },
  { NETWORK_DNS_TYPE_NINFO     , "NINFO"      },
  { NETWORK_DNS_TYPE_RKEY      , "RKEY"       },
  { NETWORK_DNS_TYPE_TALINK    , "TALINK"     },
  { NETWORK_DNS_TYPE_CDS       , "CDS"        },
  { NETWORK_DNS_TYPE_CDNSKEY   , "CDNSKEY"    },
  { NETWORK_DNS_TYPE_OPENPGPKEY, "OPENPGPKEY" },
  { NETWORK_DNS_TYPE_CSYNC     , "CSYNC"      },
  { NETWORK_DNS_TYPE_ZONEMD    , "ZONEMD"     },
  { NETWORK_DNS_TYPE_SVCB      , "SVCB"       },
  { NETWORK_DNS_TYPE_HTTPS     , "HTTPS"      },
  { NETWORK_DNS_TYPE_SPF       , "SPF"        },
  { NETWORK_DNS_TYPE_UINFO     , "UINFO"      },
  { NETWORK_DNS_TYPE_UID       , "UID"        },
  { NETWORK_DNS_TYPE_GID       , "GID"        },
  { NETWORK_DNS_TYPE_UNSPEC    , "UNSPEC"     },
  { NETWORK_DNS_TYPE_NID       , "NID"        },
  { NETWORK_DNS_TYPE_L32       , "L32"        },
  { NETWORK_DNS_TYPE_L64       , "L64"        },
  { NETWORK_DNS_TYPE_LP        , "LP"         },
  { NETWORK_DNS_TYPE_EUI48     , "EUI48"      },
  { NETWORK_DNS_TYPE_EUI64     , "EUI64"      },
  { NETWORK_DNS_TYPE_TKEY      , "TKEY"       },
  { NETWORK_DNS_TYPE_TSIG      , "TSIG"       },
  { NETWORK_DNS_TYPE_IXFR      , "IXFR"       },
  { NETWORK_DNS_TYPE_AXFR      , "AXFR"       },
  { NETWORK_DNS_TYPE_MAILB     , "MAILB"      },
  { NETWORK_DNS_TYPE_MAILA     , "MAILA"      },
 // { NETWORK_DNS_TYPE_ANY     , "ANY"        },
  { NETWORK_DNS_TYPE_URI       , "URI"        },
  { NETWORK_DNS_TYPE_CAA       , "CAA"        },
  { NETWORK_DNS_TYPE_AVC       , "AVC"        },
  { NETWORK_DNS_TYPE_DOA       , "DOA"        },
  { NETWORK_DNS_TYPE_AMTRELAY  , "AMTRELAY"   },
  { NETWORK_DNS_TYPE_TA        , "TA"         },
  { NETWORK_DNS_TYPE_DLV       , "DLV"        },
  { NETWORK_DNS_TYPE_ANY       , NULL         },
};


uint16_t
network_dns_type_parse(const char *rtype) {
  struct network_dns_type *types = NULL;

  for (types = network_dns_types; types->string; types++)
    if (string_equals(types->string, rtype, false))
      return types->u16;

  return NETWORK_DNS_TYPE_ANY;
}

const char *
network_dns_type_format(uint16_t rtype) {
  struct network_dns_type *types = NULL;

  for (types = network_dns_types; types->string; types++)
    if (types->u16 == rtype)
      return types->string;

  return "ANY";
}


struct network_dns_class {
  uint16_t u16;
  const char *string;
};

static struct network_dns_class network_dns_classes[] = {
  { NETWORK_DNS_CLASS_IN  , "IN"   }, /* Internet Class         */
  { NETWORK_DNS_CLASS_CS  , "CS"   }, /* CsNet Class (Obsolete) */
  { NETWORK_DNS_CLASS_CH  , "CH"   }, /* Chaos Class            */
  { NETWORK_DNS_CLASS_HS  , "HS"   }, /* Hesiod Class           */
  { NETWORK_DNS_CLASS_NONE, "NONE" }, /* None Class             */
// { NETWORK_DNS_CLASS_ANY, "ANY"  }, /* Any Class              */
  { NETWORK_DNS_CLASS_ANY , NULL   }, /* Any Class              */
};


uint16_t
network_dns_class_parse(const char *rclass) {
  struct network_dns_class *classes = NULL;

  for (classes = network_dns_classes; classes->string; classes++)
    if (string_equals(classes->string, rclass, false))
      return classes->u16;

  return NETWORK_DNS_CLASS_ANY;
}

const char *
network_dns_class_format(uint16_t rclass) {
  struct network_dns_class *classes = NULL;

  for (classes = network_dns_classes; classes->string; classes++)
    if (classes->u16 == rclass)
      return classes->string;

  return "ANY";
}

int
network_dns_name_new(network_dns_name_t name, const char *string) {
  uint16_t index = 0, start = 0, shift = 0;

  if (string_length(string) > NETWORK_DNS_DOMAIN_LENGTH)
    return -1;

  for (;; index++)
    if (!string[index] || string[index] == '.') {
      if ((size_t)shift < NETWORK_DNS_DOMAIN_LENGTH)
        name[shift++] = (char)(index-start);

      for (; (size_t)shift < NETWORK_DNS_DOMAIN_LENGTH && start < index; start++)
        name[shift++] = string[start];

      if (!string[index]) {
        name[shift] = '\0';
        break;
      }

      start++;
    }

  return 1;
}

static int
network_dns_name_decode(network_dns_name_t name, const uint8_t *response, size_t response_length, size_t *shift) {
  uint8_t byte, count = 0;
  uint16_t ptr;
  size_t index = *shift;

  size_t length = NETWORK_DNS_DOMAIN_LENGTH;

  bool ptrjmp = false;

  if (index < response_length && response[index] == 0) {
    if (name)
      string_write(name, ".", 1);

    *shift += 1;
    return 1;
  }

  while (index < response_length && response[index]) {
    byte = response[index];

    if ((byte & 0xC0) == 0xC0) {
      if (index+1 >= response_length)
        return -1;

      ptr = (uint16_t)(((byte & 0x3F) << 8) | response[index+1]);
      if (ptr >= response_length)
        return -1;

      index = ptr;
      if (!ptrjmp)
        *shift += 2;

      ptrjmp = true;
      continue;
    }

    if ((index+1+byte) >= response_length) {
      return -1;
    }

    if (!ptrjmp)
      *shift += byte+1;

    index++;
    for (count = 0; count < byte; count++, index++) {
      if (index >= response_length) {
        return -1;
      }
      if (name && length) {
        *name++ = (char)response[index];
        length--;
      }
    }

    if (index < response_length && response[index]) {
      if (name && length) {
        *name++ = '.';
        length--;
      }
    }

    if (!ptrjmp && index < response_length && !response[index])
      *shift += 1;
  }

  if (name && length)
    *name = '\0';

  return 1;
}

static inline int
network_dns_addr_reverse_in4_arpa(network_dns_addr_t addr, network_ipaddr_t *ip) {
#define NETWORK_DNS_IN4_ARPA_FORMAT "%u.%u.%u.%u.in-addr.arpa"
  return snprintf(addr, NETWORK_DNS_ADDR_LENGTH, NETWORK_DNS_IN4_ARPA_FORMAT, ip->v4.u8[3], ip->v4.u8[2], ip->v4.u8[1], ip->v4.u8[0])+2;
}

static inline int
network_dns_addr_reverse_in6_arpa(network_dns_addr_t addr, network_ipaddr_t *ip) {
#define NETWORK_DNS_IN6_ARPA_FORMAT "%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.ip6.arpa"
#define v6(x) ip->v6.u8[x]%16, ip->v6.u8[x]/16, ip->v6.u8[x-1]%16, ip->v6.u8[x-1]/16
  return snprintf(addr, NETWORK_DNS_ADDR_LENGTH, NETWORK_DNS_IN6_ARPA_FORMAT, v6(15), v6(13), v6(11), v6(9), v6(7), v6(5), v6(3), v6(1))+2;
}

int
network_dns_addr_new(network_dns_addr_t addr, const char *string) {
  network_dns_addr_t reverse_addr = {0};
  network_ipaddr_t ip = {0};

  switch (network_ipaddr_parse(&ip, string)) {
    case NETWORK_IPADDR4:
      network_dns_addr_reverse_in4_arpa(reverse_addr, &ip);
      break;
    
    case NETWORK_IPADDR6:
      network_dns_addr_reverse_in6_arpa(reverse_addr, &ip);
      break;

    default:
      return -1;
  }

  return network_dns_name_new(addr, (const char *)reverse_addr);
}

int
network_dns_a_new(struct network_dns_a *a, network_dns_addr4_t addr) {
  if (network_ipaddr(addr) != NETWORK_IPADDR4)
    return -1;

  string_write(a->a, addr, NETWORK_IPADDR4_STRLENGTH);

  return NETWORK_IPADDR4_SIZE;
}

int
network_dns_aaaa_new(struct network_dns_aaaa *aaaa, network_dns_addr6_t addr) {
  if (network_ipaddr(addr) != NETWORK_IPADDR6)
    return -1;

  string_write(aaaa->aaaa, addr, NETWORK_IPADDR4_STRLENGTH);

  return NETWORK_IPADDR6_SIZE;
}

int
network_dns_txt_new(struct network_dns_txt *txt, const char *string) {
  long shift = 0;
  size_t length;

  if ((length = string_length(string)) > NETWORK_DNS_TXT_DATA_LENGTH)
    return -1;

  txt->txt[shift] = (char)length;
  shift += (long)sizeof(uint8_t);

  memcpy(&txt->txt[shift], string, length);

  return 1;
}

static int
network_dns_txt_decode(network_dns_txt_data_t txt, const uint8_t *response, size_t response_length, size_t *shift) {
  uint8_t length = response[*shift];

  if (*shift+length+sizeof(uint8_t) > response_length)
    return -1;

  *shift += sizeof(uint8_t);

  memcpy(txt, &response[*shift], length);
  *shift += length;

  return 1;
}


struct network_dns_parser {
  uint16_t questions, answers, authority, additional;

  size_t sections[NETWORK_DNS_SECTION_FINAL+1];
  network_dns_section_t section;
  uint16_t section_count;

  struct network_dns_resource_record *resource;
  struct network_dnshdr header;

  const uint8_t *buffer;
  size_t length, shift;
};


static inline void
network_dns_parser_section_header(network_dns_parser_t *parser) {
  parser->sections[NETWORK_DNS_SECTION_HEADER] = parser->shift = sizeof(struct network_dnshdr);
}

static void
network_dns_parser_section_question(network_dns_parser_t *parser) {
  uint16_t section_count = parser->questions;

  parser->sections[NETWORK_DNS_SECTION_QUESTION] = parser->shift;
  parser->section = NETWORK_DNS_SECTION_QUESTION;

  for (; section_count; section_count--)
    network_dns_parser_skipquestion(parser);
}

static void
network_dns_parser_section_resources(network_dns_parser_t *parser, network_dns_section_t section, uint16_t section_count) {
  parser->sections[section] = parser->shift;

  for (; section_count; section_count--)
    network_dns_parser_skipresource(parser);
}

static inline void
network_dns_parser_section_answer(network_dns_parser_t *parser) {
  network_dns_parser_section_resources(parser, NETWORK_DNS_SECTION_ANSWER, parser->answers);
}

static inline void
network_dns_parser_section_authority(network_dns_parser_t *parser) {
  network_dns_parser_section_resources(parser, NETWORK_DNS_SECTION_AUTHORITY, parser->authority);
}

static void
network_dns_parser_section_additional(network_dns_parser_t *parser) {
  network_dns_parser_section_resources(parser, NETWORK_DNS_SECTION_ADDITIONAL, parser->additional);
}

static inline void
network_dns_parser_section_final(network_dns_parser_t *parser) {
  parser->sections[NETWORK_DNS_SECTION_FINAL] = parser->length;
}

static inline void
network_dns_parser_sections(network_dns_parser_t *parser) {
  network_dns_parser_section_header(parser);

  if (network_dns_parser_nquestions(parser))
    network_dns_parser_section_question(parser);

  if (network_dns_parser_nanswers(parser))
    network_dns_parser_section_answer(parser);

  if (network_dns_parser_nauthority(parser))
    network_dns_parser_section_authority(parser);

  if (network_dns_parser_nadditional(parser))
    network_dns_parser_section_additional(parser);

  network_dns_parser_section_final(parser);
}

static void
network_dns_parser_set_section(network_dns_parser_t *parser, network_dns_section_t section) {
  parser->shift = parser->sections[section];
  parser->section = section;

  switch (section) {
    case NETWORK_DNS_SECTION_HEADER:
      break;

    case NETWORK_DNS_SECTION_QUESTION:
      parser->section_count = parser->questions;
      break;

    case NETWORK_DNS_SECTION_ANSWER:
      parser->section_count = parser->answers;
      break;

    case NETWORK_DNS_SECTION_AUTHORITY:
      parser->section_count = parser->authority;
      break;

    case NETWORK_DNS_SECTION_ADDITIONAL:
      parser->section_count = parser->additional;
      break;

    case NETWORK_DNS_SECTION_FINAL:
      parser->section_count = 0;
      break;
  }
}

struct network_dnshdr *
network_dns_parser_header(network_dns_parser_t *parser) {
  return &parser->header;
}

static inline bool
network_dns_parser_section_match(network_dns_parser_t *parser, network_dns_section_t section) {
  return parser->section == section;
}

static inline bool
network_dns_parser_section_end(network_dns_parser_t *parser) {
  return !parser->section;
}

int
network_dns_parser_question(network_dns_parser_t *parser, struct network_dns_question *question) {
  struct network_dns_question_record *record = NULL;
  size_t shift = parser->shift;

  if (!network_dns_parser_section_match(parser, NETWORK_DNS_SECTION_QUESTION))
    network_dns_parser_set_section(parser, NETWORK_DNS_SECTION_QUESTION);

  if (network_dns_parser_section_end(parser))
    return -1;

  if (network_dns_name_decode(question->name, parser->buffer, parser->length, &shift) == -1)
    return -1;

  if (shift+sizeof(struct network_dns_question_record) > parser->length)
    return -1;

  record = (struct network_dns_question_record *)&parser->buffer[shift];
  question->qtype  = (network_dns_type_t)network_dns_u16_network_to_host(record->qtype);
  question->qclass = (network_dns_class_t)network_dns_u16_network_to_host(record->qclass);
  shift += sizeof(struct network_dns_question_record);

  parser->section_count--;
  parser->shift = shift;
  return 1;
}

int
network_dns_parser_skipquestion(network_dns_parser_t *parser) {
  size_t shift = parser->shift;

  if (!network_dns_parser_section_match(parser, NETWORK_DNS_SECTION_QUESTION))
    return 1;

  if (network_dns_name_decode(NULL, parser->buffer, parser->length, &shift) == -1)
    return -1;

  if (shift+sizeof(struct network_dns_question_record) > parser->length)
    return -1;

  shift += sizeof(struct network_dns_question_record);

  parser->section_count--;
  parser->shift = shift;
  return 1;
}

void
network_dns_parser_questions(network_dns_parser_t *parser) {
  network_dns_parser_set_section(parser, NETWORK_DNS_SECTION_QUESTION);
}

int
network_dns_parser_skipquestions(network_dns_parser_t *parser) {
  if (!network_dns_parser_section_match(parser, NETWORK_DNS_SECTION_QUESTION))
    return -1;

  network_dns_parser_set_section(parser, NETWORK_DNS_SECTION_ANSWER);
  return 1;
}

uint16_t
network_dns_parser_nquestions(network_dns_parser_t *parser) {
  return parser->questions;
}

void
network_dns_parser_answers(network_dns_parser_t *parser) {
  network_dns_parser_set_section(parser, NETWORK_DNS_SECTION_ANSWER);
}

void
network_dns_parser_skipanswers(network_dns_parser_t *parser) {
  if (!network_dns_parser_section_match(parser, NETWORK_DNS_SECTION_ANSWER))
    return;

  network_dns_parser_set_section(parser, NETWORK_DNS_SECTION_AUTHORITY);
}

uint16_t
network_dns_parser_nanswers(network_dns_parser_t *parser) {
  return parser->answers;
}

void
network_dns_parser_authority(network_dns_parser_t *parser) {
  network_dns_parser_set_section(parser, NETWORK_DNS_SECTION_AUTHORITY);
}

void
network_dns_parser_skipauthority(network_dns_parser_t *parser) {
  if (!network_dns_parser_section_match(parser, NETWORK_DNS_SECTION_AUTHORITY))
    return;

  network_dns_parser_set_section(parser, NETWORK_DNS_SECTION_ADDITIONAL);
}

uint16_t
network_dns_parser_nauthority(network_dns_parser_t *parser) {
  return parser->authority;
}

void
network_dns_parser_additional(network_dns_parser_t *parser) {
  network_dns_parser_set_section(parser, NETWORK_DNS_SECTION_ADDITIONAL);
}

void
network_dns_parser_skipadditional(network_dns_parser_t *parser) {
  if (!network_dns_parser_section_match(parser, NETWORK_DNS_SECTION_ADDITIONAL))
    return;

  network_dns_parser_set_section(parser, NETWORK_DNS_SECTION_FINAL);
}

uint16_t
network_dns_parser_nadditional(network_dns_parser_t *parser) {
  return parser->additional;
}

static inline bool
network_dns_parser_resource_peek(network_dns_parser_t *parser) {
  return parser->resource != NULL;
}

static inline void
network_dns_parser_resource_pop(network_dns_parser_t *parser) {
  parser->resource = NULL;
}

static inline size_t
network_dns_parser_resource_data_skip(network_dns_parser_t *parser) {
  if (network_dns_parser_resource_peek(parser)) {
    parser->shift += parser->resource->length;
    network_dns_parser_resource_pop(parser);
  }

  return parser->shift;
}

int
network_dns_parser_resource(network_dns_parser_t *parser, struct network_dns_resource *resource) {
  size_t shift = parser->shift;

  if (network_dns_parser_section_match(parser, NETWORK_DNS_SECTION_HEADER) || network_dns_parser_section_match(parser, NETWORK_DNS_SECTION_QUESTION))
    return -1;

  if (network_dns_parser_section_end(parser))
    return -1;

  if (network_dns_parser_resource_peek(parser))
    shift = network_dns_parser_resource_data_skip(parser);

  if (network_dns_name_decode(resource->name, parser->buffer, parser->length, &shift) == -1)
    return -1;

  if (shift+sizeof(struct network_dns_resource_record) > parser->length)
    return -1;

  parser->resource = (struct network_dns_resource_record *)&parser->buffer[shift];
  parser->resource->rtype  = network_dns_u16_network_to_host(parser->resource->rtype);
  parser->resource->rclass = network_dns_u16_network_to_host(parser->resource->rclass);
  parser->resource->ttl    = network_dns_u32_network_to_host(parser->resource->ttl);
  parser->resource->length = network_dns_u16_network_to_host(parser->resource->length);
  resource->rtype  = (network_dns_type_t)parser->resource->rtype;
  resource->rclass = (network_dns_class_t)parser->resource->rclass;
  resource->ttl    = parser->resource->ttl;
  resource->length = parser->resource->length;
  shift += sizeof(struct network_dns_resource_record);
// parser->shift += resource->length;

  parser->section_count--;
  parser->shift = shift;
  return 1;
}

int
network_dns_parser_skipresource(network_dns_parser_t *parser) {
  struct network_dns_resource_record *record = NULL;
  size_t shift = parser->shift;

  if (network_dns_parser_section_match(parser, NETWORK_DNS_SECTION_HEADER) || network_dns_parser_section_match(parser, NETWORK_DNS_SECTION_QUESTION))
    return -1;

  if (network_dns_parser_section_end(parser))
    return -1;

  if (network_dns_parser_resource_peek(parser)) {
    network_dns_parser_resource_data_skip(parser);
    return 1;
  }

  if (network_dns_name_decode(NULL, parser->buffer, parser->length, &shift) == -1)
    return -1;

  if (shift+sizeof(struct network_dns_resource_record) > parser->length)
    return -1;

  record = (struct network_dns_resource_record *)&parser->buffer[shift];
  shift += sizeof(struct network_dns_resource_record);

  if (shift+network_dns_u16_network_to_host(record->length) > parser->length)
    return -1;

  shift += network_dns_u16_network_to_host(record->length);

  parser->section_count--;
  parser->shift = shift;
  return 1;
}

static inline bool
network_dns_parser_resource_type_match(network_dns_parser_t *parser, network_dns_type_t rtype) {
  return parser->resource && parser->resource->rtype == rtype;
}

static inline int
network_dns_parser_a_resource(network_dns_parser_t *parser, struct network_dns_a *a) {
  size_t shift = parser->shift;

  if (shift+NETWORK_IPADDR4_SIZE > parser->length)
    return -1;

  network_ipaddr4_format((network_ipaddr4_t *)&parser->buffer[shift], a->a, sizeof(a->a));
  shift += NETWORK_IPADDR4_SIZE;

  parser->shift = shift;
  return 1;
}

struct network_dns_a *
network_dns_parser_a(network_dns_parser_t *parser, struct network_dns_a *a) {
  if (!network_dns_parser_resource_type_match(parser, NETWORK_DNS_TYPE_A))
    return NULL;

  if (network_dns_parser_a_resource(parser, a) == -1)
    return NULL;

  network_dns_parser_resource_pop(parser);
  return a;
}

static inline int
network_dns_parser_aaaa_resource(network_dns_parser_t *parser, struct network_dns_aaaa *aaaa) {
  size_t shift = parser->shift;

  if (shift+NETWORK_IPADDR6_SIZE > parser->length)
    return -1;

  network_ipaddr6_format((network_ipaddr6_t *)&parser->buffer[shift], aaaa->aaaa, sizeof(aaaa->aaaa));
  shift += NETWORK_IPADDR6_SIZE;

  parser->shift = shift;
  return 1;
}

struct network_dns_aaaa *
network_dns_parser_aaaa(network_dns_parser_t *parser, struct network_dns_aaaa *aaaa) {
  if (!network_dns_parser_resource_type_match(parser, NETWORK_DNS_TYPE_AAAA))
    return NULL;

  if (network_dns_parser_aaaa_resource(parser, aaaa) == -1)
    return NULL;

  network_dns_parser_resource_pop(parser);
  return aaaa;
}

static inline int
network_dns_parser_cname_resource(network_dns_parser_t *parser, struct network_dns_cname *cname) {
  size_t shift = parser->shift;

  if (network_dns_name_decode(cname->cname, parser->buffer, parser->length, &shift) == -1)
    return -1;

  parser->shift = shift;
  return 1;
}

struct network_dns_cname *
network_dns_parser_cname(network_dns_parser_t *parser, struct network_dns_cname *cname) {
  if (!network_dns_parser_resource_type_match(parser, NETWORK_DNS_TYPE_CNAME))
    return NULL;

  if (network_dns_parser_cname_resource(parser, cname) == -1)
    return NULL;

  network_dns_parser_resource_pop(parser);
  return cname;
}

static inline int
network_dns_parser_mx_resource(network_dns_parser_t *parser, struct network_dns_mx *mx) {
  struct network_dns_mx_record *record = NULL;
  size_t shift = parser->shift;

  if (shift+sizeof(struct network_dns_mx_record) > parser->length)
    return -1;

  record = (struct network_dns_mx_record *)&parser->buffer[shift];
  mx->preference = network_dns_u16_network_to_host(record->preference);
  shift += sizeof(struct network_dns_mx_record);

  if (network_dns_name_decode(mx->exchange, parser->buffer, parser->length, &shift) == -1)
    return -1;

  parser->shift = shift;
  return 1;
}

struct network_dns_mx *
network_dns_parser_mx(network_dns_parser_t *parser, struct network_dns_mx *mx) {
  if (!network_dns_parser_resource_type_match(parser, NETWORK_DNS_TYPE_MX))
    return NULL;

  if (network_dns_parser_mx_resource(parser, mx) == -1)
    return NULL;

  network_dns_parser_resource_pop(parser);
  return mx;
}

static inline int
network_dns_parser_ns_resource(network_dns_parser_t *parser, struct network_dns_ns *ns) {
  size_t shift = parser->shift;

  if (network_dns_name_decode(ns->nsname, parser->buffer, parser->length, &shift) == -1)
    return -1;

  parser->shift = shift;
  return 1;
}

struct network_dns_ns *
network_dns_parser_ns(network_dns_parser_t *parser, struct network_dns_ns *ns) {
  if (!network_dns_parser_resource_type_match(parser, NETWORK_DNS_TYPE_NS))
    return NULL;

  if (network_dns_parser_ns_resource(parser, ns) == -1)
    return NULL;

  network_dns_parser_resource_pop(parser);
  return ns;
}

static inline int
network_dns_parser_ptr_resource(network_dns_parser_t *parser, struct network_dns_ptr *ptr) {
  size_t shift = parser->shift;

  if (network_dns_name_decode(ptr->ptr, parser->buffer, parser->length, &shift) == -1)
    return -1;

  parser->shift = shift;
  return 1;
}

struct network_dns_ptr *
network_dns_parser_ptr(network_dns_parser_t *parser, struct network_dns_ptr *ptr) {
  if (!network_dns_parser_resource_type_match(parser, NETWORK_DNS_TYPE_PTR))
    return NULL;

  if (network_dns_parser_ptr_resource(parser, ptr) == -1)
    return NULL;

  network_dns_parser_resource_pop(parser);
  return ptr;
}

static inline int
network_dns_parser_soa_resource(network_dns_parser_t *parser, struct network_dns_soa *soa) {
  struct network_dns_soa_record *record = NULL;
  size_t shift = parser->shift;

  if (network_dns_name_decode(soa->mname, parser->buffer, parser->length, &shift) == -1)
    return -1;
  if (network_dns_name_decode(soa->rname, parser->buffer, parser->length, &shift) == -1)
    return -1;

  if (shift+sizeof(struct network_dns_soa_record) > parser->length)
    return -1;

  record = (struct network_dns_soa_record *)&parser->buffer[shift];
  soa->serial  = network_dns_u32_network_to_host(record->serial);
  soa->refresh = network_dns_u32_network_to_host(record->refresh);
  soa->retry   = network_dns_u32_network_to_host(record->retry);
  soa->expire  = network_dns_u32_network_to_host(record->expire);
  soa->minimum = network_dns_u32_network_to_host(record->minimum);
  shift += sizeof(struct network_dns_soa_record);

  parser->shift = shift;
  return 1;
}

struct network_dns_soa *
network_dns_parser_soa(network_dns_parser_t *parser, struct network_dns_soa *soa) {
  if (!network_dns_parser_resource_type_match(parser, NETWORK_DNS_TYPE_SOA))
    return NULL;

  if (network_dns_parser_soa_resource(parser, soa) == -1)
    return NULL;

  network_dns_parser_resource_pop(parser);
  return soa;
}

static inline int
network_dns_parser_txt_resource(network_dns_parser_t *parser, struct network_dns_txt *txt) {
  size_t shift = parser->shift;

  if (network_dns_txt_decode(txt->txt, parser->buffer, parser->length, &shift) == -1)
    return -1;

  parser->shift = shift;
  return 1;
}

struct network_dns_txt *
network_dns_parser_txt(network_dns_parser_t *parser, struct network_dns_txt *txt) { 
  if (!network_dns_parser_resource_type_match(parser, NETWORK_DNS_TYPE_TXT))
    return NULL;

  if (network_dns_parser_txt_resource(parser, txt) == -1)
    return NULL;

  network_dns_parser_resource_pop(parser);
  return txt;
}

network_dns_parser_t *
network_dns_parser_new(const uint8_t *response, size_t length) {
  network_dns_parser_t *parser = NULL;

  if (!(parser = (network_dns_parser_t *)calloc(1, sizeof(network_dns_parser_t))))
    goto _return;

  parser->buffer = response;

  if ((parser->length = length) < sizeof(struct network_dnshdr))
    goto _return;

  memcpy(&parser->header, response, sizeof(struct network_dnshdr));
  parser->questions  = network_dns_u16_network_to_host(parser->header.questions);
  parser->answers    = network_dns_u16_network_to_host(parser->header.answers);
  parser->authority  = network_dns_u16_network_to_host(parser->header.authority);
  parser->additional = network_dns_u16_network_to_host(parser->header.additional);

  network_dns_parser_sections(parser);
  network_dns_parser_set_section(parser, NETWORK_DNS_SECTION_QUESTION);

  return parser;

_return:
  network_dns_parser_free(parser);
  return NULL;
}

void
network_dns_parser_free(network_dns_parser_t *parser) {
  if (parser) {
    free(parser);
  }
}


struct network_dns_builder {
  uint16_t questions, answers, authority, additional;

// size_t sections[NETWORK_DNS_SECTION_FINAL];
  network_dns_section_t section;
  uint16_t section_count;

  struct network_dns_resource_record *resource;
  struct network_dnshdr *header;

  uint8_t *buffer;
  size_t length, shift;

  bool compression;
};


void
network_dns_builder_setcompression(network_dns_builder_t *builder, bool compression) {
  builder->compression = compression;
}

static size_t
network_dns_builder_name_find(network_dns_builder_t *builder, network_dns_name_t name, size_t length) {
  size_t shift;

  for (shift = sizeof(struct network_dnshdr); shift < builder->shift; shift++)
    if (!memcmp(&builder->buffer[shift], name, length))
      return shift;

  return 0;
}

static size_t
network_dns_builder_name_pointer(network_dns_builder_t *builder, size_t shift, uint16_t addr) {
  uint16_t *ptr = (uint16_t *)&builder->buffer[builder->shift+shift];
  *ptr = htons(0xC000 | addr);

  return sizeof(uint16_t);
}

static size_t
network_dns_builder_name_encode(network_dns_builder_t *builder, network_dns_name_t name) {
  size_t byte, length, ptr, shift = 0;

  length = string_length(name)+1;

  for (byte = 0; byte < length; byte++) {
    if (builder->compression && name[byte] && (ptr = network_dns_builder_name_find(builder, &name[byte], length-byte))) {
      shift += network_dns_builder_name_pointer(builder, shift, (uint16_t)ptr);
      byte += length-byte;
      continue;
    }

    builder->buffer[builder->shift+shift++] = (uint8_t)name[byte];
  }

  return shift;
}

void
network_dns_builder_questions(network_dns_builder_t *builder) {
  if (builder->section > NETWORK_DNS_SECTION_QUESTION)
    return;

  builder->section = NETWORK_DNS_SECTION_QUESTION;
}

static inline size_t
network_dns_builder_question_length(struct network_dns_question *question) {
  return string_length(question->name)+sizeof(struct network_dns_question_record);
}

void
network_dns_builder_question(network_dns_builder_t *builder, struct network_dns_question *question) {
  struct network_dns_question_record *record = NULL;

  if (builder->section != NETWORK_DNS_SECTION_QUESTION)
    return;
  if (network_dns_builder_question_length(question)+builder->shift > builder->length)
    return;

  builder->shift += network_dns_builder_name_encode(builder, question->name);
  record = (struct network_dns_question_record *)&builder->buffer[builder->shift];
  record->qtype  = htons(question->qtype);
  record->qclass = htons(question->qclass);
  builder->shift += sizeof(struct network_dns_question_record);

  builder->questions++;
}

uint16_t
network_dns_builder_nquestions(network_dns_builder_t *builder) {
  return builder->questions;
}

void
network_dns_builder_answers(network_dns_builder_t *builder) {
  if (builder->section > NETWORK_DNS_SECTION_ANSWER)
    return;

  builder->section = NETWORK_DNS_SECTION_ANSWER;
}

uint16_t
network_dns_builder_nanswers(network_dns_builder_t *builder) {
  return builder->answers;
}

void
network_dns_builder_authority(network_dns_builder_t *builder) {
  if (builder->section > NETWORK_DNS_SECTION_AUTHORITY)
    return;

  builder->section = NETWORK_DNS_SECTION_AUTHORITY;
}

uint16_t
network_dns_builder_nauthority(network_dns_builder_t *builder) {
  return builder->authority;
}

void
network_dns_builder_additional(network_dns_builder_t *builder) {
  if (builder->section > NETWORK_DNS_SECTION_ADDITIONAL)
    return;

  builder->section = NETWORK_DNS_SECTION_ADDITIONAL;
}

uint16_t
network_dns_builder_nadditional(network_dns_builder_t *builder) {
  return builder->additional;
}

static uint16_t
network_dns_builder_resource_data_length(network_dns_type_t rtype, const void *rdata) {
  switch (rtype) {
    case NETWORK_DNS_TYPE_CNAME: {
      struct network_dns_cname *cname = (struct network_dns_cname *)rdata;
      return (uint16_t)string_length(cname->cname);
    }

    case NETWORK_DNS_TYPE_MX: {
      struct network_dns_mx *mx = (struct network_dns_mx *)rdata;
      return (uint16_t)(sizeof(struct network_dns_mx_record)+string_length(mx->exchange));
    }

    case NETWORK_DNS_TYPE_NS: {
      struct network_dns_ns *ns = (struct network_dns_ns *)rdata;
      return (uint16_t)string_length(ns->nsname);
    }

    case NETWORK_DNS_TYPE_A: {
      return NETWORK_IPADDR4_SIZE;
    }

    case NETWORK_DNS_TYPE_AAAA: {
      return NETWORK_IPADDR6_SIZE;
    }

    case NETWORK_DNS_TYPE_PTR: {
      struct network_dns_ptr *ptr = (struct network_dns_ptr *)rdata;
      return (uint16_t)string_length(ptr->ptr);
    }

    case NETWORK_DNS_TYPE_SOA: {
      struct network_dns_soa *soa = (struct network_dns_soa *)rdata;
      return (uint16_t)(string_length(soa->mname)+string_length(soa->rname)+sizeof(struct network_dns_soa_record));
    }

    case NETWORK_DNS_TYPE_TXT: {
      struct network_dns_txt *txt = (struct network_dns_txt *)rdata;
      return (uint16_t)string_length(txt->txt);
    }
  }

  return 0;
}

static inline bool
network_dns_builder_resource_type_matches(network_dns_builder_t *builder, network_dns_type_t rtype) {
  return builder->resource && ntohs(builder->resource->rtype) == rtype;
}

static inline bool
network_dns_builder_resource_verify_length(network_dns_builder_t *builder, network_dns_type_t rtype, const void *rdata) {
  uint16_t length = ((!builder->resource->length) ? network_dns_builder_resource_data_length(rtype, rdata) : ntohs(builder->resource->length));

  if (builder->shift+length > builder->length)
    return false;

  if (!builder->resource->length)
    builder->resource->length = htons(length);

  return true;
}

static inline bool
network_dns_builder_resource_sanity_check(network_dns_builder_t *builder, network_dns_type_t rtype, const void *rdata) {
  if (!network_dns_builder_resource_type_matches(builder, rtype))
    return false;

  if (!network_dns_builder_resource_verify_length(builder, rtype, rdata))
    return false;

  return true;
}

static inline size_t
network_dns_builder_resource_length(struct network_dns_resource *resource) {
  return string_length(resource->name)+sizeof(struct network_dns_resource_record)+resource->length;
}

void
network_dns_builder_resource(network_dns_builder_t *builder, struct network_dns_resource *resource) {
  if (network_dns_builder_resource_length(resource)+builder->shift > builder->length)
    return;

  builder->shift += network_dns_builder_name_encode(builder, resource->name);
  builder->resource = (struct network_dns_resource_record *)&builder->buffer[builder->shift];
  builder->resource->rtype  = htons(resource->rtype);
  builder->resource->rclass = htons(resource->rclass);
  builder->resource->ttl    = htonl(resource->ttl);
  builder->resource->length = htons(resource->length);
  builder->shift += sizeof(struct network_dns_resource_record);
}

static inline void
network_dns_builder_a_resource(network_dns_builder_t *builder, struct network_dns_a *a) {
  if (network_ipaddr4_parse((network_ipaddr4_t *)&builder->buffer[builder->shift], a->a) == -1)
    return;

  builder->shift += NETWORK_IPADDR4_SIZE;
}

void
network_dns_builder_a(network_dns_builder_t *builder, struct network_dns_a *a) {
  if (!network_dns_builder_resource_sanity_check(builder, NETWORK_DNS_TYPE_A, a))
    return;

  network_dns_builder_a_resource(builder, a);
}

static inline void
network_dns_builder_aaaa_resource(network_dns_builder_t *builder, struct network_dns_aaaa *aaaa) {
  if (network_ipaddr6_parse((network_ipaddr6_t *)&builder->buffer[builder->shift], aaaa->aaaa) == -1)
    return;

  builder->shift += NETWORK_IPADDR6_SIZE;
}

void
network_dns_builder_aaaa(network_dns_builder_t *builder, struct network_dns_aaaa *aaaa) {
  if (!network_dns_builder_resource_sanity_check(builder, NETWORK_DNS_TYPE_AAAA, aaaa))
    return;

  network_dns_builder_aaaa_resource(builder, aaaa);
}

static inline void
network_dns_builder_cname_resource(network_dns_builder_t *builder, struct network_dns_cname *cname) {
  builder->shift += network_dns_builder_name_encode(builder, cname->cname);
}

void
network_dns_builder_cname(network_dns_builder_t *builder, struct network_dns_cname *cname) {
  if (!network_dns_builder_resource_sanity_check(builder, NETWORK_DNS_TYPE_CNAME, cname))
    return;

  network_dns_builder_cname_resource(builder, cname);
}

static inline void
network_dns_builder_mx_resource(network_dns_builder_t *builder, struct network_dns_mx *mx) {
  struct network_dns_mx_record *record = (struct network_dns_mx_record *)&builder->buffer[builder->shift];

  record->preference = htons(mx->preference);
  builder->shift += sizeof(struct network_dns_mx_record);
  builder->shift += network_dns_builder_name_encode(builder, mx->exchange);
}

void
network_dns_builder_mx(network_dns_builder_t *builder, struct network_dns_mx *mx) {
  if (!network_dns_builder_resource_sanity_check(builder, NETWORK_DNS_TYPE_MX, mx))
    return;

  network_dns_builder_mx_resource(builder, mx);
}

static inline void
network_dns_builder_ns_resource(network_dns_builder_t *builder, struct network_dns_ns *ns) {
  builder->shift += network_dns_builder_name_encode(builder, ns->nsname);
}

void
network_dns_builder_ns(network_dns_builder_t *builder, struct network_dns_ns *ns) {
  if (!network_dns_builder_resource_sanity_check(builder, NETWORK_DNS_TYPE_NS, ns))
    return;

  network_dns_builder_ns_resource(builder, ns);
}

static inline void
network_dns_builder_ptr_resource(network_dns_builder_t *builder, struct network_dns_ptr *ptr) {
  builder->shift += network_dns_builder_name_encode(builder, ptr->ptr);
}

void
network_dns_builder_ptr(network_dns_builder_t *builder, struct network_dns_ptr *ptr) {
  if (!network_dns_builder_resource_sanity_check(builder, NETWORK_DNS_TYPE_PTR, ptr))
    return;

  network_dns_builder_ptr_resource(builder, ptr);
}

static inline void
network_dns_builder_soa_resource(network_dns_builder_t *builder, struct network_dns_soa *soa) {
  struct network_dns_soa_record *record = NULL;

  builder->shift += network_dns_builder_name_encode(builder, soa->mname);
  builder->shift += network_dns_builder_name_encode(builder, soa->rname);
  record = (struct network_dns_soa_record *)&builder->buffer[builder->shift];
  record->serial  = htonl(soa->serial);
  record->refresh = htonl(soa->refresh);
  record->retry   = htonl(soa->retry);
  record->expire  = htonl(soa->expire);
  record->minimum = htonl(soa->minimum);
  builder->shift += sizeof(struct network_dns_soa_record);
}

void
network_dns_builder_soa(network_dns_builder_t *builder, struct network_dns_soa *soa) {
  if (!network_dns_builder_resource_sanity_check(builder, NETWORK_DNS_TYPE_SOA, soa))
    return;

  network_dns_builder_soa_resource(builder, soa);
}

static size_t
network_dns_builder_txt_encode(network_dns_builder_t *builder, network_dns_txt_data_t txt) {
  const uint8_t length = (const uint8_t)txt[0];
  size_t shift;

  memcpy(&builder->buffer[builder->shift], txt, length+1);
  shift = sizeof(uint8_t)+length;

  return shift;
}

static inline void
network_dns_builder_txt_resource(network_dns_builder_t *builder, struct network_dns_txt *txt) {
  builder->shift += network_dns_builder_txt_encode(builder, txt->txt);
}

void
network_dns_builder_txt(network_dns_builder_t *builder, struct network_dns_txt *txt) {
  if (!network_dns_builder_resource_sanity_check(builder, NETWORK_DNS_TYPE_TXT, txt))
    return;

  network_dns_builder_txt_resource(builder, txt);
}

size_t
network_dns_builder_build(network_dns_builder_t *builder) {
  return builder->shift;
}

network_dns_builder_t *
network_dns_builder_new(struct network_dnshdr *dnsh, uint8_t *buffer, size_t length) {
  network_dns_builder_t *builder = NULL;

  if (!(builder = (network_dns_builder_t *)calloc(1, sizeof(network_dns_builder_t))))
    goto _return;

  builder->buffer = buffer;
  
  if ((builder->length = length) < sizeof(struct network_dnshdr))
    goto _return;

  builder->header = (struct network_dnshdr *)builder->buffer;
  builder->header->id                  = htons(dnsh->id);
  builder->header->response            = dnsh->response;
  builder->header->opcode              = dnsh->opcode;
  builder->header->authoritative       = dnsh->authoritative;
  builder->header->truncated           = dnsh->truncated;
  builder->header->recursion_desired   = dnsh->recursion_desired;
  builder->header->recursion_available = dnsh->recursion_available;
  builder->header->zero                = dnsh->zero;
  builder->header->authetic_data       = dnsh->authetic_data;
  builder->header->check_desired       = dnsh->check_desired;
  builder->header->rcode               = dnsh->rcode;
  builder->header->questions           = htons(dnsh->questions);
  builder->header->answers             = htons(dnsh->answers);
  builder->header->authority           = htons(dnsh->authority);
  builder->header->additional          = htons(dnsh->additional);

  builder->questions  = dnsh->questions;
  builder->answers    = dnsh->answers;
  builder->authority  = dnsh->authority;
  builder->additional = dnsh->additional;

  builder->shift = sizeof(struct network_dnshdr);

  builder->section = NETWORK_DNS_SECTION_HEADER;

  return builder;

_return:
  network_dns_builder_free(builder);
  return NULL;
}

void
network_dns_builder_free(network_dns_builder_t *builder) {
  if (builder) {
    free(builder);
  }
}
