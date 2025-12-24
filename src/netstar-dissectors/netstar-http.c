#include "netstar-dissectors.h"
// #include "netstar-http.h"

#include <strings/strings.h>

#include <netstar.h>


extern long string_safefind(const char *, size_t, const char *, size_t, bool);


static bool
netstar_dissector_dissect_http_version_supported(const char *payload, size_t payload_length) {
  static char *versions[] = { "HTTP/0.9", "HTTP/1.0", "HTTP/1.1", NULL };
  size_t version;

  for (version = 0; versions[version]; version++)
    if (string_safefind(payload, payload_length, versions[version], 8, false) != -1)
      return true;

  return false;
}

static inline bool
netstar_dissector_dissect_http_header(const char *request, const char *header, long *start, long *end) {
  if ((*start = string_find(request, header, false)) == -1)
    return false;
  *start += string_length(header);

  if ((*end = string_find(request+*start, "\n", true)) <= 0)
    return false;
  if (request[(*end)-1] == '\r')
    (*end)--;

  return true;
}

static inline void
netstar_dissector_dissect_http(netstar_dissector_t *dissector, struct netstar_capture_packet *packet) {
  const char *payload = (const char *)packet->layer4.payload;
  long breakline = -1, headers = -1, start = -1, end = -1;

  if (!payload || !packet->layer4.payload_length)
    return;
  if (!netstar_dissector_dissect_http_version_supported(payload, packet->layer4.payload_length))
    return;

/*
  if ((breakline = string_safefind(payload, packet->layer4.payload_length, "\n", 1, true)) <= 0)
    return;
  if (payload[breakline-1] == '\r')
    breakline--;

  if ((headers = string_safefind(payload, packet->layer4.payload_length, "\n\n", 2, true)) <= 0 &&
      (headers = string_safefind(payload, packet->layer4.payload_length,  "\r\n\r\n", 4, true)) <= 0)
    return;

  netstar_dissector_write(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, payload, (size_t)breakline);

/*
  if (netstar_dissector_dissect_http_header(payload, "Host: ", &start, &end))
    netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, "  %.*s", (size_t)end, payload+start);

  if (netstar_dissector_dissect_http_header(payload, "User-Agent: ", &start, &end))
    netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, "  %.*s", (size_t)end, payload+start);

  if (netstar_dissector_dissect_http_header(payload, "Server: ", &start, &end))
    netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, "  %.*s", (size_t)end, payload+start);

  if ((server = string_find(payload, "Server: ", false)) > 0 && host < headers) {
    if ((breakline = string_find(payload+server+8, "\n", true)) <= 0)
      return;
    if (payload[server+8+breakline-1] == '\r')
      breakline--;

    netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, " %.*s", (size_t)breakline, payload+server+8);
  }

  if ((host = string_find(payload, "Host: ", false)) > 0 && host < headers) {
    if ((breakline = string_find(payload+host+6, "\n", true)) <= 0)
      return;
    if (payload[host+6+breakline-1] == '\r')
      breakline--;

    netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, " %.*s", (size_t)breakline, payload+host+6);
  }

  if ((useragent = string_find(payload, "User-Agent: ", false)) > 0 && useragent < headers) {
    if ((breakline = string_find(payload+useragent+12, "\n", true)) <= 0)
      return;
    if (payload[useragent+12+breakline-1] == '\r')
      breakline--;

    netstar_dissector_format(dissector, NETSTAR_DISSECTOR_PACKET_INFORMATION, " %.*s", (size_t)breakline, payload+useragent+12);
  }
*/
}


static struct netstar_dissector_protocol netstar_dissector_http = {
  .routine = netstar_dissector_dissect_http, .type = NETSTAR_DISSECTOR_TCP_PROTOCOL, .layer = NETSTAR_DISSECTOR_LAYER7, .service = NETSTAR_HTTP_PROTOCOL_PORT
};


__attribute__((__constructor__))
static void
netstar_dissector_http_register(void) {
  netstar_dissectors_register(&netstar_dissector_http);
}
