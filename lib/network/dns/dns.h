#ifndef _NETWORK_DNS_DNS_H
#define _NETWORK_DNS_DNS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <network/network.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define NETWORK_DNS_PROTOCOL_PORT  53

#define NETWORK_DNS_MESSAGE_SIZE   0xFFFF
#define NETWORK_DNS_UMESSAGE_SIZE  0x0200

#define NETWORK_DNS_URESPONSE_SIZE 0x0200
#define NETWORK_DNS_RESPONSE_SIZE  0xFFFF


/*
 *  +---------------------+
 *  |        Header       |
 *  +---------------------+
 *  |       Question      | the question for the name server
 *  +---------------------+
 *  |        Answer       | RRs answering the question
 *  +---------------------+
 *  |      Authority      | RRs pointing toward an authority
 *  +---------------------+
 *  |      Additional     | RRs holding additional information
 *  +---------------------+
 */
typedef enum {
  NETWORK_DNS_SECTION_HEADER, NETWORK_DNS_SECTION_QUESTION, NETWORK_DNS_SECTION_ANSWER, NETWORK_DNS_SECTION_AUTHORITY, NETWORK_DNS_SECTION_ADDITIONAL, NETWORK_DNS_SECTION_FINAL
} network_dns_section_t;

typedef enum {
  NETWORK_DNS_OPCODE_QUERY  = 0x0000, /* Query                            */
  NETWORK_DNS_OPCODE_IQUERY = 0x0001, /* IQuery (Inverse Query, Obsolete) */
  NETWORK_DNS_OPCODE_STATUS = 0x0002, /* Status                           */
  NETWORK_DNS_OPCODE_NOTIFY = 0x0004, /* Notify                           */
  NETWORK_DNS_OPCODE_UPDATE = 0x0005, /* Update                           */
  NETWORK_DNS_OPCODE_DSO    = 0x0006  /* DNS Stateful Operations (DSO)    */
} network_dns_opcode_t;


extern const char *network_dns_opcode_format(uint8_t);


typedef enum {
  NETWORK_DNS_RCODE_NOERROR  = 0x0000, /* No Error                          */
  NETWORK_DNS_RCODE_FORMER   = 0x0001, /* Format Error                      */
  NETWORK_DNS_RCODE_SERVFAIL = 0x0002, /* Server Failure                    */
  NETWORK_DNS_RCODE_NXDOMAIN = 0x0003, /* Non-Existent Domain               */
  NETWORK_DNS_RCODE_NOTIMP   = 0x0004, /* Not Implemented                   */
  NETWORK_DNS_RCODE_REFUSED  = 0x0005, /* Query Refused                     */
  NETWORK_DNS_RCODE_YXDOMAIN = 0x0006, /* Name Exists When it Should Not    */
  NETWORK_DNS_RCODE_YXRRSET  = 0x0007, /* RR Set Exists when it Should Not  */
  NETWORK_DNS_RCODE_NXRRSET  = 0x0008, /* RR Set That Should Exist Does Not */
  NETWORK_DNS_RCODE_NOTAUTH  = 0x0009, /* Server Not Authoritative For Zone */
  NETWORK_DNS_RCODE_NOTZONE  = 0x000A  /* Name Not Contained in Zone        */
} network_dns_rcode_t;


extern const char *network_dns_rcode_reason(uint8_t);


/*
 *                                  1  1  1  1  1  1
 *    0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  |                      ID                       |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  |QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  |                    QDCOUNT                    |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  |                    ANCOUNT                    |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  |                    NSCOUNT                    |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  |                    ARCOUNT                    |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 */
