#ifndef _URI_URI_H
#define _URI_URI_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  char *scheme, *userinfo, *host, *port, *authority, *path, *query, *pathquery, *fragment, *request;
  const char *url;
} URI, uri_t;


uri_t *uri_new(const char *);
#define URI_new(url) uri_new(url)
void uri_free(uri_t *);
#define URI_free(uri) uri_free(uri)

#ifdef __cplusplus
}
#endif

#endif
