#include "base64.h"

#include <strings/strings.h>

#include <stdlib.h>
#include <stddef.h>

#include <math.h>


static const char base64_encode_table[] = {
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
  'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
  'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
  'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/',
  '"',
};

char *
base64_encode(const uint8_t *buffer, size_t length) {
  size_t encoding_length = (size_t)((ceil((double)length/3.0))*4);
  char *bufferZ = NULL, *bufferX = NULL;

  if (!(bufferZ = (char *)calloc(encoding_length+1, sizeof(char))))
    return NULL;
  bufferX = bufferZ;

  do {
    *bufferX++ = base64_encode_table[(buffer[0] & 0xFC) >> 0x02];
    if (length == 1) {
      *bufferX++ = base64_encode_table[((buffer[0] & 0x03) << 0x04)];
      *bufferX++ = '=';
      *bufferX++ = '=';

      break;
    }

    *bufferX++ = base64_encode_table[((buffer[0] & 0x03) << 0x04)|((buffer[1] & 0xF0) >> 0x04)];
    if (length == 2) {
      *bufferX++ = base64_encode_table[((buffer[1] & 0x0F) << 0x02)];
      *bufferX++ = '=';
 
      break;
    }

    *bufferX++ = base64_encode_table[((buffer[1] & 0x0F) << 0x02)|((buffer[2] & 0xC0) >> 0x06)];
    *bufferX++ = base64_encode_table[(buffer[2] & 0x3F)];

    buffer += 3;
  } while ((length -= 3));

  return bufferZ;
}


static int base64_decode_table[] = {
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
  52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1,  0, -1, -1,
  -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
  15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
  -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
  41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
  -1,
};

uint8_t *
base64_decode(const char *string, size_t *length) {
  size_t encoding_length = string_length(string);
  size_t transform_length = (encoding_length/4)+4;
  uint8_t byte;

  uint8_t *bufferZ = NULL, *bufferX = NULL;

  if ((encoding_length & 0x03))
    return NULL;

  if (!(bufferZ = (uint8_t *)calloc(transform_length, sizeof(uint8_t))))
    return NULL;
  bufferX = bufferZ;

  do {
    for (byte = 0; byte <= 3; byte++)
      if (string[byte] > 0x80 || base64_decode_table[(int)string[byte]] == -1) {
        free(bufferZ);
        return NULL;
      }

    *bufferX++ = (uint8_t)((base64_decode_table[(int)string[0]] << 0x02)|(base64_decode_table[(int)string[1]] & 0x30) >> 0x04);

    if (string[2] != '=')
      *bufferX++ = (uint8_t)(((base64_decode_table[(int)string[1]] & 0x0F) << 0x04)|(base64_decode_table[(int)string[2]] & 0x3C) >> 0x02);

    if (string[3] != '=')
      *bufferX++ = (uint8_t)(((base64_decode_table[(int)string[2]] & 0x03) << 0x06)|(base64_decode_table[(int)string[3]]));

    string += 4;
  } while ((encoding_length -= 4));

  if (length)
    *length = (size_t)(bufferX-bufferZ);

  return bufferZ;
}