/*
struct network_dnshdr {
  uint16_t     id;

#if __BYTE_ORDER == __LITTLE_ENDIAN || defined __LITTLE_ENDIAN || defined __LITTLE_ENDIAN_BITFIELD
  bool         recursion_desired:1,
               truncated:1,
               authoritative:1;
  network_dns_opcode_t opcode:4;
  bool         response:1;
  network_dns_rcode_t  rcode:4;
  bool         check_desired:1,
               authetic_data:1,
               zero:1,
               recursion_available:1; 

#elif __BYTE_ORDER == __BIG_ENDIAN || defined __BIG_ENDIAN || defined __BIG_ENDIAN_BITFIELD
  bool         response:1;
  network_dns_opcode_t opcode:4;
  bool         authoritative:1,
               truncated:1,
               recursion_desired:1,
               recursion_available:1,
               zero:1,
               authetic_data:1,
               check_desired:1;
  network_dns_rcode_t  rcode:4;

#else
  #error "Adjust your <asm/byteorder.h> defines"

#endif
  uint16_t     questions,
               answers,
               authority,
               additional;
};
*/
struct network_dnshdr {
  uint16_t     id;

#if __BYTE_ORDER == __LITTLE_ENDIAN || defined __LITTLE_ENDIAN || defined __LITTLE_ENDIAN_BITFIELD
  uint8_t      recursion_desired:1,
               truncated:1,
               authoritative:1,
               opcode:4,
               response:1,
               rcode:4,
               check_desired:1,
               authetic_data:1,
               zero:1,
               recursion_available:1; 

#elif __BYTE_ORDER == __BIG_ENDIAN || defined __BIG_ENDIAN || defined __BIG_ENDIAN_BITFIELD
  uint8_t      response:1,
               opcode:4,
               authoritative:1,
               truncated:1,
               recursion_desired:1,
               recursion_available:1,
               zero:1,
               authetic_data:1,
               check_desired:1,
               rcode:4;

#else
  #error "Adjust your <asm/byteorder.h> defines"

#endif
  uint16_t     questions,
               answers,
               authority,
               additional;
};

#define NETWORK_DNS_DOMAIN_LENGTH 0x00FF

typedef char network_dns_name_t[NETWORK_DNS_DOMAIN_LENGTH];


extern int network_dns_name_new(network_dns_name_t, const char *);


#define NETWORK_DNS_ADDR_LENGTH   NETWORK_DNS_DOMAIN_LENGTH 

typedef network_dns_name_t network_dns_addr_t;


extern int network_dns_addr_new(network_dns_addr_t, const char *);


