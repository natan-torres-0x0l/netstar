#include "netstar-http-server.h"

#include <netstar-services.h>

#include <netstar-utils.h>

#include <collections/hashmap/hashmap.h>
#include <buffers/buffers.h>

#include <strings/strings.h>
#include <string.h>

#include <stdlib.h>
#include <stdio.h>

#define NETSTAR_HTTP_SERVER_FILESYSTEM       "share/server"

#define NETSTAR_HTTP_SERVER_NAME             "netstar/1.0"
#define NETSTAR_HTTP_SERVER_PROTOCOL_VERSION "HTTP/1.1"

/* 1xx Informational */
#define NETSTAR_HTTP_STATUS_CONTINUE                        "100 Continue"
#define NETSTAR_HTTP_STATUS_SWITCHING_PROTOCOLS             "101 Switching Protocols"
#define NETSTAR_HTTP_STATUS_PROCESSING                      "102 Processing"
#define NETSTAR_HTTP_STATUS_EARLY_HINTS                     "103 Early Hints"

/* 2xx Success */
#define NETSTAR_HTTP_STATUS_OK                              "200 OK"
#define NETSTAR_HTTP_STATUS_CREATED                         "201 Created"
#define NETSTAR_HTTP_STATUS_ACCEPTED                        "202 Accepted"
#define NETSTAR_HTTP_STATUS_NON_AUTHORITATIVE_INFO          "203 Non-Authoritative Information"
#define NETSTAR_HTTP_STATUS_NO_CONTENT                      "204 No Content"
#define NETSTAR_HTTP_STATUS_RESET_CONTENT                   "205 Reset Content"
#define NETSTAR_HTTP_STATUS_PARTIAL_CONTENT                 "206 Partial Content"
#define NETSTAR_HTTP_STATUS_MULTI_STATUS                    "207 Multi-Status"
#define NETSTAR_HTTP_STATUS_ALREADY_REPORTED                "208 Already Reported"
#define NETSTAR_HTTP_STATUS_IM_USED                         "226 IM Used"

/* 3xx Redirection */
#define NETSTAR_HTTP_STATUS_MULTIPLE_CHOICES                "300 Multiple Choices"
#define NETSTAR_HTTP_STATUS_MOVED_PERMANENTLY               "301 Moved Permanently"
#define NETSTAR_HTTP_STATUS_FOUND                           "302 Found"
#define NETSTAR_HTTP_STATUS_SEE_OTHER                       "303 See Other"
#define NETSTAR_HTTP_STATUS_NOT_MODIFIED                    "304 Not Modified"
#define NETSTAR_HTTP_STATUS_USE_PROXY                       "305 Use Proxy"
/* 306 Unused */
#define NETSTAR_HTTP_STATUS_TEMPORARY_REDIRECT              "307 Temporary Redirect"
#define NETSTAR_HTTP_STATUS_PERMANENT_REDIRECT              "308 Permanent Redirect"

