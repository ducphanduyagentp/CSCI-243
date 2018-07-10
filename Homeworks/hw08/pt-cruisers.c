/// file: pt-cruisers.c
/// description: main program for homework 8
/// author: Duc Phan - ddp3945@rit.edu

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include "racer.h"
#include "display.h"

int main(int argc, char **argv) {
    srand(time(NULL));
    int maxSpeedDelay = DEFAULT_WAIT;
    int startIdx = 1;
    
    if (argc < 3) {
        fprintf(stderr, "Usage: pt-cruisers [max-speed-delay] name1 name2 [name3...]\n");
        return EXIT_FAILURE;
    } else {
        bool isNumeric = true;
        int countDot = 0;
        for (unsigned i = 0; i < strlen(argv[1]); i++) {
            isNumeric = isNumeric && (argv[1][i] == '.' || ('0' <= argv[1][i] && argv[1][i] <= '9'));
            countDot += (argv[1][i] == '.');
        }
        isNumeric = isNumeric && (countDot <= 1);
        if (isNumeric) {
            if (argc < 4) {
                fprintf(stderr, "Usage: pt-cruisers [max-speed-delay] name1 name2 [name3...]\n");
                return EXIT_FAILURE;
            }
            int num = strtol(argv[1], &argv[1], 10);
            maxSpeedDelay = num != 0 ? num : DEFAULT_WAIT;
            for (int i = 2; i < argc; i++) {
                if (strlen(argv[i]) > MAX_NAME_LEN) {
                    fprintf(stderr, "Error: racer names must not exceed length %d.", MAX_NAME_LEN);
                    return EXIT_FAILURE;
                }
            }
            for (int i = 2; i < argc; i++) {
                
            }
        } else {
            for (int i = 1; i < argc; i++) {
                if (strlen(argv[i]) > MAX_NAME_LEN) {
                    fprintf(stderr, "Error: racer names must not exceed length %d.", MAX_NAME_LEN);
                    return EXIT_FAILURE;
                }
            }
            for (int i = 1; i < argc; i++) {
                
            }
        }
        startIdx += isNumeric;
    }

    pthread_t threads[argc - startIdx];
    void *racers[argc - startIdx];
    for (int i = 0; i < argc - startIdx; i++) {
        racers[i] = (void *) makeRacer(argv[startIdx + i], i + 1);
    }

    initRacers(maxSpeedDelay);
    clear();
    
    for (int i = 0; i < argc - startIdx; i++) {
        if (pthread_create(&threads[i], NULL, run, racers[i])) {
            fprintf(stderr, "ERROR; pthread_create() failed\n");
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < argc - startIdx; i++) {
        if (pthread_join(threads[i], NULL)) {
            fprintf(stderr, "ERROR; pthread_join() failed\n");
            return EXIT_FAILURE;
        }
        destroyRacer((Racer *) racers[i]);
    }
    set_cur_pos(argc - startIdx + 1, 1);
    return 0;
}
