#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bencode_utils.h"

int main(int argc, char *argv[]) {
  // Disable output buffering
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);

  if (argc < 3) {
    fprintf(stderr, "Usage: your_program.sh <command> <args>\n");
    return 1;
  }

  const char *command = argv[1];

  if (strcmp(command, "decode") == 0) {
    fprintf(stderr, "Logs from your program will appear here!\n");

    const char *encoded_str = argv[2];
    if (is_digit(encoded_str[0])) { // if string
      char *decoded_str = decode_bencode_string(encoded_str);
      printf("\"%s\"\n", decoded_str);
      free(decoded_str);
    } else if (encoded_str[0] == 'i') { // if number
      long long decoded_int = decode_bencode_int(encoded_str);
      printf("%lld\n", decoded_int);
    } else if (encoded_str[0] == 'l') { // if list
      char *decoded_list = decode_bencode_list(encoded_str);
      printf("%s\n", decoded_list);
      free(decoded_list);
    } else {
      fprintf(stderr, "Invalid format (not string or int or list)\n");
      exit(1);
    }
  } else {
    fprintf(stderr, "Unknown command: %s\n", command);
    return 1;
  }

  return 0;
}