/* 4xx Client Error */
#define NETSTAR_HTTP_STATUS_BAD_REQUEST                     "400 Bad Request"
#define NETSTAR_HTTP_STATUS_UNAUTHORIZED                    "401 Unauthorized"
#define NETSTAR_HTTP_STATUS_PAYMENT_REQUIRED                "402 Payment Required"
#define NETSTAR_HTTP_STATUS_FORBIDDEN                       "403 Forbidden"
#define NETSTAR_HTTP_STATUS_NOT_FOUND                       "404 Not Found"
#define NETSTAR_HTTP_STATUS_METHOD_NOT_ALLOWED              "405 Method Not Allowed"
#define NETSTAR_HTTP_STATUS_NOT_ACCEPTABLE                  "406 Not Acceptable"
#define NETSTAR_HTTP_STATUS_PROXY_AUTH_REQUIRED             "407 Proxy Authentication Required"
#define NETSTAR_HTTP_STATUS_REQUEST_TIMEOUT                 "408 Request Timeout"
#define NETSTAR_HTTP_STATUS_CONFLICT                        "409 Conflict"
#define NETSTAR_HTTP_STATUS_GONE                            "410 Gone"
#define NETSTAR_HTTP_STATUS_LENGTH_REQUIRED                 "411 Length Required"
#define NETSTAR_HTTP_STATUS_PRECONDITION_FAILED             "412 Precondition Failed"
#define NETSTAR_HTTP_STATUS_REQUEST_ENTITY_TOO_LARGE        "413 Request Entity Too Large"
#define NETSTAR_HTTP_STATUS_REQUEST_URI_TOO_LONG            "414 Request-URI Too Long"
#define NETSTAR_HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE          "415 Unsupported Media Type"
#define NETSTAR_HTTP_STATUS_REQUESTED_RANGE_NOT_SATISFIABLE "416 Requested Range Not Satisfiable"
#define NETSTAR_HTTP_STATUS_EXPECTATION_FAILED              "417 Expectation Failed"
#define NETSTAR_HTTP_STATUS_TEAPOT                          "418 I'm a teapot"
#define NETSTAR_HTTP_STATUS_MISDIRECTED_REQUEST             "421 Misdirected Request"
#define NETSTAR_HTTP_STATUS_UNPROCESSABLE_ENTITY            "422 Unprocessable Entity"
#define NETSTAR_HTTP_STATUS_LOCKED                          "423 Locked"
#define NETSTAR_HTTP_STATUS_FAILED_DEPENDENCY               "424 Failed Dependency"
#define NETSTAR_HTTP_STATUS_TOO_EARLY                       "425 Too Early"
#define NETSTAR_HTTP_STATUS_UPGRADE_REQUIRED                "426 Upgrade Required"
#define NETSTAR_HTTP_STATUS_PRECONDITION_REQUIRED           "428 Precondition Required"
#define NETSTAR_HTTP_STATUS_TOO_MANY_REQUESTS               "429 Too Many Requests"
#define NETSTAR_HTTP_STATUS_REQUEST_HEADER_FIELDS_TOO_LARGE "431 Request Header Fields Too Large"
#define NETSTAR_HTTP_STATUS_UNAVAILABLE_FOR_LEGAL_REASONS   "451 Unavailable For Legal Reasons"

/* 5xx Server Error */
#define NETSTAR_HTTP_STATUS_INTERNAL_SERVER_ERROR           "500 Internal Server Error"
#define NETSTAR_HTTP_STATUS_NOT_IMPLEMENTED                 "501 Not Implemented"
#define NETSTAR_HTTP_STATUS_BAD_GATEWAY                     "502 Bad Gateway"
#define NETSTAR_HTTP_STATUS_SERVICE_UNAVAILABLE             "503 Service Unavailable"
#define NETSTAR_HTTP_STATUS_GATEWAY_TIMEOUT                 "504 Gateway Timeout"
#define NETSTAR_HTTP_STATUS_HTTP_VERSION_NOT_SUPPORTED      "505 HTTP Version Not Supported"
#define NETSTAR_HTTP_STATUS_VARIANT_ALSO_NEGOTIATES         "506 Variant Also Negotiates"
#define NETSTAR_HTTP_STATUS_INSUFFICIENT_STORAGE            "507 Insufficient Storage"
#define NETSTAR_HTTP_STATUS_LOOP_DETECTED                   "508 Loop Detected"
#define NETSTAR_HTTP_STATUS_NOT_EXTENDED                    "510 Not Extended"
#define NETSTAR_HTTP_STATUS_NETWORK_AUTHENTICATION_REQUIRED "511 Network Authentication Required"

#define NETSTAR_HTTP_SERVER_PORT             8080
#define NETSTAR_HTTP_SERVER_BACKLOG          SOMAXCONN

