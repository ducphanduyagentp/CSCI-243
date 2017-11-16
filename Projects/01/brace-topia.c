/// file: brace-topid.c
/// description: Implemetation for project 1: brace topia simulation
/// @author: Duc Phan - ddp3945@rit.edu

#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include "display.h"

/**
    Function to print out the usage message
    @param isError: indicate if the message is printed out due to error or not.
*/
void printHelpMessage(bool isError) {
    fprintf(stderr, "usage:\n");
    fprintf(stderr, "brace-topia [-h] [-t N] [-c N] [-d dim] [-s %%str] [-v %%vac] [-e %%end]\n");
    if (isError) {
        exit(EXIT_FAILURE + 1);
    }
    fprintf(stderr, "Option\tDefault\tExample\tDescription\n");
    fprintf(stderr, "\'-h\'\t\tNA\t-h\tprint this usage message.\n");
    fprintf(stderr, "\'-t N\'\t900000\t-t 5000\tmicroseconds cycle delay.\n");
    fprintf(stderr, "\'-c N\'\t-1\t-c4\tcount cycle maximum value.\n");
    fprintf(stderr, "\'-d dim'\t15\t-d 7\twidth and height dimension.\n");
    fprintf(stderr, "\'-s %%str'\t50\t-s 30\tstrength of preference.\n");
    fprintf(stderr, "\'-v %%vac'\t20\t-v30\tpercent vacancies.\n");
    fprintf(stderr, "\'-e %%end'\t60\t-e75\tpercent Endline braces. Others want Newline.\n");
    
}

/**
    A function to randomly populate the grid
    @param types: the quantities of each type of cell to be populated.
    @param dim: The dimession of the grid.
    @param grid: The grid to be populated.
*/
void populateGrid(int types[3], int dim, char grid[][dim]) {
    char c[] = {'.', 'e', 'n'};
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            int idx = random() % 3;
            while(types[idx] <= 0) {
                idx = random() % 3;
            }
            grid[i][j] = c[idx];
            types[idx]--;
        }
    }
}

/**
    A function to determine of a coordinate is valid or not
    @param r: the row number
    @param c: the column number
    @param dim: The dimession of the grid.
    @return: true if it's a valid coordinate, false otherwise
*/
bool isInside(int r, int c, int dim) {
    return (0 <= r && r < dim && 0 <= c && c < dim);
}

/**
    A function to print out the current configuration
    @param dim: The dimession of the grid.
    @param grid: The grid to be populated.
*/
void printGrid(int dim, char grid[][dim]) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            putchar(grid[i][j]);
        }
        puts("");
    }
}

/**
    A function to calculate the happiness of an agent
    @param r: the row number
    @param c: the column number
    @param dim: The dimession of the grid.
    @param grid: The grid to be populated.
    @return: the happiness of the agent.
*/
double calculateAgentHappiness(int r, int c, int dim, char grid[][dim]) {
    int dr[] = {-1, -1 , -1, 0, 0, 1, 1, 1};
    int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    int nTotal = 0;
    int nSame = 0;
    for (int i = 0; i < 8; i++) {
        int rr = r + dr[i];
        int cc = c + dc[i];
        if (!isInside(rr, cc, dim)) {
            continue;
        }

        nTotal += (grid[rr][cc] != '.');
        nSame += (grid[rr][cc] == grid[r][c]);
    }
    return nTotal != 0 ? nSame * 1.0 / nTotal : 1.0;
}

/**
    A function to calculate the happiness of the team.
    @param dim: The dimession of the grid.
    @param grid: The grid to be populated.
    @return: the happiness of the team.
*/
double calculateHappiness(int dim, char grid[][dim]) {
    int nPopulation = 0;
    double teamHappiness = 0.0;
    for (int r = 0; r < dim; r++) {
        for (int c = 0; c < dim; c++) {
            if (grid[r][c] == '.') {
                continue;
            }
            nPopulation++;
            teamHappiness += calculateAgentHappiness(r, c, dim, grid);
        }
    }
    return teamHappiness / nPopulation;
}

/**
    A function to move all the unhappy agents (if possible)
    @param dim: The dimession of the grid.
    @param grid: The grid to be populated.
    @param threshold: The threshold to determine if an agent is happy or not.
    @return: the number of move made in this cycle.
*/
int move(int dim, char grid[][dim], double threshold) {
    int numMove = 0;

    bool validVacancy[dim][dim];
    bool movedThisCycle[dim][dim];
    double agentHappiness[dim][dim];

    for (int r = 0; r < dim; r++) {
        for (int c = 0; c < dim; c++) {
            validVacancy[r][c] = (grid[r][c] == '.');
            movedThisCycle[r][c] = false;
            if (grid[r][c] != '.') {
                agentHappiness[r][c] = calculateAgentHappiness(r, c, dim, grid);
            } else {
                agentHappiness[r][c] = 0;
            }
        }
    }

    for (int r = 0; r < dim; r++) {
        for (int c = 0; c < dim; c++) {
            if (movedThisCycle[r][c] || grid[r][c] == '.') {
                continue;
            }
            bool flag = false;
            if (agentHappiness[r][c] < threshold / 100.0) {
                for (int i = 0; i < dim; i++) {
                    for (int j = 0; j < dim; j++) {
                        if (!validVacancy[i][j]) {
                            continue;
                        }
                        validVacancy[i][j] = false;
                        movedThisCycle[i][j] = true;
                        grid[i][j] = grid[r][c];
                        grid[r][c] = '.';
                        flag = true;
                        numMove++;
                        break;
                    }
                    if (flag) {
                        break;
                    }
                }
            }
        }
    }

    return numMove;
}

