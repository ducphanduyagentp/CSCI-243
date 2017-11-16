// file: build_graph.c
// description: 
// implementation of main program for homework 5
// author: Duc Phan, ddp3945@rit.edu
// date: 10/21/2017

#include <stdio.h>
#include <string.h>
#include <object_list.h>
#include "simple_graph.h"

// The main function for the homework that loads the graph from standard input,
// prints the graph and deletes the graph.
int main() {
    ObjectList *graph = ol_create();
    grh_load_file(graph, stdin);
    grh_print_graph(graph);
    grh_delete_graph(graph);
    
    return 0;
}