#define NETSTAR_HTTP_SERVER_RESPONSE_LENGTH  4096
#define NETSTAR_HTTP_REQUEST_LENGTH          4096


static bool netstar_http_server_initialized, netstar_http_server_started;


#define NETSTAR_HTTP_REQUEST_METHOD_LENGTH    10  // @ supported methods: GET|POST
#define NETSTAR_HTTP_REQUEST_VERSION_LENGTH   10  // @ supported version: HTTP/1.1
#define NETSTAR_HTTP_REQUEST_RESOURCE_LENGTH  256
#define NETSTAR_HTTP_REQUEST_USERAGENT_LENGTH 512

struct netstar_http_request {
  char method[NETSTAR_HTTP_REQUEST_METHOD_LENGTH], version[NETSTAR_HTTP_REQUEST_VERSION_LENGTH];
  char resource[NETSTAR_HTTP_REQUEST_RESOURCE_LENGTH];

  hashmap_t *headers;
};

static const char *netstar_http_request_supported_methods[] = {
  "HEAD", "GET", "POST", "PUT", NULL
};

static bool
netstar_http_request_method_is_supported(const char *method) {
  const char **supported_methods = netstar_http_request_supported_methods;

  for (; *supported_methods; supported_methods++)
    if (string_equals(method, *supported_methods, false))
      return true;

  return false;
}

static const char *netstar_http_request_supported_versions[] = {
  "HTTP/0.9", "HTTP/1.0", "HTTP/1.1", NULL
};

static bool
netstar_http_request_version_is_supported(const char *version) {
  const char **supported_versions = netstar_http_request_supported_versions;

  for (; *supported_versions; supported_versions++)
    if (string_equals(version, *supported_versions, false))
      return true;

  return false;
}

static int
netstar_http_request_new(struct netstar_http_request *request, const char *buffer) {
  char *request_line = NULL, **request_fields = NULL;
  long end = -1;

  int status = -1;

  if ((end = string_find(buffer, "\r\n\r\n", true)) == -1 && (end = string_find(buffer, "\n\n", true)) == -1)
    goto _return;
  if ((end = string_find(buffer, "\r\n", true)) == -1 && (end = string_find(buffer, "\n", true)) == -1)
    goto _return;

  if (!(request_line = string_substr(buffer, 0, end)))
    goto _return;
  buffer += (size_t)end;

  if (!(request_fields = string_fields(request_line)))
    goto _return;

  if (!request_fields[0] || !netstar_http_request_method_is_supported(request_fields[0]))
    goto _return;
  string_safecopy(request->method, sizeof(request->method), request_fields[0], string_length(request_fields[0]));

  if (!request_fields[1])
    goto _return;
  string_safecopy(request->resource, sizeof(request->resource), request_fields[1], string_length(request_fields[1]));

  if (!request_fields[2] || !netstar_http_request_version_is_supported(request_fields[2]))
    goto _return;
  string_safecopy(request->version, sizeof(request->version), request_fields[2], string_length(request_fields[2]));

// if (!(request->headers = netstar_http_request_headers_new(buffers, free)))
//   goto _return;

  status = 1;

_return:
  netstar_utils_strings_free(request_fields);
  free(request_line);

  return status;
}


typedef enum {
  NETSTAR_HTTP_SERVER_CLIENT_STATE_IDLE,
  NETSTAR_HTTP_SERVER_CLIENT_STATE_RECEIVE,
  NETSTAR_HTTP_SERVER_CLIENT_STATE_REQUEST,
  NETSTAR_HTTP_SERVER_CLIENT_STATE_RESPONSE,
  NETSTAR_HTTP_SERVER_CLIENT_STATE_TERMINATE
} netstar_http_server_client_state_t;

struct netstar_http_server_client {
  netstar_http_server_client_state_t state;

  struct netstar_http_request request;

  struct network_endpoint addr;
  socket_t socket;

  struct netstar_http_server *server;

  buffer_t *buffer;