typedef enum {
  NETWORK_DNS_TYPE_A          = 0x0001, /* Host Address                                       */
  NETWORK_DNS_TYPE_NS         = 0x0002, /* Authoritative Name Server                          */
  NETWORK_DNS_TYPE_MD         = 0x0003, /* Mail Destination (Obsolete - Use MX)               */
  NETWORK_DNS_TYPE_MF         = 0x0004, /* Mail Forwarder (Obsolete - Use MX)                 */
  NETWORK_DNS_TYPE_CNAME      = 0x0005, /* Canonical Name For an Alias                        */
  NETWORK_DNS_TYPE_SOA        = 0x0006, /* Marks The Start of a Zone of Authority             */
  NETWORK_DNS_TYPE_MB         = 0x0007, /* Mailbox Domain Name (EXPERIMENTAL)                 */
  NETWORK_DNS_TYPE_MG         = 0x0008, /* Mail Group Member (EXPERIMENTAL)                   */
  NETWORK_DNS_TYPE_MR         = 0x0009, /* Mail Rename Domain Name (EXPERIMENTAL)             */
  NETWORK_DNS_TYPE_NULL       = 0x000A, /* Null RR (EXPERIMENTAL)                             */
  NETWORK_DNS_TYPE_WKS        = 0x000B, /* Well Known Service Description                     */
  NETWORK_DNS_TYPE_PTR        = 0x000C, /* Domain Name Pointer                                */
  NETWORK_DNS_TYPE_HINFO      = 0x000D, /* Host Information                                   */
  NETWORK_DNS_TYPE_MINFO      = 0x000E, /* Mailbox or Mail List Information                   */
  NETWORK_DNS_TYPE_MX         = 0x000F, /* Mail Exchange                                      */
  NETWORK_DNS_TYPE_TXT        = 0x0010, /* Text Strings                                       */
  NETWORK_DNS_TYPE_RP         = 0x0011, /* For Responsible Person                             */
  NETWORK_DNS_TYPE_AFSDB      = 0x0012, /* For AFS Data Base Location                         */
  NETWORK_DNS_TYPE_X25        = 0x0013, /* For X.25 PSDN Address                              */
  NETWORK_DNS_TYPE_ISDN       = 0x0014, /* For ISDN Address                                   */
  NETWORK_DNS_TYPE_RT         = 0x0015, /* For Route Through                                  */
  NETWORK_DNS_TYPE_NSAP       = 0x0016, /* For NSAP Address, NSAP Style a Record              */
  NETWORK_DNS_TYPE_NSAPPTR    = 0x0017, /* For Domain Name Pointer, NSAP Style                */
  NETWORK_DNS_TYPE_SIG        = 0x0018, /* For Security Signature                             */
  NETWORK_DNS_TYPE_KEY        = 0x0019, /* For Security Key                                   */
  NETWORK_DNS_TYPE_PX         = 0x001A, /* X.400 Mail Mapping Information                     */
  NETWORK_DNS_TYPE_GPOS       = 0x001B, /* Geographical Position                              */
  NETWORK_DNS_TYPE_AAAA       = 0x001C, /* IP6 Address                                        */
  NETWORK_DNS_TYPE_LOC        = 0x001D, /* Location Information                               */
  NETWORK_DNS_TYPE_NXT        = 0x001E, /* Next Domain (OBSOLETE)                             */
  NETWORK_DNS_TYPE_EID        = 0x001F, /* Endpoint Identifier                                */
  NETWORK_DNS_TYPE_NIMLOC     = 0x0020, /* Nimrod Locator                                     */
  NETWORK_DNS_TYPE_SRV        = 0x0021, /* Server Selection                                   */
  NETWORK_DNS_TYPE_ATMA       = 0x0022, /* ATM Address                                        */
  NETWORK_DNS_TYPE_NAPTR      = 0x0023, /* Naming Authority Pointer                           */
  NETWORK_DNS_TYPE_KX         = 0x0024, /* Key Exchanger                                      */
  NETWORK_DNS_TYPE_CERT       = 0x0025, /* CERT                                               */
  NETWORK_DNS_TYPE_A6         = 0x0026, /* A6 (Obsolete - Use AAAA)                           */
  NETWORK_DNS_TYPE_DNAME      = 0x0027, /* DNAME                                              */
  NETWORK_DNS_TYPE_SINK       = 0x0028, /* SINK                                               */
  NETWORK_DNS_TYPE_OPT        = 0x0029, /* OPT                                                */
  NETWORK_DNS_TYPE_APL        = 0x002A, /* APL                                                */
  NETWORK_DNS_TYPE_DS         = 0x002B, /* Delegation Signer                                  */
  NETWORK_DNS_TYPE_SSHFP      = 0x002C, /* SSH Key Fingerprint                                */
  NETWORK_DNS_TYPE_IPSECKEY   = 0x002D, /* IPSECKEY                                           */
  NETWORK_DNS_TYPE_RRSIG      = 0x002E, /* RRSIG                                              */
  NETWORK_DNS_TYPE_NSEC       = 0x002F, /* NSEC                                               */
  NETWORK_DNS_TYPE_DNSKEY     = 0x0030, /* DNSKEY                                          	 */
  NETWORK_DNS_TYPE_DHCID      = 0x0031, /* DHCID                                              */
  NETWORK_DNS_TYPE_NSEC3      = 0x0032, /* NSEC3                                              */
  NETWORK_DNS_TYPE_NSEC3PARAM = 0x0033, /* NSEC3PARAM                                         */
  NETWORK_DNS_TYPE_TLSA       = 0x0034, /* TLSA                                               */
  NETWORK_DNS_TYPE_SMIMEA     = 0x0036, /* S/MIME Cert Association                            */
  NETWORK_DNS_TYPE_HIP        = 0x0037, /* Host Identity Protocol                             */
  NETWORK_DNS_TYPE_NINFO      = 0x0038, /* NINFO                                              */
  NETWORK_DNS_TYPE_RKEY       = 0x0039, /* RKEY                                               */
  NETWORK_DNS_TYPE_TALINK     = 0x003A, /* Trust Anchor LINK                                  */
  NETWORK_DNS_TYPE_CDS        = 0x003B, /* Child DS                                           */
  NETWORK_DNS_TYPE_CDNSKEY    = 0x003C, /* DNSKEY(s) the Child wants reflected in DS          */
  NETWORK_DNS_TYPE_OPENPGPKEY = 0x003D, /* OpenPGP Key                                        */
  NETWORK_DNS_TYPE_CSYNC      = 0x003E, /* Child-To-Parent Synchronization                    */
  NETWORK_DNS_TYPE_ZONEMD     = 0x003F, /* Message Digest For DNS Zone                        */
  NETWORK_DNS_TYPE_SVCB       = 0x0040, /* Service Binding                                    */
  NETWORK_DNS_TYPE_HTTPS      = 0x0041, /* HTTPS Binding                                      */
  NETWORK_DNS_TYPE_SPF        = 0x0063, /*                                                    */
  NETWORK_DNS_TYPE_UINFO      = 0x0064, /* IANA-Reserved                                      */
  NETWORK_DNS_TYPE_UID        = 0x0065, /* IANA-Reserved                                      */
  NETWORK_DNS_TYPE_GID        = 0x0066, /* IANA-Reserved                                      */
  NETWORK_DNS_TYPE_UNSPEC     = 0x0067, /* IANA-Reserved                                      */
  NETWORK_DNS_TYPE_NID        = 0x0068, /*                                                    */
  NETWORK_DNS_TYPE_L32        = 0x0069, /*                                                    */
  NETWORK_DNS_TYPE_L64        = 0x006A, /*                                                    */
  NETWORK_DNS_TYPE_LP         = 0x006B, /*                                                    */
  NETWORK_DNS_TYPE_EUI48      = 0x006C, /* EUI-48 Address                                     */
  NETWORK_DNS_TYPE_EUI64      = 0x006D, /* EUI-64 Address                                     */
  NETWORK_DNS_TYPE_TKEY       = 0x00F9, /* Transaction Key                                    */
  NETWORK_DNS_TYPE_TSIG       = 0x00FA, /* Transaction Signature                              */
  NETWORK_DNS_TYPE_IXFR       = 0x00FB, /* Incremental Transfer                               */
  NETWORK_DNS_TYPE_AXFR       = 0x00FC, /* Request For a Transfer of an Entire Zone           */
  NETWORK_DNS_TYPE_MAILB      = 0x00FD, /* Request For Mailbox-Related Records (MB, MG or MR) */
  NETWORK_DNS_TYPE_MAILA      = 0x00FE, /* Request For Mail Agent RRs (Obsolete - Use MX)     */
  NETWORK_DNS_TYPE_ANY        = 0x00FF, /* Request For All Records                            */
  NETWORK_DNS_TYPE_URI        = 0x0100, /* URI                                                */
  NETWORK_DNS_TYPE_CAA        = 0x0101, /* Certification Authority Restriction                */
  NETWORK_DNS_TYPE_AVC        = 0x0102, /* Application Visibility and Control                 */
  NETWORK_DNS_TYPE_DOA        = 0x0103, /* Digital Object Architecture                        */
  NETWORK_DNS_TYPE_AMTRELAY   = 0x0104, /* Automatic Multicast Tunneling Relay                */
  NETWORK_DNS_TYPE_TA         = 0x8000, /* DNSSEC Trust Authorities                           */
  NETWORK_DNS_TYPE_DLV        = 0x8001  /* DNSSEC Lookaside Validation (Obsolete)             */
} network_dns_type_t;


