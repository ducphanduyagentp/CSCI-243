/// file: readcsv.c
/// description: Implemetation for homework 2: parsing csv files
/// @author: Duc Phan - ddp3945@rit.edu

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// main program that reads the input file line by line, splits the data 
// with the comma as the delimiter (a.k.a parsing csv files) and prints 
// a report to stdout.
int main() {
    char input[1024];
    char *endptr;
    long long nZipCodes = 0;
    long long nPopulation = 0;
    long long minZipCode = 0;
    long long minPopulation = LLONG_MAX;
    long long maxZipCode = 0;
    long long maxPopulation = 0;

    fgets(input, 1000, stdin);
    while (fgets(input, 1000, stdin)) {
        nZipCodes ++;
        long long number;
        endptr = input;
        int thisZipCode = 0;
        for (int i = 0; i < 7; i++, endptr++) {
            if (i == 2 || i == 6) {
                strtof(endptr, &endptr);
                continue;
            }

            number = strtoll(endptr, &endptr, 10);

            if (i == 0) {
                thisZipCode = number;
            } else if (i == 1) {
                nPopulation += number;
                if (number > maxPopulation) {
                    maxPopulation = number;
                    maxZipCode = thisZipCode;
                }
                if (number < minPopulation) {
                    minPopulation = number;
                    minZipCode = thisZipCode;
                }
            }
        }
    }

    printf("Total population across %lld zipcodes is %lld.\n", nZipCodes, nPopulation);
    printf("The fewest people live in %lld, population %lld.\n", minZipCode, minPopulation);
    printf("The most people live in %lld, population %lld.\n", maxZipCode, maxPopulation);
}
