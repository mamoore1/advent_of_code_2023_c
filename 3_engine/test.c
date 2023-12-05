// test.c

// This is going to be difficult to test manually, so we want some actual
// tests

#include <assert.h>
#include <string.h>
#include "parse.h"

int assert_true(int);
int test_parse_file_to_string(void);

int main(void) {

    if (test_parse_file_to_string()) {
        printf("Failure in test test_parse_file_to_string\n");
    }

    return 0;
}


int test_parse_file_to_string() {
    FILE *file = fopen("test.txt", "r");
    int len_buffer = 1024;
    char buffer[len_buffer];

    // Test parse_file_to_string
    if (parse_file_to_string(file, buffer, len_buffer) == 0) {
        printf("Failed to parse file");
    }
    fclose(file);

    if (!(strlen(buffer) == 704)) {
        printf("[Error]: buffer length %ld != expected length %d\n", strlen(buffer), 704);
        return 1;
    }

    return 0;
}