  netstar_timer_t deadline_timer;
  netstar_time_t deadline;
};


static void netstar_http_server_client_free(struct netstar_http_server_client *);


static struct netstar_http_server_client *
netstar_http_server_client_accept(struct netstar_http_server *server) {
  struct netstar_http_server_client *client = NULL;

  if (!(client = (struct netstar_http_server_client *)calloc(1, sizeof(struct netstar_http_server_client))))
    goto _return;

  network_endpoint_new(&client->addr, "::", 0);

  if ((client->socket = socket_accept(server->socket, (struct sockaddr *)&client->addr, &client->addr.size, &SOCKET_NONBLOCKING)) == SOCKET_INVALID)
    goto _return;

  if (socket_settcpnodelay(client->socket, true) == -1)
    goto _return;

  if (!(client->buffer = buffer_new()))
    goto _return;

  netstar_timer_start(&client->deadline_timer, 4 /* 10s */);

  client->state = NETSTAR_HTTP_SERVER_CLIENT_STATE_RECEIVE;

  return client;

_return:
  netstar_http_server_client_free(client);
  return NULL;
}

static void
netstar_http_server_client_free(struct netstar_http_server_client *client) {
  if (client) {
    if (client->socket != SOCKET_INVALID)
      socket_close(client->socket), client->socket = SOCKET_INVALID;
    
    buffer_free(client->buffer);

    free(client);
  }
}


typedef void (*netstar_http_server_handle_routine_t)(struct netstar_http_request *, struct netstar_http_server_client *);

struct netstar_http_server_handle {
  char pattern[NETSTAR_HTTP_REQUEST_RESOURCE_LENGTH];
  netstar_http_server_handle_routine_t routine;
};


static void
netstar_http_server_handle(struct netstar_http_server *server, const char *pattern, netstar_http_server_handle_routine_t routine) {
  struct netstar_http_server_handle handle = { .routine = routine };

  string_safecopy(handle.pattern, sizeof(handle.pattern), pattern, string_length(pattern));

  hashset_insert(server->handlers, hashset_clvalue(&handle, sizeof(struct netstar_http_server_handle)));
}

static void
netstar_http_server_handlers(struct netstar_http_server *server, struct netstar_http_request *request, struct netstar_http_server_client *client) {
  struct netstar_http_server_handle *handler = NULL, *redirect_handler = NULL;
  hashset_iterator_t handlers_iter = NULL;

  size_t pattern_length, preffix_length = 0;

  for (handlers_iter = hashset_begin(server->handlers); handlers_iter; handlers_iter = hashset_next(handlers_iter)) {
    handler = (struct netstar_http_server_handle *)hashset_value(handlers_iter);
    pattern_length = string_length(handler->pattern);

    if (string_match(request->resource, handler->pattern, (long)pattern_length, true) && pattern_length > preffix_length) {
      preffix_length = pattern_length;
      redirect_handler = handler;
    }
  }

  if (redirect_handler)
    redirect_handler->routine(request, client);
}

static int
netstar_http_server_client_read(struct netstar_http_server_client *client) {
  char buffer[NETSTAR_HTTP_REQUEST_LENGTH] = {0};
  ssize_t received = -1;

  for (;;) {
    received = socket_recv(client->socket, buffer, sizeof(buffer), 0, &SOCKET_NONBLOCKING);
    if (received <= 0 && socket_geterror())
      return 0;
    if (received == -1)
      return -1;

    buffer_append(client->buffer, buffer, (size_t)received);

    if (string_endswith(buffer, "\r\n\r\n", true) || string_endswith(buffer, "\n\n", true)) {
      buffer_append(client->buffer,"\0", 1);

      client->state = NETSTAR_HTTP_SERVER_CLIENT_STATE_REQUEST;
      break;
    }
  }

  return 1;
}

