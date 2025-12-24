#include "macieee802.h"

#include <strings/strings.h>
#include <string.h>

#include <stdio.h>


const struct macieee802 macieee802_broadcast    = MACIEEE802_IBROADCAST;

const struct macieee802 macieee802_v6allnodes   = MACIEEE802_IV6ALLNODES;
const struct macieee802 macieee802_v6allrouters = MACIEEE802_IV6ALLROUTERS;

const struct macieee802 macieee802_v4allnodes   = MACIEEE802_IV4ALLNODES;
const struct macieee802 macieee802_v4allrouters = MACIEEE802_IV4ALLROUTERS;


static inline int8_t
macieee802_hexadecimal(char character) {
  if (character >= '0' && character <= '9')
    return character-'0';
  
  if (character >= 'a' && character <= 'f')
    return character-'a'+10;

  if (character >= 'A' && character <= 'F')
    return character-'A'+10;

  return -1;
}

int
macieee802_parse(struct macieee802 *network, const char *string) {
  uint8_t bytes[MACIEEE802_SIZE] = {0};
  uint8_t value, size = 0, symbols;

  char delimiter;

  if (!string || !*string || string_length(string) != MACIEEE802_STRLENGTH-1)
    return -1;

  delimiter = string[2];

  if (delimiter != ':' && delimiter != '-')
    return -1;

  for (;;) {
    for (symbols = 0, value = 0; symbols < 2 && *string && macieee802_hexadecimal(*string) >= 0; string++, symbols++)
      value = (uint8_t)(16*value+(uint8_t)macieee802_hexadecimal(*string));

    if ((*string && *string != delimiter) || (*string && !*(string+1) && *string == delimiter))
      return -1;
    if (symbols == 0 || size > 5)
      return -1;

    bytes[size++] = value;

    if (!*string) {
      if (size != MACIEEE802_SIZE /* 00:00:00:00:00:00 */)
        return -1;

      break;
    }

    string++;
  }

  if (network)
    memcpy(network, bytes, MACIEEE802_SIZE);

  return 1;
}

char *
macieee802_format(const struct macieee802 *network, char *string, size_t length) {
  char source[MACIEEE802_STRLENGTH] = {0};

  string_zero(source, sizeof(source));

  snprintf(source, sizeof(source), "%02X:%02X:%02X:%02X:%02X:%02X", network->u8[0], network->u8[1], network->u8[2], network->u8[3], network->u8[4], network->u8[5]);

  if (string)
    return (char *)memcpy(string, source, length);

  return string_new(source);
}

int
macieee802_compare(const struct macieee802 *network1, const struct macieee802 *network2) {
  uint8_t byte;

  for (byte = 0; byte < MACIEEE802_SIZE; byte++)
    if (network1->u8[byte] < network2->u8[byte])
      return -1;
    else if (network1->u8[byte] > network2->u8[byte])
      return 1;

  return 0;
}
