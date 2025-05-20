#ifndef BENCODE_UTILS_H__
#define BENCODE_UTILS_H__

#include <stdbool.h>

bool is_digit(char c);

char *decode_bencode_string(const char *bencoded_value);

long long decode_bencode_int(const char *bencoded_value);

char *decode_bencode_list(const char *bencoded_value);

#endif