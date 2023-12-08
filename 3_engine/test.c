// test.c

// This is going to be difficult to test manually, so we want some actual
// tests

#include <string.h>
#include "check_engine.h"   
#include "parse.h"


#define assert_is_equal_int(expected, actual) if (expected != actual) {printf("[Assertion Error]: expected %d, got %d\n", expected, actual); return 1;}


// Helper definitions
void print_int_array(int *, int);
int compare_arrays_n(void *, void *, int);
int assert_int_arrays_equal(int *, int *, int);

// Test definitions: could really autogenerate these in a similar way to pytest
int test_parse_file_to_string(void);
int test_find_all_symbol_indices(void);
int test_find_part_number_total();
int test_determine_engine_part_total_for_schematic();
int test_find_asterisks();
int test_find_all_adjacent_indices_for_symbols();
int test_scan_full_number();
int test_find_gear_total();


int main(void) {

    int all_passed = 1;

    // if (test_parse_file_to_string()) {
    //     all_passed = 0;
    //     printf("Failure in test test_parse_file_to_string\n");
    // }

    // if (test_find_all_symbol_indices()) {
    //     all_passed = 0;
    //     printf("Failure in test test_find_all_symbol_indices\n");
    // }

    // if (test_find_part_number_total()) {
    //     all_passed = 0;
    //     printf("Failure in test find_all_part_numbers\n");
    // }

    // if (test_determine_engine_part_total_for_schematic()) {
    //     all_passed = 0;
    //     printf("Failure in test test_determine_engine_part_total_for_schematic\n");
    // }

    // if (test_find_asterisks()) {
    //     all_passed = 0;
    //     printf("Failure in test test_find_asterisks\n");
    // }

    // if (test_find_all_adjacent_indices_for_symbols()) {
    //     all_passed = 0;
    //     printf("Failure in test test_find_all_adjacent_indices_for_symbols\n");
    // }

    // if (test_scan_full_number()) {
    //     all_passed = 0;
    //     printf("Failure in test test_scan_full_number\n");
    // }

    if (test_find_gear_total()) {
        all_passed = 0;
        printf("Failure in test test_find_gear_total\n");
    }

    if (all_passed) {
        printf("All tests passed!\n");
    }
}


int test_parse_file_to_string() {
    FILE *file = fopen("test.txt", "r");
    int len_buffer = 1024;
    char buffer[len_buffer];

    // Test parse_file_to_string
    if (parse_file_to_string(file, buffer, len_buffer) == 0) {
        printf("Failed to parse file");
        return 1;
    }
    fclose(file);

    assert_is_equal_int((int) strlen(buffer), 704)
    return 0;
}


int test_find_all_symbol_indices() {
    char test_string[] =
    "...*\n"
    ".../\n"
    "...*\n"
    "...%\n"
    "....\0";
    int expected_length = 4;
    int expected_indices[] = {3, 8, 13, 18};
    int actual_results[4];
    int actual_length;

    actual_length = find_all_symbol_indices(test_string, actual_results);

    if (!assert_int_arrays_equal(expected_indices, actual_results, actual_length)) {
        return 1;
    }
    assert_is_equal_int(expected_length, actual_length)
    assert_is_equal_int(4, actual_length)

    return 0;
}


int test_find_asterisks() {
    char test_string[] = 
    ".....\n"
    ".*./.\n"
    "..&.*\n"
    ".-.*.\n"
    "..=..\n";
    int expected_indices[] = {7, 16, 21};
    int expected_length = 3;
    int actual_results[4];
    memset(actual_results, 0, (size_t) 4 * sizeof(int));
    int actual_length;

    actual_length = find_asterisks(test_string, actual_results);

    if (!assert_int_arrays_equal(expected_indices, actual_results, actual_length)) {
        return 1;
    }
    assert_is_equal_int(expected_length, actual_length);

    return 0;
}


int test_find_all_adjacent_indices_for_symbols() {

    char test_string[] = 
    "....\n"
    ".*..\n"
    "....\n"
    "....\0";
    int expected_results[] = {0, 1, 2, 5, 6, 7, 10, 11, 12};
    int actual_results[12];
    memset(actual_results, 0, (size_t) 12 * sizeof(int));
    int actual_length = 0;

    int symbols[1];
    memset(symbols, 0, (size_t) 1 * sizeof(int));

    find_all_symbol_indices(test_string, symbols);
    actual_length = find_all_adjacent_indices_for_symbols(test_string, symbols, actual_results);
    
    if (!assert_int_arrays_equal(expected_results, actual_results, actual_length)) {
        return 1;
    }
    assert_is_equal_int(9, actual_length);

    return 0;
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
    assert_is_equal_int(19, total);

    return 0;
}


int test_determine_engine_part_total_for_schematic() {
    FILE *file = fopen("test.txt", "r");
    int len_buffer = 1024;
    char buffer[len_buffer];

    parse_file_to_string(file, buffer, len_buffer);
    fclose(file);

    int total = determine_engine_part_total_for_schematic(buffer);
    assert_is_equal_int(13463, total)

    return 0;
}


int test_scan_full_number() {
    char test_string[] =
    ".1.2..\n"
    ".23.4.\n"
    "..567.\n";
    int index = 10;
    int expected_value = 23;

    int actual_value = scan_full_number(test_string, index);
    assert_is_equal_int(expected_value, actual_value)

    return 0;
}


int test_find_gear_total() {
    char test_string[] =
    ".2.3..\n"
    "..*4..\n"
    "12....\n"
    ".*3..2\n"
    "....*.\n"
    ".321..\n";
    int expected_value = 678;
    int asterisk_squares[9];
    memset(asterisk_squares, 0, (size_t) 9 * sizeof(int));
    int asterisk_adjacent_squares[40];
    memset(asterisk_adjacent_squares, 0, (size_t) 40 * sizeof(int));
    int num_adjacent_squares;

    find_asterisks(test_string, asterisk_squares);
    num_adjacent_squares = find_all_adjacent_indices_for_symbols(test_string, asterisk_squares, asterisk_adjacent_squares);

    int actual_value = find_gear_total(test_string, asterisk_adjacent_squares, num_adjacent_squares);

    assert_is_equal_int(expected_value, actual_value);

    return 0;
}


void print_int_array(int *array, int array_length) {
    printf("{");
    for (int i = 0; i < array_length; i++) {
        printf("%d, ", array[i]);
    }
    printf("}");
}


int assert_int_arrays_equal(int *expected, int *actual, int n) {
    if (!compare_arrays_n(expected, actual, n)) {
        printf("[Error]: expected results: ");
        print_int_array(expected, n);
        printf("not equal to actual results: ");
        print_int_array(actual, n);
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
