// Parse.C

// File containing functions used to extract information from a file
#include "parse.h"


// Gets all the information from a file and parses it into a string
// Takes the destination string and the length of the string
int parse_file_to_string(FILE *file, char *destination, int length) {
    int c, index = 0;
    while ((c = fgetc(file)) != EOF) {
        destination[index++] = c;
    }
    destination[index] = '\0';

    return index;
}
