// Main.C

// Open the input file, find the first and last digit on each line and combine
// them to get each calibration value. Sum all of the calibration values and 
// return.
// NB. if there is only one digit in the line, that counts as both the 
// first and last value

// Part two: some of the calibration values have been written in text form,
// e.g., 8 as eight. Parse these values as well.

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>

#define LINE_LENGTH (256)


int main(void) {

    int result, calibration_sum = 0;
    FILE *file;
    char line[LINE_LENGTH];  // Chosen arbitrarily, but the line lengths are shorter 
    // than this

    if ((file = fopen("input.txt", "r")) == NULL) {
        printf("Failed to open input file\n");
        return 1;
    }

    // This is pretty hacky, but we can just iterate through all of the digits
    // and try and to find and replace them
    // (It would be a lot more efficient to do this for the whole file,
    // rather than line by line.)
    
    const char *digit_strings[] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    
    while (fgets(line, LINE_LENGTH, file)) {
        int first_digit = -1, last_digit;
        char *p = line;

        for (int i = 0; i < 9; i++) {        
            char *index;
            
            // As a hack, we can just replace the second
            // character of the substring with the relevant integer
            // (Can't overwrite first character as this will mean numbers can overwrite 
            // numbers that appear before them in the line, making them 
            // undetectable)
            while (index = strstr(line, digit_strings[i])) {
                    *(index + 1) = '0' + (i + 1);
            }
        }

        while (*p) {
            if (isdigit(*p)) {
                // Whatever remains in last digit after we've finished parsing the whole line will be the final digit
                last_digit = p[0] - '0';
                if (first_digit < 0) {
                    first_digit = last_digit;
                }
            }
                p++;
        }

        calibration_sum += (first_digit * 10 + last_digit);
    }

    fclose(file);

    printf("%d\n", calibration_sum);

    return 0;
}