static int
netstar_http_server_handle_client_request(struct netstar_http_server *server, struct netstar_http_server_client *client) {
  if (netstar_http_request_new(&client->request, (const char *)buffer_ptr(client->buffer)) == -1)
    return -1;

  netstar_http_server_handlers(server, &client->request, client);
  return 1;
}

#define netstar_http_server_client_disconnect(client) {              \
  socket_reactor_remove(server_reactor, client->socket);             \
  hashmap_remove(server_clients, &client->socket, sizeof(socket_t)); \
  netstar_http_server_client_free(client);                           \
}

static void *
netstar_http_server_handle_client(void *context) {
  netstar_thread_t *thread = (netstar_thread_t *)context;
  struct netstar_http_server *server = (struct netstar_http_server *)thread->args;

  static struct socket_timeout server_reactor_timeout = { .sec = 2, .usec = 0 }; 
  socket_reactor_t *server_reactor = NULL;

// hashmap_iterator_t server_clients_iter = NULL;
  hashmap_t *server_clients = NULL;

  if (!(server_reactor = socket_reactor_new()) || !(server_clients = hashmap_new(free, NULL)))
    goto _return;

  socket_reactor_add(server_reactor, server->socket, SOCKET_READ);

  for (; thread->status;) {
    struct netstar_http_server_client *client = NULL;
    
    struct socket_event server_events[1024] = {0};
    int server_events_count, event;

    if ((server_events_count = socket_reactor_poll(server_reactor, server_events, 1024, &server_reactor_timeout)) <= 0)
      ; // continue;

    for (event = 0; event < server_events_count; event++) {
      if (server_events[event].socket == server->socket && (server_events[event].events & SOCKET_READ)) {
        if ((client = netstar_http_server_client_accept(server))) {
          network_ipaddr6_t *addr6 = network_endpoint_addr6(&client->addr);
          char addr[NETWORK_IPADDR6_STRLENGTH] = {0};

          netstar_log("\b \b[ http@server ] client %s:%u connected\r\n",
            network_ipaddr6_isv4mapped(addr6) ? network_ipaddr4_format((network_ipaddr4_t *)&addr6->u8[12], addr, sizeof(addr)) : network_ipaddr6_format(addr6, addr, sizeof(addr)), ntohs(network_endpoint_port(&client->addr))
          );

          hashmap_insert(server_clients, hashmap_clkey(&client->socket, sizeof(socket_t)), hashmap_rvalue(client, sizeof(struct netstar_http_server_client *)));
          socket_reactor_add(server_reactor, client->socket, SOCKET_READ);
        }

        continue;
      }

      if (!(client = (struct netstar_http_server_client *)hashmap_get(server_clients, &server_events[event].socket, sizeof(socket_t))))
        continue;

      switch (client->state) {
        case NETSTAR_HTTP_SERVER_CLIENT_STATE_RECEIVE: {
          int read = -1;

       // client->deadline = netstar_timer(&client->deadline_timer);

          if (/* !client->deadline || */ (read = netstar_http_server_client_read(client)) == 0) {
            client->state = NETSTAR_HTTP_SERVER_CLIENT_STATE_TERMINATE;
            break;
          } else if (read == -1)
            break;

          client->state = NETSTAR_HTTP_SERVER_CLIENT_STATE_REQUEST;
       // fallthrough
        }

        case NETSTAR_HTTP_SERVER_CLIENT_STATE_REQUEST: {
          network_ipaddr6_t *addr6 = network_endpoint_addr6(&client->addr);
          char addr[NETWORK_IPADDR6_STRLENGTH] = {0};

          if (netstar_http_server_handle_client_request(server, client) == -1) {
            client->state = NETSTAR_HTTP_SERVER_CLIENT_STATE_TERMINATE;
            break;
          }

          netstar_log("\b \b[ http@server ] client %s:%u %s %s %s\r\n",
            network_ipaddr6_isv4mapped(addr6) ? network_ipaddr4_format((network_ipaddr4_t *)&addr6->u8[12], addr, sizeof(addr)) : network_ipaddr6_format(addr6, addr, sizeof(addr)), ntohs(network_endpoint_port(&client->addr)),
            client->request.method,
            client->request.resource,
            client->request.version
          );

          client->state = NETSTAR_HTTP_SERVER_CLIENT_STATE_TERMINATE;
       // fallthrough
        }

        case NETSTAR_HTTP_SERVER_CLIENT_STATE_TERMINATE:
        default: {
          network_ipaddr6_t *addr6 = network_endpoint_addr6(&client->addr);
          char addr[NETWORK_IPADDR6_STRLENGTH] = {0};

          netstar_log("\b \b[ http@server ] client %s:%u closed\r\n",
            network_ipaddr6_isv4mapped(addr6) ? network_ipaddr4_format((network_ipaddr4_t *)&addr6->u8[12], addr, sizeof(addr)) : network_ipaddr6_format(addr6, addr, sizeof(addr)), ntohs(network_endpoint_port(&client->addr))
          );

          netstar_http_server_client_disconnect(client);
        }
      }
    }
  }

_return:
  socket_reactor_free(server_reactor);
  hashmap_free(server_clients);

  netstar_thread_exit(thread, NULL);
  return NULL;
}

