// Check_Engine.C

// File holding the functions used when checking the engine schematics

#include <ctype.h>
#include <string.h>
#include <stdio.h>

#define SYMBOL_COUNT (1000)  // Adjust as necessary
#define ADJACENT_COUNT SYMBOL_COUNT * 9  // 9 adjacent squares per symbol

void find_all_symbol_indexes(char *, int *, int);


// For a given engine schematic, find all the symbol, get their indexes and
// then return a list of the adjacent indexes
void find_all_symbol_adjacent_indexes(char *engine_schematic, int line_length, int *results, int resultsize) {
    int symbol_indexes[SYMBOL_COUNT];
    int s_index, r_index;
    
    find_all_symbol_indexes(engine_schematic, symbol_indexes, SYMBOL_COUNT);

    for (s_index = 0; symbol_indexes[s_index] != 0; s_index++) {
        // Taking advantage of the fact that there are no symbol in the edge rows/columns
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
}


// Find all symbols in the string and return a array of their locations
void find_all_symbol_indexes(char *source, int *results, int resultssize) {
    int source_index, result_index = 0;

    memset(results, resultssize, 0);
    
    for (source_index = 0; source_index < strlen(source); source_index++) {
        if (
            !isalnum(source[source_index]) 
            && source[source_index] != '\n'
            && source[source_index] != '\0' 
            && source[source_index] != '.') {
            results[result_index++] = source_index;
        }
    }
}