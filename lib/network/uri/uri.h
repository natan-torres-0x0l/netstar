#ifndef _NETWORK_URI_URI_H
#define _NETWORK_URI_URI_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  char *scheme, *userinfo, *host, *port, *authority, *path, *query, *pathquery, *fragment, *request;
  const char *url;
} URI, uri_t;


uri_t *uri_new(const char *);
void uri_free(uri_t *);

#ifdef __cplusplus
}
#endif

#endif