static buffer_t *
netstar_http_server_filesystem_read(const char *resource, const char *mode) {
  char resource_path[sizeof(NETSTAR_HTTP_SERVER_FILESYSTEM)+NETSTAR_HTTP_REQUEST_RESOURCE_LENGTH] = {0};

  snprintf(resource_path, sizeof(resource_path), "%s/%s", NETSTAR_HTTP_SERVER_FILESYSTEM, resource);

  return netstar_utils_filesystem_read(resource_path, mode);
}

static ssize_t
netstar_http_server_response_send(struct netstar_http_server_client *client, const void *buffer, size_t buffer_length) {
  size_t remaining_length, chunk, total_sent = 0;
  ssize_t sent;

  while (total_sent < buffer_length) {
    remaining_length = buffer_length-total_sent;
    chunk = ((remaining_length > 4096) ? 4096 : remaining_length);

    if ((sent = socket_send(client->socket, (const char *)buffer+total_sent, chunk, 0)) == -1)
      return -1;
    if (sent == 0)
      return 0;

    total_sent += (size_t)sent;
  }

  return (ssize_t)total_sent;
}

static ssize_t
netstar_http_server_response_headers(struct netstar_http_server_client *client, const char *status, const char *content_type, size_t content_length) {
  char headers[NETSTAR_HTTP_SERVER_RESPONSE_LENGTH] = {0};
  size_t shift = 0;

  snprintf(headers, sizeof(headers), NETSTAR_HTTP_SERVER_PROTOCOL_VERSION " %s\r\n"
                                     "Server: " NETSTAR_HTTP_SERVER_NAME "\r\n", status);

  shift += string_length(headers);
  snprintf(headers+shift, sizeof(headers)-shift, "Content-Type: %s\r\n"
                                                 "Content-Length: %zu\r\n"
                                                 "Connection: close\r\n\r\n", content_type, content_length);

  return netstar_http_server_response_send(client, headers, string_length(headers));
}

static ssize_t
netstar_http_server_response_content(struct netstar_http_server_client *client, buffer_t *content) {
  return netstar_http_server_response_send(client, buffer_ptr(content), buffer_length(content));
}

static void
netstar_http_server_response(struct netstar_http_request *request, struct netstar_http_server_client *client, const char *resource, const char *mime) {
  const char *mode = ((string_startswith(mime, "image", false)) ? "rb" : "r");
  buffer_t *content = NULL;

  if (!(content = netstar_http_server_filesystem_read(resource, mode)))
    return;

  if (netstar_http_server_response_headers(client, NETSTAR_HTTP_STATUS_OK, mime, buffer_length(content)) <= 0)
    return;
  if (netstar_http_server_response_content(client, content) <= 0)
    return;

  buffer_free(content);
}

