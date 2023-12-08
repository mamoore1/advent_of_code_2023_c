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
    int part_total;

    if ((file = fopen("input.txt", "r")) == NULL) {
        printf("Failed to open input file\n");
        return 1;
    }

    parse_file_to_string(file, file_buffer, FILE_LENGTH);


    part_total = determine_engine_part_total_for_schematic(file_buffer);
    int gear_total = determine_gear_total(file_buffer);

    printf("part total %d\n", part_total);
    printf("gear total %d\n", gear_total);

}