extern uint16_t network_dns_type_parse(const char *);
extern const char *network_dns_type_format(uint16_t);


typedef enum {
  NETWORK_DNS_CLASS_IN   = 0x0001, /* Internet Class         */
  NETWORK_DNS_CLASS_CS   = 0x0002, /* CsNet Class (Obsolete) */
  NETWORK_DNS_CLASS_CH   = 0x0003, /* Chaos Class            */
  NETWORK_DNS_CLASS_HS   = 0x0004, /* Hesiod Class           */
  NETWORK_DNS_CLASS_NONE = 0x00FE, /* None Class             */
  NETWORK_DNS_CLASS_ANY  = 0x00FF  /* Any Class              */
} network_dns_class_t;


extern uint16_t network_dns_class_parse(const char *);
extern const char *network_dns_class_format(uint16_t);


/*
 *                                  1  1  1  1  1  1
 *    0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  |                                               |
 *  /                     QNAME                     /
 *  /                                               /
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  |                     QTYPE                     |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  |                     QCLASS                    |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 */
struct network_dns_question {
  network_dns_name_t  name;
  network_dns_type_t  qtype;
  network_dns_class_t qclass;
};

struct network_dns_question_record {
  uint16_t qtype,
           qclass;
};

/*
 *                                  1  1  1  1  1  1
 *    0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  |                                               |
 *  /                                               /
 *  /                      NAME                     /
 *  |                                               |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  |                      TYPE                     |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  |                     CLASS                     |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  |                      TTL                      |
 *  |                                               |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  |                   RDLENGTH                    |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--|
 *  /                     RDATA                     /
 *  /                                               /
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 */
struct network_dns_resource {
	network_dns_name_t  name;
  network_dns_type_t  rtype;
  network_dns_class_t rclass;
  uint32_t            ttl;
  uint16_t            length;
};

