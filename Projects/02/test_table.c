/// \file test_table.c
/// \brief A test program for the hash table.
/// Author: Sean Strout (RIT CS)

#include <assert.h>  // assert
#include <stdio.h>   // printf, fprintf
#include <stdlib.h>  // rand, srand, EXIT_SUCCESS
#include <string.h>  // strcmp
#include <time.h>    // time
#include "hash.h"    // longHash, longEquals, longStrPrint, strHash, strEquals
                     // strLongPrint, longlongPrint
#include "table.h"   // create, destroy, dump, get, has, put

/// Test function for long keys with c-string values.
void testLongStr() {
    // The test data
    long elements[] = {10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160};
    char* names[] = {"ten","twenty","thirty","forty","fifty","sixty","seventy",
            "eighty","ninety","one hundred","one hundred ten","one hundred twenty",
            "one hundred thirty","one hundred forty","one hundred fifty",
            "one hundred sixty"};
    const int NUM_ELEMENTS = INITIAL_CAPACITY;
    long missing = 0;
    char* ten = "TEN";

    printf("testIntStr()...\n");
    Table* t = create(longHash, longEquals, longStrPrint);

    // the hash table will rehash when the 13th key, 130, is put
    for (int i=0; i<NUM_ELEMENTS; ++i) {
        put(t, (void*)elements[i], (void*)names[i]);
    }
	// update, new value won't be found later!
    char* oldValue = (char*)put(t, (void*)elements[0], ten);
    if (oldValue == NULL || strcmp(oldValue, names[0]) != 0) {
        printf("Error, put did not return old value for a key upon update!\n");
    } else {
        printf("Success, put returned correct old value for a key upon update!\n");
    }
    dump(t, true);

    // check for existence of all keys
    for (int i=0; i<NUM_ELEMENTS; ++i) {
        printf("has(%lu)? %d\n", elements[i], has(t, (void*)elements[i]));
    }
    printf("has(0)? %d\n", has(t, (void*)missing));  // missing key

    // check that all values match for all keys
    for (int i=0; i<NUM_ELEMENTS; ++i) {
        printf("get(%lu): %s\n", elements[i], (char*) get(t, (void*)elements[i]));
    }

    // grab all keys and make sure they were all found
    long** keyData = (long**) keys(t);
    for (int i=0; i<NUM_ELEMENTS; ++i) {
        bool found = false;
        for (int j=0; j<NUM_ELEMENTS; ++j) {
            if ((long)keyData[i] == elements[j]) {
                found = true;
                break;
            }
        }
        printf("key: %lu,%sfound!\n", (long)keyData[i], found ? " " : " not ");
    }
    free(keyData);  // must do this, we claimed ownership by calling keys()

    // grab all values and make sure they were all found
    char** valData = (char**) values(t);
    for (int i=0; i<NUM_ELEMENTS; ++i) {
        bool found = false;
        for (int j=0; j<NUM_ELEMENTS; ++j) {
            if (strcmp(valData[i], names[j]) == 0) {
                found = true;
                break;
            }
        }
        printf("value: %s,%sfound!\n", valData[i], found ? " " : " not ");
    }
    free(valData);  // must do this, we claimed ownership by calling values()

    dump(t, false);
    destroy(t);
}

/// Test function for c-string keys with long values.  It doesn't re-test
/// a put update, keys, or values
void testStrLong() {
    // Same data as the first, just associated the other way
    char* names[] = {"ten","twenty","thirty","forty","fifty","sixty","seventy",
            "eighty","ninety","one hundred","one hundred ten","one hundred twenty",
            "one hundred thirty","one hundred forty","one hundred fifty",
            "one hundred sixty"};
    long elements[] = {10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160};
    int NUM_ELEMENTS = INITIAL_CAPACITY;
    char* missing = "missing";

    printf("testStrInt()...\n");
    Table* t = create(strHash, strEquals, strLongPrint);

    // rehash happens with 13th name, "one hundred thirty"
    for (int i=0; i<NUM_ELEMENTS; ++i) {
        put(t, (void*)names[i], (void*)elements[i]);
    }
    dump(t, true);

    // check that all keys are found
    for (int i=0; i<NUM_ELEMENTS; ++i) {
        printf("has(%s)? %d\n", names[i], has(t, (void*)names[i]));
    }
    printf("has(missing)? %d\n", has(t, missing));  // missing key

    // check that all values are retrievable for all keys
    for (int i=0; i<NUM_ELEMENTS; ++i) {
        printf("get(%s): %lu\n", names[i], (long)get(t, (void*)names[i]));
    }

    dump(t, false);
    destroy(t);
}

/// This is the third test, a stress test, which uses a hash table of
/// long keys and long values (negated).
/// @param seed a seed for the random number generator
void testStress(int seed) {
    const size_t NUM_ELEMENTS = 1000000;  // 1 million elements (pinky to mouth)
    long* elements = (long*) malloc(NUM_ELEMENTS * sizeof(long));
    if (elements == NULL) {
        fprintf(stderr, "testStress failed\n");
        assert(NULL);
    }

    printf("testStress()...\n");

    srand(seed);  // seed the rng

    // populate elements with random integers
    for (size_t i=0; i<NUM_ELEMENTS; ++i) {
        elements[i] = rand();
    }

    // create hash table, key=int, value=int
    Table* t = create(longHash, longEquals, longLongPrint);

    // put all elements
    for (size_t i=0; i<NUM_ELEMENTS; ++i) {
        put(t, (void*)elements[i], (void*)-elements[i]);
    }

    // check that has finds each element
    for (size_t i=0; i<NUM_ELEMENTS; ++i) {
        if (has(t, (void*)elements[i]) != true) {
            fprintf(stderr, "testStress, has() checked failed");
            free(elements);
            assert(NULL);
        }
    }

    // check that get works
    for (size_t i=0; i<NUM_ELEMENTS; ++i) {
        if (-(long)get(t, (void*)elements[i]) != elements[i]) {
            fprintf(stderr, "testStress, get() checked failed");
            free(elements);
            assert(NULL);
        }
    }

    // these results depend on whether any duplicate values are randomly
    // generated
    dump(t, false);
    destroy(t);
    free(elements);
}

/// The main function runs the three test functions.
/// Usage: test_table #
/// @param argc command line argument count
/// @param argv command line arguments
/// @return EXIT_SUCCESS
int main(int argc, char* argv[]) {
    testLongStr();  // first test for long keys and c-string values
    testStrLong();  // second test for c-string keys and long values

    // If no number is specified on the command line, seed on current time
    int seed = time(0);
    if (argc == 2) {
        seed = atoi(argv[1]);
    }
    testStress(seed);  // third test for int keys and int values

    return EXIT_SUCCESS;
}
