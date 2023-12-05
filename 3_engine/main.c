// Main.c

// For each number in the engine schematic, if it is adjacent (including diagonally)
// to a symbol, it is a part number and we need to add it to the total amount
// and return.

// We can do this with a 2 pass approach: scan through once, find the locations
// of all the symbols (i.e., row number and column), create a list containing
// all of the spaces adjacent to symbols, and then on a second scan through,
// for all the numbers we find, if their location is in the list
// we can add their value to the part total

#include <stdio.h>
#include <stdlib.h>
#include "check_engine.h"
#include "parse.h"

#define LINE_LENGTH (256)    // Actual line length is 140
#define FILE_LENGTH (20000)  // Actual file length is 19600
#define SYMBOL_COUNT (1000)  // Adjust as necessary

int main(void) {

    FILE *file;
    char file_buffer[FILE_LENGTH];
    int symbol_locations[SYMBOL_COUNT];
    int symbol_count;

    if ((file = fopen("input.txt", "r")) == NULL) {
        printf("Failed to open input file\n");
        return 1;
    }

    parse_file_to_string(file, file_buffer, FILE_LENGTH);
    find_all_symbol_indexes(file_buffer, symbol_locations);

    while (symbol_locations[symbol_count] != 0) {
        symbol_count++;
    }
    if (symbol_count >= SYMBOL_COUNT) {
        printf("[Warning]: symbol count equals the max symbol count; consider increasing this value");
    }

}