struct __attribute__((packed)) network_dns_resource_record {
  uint16_t rtype,
           rclass;
  uint32_t ttl;
  uint16_t length;
};


/*
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  |                    ADDRESS                    |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 */
typedef char network_dns_addr4_t[NETWORK_IPADDR4_STRLENGTH];

typedef struct network_dns_a {
  network_dns_addr4_t a;
} NETWORK_DNS_A;


extern int network_dns_a_new(struct network_dns_a *, network_dns_addr4_t);


typedef char network_dns_addr6_t[NETWORK_IPADDR6_STRLENGTH];

typedef struct network_dns_aaaa {
  network_dns_addr6_t aaaa;
} NETWORK_DNS_AAAA;


extern int network_dns_aaaa_new(struct network_dns_aaaa *, network_dns_addr6_t);


/*
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  /                     CNAME                     /
 *  /                                               /
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 */
typedef struct network_dns_cname {
  network_dns_name_t cname;
} NETWORK_DNS_CNAME;


/*
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  |                  PREFERENCE                   |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  /                   EXCHANGE                    /
 *  /                                               /
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 */
typedef struct network_dns_mx {
  uint16_t           preference;
  network_dns_name_t exchange;
} NETWORK_DNS_MX;

struct network_dns_mx_record {
  uint16_t preference;
};


/*
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  /                   NSDNAME                     /
 *  /                                               /
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 */
typedef struct network_dns_ns {
  network_dns_name_t nsname;
} NETWORK_DNS_NS;


/*
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  /                   PTRDNAME                    /
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 */
typedef struct network_dns_ptr {
  network_dns_name_t ptr;
} NETWORK_DNS_PTR;


/*
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  /                     MNAME                     /
 *  /                                               /
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  /                     RNAME                     /
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  |                    SERIAL                     |
 *  |                                               |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  |                    REFRESH                    |
 *  |                                               |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  |                     RETRY                     |
 *  |                                               |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  |                    EXPIRE                     |
 *  |                                               |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  |                    MINIMUM                    |
 *  |                                               |
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 */
typedef struct network_dns_soa {
  network_dns_name_t mname,
                     rname;
  uint32_t           serial,
                     refresh,
                     retry,
                     expire,
                     minimum;
} NETWORK_DNS_SOA;

struct network_dns_soa_record {
  uint32_t serial,
           refresh,
           retry,
           expire,
           minimum;
};


/*
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *  /                   TXT-DATA                    /
 *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 */
#define NETWORK_DNS_TXT_DATA_LENGTH 0x00FF

typedef char network_dns_txt_data_t[NETWORK_DNS_TXT_DATA_LENGTH];


typedef struct network_dns_txt {
  network_dns_txt_data_t txt;
} NETWORK_DNS_TXT;


extern int network_dns_txt_new(struct network_dns_txt *, const char *);


typedef struct network_dns_parser network_dns_parser_t;


extern struct network_dnshdr *network_dns_parser_header(network_dns_parser_t *);

