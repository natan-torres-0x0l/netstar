#include "uri.h"

#include <strings/strings.h>
#include <stdlib.h>

#include <regex.h>

#include <stdbool.h>
#include <stddef.h>


enum {
  REGEX_SCHEME = 1, REGEX_USERINFO, REGEX_HOST, REGEX_PORT, REGEX_PATH, REGEX_QUERY, REGEX_FRAGMENT,
  REGEX_MATCHES,
};


static const char *regex_pattern = "^([A-Za-z][A-Za-z0-9\\+\\-\\.]+://)?([A-Za-z0-9\\-\\.\\_%\\!\\$\\&\\'\\(\\)\\*\\+\\,;=:]+@)?([A-Za-z0-9\\-\\.\\_%]+|\\[[A-Fa-f0-9:\\.]+\\])(:[0-9]+)?(/[a-zA-Z0-9\\-\\.\\_%\\!\\$\\&\\'\\(\\)\\*\\+\\,;=:@]+)?(\\?[a-zA-Z0-9\\-\\.\\_%\\!\\$\\&\\'\\(\\)\\*\\+\\,;=:@?/]+)?(#[a-zA-Z0-9\\-\\.\\_%\\!\\$\\&\\'\\(\\)\\*\\+\\,;=:@?/]+)?$"; // [0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}
static regex_t regex;


__attribute__((__constructor__))
void
regex_pattern_compile(void) {
  regcomp(&regex, regex_pattern, REG_EXTENDED);
}

__attribute__((__destructor__))
void
regex_pattern_free(void) {
  regfree(&regex);
}

uri_t *
uri_new(const char *url) {
/*
 *  [ scheme ]://[ userinfo ]@[ host ]:[ port ]/[ path ]?[ query ]#[ fragment ]
 *                    |                   |       |          |           |
 *                    +--------+----------+       +-----+----+           |
 *                             |                        |                |
 *                        [ authority ]          [ path & query ]        |
 *                                                      |                |
 *                                                      +-------+--------+
 *                                                              |
 *                                                        [ request URI ]
 */

  regmatch_t regex_matches[REGEX_MATCHES] = {0};

  uri_t *uri = NULL;

  if (regexec(&regex, url, REGEX_MATCHES, regex_matches, 0) != 0 || !(uri = (uri_t *)calloc(1, sizeof(uri_t))))
    return NULL; 

  uri->url = url;

/*
 *  [ scheme ]://
 */
  uri->scheme = string_substr(uri->url, regex_matches[REGEX_SCHEME].rm_so, regex_matches[REGEX_SCHEME].rm_eo-3);

/*
 *  @[ userinfo ]
 */
  uri->userinfo = string_substr(uri->url, regex_matches[REGEX_USERINFO].rm_so, regex_matches[REGEX_USERINFO].rm_eo-1);

/*
 *  IP-v6: [ FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF ]
 *  IP-v4: [ 255.255.255.255 ]
 *  Name:  [ www.example.com ]
 */
  if (uri->url[regex_matches[REGEX_HOST].rm_so] == '[')
    uri->host = string_substr(uri->url, regex_matches[REGEX_HOST].rm_so+1, regex_matches[REGEX_HOST].rm_eo-1);
  else
    uri->host = string_substr(uri->url, regex_matches[REGEX_HOST].rm_so, regex_matches[REGEX_HOST].rm_eo);

/*
 *  :[ port ]
 */
  uri->port = string_substr(uri->url, regex_matches[REGEX_PORT].rm_so+1, regex_matches[REGEX_PORT].rm_eo);

/*
 *  [ userinfo ]@[ host ]:[ port ]
 */
  if (uri->url[regex_matches[REGEX_HOST].rm_so] == '[')
    uri->authority = string_format("%s%s[%s]%s%s", ((uri->userinfo) ? uri->userinfo : ""), ((uri->userinfo) ? "@" : ""), uri->host, ((uri->port) ? ":" : ""), ((uri->port) ? uri->port : ""));
  else
    uri->authority = string_format("%s%s%s%s%s", ((uri->userinfo) ? uri->userinfo : ""), ((uri->userinfo) ? "@" : ""), uri->host, ((uri->port) ? ":" : ""), ((uri->port) ? uri->port : ""));

/*
 *  /[ path ]
 */
  if (!(uri->path = string_substr(uri->url, regex_matches[REGEX_PATH].rm_so, regex_matches[REGEX_PATH].rm_eo)))
    uri->path = string_new("/");

/*
 *  /[ path ]?[ query ]
 */
  uri->pathquery = string_format("%s%s%s", uri->path, ((uri->query) ? "?" : ""), ((uri->query) ? uri->query : ""));

/*
 *  ?[ query ]
 */
  uri->query = string_substr(uri->url, regex_matches[REGEX_QUERY].rm_so+1, regex_matches[REGEX_QUERY].rm_eo);

/*
 *  #[ fragment ]
 */
  uri->fragment = string_substr(uri->url, regex_matches[REGEX_FRAGMENT].rm_so+1, regex_matches[REGEX_FRAGMENT].rm_eo);

/*
 *  /[ path ]?[ query ]#[ fragment ]
 */
  uri->request = string_format("%s%s%s", uri->pathquery, ((uri->pathquery) ? "#" : ""), ((uri->fragment) ? uri->fragment : "")); 

  return uri;
}

void
uri_free(uri_t *uri) {
  if (uri) {
    if (uri->scheme)
      free(uri->scheme), uri->scheme = NULL;

    if (uri->userinfo)
      free(uri->userinfo), uri->userinfo = NULL;

    if (uri->host)
      free(uri->host), uri->host = NULL;

    if (uri->port)
      free(uri->port), uri->port = NULL;

    if (uri->authority)
      free(uri->authority), uri->authority = NULL;

    if (uri->path)
      free(uri->path), uri->path = NULL;

    if (uri->query)
      free(uri->query), uri->query = NULL;

    if (uri->pathquery)
      free(uri->pathquery), uri->pathquery = NULL;

    if (uri->fragment)
      free(uri->fragment), uri->fragment = NULL;

    if (uri->request)
      free(uri->request), uri->request = NULL; 

    uri->url = NULL;
    free(uri);
  }
}
