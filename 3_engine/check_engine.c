// Check_Engine.C

// File holding the functions used when checking the engine schematics

#include <ctype.h>
#include <string.h>
#include <stdio.h>

#define SYMBOL_COUNT (1000)  // Adjust as necessary
#define ADJACENT_COUNT SYMBOL_COUNT * 9  // 9 adjacent squares per symbol

int find_all_symbol_indexes(char *, int *, int);


// For a given engine schematic, find all the symbol, get their indexes and
// then return a list of the adjacent indexes
int find_all_symbol_adjacent_indexes(char *engine_schematic, int *results, int resultsize) {
    int symbol_indexes[SYMBOL_COUNT];
    memset(symbol_indexes, SYMBOL_COUNT, 0);

    int s_index = 0, r_index = 0;
    int line_length = (int) (strchr(engine_schematic, '\n') - engine_schematic) + 1;

    find_all_symbol_indexes(engine_schematic, symbol_indexes, SYMBOL_COUNT);

    for (s_index = 0; symbol_indexes[s_index] != 0; s_index++) {
        // Taking advantage of the fact that there are no symbols in the edge rows/columns
        results[r_index++] = symbol_indexes[s_index] - line_length - 1;
        results[r_index++] = symbol_indexes[s_index] - line_length;
        results[r_index++] = symbol_indexes[s_index] - line_length + 1;
        
        results[r_index++] = symbol_indexes[s_index] - 1;
        results[r_index++] = symbol_indexes[s_index];
        results[r_index++] = symbol_indexes[s_index]+ 1;
        
        results[r_index++] = symbol_indexes[s_index] + line_length - 1;
        results[r_index++] = symbol_indexes[s_index] + line_length;
        results[r_index++] = symbol_indexes[s_index] + line_length + 1;
    }

    return r_index;
}


// Find all symbols in the string and update the results array with their locations;
// also returns an integer with the length of the result survey
int find_all_symbol_indexes(char *source, int *results, int resultssize) {
    int source_index, result_index = 0;
    
    for (source_index = 0; source_index < strlen(source); source_index++) {
        if (
            !isalnum(source[source_index]) 
            && source[source_index] != '\n'
            && source[source_index] != '\0' 
            && source[source_index] != '.') {
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
    
    int i, char_index = 0;  // Char index represents which symbol we're at, discounting newlines
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
    int symbol_adjacent_spaces[ADJACENT_COUNT];
    memset(symbol_adjacent_spaces, ADJACENT_COUNT, 0);

    int adjacent_index_count = find_all_symbol_adjacent_indexes(
        engine_schematic, symbol_adjacent_spaces, ADJACENT_COUNT
    );
    return find_part_number_total(engine_schematic, symbol_adjacent_spaces, adjacent_index_count);
}