static void
netstar_http_server_netstar_logo(struct netstar_http_request *request, struct netstar_http_server_client *client) {
  netstar_http_server_response(request, client, "netstar.png", "image/png");
}

static void
netstar_http_server_netstar_icon(struct netstar_http_request *request, struct netstar_http_server_client *client) {
  netstar_http_server_response(request, client, "netstar.ico", "image/x-icon");
}

static void
netstar_http_server_index(struct netstar_http_request *request, struct netstar_http_server_client *client) {
  netstar_http_server_response(request, client, "index.html", "text/html; charset=utf-8");
}

static int
netstar_http_server_transport_initialize(struct netstar_http_server *server) {
  int status = -1;

  if ((server->socket = socket_new(AF_INET6, SOCK_STREAM, IPPROTO_TCP)) == SOCKET_INVALID)
    goto _return;

  if (socket_setreuseaddr(server->socket, true) == -1)
    goto _return;

  if (socket_setipv6only(server->socket, false) == -1)
    goto _return;

  if (socket_settcpnodelay(server->socket, true) == -1)
    goto _return;

  network_endpoint_new(&server->addr, "::", NETSTAR_HTTP_SERVER_PORT);

  if (socket_bind(server->socket, &server->addr.base, server->addr.size) == -1)
    goto _return;

  server->backlog = NETSTAR_HTTP_SERVER_BACKLOG;
  if (socket_listen(server->socket, server->backlog) == -1)
    goto _return;

  status = 1;

_return:
  return status;
}

int
netstar_http_server_new(struct netstar_http_server *server) {
  if (netstar_http_server_transport_initialize(server) == -1)
    goto _return;
  if (!(server->handlers = hashset_new(free)))
    goto _return;

  netstar_http_server_handle(server, "/netstar.png", netstar_http_server_netstar_logo);
  netstar_http_server_handle(server, "/favicon.ico", netstar_http_server_netstar_icon);
  netstar_http_server_handle(server, "/", netstar_http_server_index);

  return 1;

_return:
  netstar_http_server_free(server);
  return -1;
}

void
netstar_http_server_free(struct netstar_http_server *server) {
  if (server) {
    hashset_free(server->handlers);

    if (server->socket > 0)
      socket_close(server->socket);
  }
}

static void
netstar_http_server_initialize(netstar_t *netstar, void *context) {
  if (netstar_http_server_initialized)
    return;

  netstar_http_server_initialized = true;
}

static void
netstar_http_server_deinitialize(void *context) {
  struct netstar_http_server *server = (struct netstar_http_server *)context;

  if (!netstar_http_server_initialized)
    return;

  netstar_http_server_free(server);

  netstar_http_server_initialized = false;
}

static void
netstar_http_server_start(void *context) {
  struct netstar_http_server *server = (struct netstar_http_server *)context;

  if (netstar_http_server_started)
    return;

  for (int thread = 0; thread < 4; thread++)
    netstar_thread_spawn(netstar_http_server_handle_client, server);

  netstar_http_server_started = true;
}

static void
netstar_http_server_stop(void *context) {
  struct netstar_http_server *server = (struct netstar_http_server *)context;

  if (!netstar_http_server_started)
    return;

  netstar_thread_kill(server->thread);

  netstar_http_server_started = false;
}

static struct netstar_http_server netstar_http_server_context;

static struct netstar_service netstar_http_server_context_service = {
  netstar_http_server_initialize, netstar_http_server_deinitialize,
  netstar_http_server_start, netstar_http_server_stop,
  "netstar:http:server", &netstar_http_server_context,
};

struct netstar_service *
netstar_http_server_service(void) {
  return &netstar_http_server_context_service;
}


__attribute__((__constructor__))
static inline void
netstar_http_register_service(void) {
  netstar_services_register(netstar_http_server_service());
}
