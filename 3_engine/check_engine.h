// Check_Engine.H

// Headers for functions for checking values in the engine diagram

#ifndef _CHECK_ENGINE_H

#define _CHECK_ENGINE_H

int find_part_number_total(char *, int *, int);
int find_all_symbol_indices(char *, int *);
int determine_engine_part_total_for_schematic(char *);
int find_asterisks(char *, int *);
int find_all_adjacent_indices_for_symbols(char *, int *, int *);
int scan_full_number(char *, int);

#endif