extern int network_dns_parser_question(network_dns_parser_t *, struct network_dns_question *);
extern void network_dns_parser_questions(network_dns_parser_t *);
extern int network_dns_parser_skipquestion(network_dns_parser_t *);
extern int network_dns_parser_skipquestions(network_dns_parser_t *);
extern uint16_t network_dns_parser_nquestions(network_dns_parser_t *);

extern void network_dns_parser_answers(network_dns_parser_t *);
extern void network_dns_parser_skipanswers(network_dns_parser_t *);
extern uint16_t network_dns_parser_nanswers(network_dns_parser_t *);

extern void network_dns_parser_authority(network_dns_parser_t *);
extern void network_dns_parser_skipauthority(network_dns_parser_t *);
extern uint16_t network_dns_parser_nauthority(network_dns_parser_t *);

extern void network_dns_parser_additional(network_dns_parser_t *);
extern void network_dns_parser_skipadditional(network_dns_parser_t *);
extern uint16_t network_dns_parser_nadditional(network_dns_parser_t *);

extern int network_dns_parser_resource(network_dns_parser_t *, struct network_dns_resource *);
extern int network_dns_parser_skipresource(network_dns_parser_t *);

extern struct network_dns_a *network_dns_parser_a(network_dns_parser_t *, struct network_dns_a *);
extern struct network_dns_aaaa *network_dns_parser_aaaa(network_dns_parser_t *, struct network_dns_aaaa *);

extern struct network_dns_cname *network_dns_parser_cname(network_dns_parser_t *, struct network_dns_cname *);

extern struct network_dns_mx *network_dns_parser_mx(network_dns_parser_t *, struct network_dns_mx *);

extern struct network_dns_ns *network_dns_parser_ns(network_dns_parser_t *, struct network_dns_ns *);

extern struct network_dns_ptr *network_dns_parser_ptr(network_dns_parser_t *, struct network_dns_ptr *);

extern struct network_dns_soa *network_dns_parser_soa(network_dns_parser_t *, struct network_dns_soa *);

extern struct network_dns_txt *network_dns_parser_txt(network_dns_parser_t *, struct network_dns_txt *);

extern network_dns_parser_t *network_dns_parser_new(const uint8_t *, size_t);
extern void network_dns_parser_free(network_dns_parser_t *);


typedef struct network_dns_builder network_dns_builder_t;


extern void network_dns_builder_setcompression(network_dns_builder_t *, bool);

extern void network_dns_builder_questions(network_dns_builder_t *);
extern void network_dns_builder_question(network_dns_builder_t *, struct network_dns_question *);
extern uint16_t network_dns_builder_nquestions(network_dns_builder_t *);

extern void network_dns_builder_answers(network_dns_builder_t *);
extern uint16_t network_dns_builder_nanswers(network_dns_builder_t *);

extern void network_dns_builder_authority(network_dns_builder_t *);
extern uint16_t network_dns_builder_nauthority(network_dns_builder_t *);

extern void network_dns_builder_additional(network_dns_builder_t *);
extern uint16_t network_dns_builder_nadditional(network_dns_builder_t *);

extern void network_dns_builder_resource(network_dns_builder_t *, struct network_dns_resource *);

extern void network_dns_builder_a(network_dns_builder_t *, struct network_dns_a *);
extern void network_dns_builder_aaaa(network_dns_builder_t *, struct network_dns_aaaa *);

extern void network_dns_builder_cname(network_dns_builder_t *, struct network_dns_cname *);

extern void network_dns_builder_mx(network_dns_builder_t *, struct network_dns_mx *);

extern void network_dns_builder_ns(network_dns_builder_t *, struct network_dns_ns *);

extern void network_dns_builder_ptr(network_dns_builder_t *, struct network_dns_ptr *);

extern void network_dns_builder_soa(network_dns_builder_t *, struct network_dns_soa *);

extern void network_dns_builder_txt(network_dns_builder_t *, struct network_dns_txt *);

extern size_t network_dns_builder_build(network_dns_builder_t *);

extern network_dns_builder_t *network_dns_builder_new(struct network_dnshdr *, uint8_t *, size_t);
extern void network_dns_builder_free(network_dns_builder_t *);

#ifdef __cplusplus
}
#endif

#endif
