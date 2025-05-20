#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bencode_utils.h"

bool is_digit(char c) { return c >= '0' && c <= '9'; }

char *decode_bencode_string(const char *bencoded_value) {

  int length = atoi(bencoded_value);
  const char *colon_index = strchr(bencoded_value, ':');
  if (colon_index != NULL) {
    const char *start = colon_index + 1;
    char *decoded_str = (char *)malloc(length + 1);
    strncpy(decoded_str, start, length);
    decoded_str[length] = '\0';
    return decoded_str;
  } else {
    fprintf(stderr, "Invalid encoded value: %s\n", bencoded_value);
    exit(1);
  }
}

long long decode_bencode_int(const char *bencoded_value) {
  char next_char = bencoded_value[0]; // initialize
  char *decoded_str = (char *)malloc(strlen(bencoded_value));

  int i = 0;
  size_t len = strlen(bencoded_value);
  while ((i + 1) < len &&
         (next_char !=
          'e')) { // iterate through bencoded value for i+1 (skipping i)
    decoded_str[i] = bencoded_value[i + 1];
    i++;
    next_char = bencoded_value[i + 1];
  }
  // TODO: Check -0, etc. errors
  decoded_str[i] = '\0';
  fprintf(stderr, "THE DECODED STR IS: %s\n", decoded_str); // test
  char *endptr;
  errno = 0;

  long long decoded_int =
      strtoll(decoded_str, &endptr, 10); // string to long long
  if (errno != 0 || *endptr != '\0') {
    fprintf(stderr, "Invalid number: %s\n", decoded_str);
    free(decoded_str);
    exit(1);
  }
  fprintf(stderr, "THE DECODED INT IS: %lld\n", decoded_int); // test
  free(decoded_str);
  return decoded_int;
}

// find the colon position in a substring of our bencode
int find_char_pos(int pos, const char *bencoded_value, char target) {
  while (bencoded_value[pos] != target) {
    pos++;
  }
  return pos;
}

// get the length in the middle of a list
int get_len(int start, int end, const char *bencoded_value) {
  int len = end - start;
  char *num_str = (char *)malloc(sizeof(char) * (len + 1));

  if (num_str == NULL) {
    // Handle error, e.g., return -1 or exit
    return -1;
  }
  memcpy(num_str, &bencoded_value[start], len);
  num_str[len] = '\0';
  int result = atoi(num_str);
  free(num_str);
  return result;
}

char *decode_bencode_list(const char *bencoded_value) {
  int pos = 1; // character position in list item after the l

  char *decoded_list = (char *)malloc(strlen(bencoded_value) +
                                      10); // TODO: calculate real len to alloc
  decoded_list[0] = '[';
  while (pos != strlen(bencoded_value) - 1) {
    if (is_digit(bencoded_value[pos])) {
      strcat(decoded_list, "\"");
      // Check until :
      int str_pos = find_char_pos(pos, bencoded_value, ':');
      // get len
      int str_len = get_len(pos, str_pos, bencoded_value);
      char substr[str_len];
      memcpy(substr, &bencoded_value[str_pos + 1], str_len);
      substr[str_len] = '\0';
      fprintf(stderr, "DECODED SUBSTR %s\n", substr); // test
      strcat(decoded_list, substr);
      strcat(decoded_list, "\", ");
      pos = str_pos + str_len + 1;
    } else if (bencoded_value[pos] == 'i') {
      // check number until e
      int end = find_char_pos(pos, bencoded_value, 'e');
      int str_len = end - pos - 1;
      char substr[str_len];
      fprintf(stderr, "str len int %d\n", str_len);
      memcpy(substr, &bencoded_value[pos + 1], str_len);
      substr[str_len] = '\0';
      fprintf(stderr, "DECODED INT str FROM LIST %s\n", substr);
      strcat(decoded_list, substr);
      strcat(decoded_list, ", ");
      pos = end + 1;
    } else {
      fprintf(stderr, "ERROR PARSING LIST, INVALID FORMAT");
      exit(1);
    }
  }

  int end = find_char_pos(0, decoded_list, '\0');
  decoded_list[end - 2] = '\0';
  strcat(decoded_list, "]\0");
  fprintf(stderr, "DECODED LIST IS %s\n", decoded_list);
  return decoded_list;
}
