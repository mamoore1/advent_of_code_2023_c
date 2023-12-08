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

    if (num_squares % 9 != 0) {
        printf("There should be 9 squares per asterisk");
        return 1;
    }

    while (1) {

    }

    return 0;
}


/* Find all the asterisk indices, find all the adjacent squares, check these squares for numbers,
 and generate a new list from this, which can be passed to an adapted version of find part number
 total (i.e., we're using a check on which asterisk adjacent squares contain numbers to determine)
 a new list of squares to check for numbers, i.e., related logic to the solution for part 1)
 
 Actually we need to do more updating to the logic, because we need to find asterisks with exactly
 two adjacent squares, so we need a way of tracking how many numbers are adjacent to the asterisks. We could
 do this with another array, but it is becoming quite hard to map adjacent squares back to the inital square.
 
 I initially avoided the approach of determining the numbers and then checking the squares around them (to avoid
 having to deal with the fact this is a 2D array), but I wonder if I've gone the wrong way.
 
 Although, to be honest, if I was working in python I would just define an object with a field that contained
 a list of adjacent squares. That obviously wouldn't be the fastest way to do it, but it would work. I could
 use a struct for the same purpose here.
 
 However, a hacky way to do this would be to notice that it is actually possible to map a specific asterisk
 back to it's set of squares; given that there will always be 9 adjacent squares (due to the convenient fact
 that there are no asterisks in edge rows), the first adjacent square will always be at the asterisks index 
 position - (line length + 1). E.g., if there is an asterisk at 141 and the line-length is 140, the first
 square will be 0. This doesn't particularly help actually.
 
 My thought is that we a) get a list of the asterisk squares, b) get a list of the numbers and their locations
 and then look for the overlap. The problem is that the numbers obviously can overlap on a number of different
 digits, so it's not that simple. After finding the number, you'd have to scan each way along the number to
 find it's total, which is kind of a pain, especially because numbers go up to the edge, so you need to make sure
 not to go too far (although nb that they're never at the actual start and end, so they'll always be bounded by
 \n)

 So my new plan: 
 1) find all the asterisk adjacent squares
 2) go through all the asterisk adjacent squares:
    a) check whether they contain precisely 2 numbers (that'll be more complicated, because you need to check it's not the same number
    in multiple squares)
        i) if they do, scan along these numbers to determine what they are, and then multiply the two numbers and add them to the total

1) is already implemented, now need to implement 2


 */
int find_gears(char *engine_schematic) {
    int asterisk_squares[SYMBOL_COUNT];
    int asterisk_adjacent_squares[ADJACENT_COUNT];

    // Find all the potential gears
    find_asterisks(engine_schematic, asterisk_squares);
    // Find the squares adjacent to the potential gears
    find_all_adjacent_indices_for_symbols(engine_schematic, asterisk_squares, asterisk_adjacent_squares);

    // Go through the numbers and check which ones are in adjacent squares

    return 0;
}