/**
    A function to simulate a cycle and print out useful information
    @param dim: The dimession of the grid.
    @param grid: The grid to be populated.
    @param cycle: The current cycle number.
    @param threshold: The threshold to determine if an agent is happy or not (in percentage).
    @param vacancies: The percentage of vacancies on the grid.
    @param percentEndline: The percentage of the population that wants Endline.
    @param infiniteMode: indicate if the program is running infinite mode or not.
*/
void runACycle(int dim, char grid[][dim], int cycle, double threshold, double vacancies, double percentEndline, bool infiniteMode) {

    int moveThisCycle = 0;
    double teamHappiness = 0.0;

    if (cycle > 0) {
        moveThisCycle = move(dim, grid, threshold);
    }

    teamHappiness = calculateHappiness(dim, grid);
    if (infiniteMode) {
        set_cur_pos(1,1);
    }
    printGrid(dim, grid);
    printf("cycle: %d                                       \n", cycle);
    printf("moves this cycle: %d                            \n", moveThisCycle);
    printf("teams\' \"happiness\": %lf                      \n", teamHappiness);
    printf("dim: %d, %%strength of preference:%3.0lf%%, %%vacancy:%3.0lf%%, %%end:%3.0lf%%\n", dim, threshold, vacancies, percentEndline);
    puts("Use Control-C to quit.");
}

int main(int argc, char **argv) {
    int delay = 900000;
    int nCycle = -1;
    int dim = 15;
    double threshold = 50.0;
    double vacancies = 20.0;
    double percentEndline = 60.0;
    
    int opt;

    srandom(41);

    if (argc > 1) {
        while ((opt = getopt(argc, argv, "ht:c:d:s:v:e:")) != -1) {
            switch (opt) {
                case 't':
                    delay = (isdigit(optarg[0]) && atoi(optarg) > 0) ? atoi(optarg) : delay;
                    break;
                case 'c':
                    nCycle = atoi(optarg);
                    if (nCycle <= 0) {
                        fprintf(stderr, "count (%d) must be a non-negative integer.\n", nCycle);
                        printHelpMessage(true);
                    }
                    break;
                case 'd':
                    dim = atoi(optarg);
                    if (dim < 5 || dim > 39) {
                        fprintf(stderr, "dimension (%d) must be a value in [5...39]\n", dim);
                        printHelpMessage(true);
                    }
                    break;
                case 's':
                    threshold = strtod(optarg, &optarg);
                    if (threshold < 1.0 || threshold > 99.0) {
                        fprintf(stderr, "preference strength (%.0lf) must be a value in [1...99]\n", threshold);
                        printHelpMessage(true);
                    }
                    break;
                case 'v':
                    vacancies = strtod(optarg, &optarg);
                    if (vacancies < 1.0 || vacancies > 99.0) {
                        fprintf(stderr, "vacancy (%.0lf) must be a value in [1...99]\n", vacancies);
                        printHelpMessage(true);
                    }
                    break;
                case 'e':
                    percentEndline = strtod(optarg, &optarg);
                    if (percentEndline < 1.0 || percentEndline > 99.0) {
                        fprintf(stderr, "endline proportion (%.0lf) must be a value in [1...99]\n", percentEndline);
                        printHelpMessage(true);
                    }
                    break;
                case 'h':
                    printHelpMessage(false);
                    exit(0);
                default:
                    printHelpMessage(true);
            }
    
        }
    }
    
    char grid[dim][dim];
    int nVacant = dim * dim * vacancies / 100.0;
    int nPopulation = dim * dim - nVacant;
    int nEndline = nPopulation * percentEndline / 100;
    int nNewline = nPopulation - nEndline;
    int types[3] = {nVacant, nEndline, nNewline};
    
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            grid[i][j] = '-';
        }
    }
    
    populateGrid(types, dim, grid);
   
    bool infiniteMode = (nCycle == -1);
    if (infiniteMode) {
        clear();
        int cycle = 0;
        do {
            runACycle(dim, grid, cycle, threshold, vacancies, percentEndline, infiniteMode);
            usleep(delay);
        } while(++cycle);
    } else {
        for (int cycle = 0; cycle <= nCycle; cycle++) {
            runACycle(dim, grid, cycle, threshold, vacancies, percentEndline, infiniteMode);
        }
    }
    return 0;
}           
