// Check_Engine.C

// File holding the functions used when checking the engine schematics

#include <ctype.h>
#include <string.h>
#include <stdio.h>

#define SYMBOL_COUNT (1000)  // Adjust as necessary
#define ADJACENT_COUNT SYMBOL_COUNT * 9  // 9 adjacent squares per symbol

int find_all_symbol_indices(char *, int *);


int find_all_adjacent_indices_for_symbols(char *engine_schematic, int *symbol_indices, int *results) {
    int s_index = 0, r_index = 0;
    int line_length = (int) (strchr(engine_schematic, '\n') - engine_schematic) + 1;

    for (s_index = 0; symbol_indices[s_index] != 0; s_index++) {
        // Taking advantage of the fact that there are no symbols in the edge rows/columns
        results[r_index++] = symbol_indices[s_index] - line_length - 1;
        results[r_index++] = symbol_indices[s_index] - line_length;
        results[r_index++] = symbol_indices[s_index] - line_length + 1;
        
        results[r_index++] = symbol_indices[s_index] - 1;
        results[r_index++] = symbol_indices[s_index];
        results[r_index++] = symbol_indices[s_index]+ 1;
        
        results[r_index++] = symbol_indices[s_index] + line_length - 1;
        results[r_index++] = symbol_indices[s_index] + line_length;
        results[r_index++] = symbol_indices[s_index] + line_length + 1;
    }

    return r_index;
}


// Find all symbols in the string and update the results array with their locations;
// also returns an integer with the length of the result survey
int find_all_symbol_indices(char *source, int *results) {
    int source_index, result_index = 0;
    
    for (source_index = 0; source_index < strlen(source); source_index++) {
        if (
            !isalnum(source[source_index]) 
            && source[source_index] != '\n'
            && source[source_index] != '\0' 
            && source[source_index] != '.'
        ) {
            results[result_index++] = source_index;
        }
    }

    return result_index;
}


// Find all asterisks in the string, update the results array with their locations, and return
// the number found. This could be refactored with a generic version that both this and 
// find_all_symbol_indices would use
int find_asterisks(char *source, int *results) {
    int source_index, result_index = 0;

    for (source_index = 0; source_index < strlen(source); source_index++) {
        if (source[source_index] == '*') {
            results[result_index++] = source_index;
        }
    }

    return result_index;
}


int is_space_adjacent(int space_index, int *adjacent_spaces, int array_length) {
    int a_index;

    for (a_index = 0; a_index < array_length; a_index++) {
        if (adjacent_spaces[a_index] == space_index) {
            return 1;
        }
    }

    return 0;
}


// If a number is in space adjacent to a symbol, then it is a part number
// Find all numbers, check if they are in an adjacent square
int find_part_number_total(char *engine_schematic, int *adjacent_space_array, int array_length) {
    
    int i;  // Char index represents which symbol we're at, discounting newlines
    int current_total = 0;
    int in_number = 0;
    int is_part_number = 0;
    int digit_value;

    int part_number_total = 0;

    for (i = 0; engine_schematic[i] != '\0'; i++) {
        if (isdigit(engine_schematic[i])) {
            if (!in_number) {
                in_number = 1;
            }
            digit_value = engine_schematic[i] - '0';
            current_total = current_total * 10 + digit_value;

            if (!is_part_number) {
                is_part_number = is_space_adjacent(i, adjacent_space_array, array_length);
            }
        
        } else if (in_number == 1) {
            if (is_part_number) {
                part_number_total += current_total;
                is_part_number = 0;
            }
            current_total = 0;
            in_number = 0;
        }
    }

    return part_number_total;
}


int determine_engine_part_total_for_schematic(char *engine_schematic) {
    int symbol_spaces[SYMBOL_COUNT];
    int symbol_adjacent_spaces[ADJACENT_COUNT];
    memset(symbol_adjacent_spaces, 0, (size_t) ADJACENT_COUNT * sizeof(int));

    find_all_symbol_indices(engine_schematic, symbol_spaces);
    int adjacent_index_count = find_all_adjacent_indices_for_symbols(
        engine_schematic, symbol_spaces, symbol_adjacent_spaces
    );
    return find_part_number_total(engine_schematic, symbol_adjacent_spaces, adjacent_index_count);
}


// Take an engine schematic and the index of a number. Find the full value of the number
// by scanning across it. Assumes that number index is not near the start or end of the 
// string
int scan_full_number(char *engine_schematic, int number_index) {
    int number = 0;
    char *p = engine_schematic + number_index;

    while (isdigit((p-1)[0])) {
        p--;
    }

    while(isdigit(p[0])) {
        number = number * 10 + p[0] - '0';
        p++;
    }

    return number;   

}


int find_gear_total(char *engine_schematic, int *asterisk_adjacent_squares, int num_squares) {

    int squares_index = 0;
    int column_index;
    int in_number = 0;
    int current_total;
    int gear_total = 0;
    int row_index;
    int number_count;

    if (num_squares % 9 != 0) {
        printf("There should be 9 squares per asterisk");
        return 1;
    }

    // Go through all the indices of adjacent squares
    while (squares_index < num_squares) {
        current_total = 1;  // The total for this gear, if it is a gear
        number_count = 0;  // The number of numbers found in this square; 2 means it's a gear
        // Work through these in 3 groups of 3
        for (row_index = 0; row_index < 3; row_index++) {
            in_number = 0;
            for (column_index = 0; column_index < 3; column_index++) {
                // Current square value
                if isdigit(engine_schematic[asterisk_adjacent_squares[squares_index + (3 * row_index) + column_index]]) {
                    // To avoid problems in cases where a row ends in a column
                    // determine this immediately
                    if (!in_number) {
                        in_number = 1;
                        int full_number = scan_full_number(engine_schematic, asterisk_adjacent_squares[squares_index + (3 * row_index) + column_index]);
                        current_total *= full_number;
                        number_count += 1;
                    }
                }
                else {
                    in_number = 0;
                }
                
            }
        }
        if (number_count == 2) {
            gear_total += current_total;
        }
        squares_index += 9;
    }

    return gear_total;
}


/* 1) find all the asterisk adjacent squares
 2) go through all the asterisk adjacent squares:
    a) check whether they contain precisely 2 numbers (that'll be more complicated, because you need to check it's not the same number
    in multiple squares)
        i) if they do, scan along these numbers to determine what they are, and then multiply the two numbers and add them to the total

 NB. this approach was very unstable and sensitive to things like garbage data. Need to keep better track of integer array lengths (possibly)
 worth implementing something like a linked list 
 */
int determine_gear_total(char *engine_schematic) {
    int asterisk_squares[SYMBOL_COUNT];
    memset(asterisk_squares, 0, (size_t) SYMBOL_COUNT * sizeof(int));
    int asterisk_adjacent_squares[ADJACENT_COUNT];
    memset(asterisk_squares, 0, (size_t) ADJACENT_COUNT * sizeof(int));
    int num_adjacent_squares;

    // Find all the potential gears
    find_asterisks(engine_schematic, asterisk_squares);
    // Find the squares adjacent to the potential gears
    num_adjacent_squares = find_all_adjacent_indices_for_symbols(engine_schematic, asterisk_squares, asterisk_adjacent_squares);

    // Go through the numbers and check which ones are in adjacent squares
    return find_gear_total(engine_schematic, asterisk_adjacent_squares, num_adjacent_squares);
}
