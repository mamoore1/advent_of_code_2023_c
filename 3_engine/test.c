// test.c

// This is going to be difficult to test manually, so we want some actual
// tests

#include <assert.h>
#include <string.h>
#include "check_engine.h"   
#include "parse.h"

int assert_true(int);
int test_parse_file_to_string(void);
int test_find_all_symbol_indexes(void);
int compare_arrays_n(void *, void *, int);
int test_find_all_symbol_adjacent_indexes();

int main(void) {

    if (!test_parse_file_to_string()) {
        printf("Failure in test test_parse_file_to_string\n");
    }

    if (!test_find_all_symbol_indexes()) {
        printf("Failure in test test_find_all_symbol_indexes");
    }

    if (!test_find_all_symbol_adjacent_indexes()) {
        printf("Failure in test test_find_all_symbol_adjacent_indexes");
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
        return 0;
    }

    return 1;
}


int test_find_all_symbol_indexes() {
    char test_string[] = "...*\n.../\n...*\n...%\n....\0";
    int expected_results[] = {3, 8, 13, 18};
    int actual_results[4];

    find_all_symbol_indexes(test_string, actual_results);

    if (!compare_arrays_n(expected_results, actual_results, 4)) {
        printf("[Error]: expected results: {");
        for (int i = 0; i < 4; i++) {
            printf("%d, ", expected_results[i]);
        }
        printf("} not equal to actual results {");
        for (int i = 0; i < 4; i++) {
            printf("%d, ", actual_results[i]);
        }
        printf("}\n");
        return 0;
    }

    return 1;
}


int test_find_all_symbol_adjacent_indexes() {
    char test_string[] = "....\n.*..\n....\n....\0";
    int expected_results[] = {1, 2, 3, 5, 6, 7, 9, 10, 11};
    int actual_results[11];

    find_all_symbol_adjacent_indexes(test_string, 4, actual_results, 11);

    if (!compare_arrays_n(expected_results, actual_results, 9)) {
        printf("[Error]: expected results: {");
        for (int i = 0; i < 9; i++) {
            printf("%d, ", expected_results[i]);
        }
        printf("} not equal to actual results {");
        for (int i = 0; i < 9; i++) {
            printf("%d, ", actual_results[i]);
        }
        printf("}\n");
        return 0;
    }

    return 1;
}


// Compare two lists of length n
int compare_arrays_n(void *first, void *second, int n) {
    for (int i = 0; i < n; i++) {
        if (((int*) first)[i] != ((int*) second)[i]) {
            return 0;
        }
    }

    return 1;
}
