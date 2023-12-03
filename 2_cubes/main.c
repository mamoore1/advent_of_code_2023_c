// Main.c

// The input file contains a collection of game results, where the game
// ID is defined with "Game <ID>:" and this is followed by a semicolon 
// separated list of counts of different coloured cubes.
// Given the constraint that the bag was initially loaded 
// with 12 red cubes, 13 green cubes and 14 blue cubes,
// we need to determine which of the games would have been
// possible and determine the sum of the IDs of those games

#include <stdio.h>
#include <ctype.h>

#define LINE_LENGTH (256) // Longer than the biggest input line
#define RED_COUNT (12)
#define GREEN_COUNT (13)
#define BLUE_COUNT (14)

int main(void) {

    int id_total = 0, id, is_viable;
    FILE *file;
    char line[LINE_LENGTH];
    char *p;

    if ((file = fopen("input.txt", "r")) == NULL) {
        if ((file = fopen("input.txt", "r")) == NULL) {
            printf("Failed to open input file\n");
            return 1;
        }
    }

    while (fgets(line, LINE_LENGTH, file)) {  // Iterate through line by line
        id = 0;
        p = line;

        // Iterate until hitting the ID
        while (!isdigit(*p)) {
            p++;
        }

        // Get the id
        while (isdigit(*p)) {
            id = id * 10 + (p[0] - '0');
            p++;
        }

        // Iterate until we hit the first number of blocks
        while (!isdigit(*p)) {
            p++;
        }

        // Until the end of the line, we want to check the next three blocks
        // and see if this game is viable; if it is, we add the id to the sum

        is_viable = 1;

        while (*p) {
            int count = 0;

            // Currently we're at a number
            while (isdigit(*p)) {
                count = count * 10 + (p[0] - '0');
                p++;
            }
            
            // Go through the whitespace or semicolons
            while (!isalpha(*p)) p++;

            // We can identify what colour blocks they are based on the first
            // character
            switch (p[0]) {
                case 'g': {
                    if (count > GREEN_COUNT) {
                        is_viable = 0;
                    }    
                    break;
                }
                case 'b':{
                    if (count > BLUE_COUNT) {
                        is_viable = 0;
                    }
                    break;
                }
                case 'r':{
                    if (count > RED_COUNT) {
                        is_viable = 0;
                    }
                    break;
                }
                default:
                    ;
            }

            while (!isdigit(*p) && *p) p++;
        }

        if (is_viable == 1) {
            id_total += id;
        } 
    }

    fclose(file);

    printf("%d\n", id_total);

    return 0;
}
