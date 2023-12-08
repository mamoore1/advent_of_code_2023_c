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
int test_find_part_number_total();
int assert_int_arrays_equal(int *, int *, int);
int test_determine_engine_part_total_for_schematic();

int main(void) {

    int all_passed = 1;

    if (!test_parse_file_to_string()) {
        all_passed = 0;
        printf("Failure in test test_parse_file_to_string\n");
    }

    if (!test_find_all_symbol_indexes()) {
        all_passed = 0;
        printf("Failure in test test_find_all_symbol_indexes\n");
    }

    if (!test_find_all_symbol_adjacent_indexes()) {
        all_passed = 0;
        printf("Failure in test test_find_all_symbol_adjacent_indexes\n");
    }

    if (!test_find_part_number_total()) {
        all_passed = 0;
        printf("Failure in test find_all_part_numbers\n");
    }

    if (!test_determine_engine_part_total_for_schematic()) {
        all_passed = 0;
        printf("Failure in test test_determine_engine_part_total_for_schematic\n");
    }

    if (all_passed) {
        printf("All tests passed!\n");
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
    char test_string[] = 
    "...*\n"
    ".../\n"
    "...*\n"
    "...%\n"
    "....\0";

    int expected_results[] = {3, 8, 13, 18};
    int actual_results[4];
    int actual_length;

    actual_length = find_all_symbol_indexes(test_string, actual_results);

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
    } else if (actual_length != 4) {
        printf("[Error]: expected length 4, got %d", actual_length);
    }

    return 1;
}


int test_find_all_symbol_adjacent_indexes() {
    char test_string[] = 
    "....\n"
    ".*..\n"
    "....\n"
    "....\0";
    int expected_results[] = {0, 1, 2, 5, 6, 7, 10, 11, 12};
    int actual_results[9];
    int actual_length;

    actual_length = find_all_symbol_adjacent_indexes(test_string, actual_results, 9);

    if (!assert_int_arrays_equal(expected_results, actual_results, 9)) {
        return 0;
    } else if (actual_length != 9) {
        printf("[Error]: expected length 9, got %d", actual_length);
        return 0;
    }

    return 1;
}


// Check that we correctly find all part numbers, i.e., numbers in the string that are next
// to a symbol
int test_find_part_number_total() {

    char test_string[] = 
    ".....\n"
    ".14..\n"
    ".5%*.\n"
    ".....\n";
    int adjacent_symbols[] = {6, 7, 8, 9, 11, 12, 13, 14, 16, 17, 18, 19};

    int total = find_part_number_total(test_string, adjacent_symbols, 12);

    if (total != 19) {
        printf("[Error]: expected total 39, got %d\n", total);
    }

    return 1;
}


int test_determine_engine_part_total_for_schematic() {
    FILE *file = fopen("test.txt", "r");
    int len_buffer = 1024;
    char buffer[len_buffer];

    parse_file_to_string(file, buffer, len_buffer);
    fclose(file);

    int total = determine_engine_part_total_for_schematic(buffer);

    if (total != 13463) {
        printf("[Error]: expected total 8535, got %d\n", total);
        return 0;
    }

    return 1;
}


int assert_int_arrays_equal(int *expected, int *actual, int n) {
    if (!compare_arrays_n(expected, actual, n)) {
        printf("[Error]: expected results: {");
        for (int i = 0; i < n; i++) {
            printf("%d, ", expected[i]);
        }
        printf("} not equal to actual results {");
        for (int i = 0; i < n; i++) {
            printf("%d, ", actual[i]);
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
